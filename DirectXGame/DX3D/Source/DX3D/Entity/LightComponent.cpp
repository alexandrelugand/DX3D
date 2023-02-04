#include <DX3D/Entity/Entity.h>
#include <DX3D/Entity/LightComponent.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Game/World.h>
#include <DX3D/Graphics/GraphicsEngine.h>

namespace DX3D
{
	LightComponent::LightComponent() {}

	LightComponent::~LightComponent()
	{
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->RemoveComponent(this);
	}

	void LightComponent::OnCreateInternal()
	{
		Component::OnCreateInternal();
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->AddComponent(this);
	}
}
