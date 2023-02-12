#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class FontUtility
	{
	public:
		static void CreateFont(const wchar_t* font_name, float font_size, const wchar_t* output_path);
	};
}
