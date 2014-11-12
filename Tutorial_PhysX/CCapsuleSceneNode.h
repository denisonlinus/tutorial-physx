//-----------------------------------------------------------------------------
// (C) Copyright 2007-2014 Denison Linus
//
// Author: Denison Linus
// Email: dlmtavar@gmail.com
//
//-----------------------------------------------------------------------------
#ifndef _CCAPSULESCENENODE_H_
#define _CCAPSULESCENENODE_H_

#include "ISceneNode.h"
#include "SMeshBuffer.h"

namespace irr
{
namespace scene
{
	class CCapsuleSceneNode : public ISceneNode
	{
	public:

		//! constructor
		CCapsuleSceneNode(f32 height, f32 radius, ISceneNode* parent, ISceneManager* mgr, s32 id=-1,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

		//! destructor
		virtual ~CCapsuleSceneNode();

		virtual void OnRegisterSceneNode();

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

		//! returns the material based on the zero based index i. To get the amount
		//! of materials used by this scene node, use getMaterialCount().
		//! This function is needed for inserting the node into the scene hirachy on a
		//! optimal position for minimizing renderstate changes, but can also be used
		//! to directly modify the material of a scene node.
		virtual video::SMaterial& getMaterial(u32 i);

		//! returns amount of materials used by this scene node.
		virtual u32 getMaterialCount();

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0);

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

	private:

		void setHeightAndRadius();
		SMeshBuffer Buffer[3];
		core::aabbox3df BBox;
		f32 Height;
		f32 Radius;
	};

} // end namespace scene
} // end namespace irr

#endif

