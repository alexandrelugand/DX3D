#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Resource/Resource.h>

namespace DX3D
{
	class Font : public Resource
	{
	public:
		Font(ResourceManager* manager, const wchar_t* full_path);
		~Font() override;

		const Font2DPtr& GetFont() { return m_font; }

	private:
		Font2DPtr m_font = nullptr;
	};
}
