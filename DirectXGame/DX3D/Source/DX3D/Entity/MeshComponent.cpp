#include <DX3D/Entity/Entity.h>
#include <DX3D/Entity/MeshComponent.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Game/World.h>
#include <DX3D/Graphics/GraphicsEngine.h>

namespace DX3D
{
	MeshComponent::MeshComponent() {}

	MeshComponent::~MeshComponent()
	{
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->RemoveComponent(this);
	}

	void MeshComponent::AddMaterial(const MaterialPtr& material)
	{
		m_materials.push_back(material);
	}

	void MeshComponent::RemoveMaterial(uint index)
	{
		if (index >= m_materials.size())
			return;

		m_materials.erase(m_materials.begin() + index);
	}

	void MeshComponent::OnCreateInternal()
	{
		Component::OnCreateInternal();
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->AddComponent(this);
	}
}
