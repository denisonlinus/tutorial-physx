//-----------------------------------------------------------------------------
// (C) Copyright 2007-2014 Denison Linus
//
// Author: Denison Linus
// Email: dlmtavar@gmail.com
//
//-----------------------------------------------------------------------------
#include "CPhysXCharacterControl.h"

//-----------------------------------------------------------------------------
#define COLLIDABLE_MASK	(1<<ECCG_COLLIDABLE_NON_PUSHABLE)// | (1<<ECCG_COLLIDABLE_PUSHABLE)
//-----------------------------------------------------------------------------
CPhysXCharacterControl::CPhysXCharacterControl(scene::ISceneNode* scenenode, NxCapsuleController* controller, NxVec3 gravity) : Controller(controller), SceneNode(scenenode), Gravity(gravity), MoveDirection(0,0,0), CharacterSpeed(1), PushCharacter(false), Jump(false), StepOffset(0), EyeHeight(0,0,0), SlopeLimit(0.0f)
{
	core::vector3df p = SceneNode->getPosition();
	StartPosition.set(p.X, p.Y, p.Z);
#ifdef _DEBUG
	setDebugName("CPhysXCharacterControl");
#endif
}
//-----------------------------------------------------------------------------
CPhysXCharacterControl::~CPhysXCharacterControl()
{
}
//-----------------------------------------------------------------------------
void CPhysXCharacterControl::update(f32 elapsedTime)
{
	NxActor* actor = Controller->getActor();
	if(Controller && actor && !actor->isSleeping())
	{
		NxMat34 pose = actor->getGlobalPose();
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
		SceneNode->setPosition(irrMat.getTranslation() + EyeHeight);
		//SceneNode->setRotation(irrMat.getRotationDegrees());
	}

	NxVec3 disp = Gravity;
	if(PushCharacter)
	{
		NxVec3 horizontalDisp = MoveDirection;
		horizontalDisp.y = 0.0f;
		horizontalDisp.normalize();

		disp += horizontalDisp * CharacterSpeed;

		PushCharacter = false;
	}

	NxF32 sharpness = 1.0f;
	NxVec3 dispVector = disp;
	NxF32 heightDelta = getHeight(elapsedTime);

	NxU32 collisionFlags;
	NxVec3 d = dispVector*elapsedTime;
	if(heightDelta!=0.0f)
		d.y+=heightDelta;

	//d.y = 0.0f;

	Controller->move(d, COLLIDABLE_MASK, 0.000001f, collisionFlags, sharpness);
	if(collisionFlags & NXCC_COLLISION_DOWN)
		stopJump();
}
//-----------------------------------------------------------------------------
core::vector3df CPhysXCharacterControl::getCharacterPosition()
{
	if(!Controller) return core::vector3df(0,0,0);
	NxExtendedVec3 p;
	p = Controller->getPosition();
	return core::vector3df(p.x,p.y,p.z);
}
//-----------------------------------------------------------------------------
scene::ISceneNode* CPhysXCharacterControl::getSceneNode() const
{
	return SceneNode;
}
//-----------------------------------------------------------------------------
NxController* CPhysXCharacterControl::getController() const
{
	return Controller;
}
//-----------------------------------------------------------------------------
bool CPhysXCharacterControl::resetCharacterPosition()
{
	return Controller->setPosition(StartPosition);
}
//-----------------------------------------------------------------------------
void CPhysXCharacterControl::jump(f32 v)
{
	startJump(v);
}
//-----------------------------------------------------------------------------
void CPhysXCharacterControl::startJump(f32 v0)
{
	if(Jump)
		return;
	JumpTime = 0.0f;
	V0	= v0 * 2.0f;
	Jump = true;
}
//-----------------------------------------------------------------------------
void CPhysXCharacterControl::stopJump()
{
	if(!Jump)
		return;
	Jump = false;
}
//-----------------------------------------------------------------------------
NxF32 CPhysXCharacterControl::getHeight(NxF32 elapsedTime)
{
	if(!Jump)
		return 0.0f;
	NxF32 G = Gravity.y * 1.5f;
	JumpTime += elapsedTime;
	NxF32 h = G * JumpTime * JumpTime + V0 * JumpTime;
	return (h - G) * elapsedTime;
}
//-----------------------------------------------------------------------------
void CPhysXCharacterControl::move(core::vector3df direction)
{
	PushCharacter = true;
	MoveDirection = NxVec3(direction.X, direction.Y, direction.Z);
}
//-----------------------------------------------------------------------------
void CPhysXCharacterControl::setCharacterSpeed(f32 speed)
{
	CharacterSpeed = speed;
}
//-----------------------------------------------------------------------------
f32 CPhysXCharacterControl::getCharacterSpeed()
{
	return CharacterSpeed;
}
//-----------------------------------------------------------------------------
void CPhysXCharacterControl::setAutoStepping(f32 stepOffset)
{
	StepOffset = stepOffset;
	Controller->setStepOffset(StepOffset);
}
//-----------------------------------------------------------------------------
f32 CPhysXCharacterControl::getAutoStepping()
{
	return StepOffset;
}
//-----------------------------------------------------------------------------
void CPhysXCharacterControl::setEyeHeight(core::vector3df eye)
{
	EyeHeight = eye;
}
//-----------------------------------------------------------------------------
core::vector3df CPhysXCharacterControl::getEyeHeight()
{
	return EyeHeight;
}
//-----------------------------------------------------------------------------
