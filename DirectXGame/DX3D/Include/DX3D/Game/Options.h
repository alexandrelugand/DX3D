#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class Options
	{
	public:
		Options(int argc, char* argv[]);
		virtual ~Options();

		bool HasOption(const std::string& option_name);
		std::string GetOption(const std::string& option_name);

	private:
		std::vector<std::string> m_args{};
	};
}
