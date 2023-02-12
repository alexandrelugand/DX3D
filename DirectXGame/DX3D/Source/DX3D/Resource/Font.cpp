#include <DX3D/Game/Game.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Resource/Font.h>
#include <DX3D/Resource/ResourceManager.h>

namespace DX3D
{
	Font::Font(ResourceManager* manager, const wchar_t* full_path)
		: Resource(manager, full_path)
	{
		m_font = manager->GetGame()->GetGraphicsEngine()->GetRenderSystem()->CreateFont(full_path);
	}

	Font::~Font() {}
}
