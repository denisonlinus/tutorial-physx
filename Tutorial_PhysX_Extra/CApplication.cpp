#include "CApplication.h"
#include "CCapsuleSceneNode.h"
#include <iostream>

//-----------------------------------------------------------------------------
CApplication::CApplication(const SIrrlichtCreationParameters& parameters, core::stringc filename, bool showFPS, bool viewLog, bool writeLog) : FileName(filename), WriteFile(0), PhysicsManager(0), ForceStrength(1000), ShowFPS(showFPS), ViewLog(viewLog), WriteLog(writeLog), PhysXCharacterControl(0)
{
	std::locale::global(std::locale("C"));
	srand(12345);

	Device = createDeviceEx(parameters);
	Device->setEventReceiver(this);
	Driver = Device->getVideoDriver();
	Smgr = Device->getSceneManager();
	Logger = Device->getLogger();
	FileSystem = Device->getFileSystem();
	OSOperator = Device->getOSOperator();
	GuiEnv = Device->getGUIEnvironment();
	
	Device->setWindowCaption(L"Physics Tutorial 01");
	if(WriteLog)
	{
		WriteFile = FileSystem->createAndWriteFile(FileName.c_str());
	}
	if(ViewLog || WriteLog)
	{
		Logger->log("Begin Log\n");
	}

	PhysicsManager = new CPhysXManager(FileSystem, Logger);
	
}
//-----------------------------------------------------------------------------
CApplication::~CApplication()
{
	unloadSceneData();
	PhysicsManager->drop();

	if(ViewLog || WriteLog)
	{
		Logger->log("\nEnd Log");
	}

	Device->drop();
}
//-----------------------------------------------------------------------------
void CApplication::loadSceneData()
{
	GuiEnv->getSkin()->setFont(GuiEnv->getFont("pak/font_small.bmp"));

	core::dimension2d<int> size = Device->getVideoDriver()->getScreenSize();
	const int lwidth = size.Width - 20;
	const int lheight = 16;
	core::rect<int> pos(10, size.Height-lheight-10, 10+lwidth, size.Height-10);
	StatusText = GuiEnv->addStaticText(L"",	pos, false);
	StatusText->setOverrideColor(video::SColor(255,255,0,0));

	Device->getCursorControl()->setVisible(false);

	Smgr->addSkyBoxSceneNode
	(
		Driver->getTexture("pak/skybox.jpg"),
		Driver->getTexture("pak/skybox.jpg"),
		Driver->getTexture("pak/skybox.jpg"),
		Driver->getTexture("pak/skybox.jpg"),
		Driver->getTexture("pak/skybox.jpg"),
		Driver->getTexture("pak/skybox.jpg")
	);


	//loadSceneSimple();
	//loadSceneStack();
	//loadSceneTerrain();
	//loadSceneBSP();
	//loadSceneSimpleJoints();
	loadSceneRagDoll();
	//loadSceneRagDollBreakable();
	//loadSceneSimpleWithCharacterControl();
	//loadSceneBSPWithCharacterControl();
	//loadSceneTerrainWithCharacterControl();

}
//-----------------------------------------------------------------------------
void CApplication::loadSceneSimple()
{
	ForceStrength = 1000;

	scene::ICameraSceneNode* camera = Smgr->addCameraSceneNodeFPS(0, -100, 200, 500);;
	camera->setPosition(core::vector3df(0, 100, 100));
	camera->setTarget(core::vector3df(0, 50, 0));

	scene::ISceneNode* ground = Smgr->addCubeSceneNode(1);
	ground->setScale(core::vector3df(2000, 1, 2000));
	ground->setPosition(core::vector3df(0, -10, 0));
	ground->setRotation(core::vector3df(0, 0, 0));
	ground->setMaterialTexture( 0, Driver->getTexture("Pak/ground.jpg") );
	ground->setMaterialFlag(video::EMF_LIGHTING, false);
	PhysicsManager->createRigidBox(CPhysXManager::generateUniqueName(), ground, core::vector3df(2000, 1, 2000), false);

	scene::ISceneNode* boxNode = Smgr->addCubeSceneNode(1);
	boxNode->setScale(core::vector3df(10, 10, 10));
	boxNode->setPosition(core::vector3df(0, 20, 0));
	boxNode->setRotation(core::vector3df(0, 0, 0));
	boxNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	boxNode->setMaterialFlag(video::EMF_LIGHTING, false);
	PhysicsManager->createRigidBox(CPhysXManager::generateUniqueName(), boxNode, core::vector3df(10, 10, 10), true);

	scene::ISceneNode* boxNode1 = Smgr->addCubeSceneNode(1);
	boxNode1->setScale(core::vector3df(20, 5, 10));
	boxNode1->setPosition(core::vector3df(0, 40, 0));
	boxNode1->setRotation(core::vector3df(0, 0, 15));
	boxNode1->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	boxNode1->setMaterialFlag(video::EMF_LIGHTING, false);
	PhysicsManager->createRigidBox(CPhysXManager::generateUniqueName(), boxNode1, core::vector3df(20, 5, 10), true);

	scene::ISceneNode* sphereNode = Smgr->addSphereSceneNode(1);
	sphereNode->setScale(core::vector3df(10, 10, 10));
	sphereNode->setPosition(core::vector3df(15, 60, 0));
	sphereNode->setRotation(core::vector3df(0, 0, 15));
	sphereNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	sphereNode->setMaterialFlag(video::EMF_LIGHTING, false);
	PhysicsManager->createRigidSphere(CPhysXManager::generateUniqueName(), sphereNode, 10, true);

	scene::CCapsuleSceneNode* capsuleNode = new scene::CCapsuleSceneNode(20, 3, Smgr->getRootSceneNode(), Smgr);
	capsuleNode->setScale(core::vector3df(1, 1, 1));
	capsuleNode->setPosition(core::vector3df(20, 80, 0));
	capsuleNode->setRotation(core::vector3df(0, 0, 15));
	capsuleNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	capsuleNode->setMaterialFlag(video::EMF_LIGHTING, false);
	PhysicsManager->createRigidCapsule(CPhysXManager::generateUniqueName(), capsuleNode, 20, 3, true);

}
//-----------------------------------------------------------------------------
void CApplication::loadSceneTerrain()
{
	ForceStrength = 2000;
	f32 sc = 0.1f;
	scene::ICameraSceneNode* camera = Smgr->addCameraSceneNodeFPS(0,100.0f,1200.f*sc);
	camera->setPosition(core::vector3df(1900*2*sc,255*2*sc,3700*2*sc));
	camera->setTarget(core::vector3df(2397*2*sc,343*2*sc,2700*2*sc));
	camera->setFarValue(12000.0f);

	scene::ITerrainSceneNode* terrain = Smgr->addTerrainSceneNode
	( 
		"PAK/terrain-heightmap.bmp",
		0,										// parent node
		-1,										// node id
		core::vector3df(0.f, 0.f, 0.f),			// position
		core::vector3df(0.f, 0.f, 0.f),			// rotation
		core::vector3df(40.f*sc, 4.4f*sc, 40.f*sc),		// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
		5,										// maxLOD
		scene::ETPS_17,							// patchSize
		4										// smoothFactor
	);

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain->setMaterialTexture(0, Driver->getTexture("PAK/terrain-texture.jpg"));
	terrain->setMaterialTexture(1, Driver->getTexture("PAK/terrain-detailmap.jpg"));
	terrain->setMaterialType(video::EMT_DETAIL_MAP);
	terrain->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	terrain->scaleTexture(1.0f, 20.0f);

	PhysicsManager->createRigidTerrain(CPhysXManager::generateUniqueName(), terrain, "Pak/terrain-cook.bin");

}
//-----------------------------------------------------------------------------
void CApplication::loadSceneRagDoll()
{
	ForceStrength = 10000;

	scene::ICameraSceneNode* camera = Smgr->addCameraSceneNodeFPS(0, -100, 200, 500);;
	camera->setPosition(core::vector3df(0, 100, 100));
	camera->setTarget(core::vector3df(0, 50, 0));

	scene::ISceneNode* ground = Smgr->addCubeSceneNode(1);
	ground->setScale(core::vector3df(2000, 1, 2000));
	ground->setPosition(core::vector3df(0, -10, 0));
	ground->setRotation(core::vector3df(0, 0, 0));
	ground->setMaterialTexture( 0, Driver->getTexture("Pak/ground.jpg") );
	ground->setMaterialFlag(video::EMF_LIGHTING, false);
	PhysicsManager->createRigidBox(CPhysXManager::generateUniqueName(), ground, core::vector3df(2000, 1, 2000), false);

	addRagDoll(core::vector3df(0, 40, 0), 4);
}
//-----------------------------------------------------------------------------
void CApplication::loadSceneRagDollBreakable()
{
	ForceStrength = 10000;

	scene::ICameraSceneNode* camera = Smgr->addCameraSceneNodeFPS(0, -100, 200, 500);;
	camera->setPosition(core::vector3df(0, 100, 100));
	camera->setTarget(core::vector3df(0, 50, 0));

	scene::ISceneNode* ground = Smgr->addCubeSceneNode(1);
	ground->setScale(core::vector3df(2000, 1, 2000));
	ground->setPosition(core::vector3df(0, -10, 0));
	ground->setRotation(core::vector3df(0, 0, 0));
	ground->setMaterialTexture( 0, Driver->getTexture("Pak/ground.jpg") );
	ground->setMaterialFlag(video::EMF_LIGHTING, false);
	PhysicsManager->createRigidBox(CPhysXManager::generateUniqueName(), ground, core::vector3df(2000, 1, 2000), false);

	addRagDoll(core::vector3df(0, 40, 0), 4, core::vector3df(0,0,0), 0, true);
}
//-----------------------------------------------------------------------------
void CApplication::loadSceneBSP()
{
	ForceStrength = 2000;

	scene::ICameraSceneNode* camera = Smgr->addCameraSceneNodeFPS(0, -100, 200, 500);;
	camera->setPosition(core::vector3df(0, 100, 60));
	camera->setTarget(core::vector3df(0, 50, 0));

	FileSystem->addZipFileArchive("Pak/physicstest.pk3");
	scene::IAnimatedMesh* sceneMesh = Smgr->getMesh("Pak/physicstest.bsp");
	scene::ISceneNode* sceneNode = Smgr->addOctTreeSceneNode(sceneMesh);
	sceneNode->setScale(core::vector3df(0.25f, 0.25f, 0.25f));
	sceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
	sceneNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	PhysicsManager->createRigidMesh(CPhysXManager::generateUniqueName(), sceneNode, sceneMesh->getMesh(0), "Pak/physicstest.bin");
}
//-----------------------------------------------------------------------------
void CApplication::loadSceneStack()
{
	const core::vector3df& pos = core::vector3df(0,10,0);
	const core::vector3df& stackDim = core::vector3df(2,15,2);
	const core::vector3df& boxDim = core::vector3df(10,10,10);

	ForceStrength = 5000;

	scene::ICameraSceneNode* camera = Smgr->addCameraSceneNodeFPS(0, -100, 200, 500);;
	camera->setPosition(core::vector3df(0, 100, 100));
	camera->setTarget(core::vector3df(0, 50, 0));

	scene::ISceneNode* ground = Smgr->addCubeSceneNode(1);
	ground->setScale(core::vector3df(2000, 1, 2000));
	ground->setPosition(core::vector3df(0, -10, 0));
	ground->setRotation(core::vector3df(0, 0, 0));
	ground->setMaterialTexture( 0, Driver->getTexture("Pak/ground.jpg") );
	ground->setMaterialFlag(video::EMF_LIGHTING, false);
	PhysicsManager->createRigidBox(CPhysXManager::generateUniqueName(), ground, core::vector3df(2000, 1, 2000), false);

	core::vector3df offset = core::vector3df(boxDim.X,0,boxDim.Z) + pos;
	f32 threshold = 1.1f;

	u32 count = 0;
	for(s32 i = -stackDim.X; i < stackDim.X; i++)
	{
		for(s32 j = 0; j < stackDim.Y; j++)
		{
			for(s32 k = -stackDim.Z; k < stackDim.Z; k++)
			{
				core::vector3df boxPos = core::vector3df(i*boxDim.X*threshold, j*boxDim.Y*threshold, k*boxDim.Z*threshold);

				scene::ISceneNode* boxNode = Smgr->addCubeSceneNode(1);
				boxNode->setScale(boxDim);
				boxNode->setPosition(boxPos + offset);
				boxNode->setRotation(core::vector3df(0, 0, 0));
				boxNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
				boxNode->setMaterialFlag(video::EMF_LIGHTING, false);
				PhysicsManager->createRigidBox(CPhysXManager::generateUniqueName(), boxNode, boxDim, true);
			}
		}
	}

}
//-----------------------------------------------------------------------------
void CApplication::loadSceneSimpleJoints()
{
	ForceStrength = 5000;

	core::vector3df position(0,100,0);
	const f32& scale = 7.0f;
	core::vector3df Position;
	core::vector3df Scale;

	scene::ISceneNode* ground = Smgr->addCubeSceneNode(1);
	ground->setScale(core::vector3df(2000, 1, 2000));
	ground->setPosition(core::vector3df(0, -10, 0));
	ground->setRotation(core::vector3df(0, 0, 0));
	ground->setMaterialTexture( 0, Driver->getTexture("Pak/ground.jpg") );
	ground->setMaterialFlag(video::EMF_LIGHTING, false);
	PhysicsManager->createRigidBox(CPhysXManager::generateUniqueName(), ground, core::vector3df(2000, 1, 2000), false);

	scene::ICameraSceneNode* camera = Smgr->addCameraSceneNodeFPS(0, -100, 200, 500);;
	camera->setPosition(core::vector3df(0, 100, 100));
	camera->setTarget(core::vector3df(0, 50, 0));

	scene::CCapsuleSceneNode* Node1 = new scene::CCapsuleSceneNode(3.5, 0.7, Smgr->getRootSceneNode(), Smgr);
	Position = (core::vector3df(-1,-3.9,0) * scale) + position;
	Scale = core::vector3df(scale,scale,scale);
	Node1->setPosition(Position);
	Node1->setScale(Scale);
	Node1->setRotation(core::vector3df(0,0,-180));
	Node1->setMaterialFlag(video::EMF_LIGHTING, false);
	Node1->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	CPhysXEntity* PENode1 = PhysicsManager->createRigidCapsule(CPhysXManager::generateUniqueName(), Node1, 3.5 * scale, 0.7 * scale, true);
	PENode1->setDynamic(false);
	
	scene::CCapsuleSceneNode* Node2 = new scene::CCapsuleSceneNode(3.5, 0.6, Smgr->getRootSceneNode(), Smgr);
	Position = (core::vector3df(-1,-8.7,0) * scale) + position;
	Scale = core::vector3df(scale,scale,scale);
	Node2->setPosition(Position);
	Node2->setScale(Scale);
	Node2->setRotation(core::vector3df(0,0,-180));
	Node2->setMaterialFlag(video::EMF_LIGHTING, false);
	Node2->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	CPhysXEntity* PENode2 = PhysicsManager->createRigidCapsule(CPhysXManager::generateUniqueName(), Node2, 3.5 * scale, 0.6 * scale, true);

	PhysicsManager->createBodySphericalJoint(PENode2, PENode1, (core::vector3df(-1,-6.5,0) * scale) + position, core::vector3df(0,1,0));
	
}
//-----------------------------------------------------------------------------
void CApplication::loadSceneSimpleWithCharacterControl()
{
	ForceStrength = 500;
	JumpSpeed = 10;


	scene::ICameraSceneNode* camera = Smgr->addCameraSceneNodeFPS(0, -100, 200, 500);;
	camera->setPosition(core::vector3df(0, 100, 100));
	camera->setTarget(core::vector3df(0, 50, 0));

	scene::ISceneNode* ground = Smgr->addCubeSceneNode(1);
	ground->setScale(core::vector3df(2000, 1, 2000));
	ground->setPosition(core::vector3df(0, -10, 0));
	ground->setRotation(core::vector3df(0, 0, 0));
	ground->setMaterialTexture( 0, Driver->getTexture("Pak/ground.jpg") );
	ground->setMaterialFlag(video::EMF_LIGHTING, false);
	PhysicsManager->createRigidBox(CPhysXManager::generateUniqueName(), ground, core::vector3df(2000, 1, 2000), false, ECCG_COLLIDABLE_NON_PUSHABLE);

	scene::ISceneNode* boxNode = Smgr->addCubeSceneNode(1);
	boxNode->setScale(core::vector3df(10, 10, 10));
	boxNode->setPosition(core::vector3df(0, 10, 0));
	boxNode->setRotation(core::vector3df(0, 0, 0));
	boxNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	boxNode->setMaterialFlag(video::EMF_LIGHTING, false);
	PhysicsManager->createRigidBox(CPhysXManager::generateUniqueName(), boxNode, core::vector3df(10, 10, 10), true, ECCG_COLLIDABLE_PUSHABLE);

	scene::ISceneNode* boxNode1 = Smgr->addCubeSceneNode(1);
	boxNode1->setScale(core::vector3df(20, 5, 10));
	boxNode1->setPosition(core::vector3df(0, 40, 0));
	boxNode1->setRotation(core::vector3df(0, 0, 15));
	boxNode1->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	boxNode1->setMaterialFlag(video::EMF_LIGHTING, false);
	PhysicsManager->createRigidBox(CPhysXManager::generateUniqueName(), boxNode1, core::vector3df(20, 5, 10), true, ECCG_COLLIDABLE_PUSHABLE);

	scene::ISceneNode* sphereNode = Smgr->addSphereSceneNode(1);
	sphereNode->setScale(core::vector3df(10, 10, 10));
	sphereNode->setPosition(core::vector3df(15, 60, 0));
	sphereNode->setRotation(core::vector3df(0, 0, 15));
	sphereNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	sphereNode->setMaterialFlag(video::EMF_LIGHTING, false);
	PhysicsManager->createRigidSphere(CPhysXManager::generateUniqueName(), sphereNode, 10, true, ECCG_COLLIDABLE_PUSHABLE);

	scene::CCapsuleSceneNode* capsuleNode = new scene::CCapsuleSceneNode(20, 3, Smgr->getRootSceneNode(), Smgr);
	capsuleNode->setScale(core::vector3df(1, 1, 1));
	capsuleNode->setPosition(core::vector3df(20, 80, 0));
	capsuleNode->setRotation(core::vector3df(0, 0, 15));
	capsuleNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	capsuleNode->setMaterialFlag(video::EMF_LIGHTING, false);
	PhysicsManager->createRigidCapsule(CPhysXManager::generateUniqueName(), capsuleNode, 20, 3, true, ECCG_COLLIDABLE_PUSHABLE);

	PhysXCharacterControl = PhysicsManager->createCharacterControl(CPhysXManager::generateUniqueName(), camera, 12, 4);
	PhysXCharacterControl->setCharacterSpeed(100);
	PhysXCharacterControl->setEyeHeight(core::vector3df(0,10,0));

	for(s32 i=0; i<5; ++i)
		CursorKeys[i] = false;


}
//-----------------------------------------------------------------------------
void CApplication::loadSceneBSPWithCharacterControl()
{
	//map-20kdm2.pk3

	ForceStrength = 1500;
	JumpSpeed = 10.0f;

	scene::ICameraSceneNode* camera = Smgr->addCameraSceneNodeFPS(0, -100, 200, 500);;
	camera->setPosition(core::vector3df(500, 150, 120));
	camera->setTarget(core::vector3df(499, 150, 120));

	FileSystem->addZipFileArchive("Pak/map-20kdm2.pk3");
	scene::IAnimatedMesh* sceneMesh = Smgr->getMesh("Pak/20kdm2.bsp");
	scene::ISceneNode* sceneNode = Smgr->addOctTreeSceneNode(sceneMesh);
	sceneNode->setScale(core::vector3df(0.25f, 0.25f, 0.25f));
	sceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
	sceneNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	PhysicsManager->createRigidMesh(CPhysXManager::generateUniqueName(), sceneNode, sceneMesh->getMesh(0), "Pak/20kdm2.bin", ECCG_COLLIDABLE_NON_PUSHABLE);

	PhysXCharacterControl = PhysicsManager->createCharacterControl(CPhysXManager::generateUniqueName(), camera, 12, 4);
	PhysXCharacterControl->setCharacterSpeed(100);
	PhysXCharacterControl->setAutoStepping(10);
	PhysXCharacterControl->setEyeHeight(core::vector3df(0,10,0));

	for(s32 i=0; i<5; ++i)
		CursorKeys[i] = false;

}
//-----------------------------------------------------------------------------
void CApplication::loadSceneTerrainWithCharacterControl()
{
	ForceStrength = 2000;
	JumpSpeed = 10;

	f32 sc = 0.1f;
	scene::ICameraSceneNode* camera = Smgr->addCameraSceneNodeFPS(0,100.0f,1200.f*sc);
	camera->setPosition(core::vector3df(1900*2*sc,255*2*sc,3700*2*sc));
	camera->setTarget(core::vector3df(2397*2*sc,343*2*sc,2700*2*sc));
	camera->setFarValue(12000.0f);

	scene::ITerrainSceneNode* terrain = Smgr->addTerrainSceneNode
	( 
		"PAK/terrain-heightmap.bmp",
		0,										// parent node
		-1,										// node id
		core::vector3df(0.f, 0.f, 0.f),			// position
		core::vector3df(0.f, 0.f, 0.f),			// rotation
		core::vector3df(40.f*sc, 4.4f*sc, 40.f*sc),		// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
		5,										// maxLOD
		scene::ETPS_17,							// patchSize
		4										// smoothFactor
	);

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain->setMaterialTexture(0, Driver->getTexture("PAK/terrain-texture.jpg"));
	terrain->setMaterialTexture(1, Driver->getTexture("PAK/terrain-detailmap.jpg"));
	terrain->setMaterialType(video::EMT_DETAIL_MAP);
	terrain->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	terrain->scaleTexture(1.0f, 20.0f);

	PhysicsManager->createRigidTerrain(CPhysXManager::generateUniqueName(), terrain, "Pak/terrain-cook.bin", ECCG_COLLIDABLE_NON_PUSHABLE);

	PhysXCharacterControl = PhysicsManager->createCharacterControl(CPhysXManager::generateUniqueName(), camera, 12, 4, 45.0f);
	PhysXCharacterControl->setCharacterSpeed(100);
	PhysXCharacterControl->setAutoStepping(10);
	PhysXCharacterControl->setEyeHeight(core::vector3df(0,10,0));

	for(s32 i=0; i<5; ++i)
		CursorKeys[i] = false;

}
//-----------------------------------------------------------------------------
void CApplication::characterFPSCameraUpdate()
{
	if(!PhysXCharacterControl)
		return;

	if(CursorKeys[0])
	{
		scene::ICameraSceneNode* camera = Smgr->getActiveCamera();
		core::vector3df position = camera->getPosition();
		core::vector3df target = camera->getTarget();
		core::vector3df irrDir = (target - position).normalize();
		PhysXCharacterControl->move(irrDir);
	}

	if(CursorKeys[1])
	{
		scene::ICameraSceneNode* camera = Smgr->getActiveCamera();
		core::vector3df position = camera->getPosition();
		core::vector3df target = camera->getTarget();
		core::vector3df irrDir = (position - target).normalize();
		PhysXCharacterControl->move(irrDir);
	}

	if(CursorKeys[2])
	{
		scene::ICameraSceneNode* camera = Smgr->getActiveCamera();
		core::vector3df position = camera->getPosition();
		core::vector3df target = camera->getTarget();
		core::vector3df up = camera->getUpVector();
		core::vector3df irrDir = (target - position);
		irrDir = irrDir.crossProduct(up).normalize();
		PhysXCharacterControl->move(irrDir);
	}

	if(CursorKeys[3])
	{
		scene::ICameraSceneNode* camera = Smgr->getActiveCamera();
		core::vector3df position = camera->getPosition();
		core::vector3df target = camera->getTarget();
		core::vector3df up = camera->getUpVector();
		core::vector3df irrDir = (position - target);
		irrDir = irrDir.crossProduct(up).normalize();
		PhysXCharacterControl->move(irrDir);
	}

	if(CursorKeys[4])
	{
		PhysXCharacterControl->jump(JumpSpeed);
	}
}
//-----------------------------------------------------------------------------
void CApplication::unloadSceneData()
{
}
//-----------------------------------------------------------------------------
void CApplication::run()
{
	wchar_t tmp[255];

	loadSceneData();

	while(Device->run() && Driver)
	if(Device->isWindowActive())
	{

		characterFPSCameraUpdate();

		PhysicsManager->update();
		
		Driver->beginScene(true, true, video::SColor(255,90,90,156));
		Smgr->drawAll();
		GuiEnv->drawAll();
		Driver->endScene();

		//FPS
		static s32 lastfps = 0;
		s32 nowfps = Driver->getFPS();
		if(nowfps != lastfps)
		{
			lastfps = nowfps;
		}
		if(ShowFPS)
		{
			swprintf(tmp, 255, L"FPS: %d\n", nowfps);
			StatusText->setText(tmp);
		}
	}

	unloadSceneData();

}
//-----------------------------------------------------------------------------
bool CApplication::OnEvent(SEvent event)
{
	if(!Device)
		return false;

	if (event.EventType == EET_KEY_INPUT_EVENT)
	{

		if(PhysXCharacterControl)
		{
			if(irr::KEY_KEY_W == event.KeyInput.Key)
			{
				CursorKeys[0] = event.KeyInput.PressedDown; 
				//return true;
			}
			if(irr::KEY_KEY_S == event.KeyInput.Key)
			{
				CursorKeys[1] = event.KeyInput.PressedDown; 
				//return true;
			}
			if(irr::KEY_KEY_A == event.KeyInput.Key)
			{
				CursorKeys[2] = event.KeyInput.PressedDown; 
				//return true;
			}
			if(irr::KEY_KEY_D == event.KeyInput.Key)
			{
				CursorKeys[3] = event.KeyInput.PressedDown; 
				//return true;
			}
			if(irr::KEY_SPACE == event.KeyInput.Key)
			{
				CursorKeys[4] = event.KeyInput.PressedDown; 
				//return true;
			}
			//return false;
		}

		if(event.KeyInput.Key == KEY_ESCAPE && event.KeyInput.PressedDown == false)
		{
			Device->closeDevice();
			return true;
		}
		else
		if(event.KeyInput.Key == KEY_KEY_1 && event.KeyInput.PressedDown == false)
		{
			shootBall();
			return true;
		}
		else
		if(event.KeyInput.Key == KEY_KEY_2 && event.KeyInput.PressedDown == false)
		{
			shootBox();
			return true;
		}
		else
		if(event.KeyInput.Key == KEY_KEY_3 && event.KeyInput.PressedDown == false)
		{
			shootCapsule();
			return true;
		}
		else
		if(event.KeyInput.Key == KEY_KEY_4 && event.KeyInput.PressedDown == false)
		{
			shootRagDoll();
			return true;
		}
		else
		if(event.KeyInput.Key == KEY_F1 && event.KeyInput.PressedDown == false)
		{
			//PhysicsManager->finalize();
			//PhysicsManager->initialize();
			return true;
		}
		/*
		else
		{
			if(Device->getSceneManager()->getActiveCamera())
			{
				Device->getSceneManager()->getActiveCamera()->OnEvent(event);
				return true;
			}
		}*/
	}
	else
	{
		if(event.EventType == EET_LOG_TEXT_EVENT)
		{
			if(WriteLog)
			{
				if(WriteFile)
				{
					WriteFile->write(event.LogEvent.Text, strlen(event.LogEvent.Text));
					WriteFile->write("\n", strlen("\n"));
				}
			}
			if(ViewLog)
			{
				printf("%s\n", event.LogEvent.Text);
			}
			return true;
		}
	}


	return false;
}
//-----------------------------------------------------------------------------
void CApplication::shootBall()
{
	scene::ICameraSceneNode* camera = Smgr->getActiveCamera();
	core::vector3df position = camera->getPosition();
	core::vector3df target = camera->getTarget();
	core::vector3df irrDir = (target - position).normalize();
	
	scene::ISceneNode* ball = Smgr->addSphereSceneNode(1);
	ball->setScale(core::vector3df(5, 5, 5));
	ball->setPosition(position);
	ball->setRotation(core::vector3df(0, 0, 0));
	ball->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	ball->setMaterialFlag(video::EMF_LIGHTING, false);
	CPhysXEntity* entity = PhysicsManager->createRigidSphere(CPhysXManager::generateUniqueName(), ball, 5, true);
	entity->applyForce(irrDir, ForceStrength);
}
//-----------------------------------------------------------------------------
void CApplication::shootBox()
{
	scene::ICameraSceneNode* camera = Smgr->getActiveCamera();
	core::vector3df position = camera->getPosition();
	core::vector3df target = camera->getTarget();
	core::vector3df irrDir = (target - position).normalize();

	scene::ISceneNode* cube = Smgr->addCubeSceneNode(1);
	cube->setScale(core::vector3df(10, 10, 10));
	cube->setPosition(position);
	cube->setRotation(core::vector3df(0, 0, 0));
	cube->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	cube->setMaterialFlag(video::EMF_LIGHTING, false);
	CPhysXEntity* entity = PhysicsManager->createRigidBox(CPhysXManager::generateUniqueName(), cube, core::vector3df(10, 10, 10), true);
	entity->applyForce(irrDir, ForceStrength);

}
//-----------------------------------------------------------------------------
void CApplication::shootCapsule()
{
	scene::ICameraSceneNode* camera = Smgr->getActiveCamera();
	core::vector3df position = camera->getPosition();
	core::vector3df target = camera->getTarget();
	core::vector3df irrDir = (target - position).normalize();

	scene::CCapsuleSceneNode* capsuleNode = new scene::CCapsuleSceneNode(20, 5, Smgr->getRootSceneNode(), Smgr);
	capsuleNode->setPosition(position);
	capsuleNode->setMaterialFlag(video::EMF_LIGHTING, false);
	capsuleNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	CPhysXEntity* entity = PhysicsManager->createRigidCapsule(CPhysXManager::generateUniqueName(), capsuleNode, 20, 5, true);
	entity->applyForce(irrDir, ForceStrength);

}
//-----------------------------------------------------------------------------
void CApplication::shootRagDoll()
{
	scene::ICameraSceneNode* camera = Smgr->getActiveCamera();
	core::vector3df position = camera->getPosition();
	core::vector3df target = camera->getTarget();
	core::vector3df irrDir = (target - position).normalize();

	addRagDoll(position, 3, irrDir, ForceStrength);

}
//-----------------------------------------------------------------------------
void CApplication::addRagDoll(const core::vector3df& position, const f32& scale, const core::vector3df& forceDir, const f32& forceStrength, const bool& isBreakable)
{
	core::vector3df Scale;
	core::vector3df Position;

	bool isDynamic = true;

	//HEAD
	scene::ISceneNode* headNode = Smgr->addSphereSceneNode(1);
	Position = (core::vector3df(0,7.3,0) * scale) + position;
	Scale = core::vector3df(1,1,1) * scale;
	headNode->setPosition(Position);
	headNode->setScale(Scale);
	headNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	headNode->setMaterialFlag(video::EMF_LIGHTING, false);
	CPhysXEntity* head = PhysicsManager->createRigidSphere(CPhysXManager::generateUniqueName(), headNode, 1 * scale, isDynamic);
	//TORSO
	scene::ISceneNode* torsoNode = Smgr->addSphereSceneNode(1);
	Position = (core::vector3df(0,4,0) * scale) + position;
	Scale = core::vector3df(2.3,2.3,2.3) * scale;
	torsoNode->setPosition(Position);
	torsoNode->setScale(Scale);
	torsoNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	torsoNode->setMaterialFlag(video::EMF_LIGHTING, false);
	CPhysXEntity* torso = PhysicsManager->createRigidSphere(CPhysXManager::generateUniqueName(), torsoNode, 2.3 * scale, isDynamic);
	//torso->setDynamic(false);
	//PELVIS
	scene::ISceneNode* pelvisNode = Smgr->addSphereSceneNode(1);
	Position = (core::vector3df(0,0,0) * scale) + position;
	Scale = core::vector3df(1.7,1.7,1.7) * scale;
	pelvisNode->setPosition(Position);
	pelvisNode->setScale(Scale);
	pelvisNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	pelvisNode->setMaterialFlag(video::EMF_LIGHTING, false);
	CPhysXEntity* pelvis = PhysicsManager->createRigidSphere(CPhysXManager::generateUniqueName(), pelvisNode, 1 * scale, isDynamic);
	//LEFT UPPER ARM
	scene::CCapsuleSceneNode* leftUpperArmNode = new scene::CCapsuleSceneNode(3, 0.7, Smgr->getRootSceneNode(), Smgr);
	Position = (core::vector3df(4,5,0) * scale) + position;
	Scale = core::vector3df(scale,scale,scale);
	leftUpperArmNode->setPosition(Position);
	leftUpperArmNode->setScale(Scale);
	leftUpperArmNode->setRotation(core::vector3df(0,0,-90));
	leftUpperArmNode->setMaterialFlag(video::EMF_LIGHTING, false);
	leftUpperArmNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	CPhysXEntity* leftUpperArm = PhysicsManager->createRigidCapsule(CPhysXManager::generateUniqueName(), leftUpperArmNode, 3 * scale, 0.7 * scale, isDynamic);
	//LEFT FORE ARM
	scene::CCapsuleSceneNode* leftForeArmNode = new scene::CCapsuleSceneNode(3, 0.5, Smgr->getRootSceneNode(), Smgr);
	Position = (core::vector3df(8.2,5,0) * scale) + position;
	Scale = core::vector3df(scale,scale,scale);
	leftForeArmNode->setPosition(Position);
	leftForeArmNode->setScale(Scale);
	leftForeArmNode->setRotation(core::vector3df(0,0,-90));
	leftForeArmNode->setMaterialFlag(video::EMF_LIGHTING, false);
	leftForeArmNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	CPhysXEntity* leftForeArm = PhysicsManager->createRigidCapsule(CPhysXManager::generateUniqueName(), leftForeArmNode, 3 * scale, 0.5 * scale, isDynamic);
	//RIGHT UPPER ARM
	scene::CCapsuleSceneNode* rightUpperArmNode = new scene::CCapsuleSceneNode(3, 0.7, Smgr->getRootSceneNode(), Smgr);
	Position = (core::vector3df(-4,5,0) * scale) + position;
	Scale = core::vector3df(scale,scale,scale);
	rightUpperArmNode->setPosition(Position);
	rightUpperArmNode->setScale(Scale);
	rightUpperArmNode->setRotation(core::vector3df(0,0,90));
	rightUpperArmNode->setMaterialFlag(video::EMF_LIGHTING, false);
	rightUpperArmNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	CPhysXEntity* rightUpperArm = PhysicsManager->createRigidCapsule(CPhysXManager::generateUniqueName(), rightUpperArmNode, 3 * scale, 0.7 * scale, isDynamic);
	//RIGHT FORE ARM
	scene::CCapsuleSceneNode* rightForeArmNode = new scene::CCapsuleSceneNode(3, 0.5, Smgr->getRootSceneNode(), Smgr);
	Position = (core::vector3df(-8.2,5,0) * scale) + position;
	Scale = core::vector3df(scale,scale,scale);
	rightForeArmNode->setPosition(Position);
	rightForeArmNode->setScale(Scale);
	rightForeArmNode->setRotation(core::vector3df(0,0,90));
	rightForeArmNode->setMaterialFlag(video::EMF_LIGHTING, false);
	rightForeArmNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	CPhysXEntity* rightForeArm = PhysicsManager->createRigidCapsule(CPhysXManager::generateUniqueName(), rightForeArmNode, 3 * scale, 0.5 * scale, isDynamic);
	//LEFT THIGH
	scene::CCapsuleSceneNode* leftThighNode = new scene::CCapsuleSceneNode(3.5, 0.7, Smgr->getRootSceneNode(), Smgr);
	Position = (core::vector3df(1,-3.9,0) * scale) + position;
	Scale = core::vector3df(scale,scale,scale);
	leftThighNode->setPosition(Position);
	leftThighNode->setScale(Scale);
	leftThighNode->setRotation(core::vector3df(0,0,180));
	leftThighNode->setMaterialFlag(video::EMF_LIGHTING, false);
	leftThighNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	CPhysXEntity* leftThigh = PhysicsManager->createRigidCapsule(CPhysXManager::generateUniqueName(), leftThighNode, 3.5 * scale, 0.7 * scale, isDynamic);
	//RIGHT THIGH
	scene::CCapsuleSceneNode* rightThighNode = new scene::CCapsuleSceneNode(3.5, 0.7, Smgr->getRootSceneNode(), Smgr);
	Position = (core::vector3df(-1,-3.9,0) * scale) + position;
	Scale = core::vector3df(scale,scale,scale);
	rightThighNode->setPosition(Position);
	rightThighNode->setScale(Scale);
	rightThighNode->setRotation(core::vector3df(0,0,-180));
	rightThighNode->setMaterialFlag(video::EMF_LIGHTING, false);
	rightThighNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	CPhysXEntity* rightThigh = PhysicsManager->createRigidCapsule(CPhysXManager::generateUniqueName(), rightThighNode, 3.5 * scale, 0.7 * scale, isDynamic);
	//LEFT CALF
	scene::CCapsuleSceneNode* leftCalfNode = new scene::CCapsuleSceneNode(3.5, 0.6, Smgr->getRootSceneNode(), Smgr);
	Position = (core::vector3df(1,-8.7,0) * scale) + position;
	Scale = core::vector3df(scale,scale,scale);
	leftCalfNode->setPosition(Position);
	leftCalfNode->setScale(Scale);
	leftCalfNode->setRotation(core::vector3df(0,0,180));
	leftCalfNode->setMaterialFlag(video::EMF_LIGHTING, false);
	leftCalfNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	CPhysXEntity* leftCalf = PhysicsManager->createRigidCapsule(CPhysXManager::generateUniqueName(), leftCalfNode, 3.5 * scale, 0.6 * scale, isDynamic);
	//RIGHT CALF
	scene::CCapsuleSceneNode* rightCalfNode = new scene::CCapsuleSceneNode(3.5, 0.6, Smgr->getRootSceneNode(), Smgr);
	Position = (core::vector3df(-1,-8.7,0) * scale) + position;
	Scale = core::vector3df(scale,scale,scale);
	rightCalfNode->setPosition(Position);
	rightCalfNode->setScale(Scale);
	rightCalfNode->setRotation(core::vector3df(0,0,-180));
	rightCalfNode->setMaterialFlag(video::EMF_LIGHTING, false);
	rightCalfNode->setMaterialTexture( 0, Driver->getTexture("Pak/objs.jpg") );
	CPhysXEntity* rightCalf = PhysicsManager->createRigidCapsule(CPhysXManager::generateUniqueName(), rightCalfNode, 3.5 * scale, 0.6 * scale, isDynamic);

	//JOINTS
	//NECK
	PhysicsManager->createBodySphericalJoint(head, torso, (core::vector3df(0,6.3,0) * scale) + position, core::vector3df(0,1,0), isBreakable);
	//SPINE
	PhysicsManager->createBodySphericalJoint(torso, pelvis, (core::vector3df(0,2,0) * scale) + position, core::vector3df(0,-1,0), isBreakable);
	//LEFT ELBOW
	PhysicsManager->createBodySphericalJoint(leftUpperArm, leftForeArm, (core::vector3df(6.2,5,0) * scale) + position, core::vector3df(0,0,-1), isBreakable);
	//RIGHT ELBOW
	PhysicsManager->createBodySphericalJoint(rightUpperArm, rightForeArm, (core::vector3df(-6.2,5,0) * scale) + position, core::vector3df(0,0,-1), isBreakable);
	//LEFT SHOULDER
	PhysicsManager->createBodySphericalJoint(leftUpperArm, torso, (core::vector3df(2,5,0) * scale) + position, core::vector3df(1,0,0), isBreakable);
	//RIGHT SHOULDER
	PhysicsManager->createBodySphericalJoint(rightUpperArm, torso, (core::vector3df(-2,5,0) * scale) + position, core::vector3df(-1,0,0), isBreakable);
	//LEFT HIP
	PhysicsManager->createBodySphericalJoint(leftThigh, pelvis, (core::vector3df(1,-1.5,0) * scale) + position, core::vector3df(0,-1,0), isBreakable);
	//RIGHT HIP
	PhysicsManager->createBodySphericalJoint(rightThigh, pelvis, (core::vector3df(-1,-1.5,0) * scale) + position, core::vector3df(0,-1,0), isBreakable);
	//LEFT KNEE
	PhysicsManager->createBodySphericalJoint(leftCalf, leftThigh, (core::vector3df(1,-6.5,0) * scale) + position, core::vector3df(1,0,0), isBreakable);
	//RIGHT KNEE
	PhysicsManager->createBodySphericalJoint(rightCalf, rightThigh, (core::vector3df(-1,-6.5,0) * scale) + position, core::vector3df(-1,0,0), isBreakable);

	if(forceStrength != 0.0f)
		torso->applyForce(forceDir, forceStrength);

}
//-----------------------------------------------------------------------------
