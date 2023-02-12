#include <DX3D/Resource/Font.h>
#include <DX3D/Resource/Material.h>
#include <DX3D/Resource/Mesh.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Resource/Texture.h>

namespace DX3D
{
	ResourceManager::ResourceManager(Game* game)
		: m_game(game) { }

	ResourceManager::~ResourceManager() {}

	ResourcePtr ResourceManager::CreateResourceFromFileConcrete(const wchar_t* file_path)
	{
		const std::filesystem::path resource_path = file_path;
		auto ext = resource_path.extension();

		auto it = m_map_resources.find(file_path);
		if (it != m_map_resources.end())
		{
			auto mat = std::dynamic_pointer_cast<Material>(it->second);
			if (mat)
				return std::make_shared<Material>(this, mat);
			return it->second;
		}

		if (resource_path.has_parent_path() && !exists(resource_path))
		{
			DX3DException("Resource doesn't exist: " << resource_path.string().c_str());
		}

		ResourcePtr res = nullptr;
		if (!ext.compare(L".obj"))
			res = std::make_shared<Mesh>(this, resource_path.c_str());
		else if (!ext.compare(L".jpg") || !ext.compare(L".png") || !ext.compare(L".bmp"))
			res = std::make_shared<Texture>(this, resource_path.c_str());
		else if (ext.empty() || !ext.compare(L".hlsl") || !ext.compare(L".fx"))
			res = std::make_shared<Material>(this, resource_path.c_str());
		else if (!ext.compare(L".font"))
			res = std::make_shared<Font>(this, resource_path.c_str());

		if (res)
		{
			m_map_resources.emplace(file_path, res);
			return res;
		}

		return nullptr;
	}
}
