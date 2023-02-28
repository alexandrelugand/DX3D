#include <DX3D/Scene/RootLoader.h>
#include <DX3D/Scene/SceneManager.h>

#include <DX3D/Scene/SceneLoaderVisitor.h>

namespace DX3D
{
	SceneManager::SceneManager(World* world, ResourceManager* resource_manager)
		: m_world(world), m_resource_manager(resource_manager) { }

	SceneManager::~SceneManager() {}

	void SceneManager::Load(const wchar_t* file_path) const
	{
		const std::filesystem::path scene_path = file_path;

		if (!exists(scene_path))
		{
			DX3DException("Scene doesn't exist: " << scene_path.string().c_str());
		}

		std::ifstream fs(scene_path);
		json scene = json::parse(fs);

		SceneLoaderVisitor visitor(m_world, m_resource_manager);
		RootLoader root(scene);
		root.Accept(reinterpret_cast<ISceneLoaderVisitor*>(&visitor));
	}
}
