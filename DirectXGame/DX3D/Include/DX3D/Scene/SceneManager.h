#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class SceneManager
	{
	public:
		SceneManager(World* world, ResourceManager* resource_manager);
		virtual ~SceneManager();

		void Load(const wchar_t* file_path) const;

	private:
		World* m_world = nullptr;
		ResourceManager* m_resource_manager = nullptr;

		std::string m_version{};
	};
}
