//-----------------------------------------------------------------------------
// (C) Copyright 2007-2014 Denison Linus
//
// Author: Denison Linus
// Email: dlmtavar@gmail.com
//
//-----------------------------------------------------------------------------
#ifndef _CAPPLICATION_H_
#define _CAPPLICATION_H_

#include "irrlicht.h"
//***********************************************
//*** EDIT Tutorial 1 ***************************
//***********************************************
#include "CPhysXManager.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

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

	//***********************************************
	//*** EDIT Tutorial 1 ***************************
	//***********************************************
	void loadSceneSimple();
	void shootBall();
	void shootBox();
	void shootCapsule();

	//***********************************************
	//*** EDIT Tutorial 2 ***************************
	//***********************************************
	void loadSceneBSPWithCharacterControl();
	void loadSceneTerrainWithCharacterControl();

};
//-----------------------------------------------------------------------------

#endif
