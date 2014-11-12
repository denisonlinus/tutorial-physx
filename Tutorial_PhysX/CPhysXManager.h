//-----------------------------------------------------------------------------
// (C) Copyright 2007-2014 Denison Linus
//
// Author: Denison Linus
// Email: dlmtavar@gmail.com
//
//-----------------------------------------------------------------------------
#ifndef _CPHYSXMANAGER_H_
#define _CPHYSXMANAGER_H_

#include "NxPhysics.h"
#include "NxCooking.h"
#include "NxControllerManager.h"
#include "IMesh.h"
#include "IUnknown.h"
#include "ISceneNode.h"
#include "CPhysXEntity.h"
#include "CPhysXCharacterControl.h"
#include "vector3d.h"
#include "irrstring.h"
#include "irrmap.h"
#include "irrarray.h"
#include "ITerrainSceneNode.h"
#include "SMeshBufferLightMap.h"
#include "IFileSystem.h"
using namespace irr;

//-----------------------------------------------------------------------------
class CPhysXManager : public virtual irr::IUnknown
{
public:
	CPhysXManager(io::IFileSystem* fs, ILogger* logger, const core::vector3df& gravity = core::vector3df(0.0f, -9.81f, 0.0f));
	~CPhysXManager();
	bool initialize();
	void finalize();
	void update();
	CPhysXEntity* createRigidBox(const core::stringc& name, scene::ISceneNode* scenenode, const core::vector3df& dimmension, const bool& isDynamic, E_CHARACTER_COLLISION_GROUP group = ECCG_COLLIDABLE_PUSHABLE, const f32& mass = 1.0f);
	CPhysXEntity* createRigidSphere(const core::stringc& name, scene::ISceneNode* scenenode, const f32& radius, const bool& isDynamic, E_CHARACTER_COLLISION_GROUP group = ECCG_COLLIDABLE_PUSHABLE, const f32& mass = 1.0f);
	CPhysXEntity* createRigidCapsule(const core::stringc& name, scene::ISceneNode* scenenode, const f32& height, const f32& radius, const bool& isDynamic, E_CHARACTER_COLLISION_GROUP group = ECCG_COLLIDABLE_PUSHABLE, const f32& mass = 1.0f);
	CPhysXEntity* createRigidMesh(const core::stringc& name, scene::ISceneNode* scenenode, const irr::scene::IMesh* mesh, const core::stringc& cookTriangleMeshFileName, E_CHARACTER_COLLISION_GROUP group = ECCG_COLLIDABLE_PUSHABLE);
	CPhysXEntity* createRigidTerrain(const core::stringc& name, scene::ITerrainSceneNode* scenenode, const core::stringc& cookTriangleMeshFileName, E_CHARACTER_COLLISION_GROUP group = ECCG_COLLIDABLE_PUSHABLE);
	bool createBodySphericalJoint(CPhysXEntity* a0, CPhysXEntity* a1, const core::vector3df& globalAnchor, const core::vector3df& globalAxis, bool isBreakable = false, f32 maxForce = 100.0f, f32 maxTorque = 100.0f);
	bool createRevoluteJoint(CPhysXEntity* a0, CPhysXEntity* a1, const core::vector3df& globalAnchor, const core::vector3df& globalAxis, bool isBreakable = false, f32 maxForce = 100.0f, f32 maxTorque = 100.0f);
	CPhysXCharacterControl* createCharacterControl(const core::stringc& name, scene::ISceneNode* scenenode, const f32& height, const f32& radius, const f32& slopeLimit = 0.0f);
	
	CPhysXEntity* getPhysicsEntity(const core::stringc& name) const;
	bool destroyPhysisEntity(const core::stringc& name) const;
	s32 getPhysicsEntityCount();
	CPhysXCharacterControl* getCharacterControl(const core::stringc& name) const;
	bool destroyCharacterControl(const core::stringc& name) const;
	s32 getCharactersCountrolCount();
	static core::stringc generateUniqueName();
	
private:
	static NxReal updateTime();
	void updateCharacter(NxReal deltaTime);
	void postUpdateCharacter();
	void getPhysicsResults();
	void getTriangleMesh(const scene::IMesh* mesh, const core::vector3df& scale, core::array<NxVec3>& vertices, core::array<NxU32>& indices);
	void getTriangleMeshBuffer(scene::SMeshBufferLightMap* meshBuffer, const core::vector3df& scale, core::array<NxVec3>& vertices, core::array<NxU32>& indices);
	NxVec3 DefaultGravity;
	NxPhysicsSDK* PhysicsSDK;
	NxControllerManager* ControllerManager;
	NxScene* Scene;
	core::map<core::stringc, CPhysXEntity*> PhysXEntityMap;
	core::map<core::stringc, CPhysXCharacterControl*> PhysXCharacterControlMap;
	io::IFileSystem* FileSystem;
	ILogger* Logger;
};
//-----------------------------------------------------------------------------

#endif
