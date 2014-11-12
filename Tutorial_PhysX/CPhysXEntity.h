#ifndef _CPHYSXENTITY_H_
#define _CPHYSXENTITY_H_

#include "IUnknown.h"
#include "ISceneNode.h"
#include "NxPhysics.h"
#include "NxCooking.h"
#include "ECharacterCollisionGroup.h"
using namespace irr;

//-----------------------------------------------------------------------------
class CPhysXEntity : public virtual irr::IUnknown
{
public:
	CPhysXEntity(scene::ISceneNode* scenenode, NxActor* actor, E_CHARACTER_COLLISION_GROUP group);
	~CPhysXEntity();
	void update();
	core::vector3df applyForce(const core::vector3df& forceDir, const f32& forceStrength);
	void setDynamic(bool isDynamic);
	bool isDynamic();
	scene::ISceneNode* getSceneNode() const;
	NxActor* getActor() const;
	void setCharacterCollisionGroup(E_CHARACTER_COLLISION_GROUP group);
	E_CHARACTER_COLLISION_GROUP getCharacterCollisionGroup();
private:
	NxActor* Actor;
	scene::ISceneNode* SceneNode;
	E_CHARACTER_COLLISION_GROUP CharacterCollisionGroup;
};
//-----------------------------------------------------------------------------

#endif
