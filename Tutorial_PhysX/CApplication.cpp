//-----------------------------------------------------------------------------
// (C) Copyright 2007-2014 Denison Linus
//
// Author: Denison Linus
// Email: dlmtavar@gmail.com
//
//-----------------------------------------------------------------------------
#include "CApplication.h"
#include "CCapsuleSceneNode.h"
#include <iostream>

//-----------------------------------------------------------------------------
CApplication::CApplication(const SIrrlichtCreationParameters& parameters, core::stringc filename, bool showFPS, bool viewLog, bool writeLog) : FileName(filename), WriteFile(0), ShowFPS(showFPS), ViewLog(viewLog), WriteLog(writeLog)
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

	//***********************************************
	//*** EDIT Tutorial 1 ***************************
	//***********************************************
	
}
//-----------------------------------------------------------------------------
CApplication::~CApplication()
{
	unloadSceneData();

	//***********************************************
	//*** EDIT Tutorial 1 ***************************
	//***********************************************

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


	//***********************************************
	loadSceneSimple();
	//***********************************************

}
//-----------------------------------------------------------------------------
void CApplication::loadSceneSimple()
{
	//***********************************************
	//*** EDIT Tutorial 1 ***************************
	//***********************************************
}
//-----------------------------------------------------------------------------
void CApplication::loadSceneBSPWithCharacterControl()
{
	//***********************************************
	//*** EDIT Tutorial 2 ***************************
	//***********************************************
}
//-----------------------------------------------------------------------------
void CApplication::loadSceneTerrainWithCharacterControl()
{
	//***********************************************
	//*** EDIT Tutorial 2 ***************************
	//***********************************************
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

		//***********************************************
		//*** EDIT Tutorial 2 ***************************
		//***********************************************

		//***********************************************
		//*** EDIT Tutorial 1 ***************************
		//***********************************************
		
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

		//***********************************************
		//*** EDIT Tutorial 2 ***************************
		//***********************************************

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
	//***********************************************
	//*** EDIT Tutorial 1 ***************************
	//***********************************************
}
//-----------------------------------------------------------------------------
void CApplication::shootBox()
{
	//***********************************************
	//*** EDIT Tutorial 1 ***************************
	//***********************************************

}
//-----------------------------------------------------------------------------
void CApplication::shootCapsule()
{
	//***********************************************
	//*** EDIT Tutorial 1 ***************************
	//***********************************************
}
//-----------------------------------------------------------------------------
