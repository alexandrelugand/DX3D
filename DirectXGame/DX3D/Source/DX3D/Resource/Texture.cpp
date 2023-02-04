#include <DX3D/Game/Game.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Resource/Texture.h>

namespace DX3D
{
	Texture::Texture(ResourceManager* manager, const wchar_t* full_path)
		: Resource(manager, full_path)
	{
		m_texture = manager->GetGame()->GetGraphicsEngine()->GetRenderSystem()->CreateTexture(full_path);
	}
}
