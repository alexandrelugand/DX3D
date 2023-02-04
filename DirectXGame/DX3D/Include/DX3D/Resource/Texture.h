#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Resource/Resource.h>

namespace DX3D
{
	class Texture : public Resource
	{
	public:
		Texture(ResourceManager* manager, const wchar_t* full_path);
		const Texture2DPtr& GetTexture() { return m_texture; }

	private:
		Texture2DPtr m_texture;

		friend class Material;
	};
}
