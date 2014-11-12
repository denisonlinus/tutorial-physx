//-----------------------------------------------------------------------------
// (C) Copyright 2007-2014 Denison Linus
//
// Author: Denison Linus
// Email: dlmtavar@gmail.com
//
//-----------------------------------------------------------------------------
#include "CPhysXEntity.h"

//-----------------------------------------------------------------------------
CPhysXEntity::CPhysXEntity(scene::ISceneNode* scenenode, NxActor* actor, E_CHARACTER_COLLISION_GROUP group) : SceneNode(scenenode), Actor(actor)
{
	setCharacterCollisionGroup(group);
#ifdef _DEBUG
	setDebugName("CPhysXEntity");
#endif
}
//-----------------------------------------------------------------------------
CPhysXEntity::~CPhysXEntity()
{
}
//-----------------------------------------------------------------------------
scene::ISceneNode* CPhysXEntity::getSceneNode() const
{
	return SceneNode;
}
//-----------------------------------------------------------------------------
void CPhysXEntity::update()
{
	//bool b = Actor->isSleeping();
	//_IRR_DEBUG_BREAK_IF(b == true);

	if(Actor && !Actor->isSleeping())
	{
		NxMat34 pose = Actor->getGlobalPose();
		const NxVec3 pos = pose.t;
		const NxMat33 orient = pose.M;
		core::matrix4 irrMat;
		irr::f32* M = irrMat.pointer();
		orient.getColumnMajorStride4(&M[0]);
		pos.get(&M[12]);
		//clear the elements we don't need:
		M[3] = M[7] = M[11] = 0.0f;
		M[15] = 1.0f;
		// with that newly made matrix, let's use it to transform/rotate the node
		SceneNode->setPosition(irrMat.getTranslation());
		SceneNode->setRotation(irrMat.getRotationDegrees());
	}
}
//-----------------------------------------------------------------------------
NxActor* CPhysXEntity::getActor() const
{
	return Actor;
}
//-----------------------------------------------------------------------------
core::vector3df CPhysXEntity::applyForce(const core::vector3df& forceDir, const f32& forceStrength)
{ 
	core::vector3df tmp = (forceStrength * forceDir);
	NxVec3 forceVec = NxVec3(tmp.X, tmp.Y, tmp.Z);
	Actor->addForce(forceVec);
	return core::vector3df(forceVec.x, forceVec.y, forceVec.z);
}
//-----------------------------------------------------------------------------
void CPhysXEntity::setDynamic(bool isDynamic)
{
	if(isDynamic)
		Actor->clearBodyFlag(NX_BF_KINEMATIC);
	else
		Actor->raiseBodyFlag(NX_BF_KINEMATIC);
}
//-----------------------------------------------------------------------------
bool CPhysXEntity::isDynamic()
{
	return Actor->readBodyFlag(NX_BF_KINEMATIC);
}
//-----------------------------------------------------------------------------
void CPhysXEntity::setCharacterCollisionGroup(E_CHARACTER_COLLISION_GROUP group)
{
    NxShape*const* shapes = Actor->getShapes();
    NxU32 nShapes = Actor->getNbShapes();
    while(nShapes--)
    {
        shapes[nShapes]->setGroup(group);
	}
	CharacterCollisionGroup = group;
	if(Actor->isDynamic() )
		Actor->wakeUp(1);
}
//-----------------------------------------------------------------------------
E_CHARACTER_COLLISION_GROUP CPhysXEntity::getCharacterCollisionGroup()
{
	return CharacterCollisionGroup;
}