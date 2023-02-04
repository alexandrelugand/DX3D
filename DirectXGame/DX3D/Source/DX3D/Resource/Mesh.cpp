#define TINYOBJLOADER_IMPLEMENTATION

#include <DX3D/Game/Game.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Resource/Mesh.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Vendor/tinyobjloader/include/tiny_obj_loader.h>

namespace DX3D
{
	Mesh::Mesh(ResourceManager* manager, const wchar_t* full_path)
		: Resource(manager, full_path)
	{
		tinyobj::attrib_t attribs;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warn;
		std::string err;

		//Convert wstring into ASCII string
		auto input_file = std::filesystem::path(full_path).string();

		std::string mtldir = input_file.substr(0, input_file.find_last_of("\\/"));

		if (!LoadObj(&attribs, &shapes, &materials, &warn, &err, input_file.c_str(), mtldir.c_str()) || !err.empty())
		{
			DX3DException("Failed to load mesh.");
		}

		std::vector<VertexMesh> list_vertices;
		std::vector<uint> list_indices;

		size_t vertex_buffer_size = 0;

		for (size_t s = 0; s < shapes.size(); s++)
		{
			vertex_buffer_size += shapes[s].mesh.indices.size();
		}

		//Reserve, in advance, size of vector to speed up insertion since each insertion leads to really big over hit
		list_vertices.reserve(vertex_buffer_size);
		list_indices.reserve(vertex_buffer_size);

		m_material_slots.resize(materials.size());

		size_t index_global_offset = 0;

		//Iterate each shape
		for (size_t m = 0; m < materials.size(); m++)
		{
			m_material_slots[m].start_index = index_global_offset;
			m_material_slots[m].material_id = m;

			//Iterate each shape
			for (size_t s = 0; s < shapes.size(); s++)
			{
				size_t index_offset = 0;

				//For faces of shape, get index vertex and find vertex coordinates
				for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
				{
					unsigned char num_face_vertices = shapes[s].mesh.num_face_vertices[f];

					if (shapes[s].mesh.material_ids[f] != static_cast<int>(m))
					{
						num_face_vertices = shapes[s].mesh.num_face_vertices[f];
						index_offset += num_face_vertices;
						continue;
					}

					Vector3D vertices_face[3];
					Vector2D texcoords_face[3];

					for (unsigned char v = 0; v < num_face_vertices; v++)
					{
						tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

						//Multiply index.vertex_index by 3 because the list of vertices stores each coordinates one after other without group them
						tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
						tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
						tinyobj::real_t vz = -attribs.vertices[index.vertex_index * 3 + 2];

						//Same things for texture coordinates
						tinyobj::real_t tx = 0;
						tinyobj::real_t ty = 0;
						if (!attribs.texcoords.empty())
						{
							tx = attribs.texcoords[index.texcoord_index * 2 + 0];
							ty = 1.0f - attribs.texcoords[index.texcoord_index * 2 + 1];
						}

						vertices_face[v] = Vector3D(vx, vy, vz);
						texcoords_face[v] = Vector2D(tx, ty);
					}

					Vector3D tangent, binormal;

					ComputeTangents(
						vertices_face[0], vertices_face[1], vertices_face[2],
						texcoords_face[0], texcoords_face[1], texcoords_face[2],
						tangent, binormal
					);

					for (size_t v = 0; v < num_face_vertices; v++)
					{
						tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

						//Multiply index.vertex_index by 3 because the list of vertices stores each coordinates one after other without group them
						tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
						tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
						tinyobj::real_t vz = -attribs.vertices[index.vertex_index * 3 + 2];

						//Same things for texture coordinates
						tinyobj::real_t tx = 0;
						tinyobj::real_t ty = 0;
						if (!attribs.texcoords.empty())
						{
							tx = attribs.texcoords[index.texcoord_index * 2 + 0];
							ty = 1.0f - attribs.texcoords[index.texcoord_index * 2 + 1];
						}

						//Same things for normals
						tinyobj::real_t nx = 0;
						tinyobj::real_t ny = 0;
						tinyobj::real_t nz = 0;
						if (!attribs.normals.empty())
						{
							nx = attribs.normals[index.normal_index * 3 + 0];
							ny = attribs.normals[index.normal_index * 3 + 1];
							nz = -attribs.normals[index.normal_index * 3 + 2];
						}

						auto v_position = Vector3D(vx, vy, vz);
						auto v_textcoord = Vector2D(tx, ty);
						auto v_normal = Vector3D(nx, ny, nz);
						Vector3D v_tangent, v_binormal;
						v_binormal = Vector3D::Cross(v_normal, tangent);
						v_tangent = Vector3D::Cross(v_binormal, v_normal);

						VertexMesh vertex(v_position, v_textcoord, v_normal, v_tangent, v_binormal);
						list_vertices.push_back(vertex);

						list_indices.push_back(static_cast<UINT>(index_global_offset + v));
					}

					index_offset += num_face_vertices;
					index_global_offset += num_face_vertices;
				}
			}

			m_material_slots[m].num_indices = index_global_offset - m_material_slots[m].start_index;
		}

		auto render_system = m_manager->GetGame()->GetGraphicsEngine()->GetRenderSystem();

		m_vertex_buffer = render_system->CreateVertexBuffer(&list_vertices[0], sizeof(VertexMesh), static_cast<UINT>(list_vertices.size()));
		m_index_buffer = render_system->CreateIndexBuffer(&list_indices[0], static_cast<UINT>(list_indices.size()));
	}

	Mesh::Mesh(ResourceManager* manager, VertexMesh* vertex_list_data, unsigned vertex_list_size, unsigned* index_list_data, unsigned index_list_size, MaterialSlot* material_slot_list, unsigned material_slot_list_size)
		: Resource(manager, L"")
	{
		const auto render_system = m_manager->GetGame()->GetGraphicsEngine()->GetRenderSystem();

		m_vertex_buffer = render_system->CreateVertexBuffer(vertex_list_data, sizeof(VertexMesh), vertex_list_size);
		m_index_buffer = render_system->CreateIndexBuffer(index_list_data, index_list_size);

		m_material_slots.resize(material_slot_list_size);

		for (uint i = 0; i < material_slot_list_size; i++)
		{
			m_material_slots[i] = material_slot_list[i];
		}
	}

	Mesh::~Mesh() {}

	void Mesh::ComputeTangents(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2, const Vector2D& t0, const Vector2D& t1, const Vector2D& t2, Vector3D& tangent, Vector3D& binormal)
	{
		const Vector3D deltaPos1 = v1 - v0;
		const Vector3D deltaPos2 = v2 - v0;

		const Vector2D deltaUV1 = t1 - t0;
		const Vector2D deltaUV2 = t2 - t0;

		const float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y);
		tangent = tangent * r;
		tangent = Vector3D::Normalize(tangent);
		binormal = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x);
		binormal = binormal * r;
		binormal = Vector3D::Normalize(binormal);
	}
}
