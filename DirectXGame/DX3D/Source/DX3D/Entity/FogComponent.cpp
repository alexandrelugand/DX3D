#include <DX3D/Entity/Entity.h>
#include <DX3D/Entity/FogComponent.h>
#include <DX3D/Entity/TransformComponent.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Game/World.h>
#include <DX3D/Graphics/GraphicsEngine.h>

namespace DX3D
{
	FogComponent::FogComponent() { }

	FogComponent::~FogComponent()
	{
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->RemoveComponent(this);
	}

	void FogComponent::OnCreateInternal()
	{
		Component::OnCreateInternal();
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->AddComponent(this);
	}
}
