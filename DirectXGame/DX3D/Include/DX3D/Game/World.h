#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class World
	{
	public:
		World(Game* game);
		~World();

		Game* GetGame() const { return m_game; }
		void Update(float delta_time);

		template <typename T>
		T* CreateEntity()
		{
			static_assert(std::is_base_of_v<Entity, T>, "Type must derive from Entity class");
			auto typeId = typeid(T).hash_code();
			auto entity = new T();
			CreateEntityInternal(entity, typeId);
			return entity;
		}

	private:
		std::map<size_t, std::map<Entity*, EntityPtr>> m_entities;
		std::set<Entity*> m_entities_to_destroy;
		Game* m_game = nullptr;

		void CreateEntityInternal(Entity* entity, size_t typeId);
		void RemoveEntity(Entity* entity);

		friend class Entity;
	};
}
