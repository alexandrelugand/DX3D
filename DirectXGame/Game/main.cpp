#include "DemoGame.h"

int main(int argc, char** argv)
{
	try
	{
		DemoGame app;
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
