#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Game/World.h>

namespace DX3D
{
	class Entity
	{
	public:
		Entity();
		virtual ~Entity();

		World* GetWorld() const { return m_world; }
		InputSystem* GetInputSystem() const { return m_world->GetGame()->GetInputSystem(); }
		TransformComponent* GetTransform() const { return m_transform; }

		void Release();

		template <typename T>
		T* CreateComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "Type must derive from Component class");
			auto c = GetComponent<T>();
			if (!c)
			{
				auto typeId = typeid(T).hash_code();
				auto component = new T();
				CreateComponentInternal(component, typeId);
				return component;
			}
			return nullptr;
		}

		template <typename T>
		T* GetComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "Type must derive from Component class");
			const auto typeId = typeid(T).hash_code();
			return static_cast<T*>(GetComponentInternal(typeId));
		}

		void RemoveComponent(size_t typeId);

	protected:
		size_t m_typeId = 0;
		World* m_world = nullptr;
		std::map<size_t, ComponentPtr> m_components;
		TransformComponent* m_transform = nullptr;

		void CreateComponentInternal(Component* component, size_t typeId);
		Component* GetComponentInternal(size_t typeId);

		virtual void OnCreate() {}
		virtual void OnUpdate(float delta_time) {}

		friend class World;
	};
}
