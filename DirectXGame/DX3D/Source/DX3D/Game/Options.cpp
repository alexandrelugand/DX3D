#include <DX3D/Game/Options.h>

namespace DX3D
{
	Options::Options(int argc, char* argv[])
	{
		if (argc > 64)
		{
			DX3DException("too many input parameters!");
		}

		m_args = std::vector<std::string>(argv + 1, argv + argc);
	}

	Options::~Options() {}

	bool Options::HasOption(const std::string& option_name)
	{
		for (auto it = m_args.begin(), end = m_args.end(); it != end; ++it)
		{
			if (*it == option_name)
				return true;
		}

		return false;
	}

	std::string Options::GetOption(const std::string& option_name)
	{
		for (auto it = m_args.begin(), end = m_args.end(); it != end; ++it)
		{
			if (*it == option_name)
				if (it + 1 != end)
					return *(it + 1);
		}

		return "";
	}
}
