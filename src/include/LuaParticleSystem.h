#ifndef CLASS_NCINE_LUAPARTICLESYSTEM
#define CLASS_NCINE_LUAPARTICLESYSTEM

struct lua_State;

namespace ncine {

class LuaStateManager;

/// Lua bindings around the `ParticleSystem` class
class LuaParticleSystem
{
  public:
	static void expose(LuaStateManager *stateManager);
	static void release(void *object);

  private:
	static int newObject(lua_State *L);
	static int cloneNode(lua_State *L);

	static int addColorAffector(lua_State *L);
	static int addSizeAffector(lua_State *L);
	static int addRotationAffector(lua_State *L);
	static int addPositionAffector(lua_State *L);
	static int addVelocityAffector(lua_State *L);

	static int numAffectors(lua_State *L);
	static int affector(lua_State *L);
	static int removeAffector(lua_State *L);
	static int clearAffectors(lua_State *L);

	static int emitParticles(lua_State *L);
	static int killParticles(lua_State *L);
	static int inLocalSpace(lua_State *L);
	static int setInLocalSpace(lua_State *L);

	static int isParticlesUpdateEnabled(lua_State *L);
	static int setParticlesUpdateEnabled(lua_State *L);
	static int areAffectorsEnabled(lua_State *L);
	static int setAffectorsEnabled(lua_State *L);

	static int numParticles(lua_State *L);
	static int numAliveParticles(lua_State *L);

	static int setTexture(lua_State *L);
	static int setTexRect(lua_State *L);
	static int setAnchorPoint(lua_State *L);
	static int setFlippedX(lua_State *L);
	static int setFlippedY(lua_State *L);
	static int setBlendingPreset(lua_State *L);
	static int setBlendingFactors(lua_State *L);
	static int setLayer(lua_State *L);
};

}

#endif
