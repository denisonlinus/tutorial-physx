#include "CApplication.h"

int main(int argc, char** argv)
{
	SIrrlichtCreationParameters parameters;
	//parameters.Fullscreen = true;
	parameters.DriverType = video::EDT_OPENGL;
	parameters.Bits = 32;
	core::stringc filename = "Pak/log.txt";

	CApplication app(parameters, filename, true, true, true);
	app.run();

	return 0;
}
