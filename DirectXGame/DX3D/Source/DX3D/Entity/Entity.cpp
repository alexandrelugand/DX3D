#include <DX3D/Entity/Component.h>
#include <DX3D/Entity/Entity.h>
#include <DX3D/Entity/TransformComponent.h>
#include <DX3D/Game/World.h>

namespace DX3D
{
	Entity::Entity() : m_transform(CreateComponent<TransformComponent>()) { }

	Entity::~Entity() {}

	void Entity::Release()
	{
		m_world->RemoveEntity(this);
	}

	void Entity::RemoveComponent(size_t typeId)
	{
		m_components.erase(typeId);
	}

	void Entity::CreateComponentInternal(Component* component, size_t typeId)
	{
		auto componentPtr = std::unique_ptr<Component>(component);
		m_components.emplace(typeId, std::move(componentPtr));
		component->m_typeId = typeId;
		component->m_entity = this;

		component->OnCreateInternal();
	}

	Component* Entity::GetComponentInternal(size_t typeId)
	{
		auto it = m_components.find(typeId);
		if (it == m_components.end())
			return nullptr;

		return it->second.get();
	}
}
