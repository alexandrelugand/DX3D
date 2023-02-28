#include <DX3D/Entity/Entity.h>
#include <DX3D/Entity/TextComponent.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Game/World.h>
#include <DX3D/Graphics/Font2D.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Resource/Font.h>

namespace DX3D
{
	TextComponent::TextComponent() {}

	TextComponent::~TextComponent()
	{
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->RemoveComponent(this);
	}

	Rect TextComponent::GetBounds()
	{
		return m_font->GetFont()->GetBounds(m_text.c_str());
	}

	void TextComponent::OnCreateInternal()
	{
		Component::OnCreateInternal();
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->AddComponent(this);
	}
}
