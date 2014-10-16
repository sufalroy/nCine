#include <cstdlib> // for exit()
#include "ncProfileVariable.h"
#include "ncServiceLocator.h"

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

ncProfileVariable::ncProfileVariable(unsigned int numValues, float rejectDelay)
	: numValues_(numValues), rejectDelay_(rejectDelay), nextIndex_(0),
	  min_(0.0f), max_(0.0f), mean_(0.0f), isFirstValue_(true)
{
	if (numValues_ > 0)
	{
		values_ = new float[numValues_];
		for (unsigned int i = 0; i < numValues_; i++)
		{
			values_[i] = 0.0f;
		}
	}
	else
	{
		ncServiceLocator::logger().write(ncILogger::LOG_FATAL, (const char *)"ncProfileVariable::ncProfileVariable - Array size should be greater than zero");
		exit(EXIT_FAILURE);
	}
}

ncProfileVariable::~ncProfileVariable()
{
	if (values_)
	{
		delete[] values_;
	}
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

/// Adds a new value for the variable
/*! /return A bool indicating wether the value has been registered or not (rejected) */
bool ncProfileVariable::addValue(float value)
{
	if (isFirstValue_)
	{
		min_ = value;
		max_ = value;
		isFirstValue_ = false;

		if (rejectDelay_ > 0.0f)
		{
			timer_.start();
		}
	}

	bool registerValue = false;
	if (rejectDelay_ > 0.0f)
	{
		// Timer expired
		if (timer_.interval() > rejectDelay_)
		{
			registerValue = true;
			timer_.start();
		}
	}
	else
	{
		registerValue = true;
	}

	// A new value has to be registered in the array
	if (registerValue)
	{
		if (value > max_)
		{
			max_ = value;
		}
		else if (value < min_)
		{
			min_ = value;
		}

		values_[nextIndex_] = value;
		mean_ = 0.0f;
		for (unsigned int i = 0; i < numValues_; i++)
		{
			mean_ += values_[(nextIndex_ - i) % numValues_];
		}
		mean_ *= 1.0f / numValues_;

		nextIndex_ = (nextIndex_ + 1) % numValues_;
	}

	// HACK: Looking for max and min across all the values, to prevent random spikes
	max_ = 0.0f;
	min_ = 0.0f;

	for (unsigned int i = 0; i < numValues_; i++)
	{
		if (values_[i] > max_)
		{
			max_ = values_[i];
		}
		else if (values_[i] < min_)
		{
			min_ = values_[i];
		}
	}

	return registerValue;
}

/// Returns the value at the specified index normalized between the two numbers provided
float ncProfileVariable::normBetweenValue(unsigned int index, float min, float max) const
{
	float value = 0.0f;

	if (index < numValues_ && (max - min > 0.0f))
	{
		value = (values_[index] - min) / (max - min);
	}

	return value;
}

/// Returns the mathematical mean normalized between the two numbers provided
float ncProfileVariable::normBetweenMean(float min, float max) const
{
	float value = 0.0f;

	if (max - min > 0.0f)
	{
		value = (mean_ - min) / (max - min);
	}

	return value;
}
