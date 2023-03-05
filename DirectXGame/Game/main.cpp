#include <DX3D/Game/Options.h>
#include "DemoGame.h"

int main(int argc, char* argv[])
{
	try
	{
		auto options = DX3D::Options(argc, argv);
		DemoGame app(options);
		app.Run();
	}
	catch (const std::exception& ex)
	{
		std::cout << "Exception: " << ex.what() << std::endl;
		std::cin.ignore();
		return -1;
	}

	return 0;
}
