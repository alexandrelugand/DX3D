#include <DX3D/Entity/Entity.h>
#include <DX3D/Entity/TerrainComponent.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Game/World.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Math/VertexMesh.h>

namespace DX3D
{
	TerrainComponent::TerrainComponent() {}

	TerrainComponent::~TerrainComponent()
	{
		delete[] m_terrain_mesh_vertices;
		delete[] m_terrain_mesh_indices;
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->RemoveComponent(this);
	}

	void TerrainComponent::OnCreateInternal()
	{
		Component::OnCreateInternal();
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->AddComponent(this);
		GenerateMesh();
	}

	void TerrainComponent::GenerateMesh()
	{
		const size_t w = 512;
		const size_t h = 512;

		//Quads
		constexpr size_t ww = w - 1;
		constexpr size_t hh = h - 1;

		m_terrain_mesh_vertices = new VertexMesh[w * h]; // NOLINT(bugprone-implicit-widening-of-multiplication-result)
		m_terrain_mesh_indices = new uint[ww * hh * 6]; //6 is the number to render a single quad

		auto i = 0;
		for (size_t x = 0; x < w; x++)
		{
			for (size_t y = 0; y < h; y++)
			{
				m_terrain_mesh_vertices[y * w + x] = {
					Vector3D(static_cast<float>(x) / static_cast<float>(ww), 0, static_cast<float>(y) / static_cast<float>(hh)), //Position
					Vector2D(static_cast<float>(x) / static_cast<float>(ww), static_cast<float>(y) / static_cast<float>(hh)), //Texture
					Vector3D(), //Normal
					Vector3D(), //Tangent
					Vector3D(), //Binormal
				};

				if (x < ww && y < hh)
				{
					//Can compute the indices of quad
					//First triangle
					m_terrain_mesh_indices[i] = static_cast<uint>((y + 1) * w + x);
					m_terrain_mesh_indices[i + 1] = static_cast<uint>(y * w + x);
					m_terrain_mesh_indices[i + 2] = static_cast<uint>(y * w + (x + 1));

					//Second triangle
					m_terrain_mesh_indices[i + 3] = static_cast<uint>(y * w + (x + 1));
					m_terrain_mesh_indices[i + 4] = static_cast<uint>((y + 1) * w + (x + 1));
					m_terrain_mesh_indices[i + 5] = static_cast<uint>((y + 1) * w + x);

					i += 6; //6 indices calculated
				}
			}
		}

		const auto render = m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->GetRenderSystem();
		m_mesh_vb = render->CreateVertexBuffer(m_terrain_mesh_vertices, sizeof(VertexMesh), w * h);
		m_mesh_ib = render->CreateIndexBuffer(m_terrain_mesh_indices, ww * hh * 6);

		m_vs = render->CreateVertexShader(L"Assets/Shaders/Terrain/VertexShader.hlsl", "main");
		m_ps = render->CreatePixelShader(L"Assets/Shaders/Terrain/PixelShader.hlsl", "main");
	}

	void TerrainComponent::UpdateData(void* data, uint size)
	{
		const auto render = m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->GetRenderSystem();
		if (!m_cb)
			m_cb = render->CreateConstantBuffer(data, size);
		else
			m_cb->Update(render->GetImmediateDeviceContext(), data);
	}
}
