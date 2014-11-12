#ifndef _CPHYSXCHARACTERCONTROL_H_
#define _CPHYSXCHARACTERCONTROL_H_

#include "IUnknown.h"
#include "ISceneNode.h"
#include "NxPhysics.h"
#include "NxExtended.h"
#include "NxCapsuleController.h"
#include "ECharacterCollisionGroup.h"
using namespace irr;

//-----------------------------------------------------------------------------
class CPhysXCharacterControl : public virtual irr::IUnknown
{
public:
	CPhysXCharacterControl(scene::ISceneNode* scenenode, NxCapsuleController* controller, NxVec3 gravity);
	~CPhysXCharacterControl();
	void update(f32 elapsedTime);
	core::vector3df getCharacterPosition();
	bool resetCharacterPosition();
	scene::ISceneNode* getSceneNode() const;
	NxController* getController() const;
	void jump(f32 initialvelocity);
	void move(core::vector3df direction);
	void setCharacterSpeed(f32 speed);
	f32 getCharacterSpeed();
	void setAutoStepping(f32 stepOffset);
	f32 getAutoStepping();
	void setEyeHeight(core::vector3df eye);
	core::vector3df getEyeHeight();
	
private:
	NxCapsuleController* Controller;
	scene::ISceneNode* SceneNode;
	NxExtendedVec3 StartPosition;
	void startJump(f32 v0);
	void stopJump();
	NxF32 getHeight(NxF32 elapsedTime);
	bool Jump;
	NxF32 V0;
	NxF32 JumpTime;
	NxVec3 Gravity;
	NxVec3 MoveDirection;
	NxF32 CharacterSpeed;
	bool PushCharacter;
	f32 StepOffset;
	f32 SlopeLimit;
	core::vector3df EyeHeight;
};
//-----------------------------------------------------------------------------

#endif
