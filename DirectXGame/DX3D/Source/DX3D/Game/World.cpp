#include <DX3D/Entity/Entity.h>
#include <DX3D/Game/World.h>

namespace DX3D
{
	World::World(Game* game)
		: m_game(game) { }

	World::~World() { }

	void World::Update(float delta_time)
	{
		for (auto e : m_entities_to_destroy)
		{
			m_entities[e->m_typeId].erase(e);
		}
		m_entities_to_destroy.clear();

		for (auto& entities : m_entities | std::views::values)
		{
			for (const auto& ptr : entities | std::views::keys)
			{
				ptr->OnUpdate(delta_time);
			}
		}
	}

	void World::CreateEntityInternal(Entity* entity, size_t typeId)
	{
		auto entityPtr = std::unique_ptr<Entity>(entity);
		m_entities[typeId].emplace(entity, std::move(entityPtr));
		entity->m_typeId = typeId;
		entity->m_world = this;

		entity->OnCreate();
	}

	void World::RemoveEntity(Entity* entity)
	{
		m_entities_to_destroy.emplace(entity);
	}
}
