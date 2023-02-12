#include <DX3D/Resource/FontUtility.h>

namespace DX3D
{
	void FontUtility::CreateFont(const wchar_t* font_name, float font_size, const wchar_t* output_path)
	{
		std::wstringstream p;
		std::wstring s = std::regex_replace(font_name, std::wregex(L"\\s+"), L"");

		p << output_path << s << ".font";

		if (!std::filesystem::exists(p.str()))
		{
			std::wstringstream command;
			command << "DX3D\\Vendor\\DirectXTK\\bin\\MakeSpriteFont ";
			command << "\"" << font_name << "\" ";
			command << p.str() << " ";
			command << "/FontSize:" << font_size;

			if (_wsystem(command.str().c_str()) != 0)
			{
				DX3DException("Failed to create font " << UnicodeToChar(font_name));
			}
		}
	}
}
