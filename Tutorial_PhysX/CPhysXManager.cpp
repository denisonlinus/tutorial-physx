//-----------------------------------------------------------------------------
// (C) Copyright 2007-2014 Denison Linus
//
// Author: Denison Linus
// Email: dlmtavar@gmail.com
//
//-----------------------------------------------------------------------------
#if defined(WIN32) || defined(WIN64)
	#define NOMINMAX
	#include <windows.h>
#endif
#include "IMeshBuffer.h"
#include "CPhysXManager.h"
#include "CPhysXStream.h"
#include "CPhysXErrorStream.h"
#include "NxUserAllocator.h"
#include "NxController.h"

//-----------------------------------------------------------------------------
class ControllerManagerAllocator : public NxUserAllocator
{
public:
	virtual void*	mallocDEBUG(size_t size, const char* fileName, int line)    { return ::malloc(size); }
	virtual void*	malloc(size_t size)                                         { return ::malloc(size); }
	virtual void*	realloc(void* memory, size_t size)                          { return ::realloc(memory, size); }
	virtual void	free(void* memory)                                          { ::free(memory); }
};
//-----------------------------------------------------------------------------
class ControllerHitReport : public NxUserControllerHitReport
{
public:
	virtual NxControllerAction  onShapeHit(const NxControllerShapeHit& hit)
	{
		return NX_ACTION_NONE;

		if(1 && hit.shape)
		{
			NxCollisionGroup group = hit.shape->getGroup();
			if(group == ECCG_NON_COLLIDABLE)
				return NX_ACTION_NONE;

			if(group != ECCG_COLLIDABLE_NON_PUSHABLE)
			{
				NxActor& actor = hit.shape->getActor();
				if(actor.isDynamic())
				{
					// We only allow horizontal pushes. Vertical pushes when we stand on dynamic objects creates
					// useless stress on the solver. It would be possible to enable/disable vertical pushes on
					// particular objects, if the gameplay requires it.
					if(hit.dir.y==0.0f)
					{
						NxF32 coeff = actor.getMass() * hit.length * 10.0f;
						actor.addForceAtLocalPos(hit.dir*coeff, NxVec3(0,0,0), NX_IMPULSE);
		//						actor.addForceAtPos(hit.dir*coeff, hit.controller->getPosition(), NX_IMPULSE);
		//						actor.addForceAtPos(hit.dir*coeff, hit.worldPos, NX_IMPULSE);
					}
				}
			}
		}

		return NX_ACTION_NONE;
	}

	virtual NxControllerAction  onControllerHit(const NxControllersHit& hit)
	{
		return NX_ACTION_NONE;
	}

};
//-----------------------------------------------------------------------------
static ControllerHitReport gControllerHitReport;
//-----------------------------------------------------------------------------
CPhysXManager::CPhysXManager(io::IFileSystem* fs, ILogger* logger, const core::vector3df& gravity) : DefaultGravity(gravity.X, gravity.Y, gravity.Z), PhysicsSDK(0), Scene(0), FileSystem(fs), Logger(logger), ControllerManager(0)
{
#ifdef _DEBUG
	setDebugName("CPhysXManager");
#endif
	initialize();
}
//-----------------------------------------------------------------------------
CPhysXManager::~CPhysXManager()
{
	finalize();
}
//-----------------------------------------------------------------------------
bool CPhysXManager::initialize()
{

	NxUserAllocator* mAllocator = new ControllerManagerAllocator();

	// Create the physics SDK
	NxPhysicsSDKDesc desc;
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
	PhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, new ErrorStream(Logger), desc, &errorCode);

	if(!PhysicsSDK)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::initialize: null NxPhysicsSDK detected!");
		return false;
	}

	// Set the physics parameters
	PhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.0f); // usually 0.02

	// Set the debug visualization parameters
	PhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	PhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	PhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);
	PhysicsSDK->setParameter(NX_VISUALIZE_JOINT_LIMITS, 1);

	// Create the scene
	NxSceneDesc sceneDesc;
	sceneDesc.gravity               = DefaultGravity;
	//sceneDesc.broadPhase            = NX_BROADPHASE_COHERENT;
	//sceneDesc.collisionDetection    = true;
	Scene = PhysicsSDK->createScene(sceneDesc);
	if(!Scene)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::initialize: null NxScene detected!");
		return false;
	}
	// Create the default material
	NxMaterial* defaultMaterial = Scene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.125f); //! 0.5
	defaultMaterial->setStaticFriction(0.5f); //! 0.5
	defaultMaterial->setDynamicFriction(0.5f); //! 0.5


	ControllerManager = NxCreateControllerManager(mAllocator);
	if(!ControllerManager)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::initialize: null NxControllerManager detected!");
		return false;
	}

	// Get the current time
	updateTime();

	// Start the first frame of the simulation
	//update();

	/*core::map<core::stringc, CPhysXEntity*>::Iterator i = PhysXEntityMap.getIterator();
	while(!i.atEnd())
	{
		i->getValue()->update();
		i++;
	}*/

	return true;
}
//-----------------------------------------------------------------------------
void CPhysXManager::finalize()
{
	core::map<core::stringc, CPhysXEntity*>::Iterator i = PhysXEntityMap.getIterator();
	while(!i.atEnd())
	{
		Scene->releaseActor(*i->getValue()->getActor());
		i->getValue()->drop();
		i++;
	}
	PhysXEntityMap.clear();

	core::map<core::stringc, CPhysXCharacterControl*>::Iterator j = PhysXCharacterControlMap.getIterator();
	while(!i.atEnd())
	{
		ControllerManager->releaseController(*j->getValue()->getController());
		j->getValue()->drop();
		j++;
	}
	PhysXCharacterControlMap.clear();

	if(Scene)
	{
		PhysicsSDK->releaseScene(*Scene);
	}
	if (PhysicsSDK)
	{
		PhysicsSDK->release();
	}
}
//-----------------------------------------------------------------------------
void CPhysXManager::update()
{
	if(Scene)
	{
		// Update the time step
		NxReal deltaTime = updateTime();

		updateCharacter(deltaTime);

		// Start collision and dynamics for delta time since the last frame
		Scene->simulate(deltaTime * 5.0f); // i multiply by five for more faster, realistic looking dynamics
		Scene->flushStream();
		getPhysicsResults();
		core::map<core::stringc, CPhysXEntity*>::Iterator i = PhysXEntityMap.getIterator();
		while(!i.atEnd())
		{
			i->getValue()->update();
			i++;
		}

		postUpdateCharacter();

	}
}
//-----------------------------------------------------------------------------
void CPhysXManager::getPhysicsResults()
{
	//blocking 
	Scene->fetchResults(NX_RIGID_BODY_FINISHED, true);

	//non-blocking
	/*
	while(!Scene->checkResults(NX_RIGID_BODY_FINISHED, false))
	{
		// do something useful
	}
	*/
	//non-blocking
	/*
	Scene->fetchResults(NX_RIGID_BODY_FINISHED, true);
	while(!Scene->fetchResults(NX_RIGID_BODY_FINISHED, false)
	{
		// do something useful
	}
	*/
}
//-----------------------------------------------------------------------------
NxReal CPhysXManager::updateTime()
{
    NxReal deltaTime;
#if defined(WIN32) || defined(WIN64)
    static __int64 gTime,gLastTime;
    __int64 freq;
    QueryPerformanceCounter((LARGE_INTEGER *)&gTime);  // Get current count
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq); // Get processor freq
    deltaTime = (double)(gTime - gLastTime)/(double)freq;
#else
    static clock_t gTime, gLastTime;
    gTime = clock();
    deltaTime = (NxReal)((double)(gTime - gLastTime) / 1000000.0f);
#endif
    gLastTime = gTime;
    return deltaTime;
}
//-----------------------------------------------------------------------------
CPhysXEntity* CPhysXManager::createRigidBox(const core::stringc& name, scene::ISceneNode* scenenode, const core::vector3df& dimmension, const bool& isDynamic, E_CHARACTER_COLLISION_GROUP group, const f32& mass)
{
	if(!scenenode)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::createRigidBox: null irr::scene::ISceneNode detected!");
		return 0;
	}

	const core::vector3df& position = scenenode->getPosition();
	const core::vector3df& rotation = scenenode->getRotation();

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	bodyDesc.angularDamping = 0.5f;

	NxBoxShapeDesc boxDesc;
	boxDesc.mass = mass;
	boxDesc.density = 1.0f;
	NxVec3 Dim(dimmension.X * 0.5f,dimmension.Y * 0.5f, dimmension.Z * 0.5f);
	boxDesc.dimensions.set(Dim);
	actorDesc.shapes.pushBack(&boxDesc);
	if(isDynamic)
	{
		actorDesc.body = &bodyDesc;
	}
	else
	{
		actorDesc.body = NULL;
	}
	actorDesc.density = 1.0f;
	actorDesc.globalPose.t = NxVec3(position.X,position.Y,position.Z);
	core::matrix4 irrMat;
	irrMat.setRotationDegrees(rotation);
	NxMat33 m;
	m.setColumnMajorStride4(&irrMat.pointer()[0]);
	actorDesc.globalPose.M = m;
	NxActor* actor = Scene->createActor(actorDesc);
	if(!actor)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::createRigidBox: null NxActor detected!");
		return 0;
	}

	CPhysXEntity* pe = new CPhysXEntity(scenenode, actor, group);
	PhysXEntityMap[name] = pe;
	return pe;

}
//-----------------------------------------------------------------------------
CPhysXEntity* CPhysXManager::createRigidSphere(const core::stringc& name, scene::ISceneNode* scenenode, const f32& radius, const bool& isDynamic, E_CHARACTER_COLLISION_GROUP group, const f32& mass)
{
	if(!scenenode)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::createRigidSphere: null irr::scene::ISceneNode detected!");
		return 0;
	}

	const core::vector3df& position = scenenode->getPosition();
	const core::vector3df& rotation = scenenode->getRotation();

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	bodyDesc.angularDamping = 0.5f;

	NxSphereShapeDesc sphereDesc;
	sphereDesc.mass = mass;
	sphereDesc.density = 1.0f;
	sphereDesc.radius = radius; // should be 0.5
	actorDesc.shapes.pushBack(&sphereDesc);
	if(isDynamic)
	{
		actorDesc.body = &bodyDesc;
	}
	else
	{
		actorDesc.body = NULL;
	}
	actorDesc.density = 1.0f;
	actorDesc.globalPose.t = NxVec3(position.X,position.Y,position.Z);
	core::matrix4 irrMat;
	irrMat.setRotationDegrees(rotation);
	NxMat33 m;
	m.setColumnMajorStride4(&irrMat.pointer()[0]);
	actorDesc.globalPose.M = m;
	NxActor* actor = Scene->createActor(actorDesc);
	if(!actor)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::createRigidSphere: null NxActor detected!");
		return 0;
	}

	CPhysXEntity* pe = new CPhysXEntity(scenenode, actor, group);
	PhysXEntityMap[name] = pe;
	return pe;

}
//-----------------------------------------------------------------------------
CPhysXEntity* CPhysXManager::createRigidCapsule(const core::stringc& name, scene::ISceneNode* scenenode, const f32& height, const f32& radius, const bool& isDynamic, E_CHARACTER_COLLISION_GROUP group, const f32& mass)
{
	if(!scenenode)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::createRigidCapsule: null irr::scene::ISceneNode detected!");
		return 0;
	}

	const core::vector3df& position = scenenode->getPosition();
	const core::vector3df& rotation = scenenode->getRotation();

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	bodyDesc.angularDamping = 0.5f;
	bodyDesc.linearDamping = 0.5f;

	NxCapsuleShapeDesc capsuleDesc;
	capsuleDesc.height = height;
	capsuleDesc.radius = radius;
	capsuleDesc.mass = mass;
	capsuleDesc.density = 1.0f;
	
	//capsuleDesc.localPose.t = NxVec3(0,radius+0.5*height,0);

	actorDesc.shapes.pushBack(&capsuleDesc);
	if(isDynamic)
	{
		actorDesc.body = &bodyDesc;
	}
	else
	{
		actorDesc.body = NULL;
	}
	actorDesc.density = 1.0f;

	actorDesc.globalPose.t = NxVec3(position.X,position.Y,position.Z);
	core::matrix4 irrMat;
	irrMat.setRotationDegrees(rotation);
	NxMat33 m;
	m.setColumnMajorStride4(&irrMat.pointer()[0]);
	actorDesc.globalPose.M = m;
	NxActor* actor = Scene->createActor(actorDesc);
	if(!actor)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::createRigidCapsule: null NxActor detected!");
		return 0;
	}

	CPhysXEntity* pe = new CPhysXEntity(scenenode, actor, group);
	PhysXEntityMap[name] = pe;
	return pe;
}
//-----------------------------------------------------------------------------
void CPhysXManager::getTriangleMeshBuffer(scene::SMeshBufferLightMap* mb, const core::vector3df& scale, core::array<NxVec3>& vertices, core::array<NxU32>& indices)
{
	if(mb->getVertexType() == irr::video::EVT_STANDARD)
	{
		irr::video::S3DVertex* mbVertices = (irr::video::S3DVertex*)mb->getVertices();
		irr::u16* mbIndices = mb->getIndices();
		irr::s32 numVertices = mb->getVertexCount();
		irr::s32 numIndices = mb->getIndexCount();

		for (int j = 0; j < numVertices; ++j)
			vertices.push_back(NxVec3(mbVertices[j].Pos.X * scale.X, mbVertices[j].Pos.Y * scale.Y, mbVertices[j].Pos.Z * scale.Z));
		for (int j = 0; j < numIndices; ++j)
			indices.push_back(NxU32(mbIndices[j]));
	}
	else
	{
		if(mb->getVertexType() == irr::video::EVT_2TCOORDS)
		{
			irr::video::S3DVertex2TCoords* mbVertices = (irr::video::S3DVertex2TCoords*)mb->getVertices();
			irr::u16* mbIndices = mb->getIndices();
			irr::s32 numVertices = mb->getVertexCount();
			irr::s32 numIndices = mb->getIndexCount();

			for (int j = 0; j < numVertices; ++j)
				vertices.push_back(NxVec3(mbVertices[j].Pos.X * scale.X, mbVertices[j].Pos.Y * scale.Y, mbVertices[j].Pos.Z * scale.Z));
			for (int j = 0; j < numIndices; ++j)
				indices.push_back(NxU32(mbIndices[j]));
		}
	}
}
//-----------------------------------------------------------------------------
void CPhysXManager::getTriangleMesh(const scene::IMesh* pMesh, const core::vector3df& scale, core::array<NxVec3>& vertices, core::array<NxU32>& indices)
{
	NxU32 tempIndexCount = 0;
	irr::u32 bufferCount = pMesh->getMeshBufferCount();
	for(int i = 0; i < bufferCount; i++)
	{
		irr::scene::IMeshBuffer* mb = pMesh->getMeshBuffer(i);
		if(mb->getVertexType() == irr::video::EVT_STANDARD)
		{
			irr::video::S3DVertex* mbVertices = (irr::video::S3DVertex*)mb->getVertices();
			irr::u16* mbIndices = mb->getIndices();
			irr::s32 numVertices = mb->getVertexCount();
			irr::s32 numIndices = mb->getIndexCount();

			for (int j = 0; j < numVertices; ++j)
				vertices.push_back(NxVec3(mbVertices[j].Pos.X * scale.X, mbVertices[j].Pos.Y * scale.Y, mbVertices[j].Pos.Z * scale.Z));
			for (int j = 0; j < numIndices; ++j)
				indices.push_back(NxU32(mbIndices[j]) + tempIndexCount);
			tempIndexCount += numVertices;
		}
		else
		{
			if(mb->getVertexType() == irr::video::EVT_2TCOORDS)
			{
				irr::video::S3DVertex2TCoords* mbVertices = (irr::video::S3DVertex2TCoords*)mb->getVertices();
				irr::u16* mbIndices = mb->getIndices();
				irr::s32 numVertices = mb->getVertexCount();
				irr::s32 numIndices = mb->getIndexCount();

				for (int j = 0; j < numVertices; ++j)
					vertices.push_back(NxVec3(mbVertices[j].Pos.X * scale.X, mbVertices[j].Pos.Y * scale.Y, mbVertices[j].Pos.Z * scale.Z));
				for (int j = 0; j < numIndices; ++j)
					indices.push_back(NxU32(mbIndices[j]) + tempIndexCount);
				tempIndexCount += numVertices;
			}
		}
	}
}
//-----------------------------------------------------------------------------
CPhysXEntity* CPhysXManager::createRigidMesh(const core::stringc& name, scene::ISceneNode* scenenode, const scene::IMesh* mesh, const core::stringc& cookTriangleMeshFileName, E_CHARACTER_COLLISION_GROUP group)
{
	if(!scenenode)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::createRigidMesh: null irr::scene::ISceneNode detected!");
		return 0;
	}
	if(!mesh)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::createRigidMesh: null irr::scene::IMesh detected!");
		return 0;
	}

	const core::vector3df& position = scenenode->getPosition();
	const core::vector3df& rotation = scenenode->getRotation();
	const core::vector3df& scale = scenenode->getScale();

	core::array<NxVec3> vertices;
	core::array<NxU32> indices;

	getTriangleMesh(mesh, scale, vertices, indices);

	NxTriangleMeshDesc mapMeshDesc;

	// Build physical model
	mapMeshDesc.numVertices = vertices.size();    
	mapMeshDesc.numTriangles = indices.size() / 3;
	mapMeshDesc.pointStrideBytes = sizeof(NxVec3);   
	mapMeshDesc.triangleStrideBytes = 3*sizeof(NxU32);    
	mapMeshDesc.points = vertices.const_pointer();    
	mapMeshDesc.triangles = indices.const_pointer();    
	mapMeshDesc.flags = 0;

	NxInitCooking();
	if(!FileSystem->existFile(cookTriangleMeshFileName.c_str()))
	{
		Logger->log("CPhysXManager Warning: CPhysXManager::createRigidMesh: Cooking The Triangle Mesh Data To Perform Efficient Collision Detection. Please wait...");
		NxCookTriangleMesh(mapMeshDesc, UserStream(cookTriangleMeshFileName.c_str(), false));
		Logger->log("CPhysXManager Warning: CPhysXManager::createRigidMesh: Cooking The Triangle Mesh Data To Perform Efficient Collision Detection: OK.");
	}
	NxTriangleMesh* TriangleMesh = PhysicsSDK->createTriangleMesh(UserStream(cookTriangleMeshFileName.c_str(), true));
	NxCloseCooking();

	NxTriangleMeshShapeDesc MeshShapeDesc;
	MeshShapeDesc.meshData = TriangleMesh;
	MeshShapeDesc.density = 1.0f;
	//MeshShapeDesc.mass = mass;

	NxBodyDesc bodyDesc;
	bodyDesc.angularDamping = 0.5f;
	
	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&MeshShapeDesc);

	/*
	if(isDynamic)
	{
		actorDesc.body= &bodyDesc;
		actorDesc.density= 1.0f;
	}
	else
	{
		actorDesc.body = NULL;
	}
	*/

	actorDesc.globalPose.t = NxVec3(position.X, position.Y, position.Z);
	core::matrix4 irrMat;
	irrMat.setRotationDegrees(rotation);
	NxMat33 m;
	m.setColumnMajorStride4(&irrMat.pointer()[0]);
	actorDesc.globalPose.M = m;
	indices.clear();
	vertices.clear();

	NxActor* actor = Scene->createActor(actorDesc);
	if(!actor)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::createRigidMesh: null NxActor detected!");
		return 0;
	}
	CPhysXEntity* pe = new CPhysXEntity(scenenode, actor, group);
	PhysXEntityMap[name] = pe;
	return pe;
}
//-----------------------------------------------------------------------------
CPhysXEntity* CPhysXManager::createRigidTerrain(const core::stringc& name, scene::ITerrainSceneNode* scenenode, const core::stringc& cookTriangleMeshFileName, E_CHARACTER_COLLISION_GROUP group)
{
	if(!scenenode)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::createRigidTerrain: null irr::scene::ISceneNode detected!");
		return 0;
	}

	const core::vector3df& position = scenenode->getPosition();
	const core::vector3df& rotation = scenenode->getRotation();
	const core::vector3df& scale = scenenode->getScale();

	core::array<NxVec3> vertices;
	core::array<NxU32> indices;

	scene::SMeshBufferLightMap mb;
	scenenode->getMeshBufferForLOD(mb, 0);
	getTriangleMeshBuffer(&mb, scale, vertices, indices);

	NxTriangleMeshDesc mapMeshDesc;

	// Build physical model
	mapMeshDesc.numVertices = vertices.size();    
	mapMeshDesc.numTriangles = indices.size() / 3;
	mapMeshDesc.pointStrideBytes = sizeof(NxVec3);   
	mapMeshDesc.triangleStrideBytes = 3*sizeof(NxU32);    
	mapMeshDesc.points = vertices.const_pointer();    
	mapMeshDesc.triangles = indices.const_pointer();    
	mapMeshDesc.flags = 0;

	NxInitCooking();
	if(!FileSystem->existFile(cookTriangleMeshFileName.c_str()))
	{
		Logger->log("CPhysXManager Warning: CPhysXManager::createRigidTerrain: Cooking The Triangle Mesh Data To Perform Efficient Collision Detection: Please wait...");
		NxCookTriangleMesh(mapMeshDesc, UserStream(cookTriangleMeshFileName.c_str(), false));
		Logger->log("CPhysXManager Warning: CPhysXManager::createRigidTerrain: Cooking The Triangle Mesh Data To Perform Efficient Collision Detection: OK.");
	}
	NxTriangleMesh* TriangleMesh = PhysicsSDK->createTriangleMesh(UserStream(cookTriangleMeshFileName.c_str(), true));
	/*
	MemoryWriteBuffer buf;
	NxCookTriangleMesh(mapMeshDesc, buf);
	MemoryReadBuffer readBuffer(buf.data);
	NxTriangleMesh* TriangleMesh = PhysicsSDK->createTriangleMesh(readBuffer);
	*/
	NxCloseCooking();

	NxTriangleMeshShapeDesc MeshShapeDesc;
	MeshShapeDesc.meshData = TriangleMesh;
	MeshShapeDesc.density = 1.0f;

	NxBodyDesc bodyDesc;
	bodyDesc.angularDamping = 0.5f;
	
	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&MeshShapeDesc);

	actorDesc.globalPose.t = NxVec3(position.X, position.Y, position.Z);
	core::matrix4 irrMat;
	irrMat.setRotationDegrees(rotation);
	NxMat33 m;
	m.setColumnMajorStride4(&irrMat.pointer()[0]);
	actorDesc.globalPose.M = m;
	indices.clear();
	vertices.clear();

	NxActor* actor = Scene->createActor(actorDesc);
	if(!actor)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::createRigidTerrain: null NxActor detected!");
		return 0;
	}
	CPhysXEntity* pe = new CPhysXEntity(scenenode, actor, group);
	PhysXEntityMap[name] = pe;
	return pe;
}
//-----------------------------------------------------------------------------
CPhysXEntity* CPhysXManager::getPhysicsEntity(const core::stringc& name) const
{
	core::map<core::stringc, CPhysXEntity*>::Node* node = PhysXEntityMap.find(name);
	if(!node)
	{
		Logger->log("CPhysXManager Warning: CPhysXManager::getPhysicsEntity: CPhysXEntity not found!");
		return 0;
	}
	return node->getValue();
}
//-----------------------------------------------------------------------------
bool CPhysXManager::destroyPhysisEntity(const core::stringc& name) const
{
	CPhysXEntity* pe = dynamic_cast<CPhysXEntity*>(getPhysicsEntity(name));
	if(!pe)
	{
		Logger->log("CPhysXManager Warning: CPhysXManager::destroyPhysisEntity: CPhysXEntity not found!");
		return false;
	}
	Scene->releaseActor(*pe->getActor());

	//TEM QUE DELETAR

	return true;
}
//-----------------------------------------------------------------------------
s32 CPhysXManager::getPhysicsEntityCount()
{
	return PhysXEntityMap.size();
}
//-----------------------------------------------------------------------------
core::stringc CPhysXManager::generateUniqueName()
{
	static unsigned int objectCount = 0;
	core::stringc name = "PhysX_";
	name += core::stringc(objectCount);
	++objectCount;
	return name;
}
//-----------------------------------------------------------------------------
static NxReal maxForce = 100;
static NxReal maxTorque = 100;
//-----------------------------------------------------------------------------
bool CPhysXManager::createBodySphericalJoint(CPhysXEntity* _a0, CPhysXEntity* _a1, const core::vector3df& _globalAnchor, const core::vector3df& _globalAxis, bool isBreakable, f32 maxForce, f32 maxTorque)
{
	NxActor* a0 = dynamic_cast<CPhysXEntity*>(_a0)->getActor();
	NxActor* a1 = dynamic_cast<CPhysXEntity*>(_a1)->getActor();
	const NxVec3& globalAnchor = NxVec3(_globalAnchor.X, _globalAnchor.Y, _globalAnchor.Z);
	const NxVec3& globalAxis = NxVec3(_globalAxis.X, _globalAxis.Y, _globalAxis.Z);

	NxSphericalJointDesc sphericalDesc;
	sphericalDesc.actor[0] = a0;
	sphericalDesc.actor[1] = a1;
	sphericalDesc.setGlobalAnchor(globalAnchor);
	sphericalDesc.setGlobalAxis(globalAxis);

	sphericalDesc.flags |= NX_SJF_TWIST_LIMIT_ENABLED;
	sphericalDesc.twistLimit.low.value = -(NxReal)0.025*NxPi;
	sphericalDesc.twistLimit.low.hardness = 0.5;
	sphericalDesc.twistLimit.low.restitution = 0.5;
	sphericalDesc.twistLimit.high.value = (NxReal)0.025*NxPi;
	sphericalDesc.twistLimit.high.hardness = 0.5;
	sphericalDesc.twistLimit.high.restitution = 0.5;

	sphericalDesc.flags |= NX_SJF_SWING_LIMIT_ENABLED;
	sphericalDesc.swingLimit.value = (NxReal)0.25*NxPi;
	sphericalDesc.swingLimit.hardness = 0.5;
	sphericalDesc.swingLimit.restitution = 0.5;

	sphericalDesc.flags |= NX_SJF_TWIST_SPRING_ENABLED;
	sphericalDesc.twistSpring.spring = 0.5;
	sphericalDesc.twistSpring.damper = 1;

	sphericalDesc.flags |= NX_SJF_SWING_SPRING_ENABLED;
	sphericalDesc.swingSpring.spring = 0.5;
	sphericalDesc.swingSpring.damper = 1;

//	sphericalDesc.flags |= NX_SJF_JOINT_SPRING_ENABLED;
//	sphericalDesc.jointSpring.spring = 0.5;
//	sphericalDesc.jointSpring.damper = 1;

	sphericalDesc.projectionDistance = (NxReal)0.15;
	sphericalDesc.projectionMode = NX_JPM_POINT_MINDIST;

	NxJoint* joint;
	joint = Scene->createJoint(sphericalDesc);
	if(isBreakable)
		joint->setBreakable(maxForce,maxTorque);    	
	if(!joint)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::createBodySphericalJoint: null NxJoint detected!");
	}
	return joint;
}
//-----------------------------------------------------------------------------
bool CPhysXManager::createRevoluteJoint(CPhysXEntity* _a0, CPhysXEntity* _a1, const core::vector3df& _globalAnchor, const core::vector3df& _globalAxis, bool isBreakable, f32 maxForce, f32 maxTorque)
{
	NxActor* a0 = dynamic_cast<CPhysXEntity*>(_a0)->getActor();
	NxActor* a1 = dynamic_cast<CPhysXEntity*>(_a1)->getActor();
	const NxVec3& globalAnchor = NxVec3(_globalAnchor.X, _globalAnchor.Y, _globalAnchor.Z);
	const NxVec3& globalAxis = NxVec3(_globalAxis.X, _globalAxis.Y, _globalAxis.Z);

	NxRevoluteJointDesc revDesc;
	revDesc.actor[0] = a0;
	revDesc.actor[1] = a1;
	revDesc.setGlobalAnchor(globalAnchor);
	revDesc.setGlobalAxis(globalAxis);

	NxJoint* joint;
	joint = Scene->createJoint(revDesc);
	if(isBreakable)
		joint->setBreakable(maxForce,maxTorque);    	
	if(!joint)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::createRevoluteJoint: null NxJoint detected!");
	}
	return joint;

}
//-----------------------------------------------------------------------------
CPhysXCharacterControl* CPhysXManager::getCharacterControl(const core::stringc& name) const
{
	core::map<core::stringc, CPhysXCharacterControl*>::Node* node = PhysXCharacterControlMap.find(name);
	if(!node)
	{
		Logger->log("CPhysXManager Warning: CPhysXManager::getCharacterControl: CPhysXCharacterControl not found!");
		return 0;
	}
	return node->getValue();

}
//-----------------------------------------------------------------------------	
bool CPhysXManager::destroyCharacterControl(const core::stringc& name) const
{
	CPhysXCharacterControl* pe = dynamic_cast<CPhysXCharacterControl*>(getCharacterControl(name));
	if(!pe)
	{
		Logger->log("CPhysXManager Warning: CPhysXManager::destroyCharacterControl: CPhysXCharacterControl not found!");
		return false;
	}
	ControllerManager->releaseController(*pe->getController());
	//pe->drop();
	//PhysXCharacterControlMap.remove(name);
	return true;
}
//-----------------------------------------------------------------------------
s32 CPhysXManager::getCharactersCountrolCount()
{
	return PhysXCharacterControlMap.size();
}
//-----------------------------------------------------------------------------
CPhysXCharacterControl* CPhysXManager::createCharacterControl(const core::stringc& name, scene::ISceneNode* scenenode, const f32& height, const f32& radius, const f32& slopeLimit)
{
	const core::vector3df& pos = scenenode->getPosition();
	NxF32 scale = scenenode->getScale().X;
	static NxF32 SKINWIDTH = 0.2f;
	NxCapsuleControllerDesc desc;
	NxVec3 tmp			= NxVec3(pos.X, pos.Y, pos.Z) * scale;
	desc.position.x		= tmp.x;
	desc.position.y		= tmp.y;
	desc.position.z		= tmp.z;
	desc.radius			= radius * scale;
	desc.height			= height * scale;
	desc.upDirection	= NX_Y;
	desc.slopeLimit		= cosf(NxMath::degToRad(slopeLimit));
	desc.skinWidth		= SKINWIDTH;
	desc.stepOffset		= radius * 0.5 * scale;
	//desc.callback		= &gControllerHitReport;

	NxCapsuleController* controller = dynamic_cast<NxCapsuleController*>(ControllerManager->createController(Scene, desc));
	if(!controller)
	{
		Logger->log("CPhysXManager Invalid Operation: CPhysXManager::createCharacterControl: null NxController detected!");
		return 0;
	}
	CPhysXCharacterControl* pe = new CPhysXCharacterControl(scenenode, controller, DefaultGravity * 10.0f);
	PhysXCharacterControlMap[name] = pe;
	return pe;
}
//-----------------------------------------------------------------------------
void CPhysXManager::updateCharacter(NxReal deltaTime)
{
	static bool bFixedStep = false;

	if(PhysXCharacterControlMap.isEmpty())
		return;

	core::map<core::stringc, CPhysXCharacterControl*>::Iterator i = PhysXCharacterControlMap.getIterator();
	while(!i.atEnd())
	{
		i->getValue()->update(deltaTime);
		i++;
	}
	static float TimeStep = 1.0f / 60.0f;
	if (bFixedStep)	
		Scene->setTiming(TimeStep, 1, NX_TIMESTEP_FIXED);
	else
		Scene->setTiming(TimeStep, 1, NX_TIMESTEP_VARIABLE);
}
//-----------------------------------------------------------------------------
void CPhysXManager::postUpdateCharacter()
{
	NxReal maxTimestep;
	NxTimeStepMethod method;
	NxU32 maxIter;
	NxU32 numSubSteps;

	Scene->getTiming(maxTimestep, maxIter, method, &numSubSteps);
	if (numSubSteps)
		ControllerManager->updateControllers();
}
//-----------------------------------------------------------------------------
