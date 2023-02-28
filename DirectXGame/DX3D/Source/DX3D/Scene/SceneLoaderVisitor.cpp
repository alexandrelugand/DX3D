#include <DX3D/Version.h>
#include <DX3D/Entity/Entity.h>
#include <DX3D/Entity/FogComponent.h>
#include <DX3D/Entity/LightComponent.h>
#include <DX3D/Entity/MeshComponent.h>
#include <DX3D/Entity/TerrainComponent.h>
#include <DX3D/Entity/TransformComponent.h>
#include <DX3D/Entity/WaterComponent.h>
#include <DX3D/Game/World.h>
#include <DX3D/Resource/Material.h>
#include <DX3D/Resource/Mesh.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Resource/Texture.h>
#include <DX3D/Scene/Loader.h>
#include <DX3D/Scene/RootLoader.h>
#include <DX3D/Scene/SceneLoaderVisitor.h>

namespace DX3D
{
	SceneLoaderVisitor::SceneLoaderVisitor(World* world, ResourceManager* resource_manager) : m_world(world), m_resource_manager(resource_manager) {}

	void SceneLoaderVisitor::Visit(RootLoader* loader)
	{
		if (!loader->m_json.contains("version"))
		{
			DX3DException("Invalid scene file format: missing 'version' field");
		}

		loader->m_version = loader->m_json["version"];
		if (loader->m_version != DX3D_VERSION)
		{
			DX3DWarning("File format version (" << loader->m_version.c_str() << ") is not the same as program (" << DX3D_VERSION << "). This could cause loading errors.");
		}

		if (loader->m_json.contains("entities"))
		{
			for (auto jentity_item : loader->m_json["entities"].items())
			{
				const auto& jentity = jentity_item.value();
				const auto entity = m_world->CreateEntity<Entity>();
				auto entity_loader = Loader(entity, jentity);
				entity_loader.Accept(this);
			}
		}
	}

	void SceneLoaderVisitor::Visit(EntityLoader* loader)
	{
		if (!loader->m_json.contains("type"))
		{
			DX3DException("Invalid scene file format: missing 'type' field in Entity node");
		}
		std::string type = loader->m_json["type"];

		if (loader->m_json.contains("name"))
			loader->m_item->SetName(loader->m_json["name"]);

		if (loader->m_json.contains("components"))
		{
			for (auto jcomponent_item : loader->m_json["components"].items())
			{
				const auto& jcomponent = jcomponent_item.value();
				if (!jcomponent.contains("type"))
				{
					DX3DException("Invalid scene file format: missing 'type' field in Component node");
				}

				type = jcomponent["type"];
				if (type == "Mesh")
				{
					auto mesh_component = loader->m_item->CreateComponent<MeshComponent>();
					auto component_loader = Loader(mesh_component, jcomponent);
					component_loader.Accept(this);
				}
				else if (type == "Light")
				{
					auto light_component = loader->m_item->CreateComponent<LightComponent>();
					auto component_loader = Loader(light_component, jcomponent);
					component_loader.Accept(this);
				}
				else if (type == "Terrain")
				{
					auto terrain_component = loader->m_item->CreateComponent<TerrainComponent>();
					auto component_loader = Loader(terrain_component, jcomponent);
					component_loader.Accept(this);
				}
				else if (type == "Water")
				{
					auto water_component = loader->m_item->CreateComponent<WaterComponent>();
					auto component_loader = Loader(water_component, jcomponent);
					component_loader.Accept(this);
				}
				else if (type == "Fog")
				{
					auto fog_component = loader->m_item->CreateComponent<FogComponent>();
					auto component_loader = Loader(fog_component, jcomponent);
					component_loader.Accept(this);
				}
			}
		}

		if (loader->m_json.contains("transform"))
		{
			const auto& jtransform = loader->m_json["transform"];
			auto transform_loader = Loader(loader->m_item->GetTransform(), jtransform);
			transform_loader.Accept(this);
		}
	}

	void SceneLoaderVisitor::Visit(MeshComponentLoader* loader)
	{
		if (!loader->m_json.contains("path"))
		{
			DX3DException("Invalid scene file format: missing 'path' field in Mesh component");
		}

		std::wstring path = StringToUnicode(loader->m_json["path"]);
		const auto mesh = m_resource_manager->CreateResourceFromFile<Mesh>(path.c_str());
		loader->m_item->SetMesh(mesh);

		if (loader->m_json.contains("materials"))
		{
			for (auto jmaterial_item : loader->m_json["materials"].items())
			{
				const auto& jmaterial = jmaterial_item.value();
				if (jmaterial.contains("name"))
				{
					path = StringToUnicode(jmaterial["name"]);
				}
				else if (jmaterial.contains("path"))
				{
					path = StringToUnicode(jmaterial["path"]);
				}
				else
				{
					DX3DException("Invalid scene file format: missing 'name' or 'path' fields in Material");
				}

				auto material = m_resource_manager->CreateResourceFromFile<Material>(path.c_str());
				auto material_loader = PtrLoader(material, jmaterial);
				material_loader.Accept(this);
				loader->m_item->AddMaterial(material);
			}
		}
	}

	void SceneLoaderVisitor::Visit(LightComponentLoader* loader)
	{
		if (!loader->m_json.contains("color"))
		{
			DX3DException("Invalid scene file format: missing 'color' field in Light component");
		}

		const auto& jlight = loader->m_json["color"];
		const Vector4D color(jlight["r"], jlight["g"], jlight["b"], jlight["a"]);
		loader->m_item->SetColor(color);
	}

	void SceneLoaderVisitor::Visit(TerrainComponentLoader* loader)
	{
		if (!loader->m_json.contains("heightMap"))
		{
			DX3DException("Invalid scene file format: missing 'heightMap' field in Terrain component");
		}

		if (!loader->m_json.contains("ground"))
		{
			DX3DException("Invalid scene file format: missing 'ground' field in Terrain component");
		}

		if (!loader->m_json.contains("wall"))
		{
			DX3DException("Invalid scene file format: missing 'wall' field in Terrain component");
		}

		const auto& jheight_map = loader->m_json["heightMap"];
		std::wstring heightMap_path = StringToUnicode(jheight_map["path"]);
		auto heightMap = m_resource_manager->CreateResourceFromFile<Texture>(heightMap_path.c_str());
		loader->m_item->SetHeightMap(heightMap);

		const auto& jground = loader->m_json["ground"];
		std::wstring ground_path = StringToUnicode(jground["path"]);
		auto ground = m_resource_manager->CreateResourceFromFile<Texture>(ground_path.c_str());
		loader->m_item->SetGroundMap(ground);

		const auto& jwall = loader->m_json["wall"];
		std::wstring wall_path = StringToUnicode(jwall["path"]);
		auto wall = m_resource_manager->CreateResourceFromFile<Texture>(wall_path.c_str());
		loader->m_item->SetWallMap(wall);
	}

	void SceneLoaderVisitor::Visit(WaterComponentLoader* loader)
	{
		if (!loader->m_json.contains("heightMap"))
		{
			DX3DException("Invalid scene file format: missing 'heightMap' field in Water component");
		}

		const auto& jheight_map = loader->m_json["heightMap"];
		const std::wstring heightMap_path = StringToUnicode(jheight_map["path"]);
		const auto heightMap = m_resource_manager->CreateResourceFromFile<Texture>(heightMap_path.c_str());
		loader->m_item->SetWaveHeightMap(heightMap);
	}

	void SceneLoaderVisitor::Visit(FogComponentLoader* loader)
	{
		if (!loader->m_json.contains("data"))
		{
			DX3DException("Invalid scene file format: missing 'data' field in Fog component");
		}

		const auto& jdata = loader->m_json["data"];
		const auto& jcolor = jdata["color"];
		const Vector4D color(jcolor["r"], jcolor["g"], jcolor["b"], jcolor["a"]);
		const float start = jdata["start"];
		const float end = jdata["end"];
		loader->m_item->SetData(color, start, end);
	}

	void SceneLoaderVisitor::Visit(MaterialLoader* loader)
	{
		if (loader->m_json.contains("cullmode"))
			loader->m_item->SetCullMode(loader->m_json["cullmode"]);

		if (loader->m_json.contains("textures"))
		{
			for (auto jtexture_item : loader->m_json["textures"].items())
			{
				const auto& jtexture = jtexture_item.value();
				if (!jtexture.contains("path"))
				{
					DX3DException("Invalid scene file format: missing 'path' field in Texture node");
				}

				std::wstring path = StringToUnicode(jtexture["path"]);
				auto texture = m_resource_manager->CreateResourceFromFile<Texture>(path.c_str());
				loader->m_item->AddTexture(texture);
			}
		}
	}

	void SceneLoaderVisitor::Visit(TransformComponentLoader* loader)
	{
		if (loader->m_json.contains("position"))
		{
			const auto& jposition = loader->m_json["position"];
			const Vector3D position(jposition["x"], jposition["y"], jposition["z"]);
			loader->m_item->SetPosition(position);
		}

		if (loader->m_json.contains("rotation"))
		{
			const auto& jrotation = loader->m_json["rotation"];
			const Vector3D rotation(jrotation["x"], jrotation["y"], jrotation["z"]);
			loader->m_item->SetRotation(rotation);
		}

		if (loader->m_json.contains("scale"))
		{
			const auto& jscale = loader->m_json["scale"];
			const Vector3D scale(jscale["x"], jscale["y"], jscale["z"]);
			loader->m_item->SetScale(scale);
		}
	}
}
