#ifndef _CAPPLICATION_H_
#define _CAPPLICATION_H_

#include "irrlicht.h"
#include "CPhysXManager.h"
using namespace irr;

//-----------------------------------------------------------------------------
class CApplication : public IEventReceiver
{
public:
	CApplication(const SIrrlichtCreationParameters& parameters, core::stringc filename, bool showFPS, bool viewLog, bool writeLog);
	~CApplication();
	void run();
	virtual bool OnEvent(SEvent event);
private:
	void loadSceneData();
	void unloadSceneData();
	IrrlichtDevice* Device;
	video::IVideoDriver* Driver;
	scene::ISceneManager* Smgr;
	io::IFileSystem* FileSystem;
	ILogger* Logger;
	IOSOperator* OSOperator;
	gui::IGUIEnvironment* GuiEnv;
	gui::IGUIStaticText* StatusText;
	bool ViewLog;
	bool WriteLog;
	bool ShowFPS;
	core::stringc FileName;
	io::IWriteFile* WriteFile;

	CPhysXManager* PhysicsManager;

	void loadSceneSimple();
	void loadSceneTerrain();
	void loadSceneRagDoll();
	void loadSceneRagDollBreakable();
	void loadSceneBSP();
	void loadSceneStack();
	void loadSceneSimpleJoints();
	void loadSceneBSPWithCharacterControl();
	void loadSceneTerrainWithCharacterControl();

	void loadSceneSimpleWithCharacterControl();
	CPhysXCharacterControl* PhysXCharacterControl;
	void characterFPSCameraUpdate();
	bool CursorKeys[5];
	f32 JumpSpeed;

	irr::f32 ForceStrength;
	void shootBall();
	void shootBox();
	void shootCapsule();
	void shootRagDoll();
	void addRagDoll(const core::vector3df& position, const f32& scale = 1.0f, const core::vector3df& forceDir = core::vector3df(0,0,0), const f32& forceStrength = 0.0f, const bool& isBreakable = false);

};
//-----------------------------------------------------------------------------

#endif
