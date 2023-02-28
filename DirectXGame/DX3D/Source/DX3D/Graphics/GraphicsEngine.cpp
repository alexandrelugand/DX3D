#include <DX3D/Entity/CameraComponent.h>
#include <DX3D/Entity/Entity.h>
#include <DX3D/Entity/FogComponent.h>
#include <DX3D/Entity/LightComponent.h>
#include <DX3D/Entity/MeshComponent.h>
#include <DX3D/Entity/TerrainComponent.h>
#include <DX3D/Entity/TextComponent.h>
#include <DX3D/Entity/TransformComponent.h>
#include <DX3D/Entity/WaterComponent.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Graphics/CommonShader.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/Font2D.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Math/Matrix4x4.h>
#include <DX3D/Resource/Font.h>
#include <DX3D/Resource/Material.h>
#include <DX3D/Resource/Mesh.h>
#include <DX3D/Resource/Texture.h>

namespace DX3D
{
	GraphicsEngine::GraphicsEngine(Game* game)
		: m_game(game)
	{
		m_render = std::make_unique<RenderSystem>();
	}

	GraphicsEngine::~GraphicsEngine() {}

	void GraphicsEngine::Update()
	{
		auto swap_chain = m_game->GetDisplay()->GetSwapChain();
		auto win_size = m_game->GetDisplay()->GetClientSize();
		auto context = m_render->GetImmediateDeviceContext();
		m_render->ClearState();
		m_render->SetWireFrameMode(m_wire_frame);

		ConstantData constantData = {};
		constantData.time = m_game->GetTotalTime();

		auto fogColor = Vector4D(0, 0, 0, 0);

		for (auto f : m_fogs)
		{
			constantData.fog.enable = true;
			constantData.fog.color = fogColor = f->GetColor();
			constantData.fog.start = f->GetStart();
			constantData.fog.end = f->GetEnd();
		}

		context->ClearRenderTargetColor(swap_chain, fogColor.x, fogColor.y, fogColor.z, 1.0f);
		context->SetViewportSize(win_size);

		for (auto c : m_cameras)
		{
			c->SetScreenArea(win_size);
			c->GetViewMatrix(constantData.camera.view);
			c->GetProjectionMatrix(constantData.camera.proj);

			constantData.camera.position = c->GetEntity()->GetTransform()->GetPosition();
		}

		for (auto l : m_lights)
		{
			auto transform = l->GetEntity()->GetTransform();
			Matrix4x4 w;
			transform->GetWorldMatrix(w);

			constantData.light.color = l->GetColor();
			constantData.light.direction = w.GetZDirection();
		}

		for (auto m : m_meshes)
		{
			const auto transform = m->GetEntity()->GetTransform();
			transform->GetWorldMatrix(constantData.world);

			const auto mesh = m->GetMesh().get();
			const auto materials = m->GetMaterials();

			context->SetVertexBuffer(mesh->m_vertex_buffer);
			context->SetIndexBuffer(mesh->m_index_buffer);

			for (size_t i = 0; i < mesh->GetNumMaterialSlots(); i++)
			{
				if (i >= materials.size())
					break;

				const auto material = materials[i];
				if (!m_wire_frame)
					m_render->SetCullMode(material->GetCullMode());

				material->SetData(&constantData, sizeof(ConstantData));
				context->SetConstantBuffer(material->m_cb);
				context->SetVertexShader(material->m_vs);
				context->SetPixelShader(material->m_ps);

				if (material->m_vec_textures.size())
					context->SetTexture(&material->m_vec_textures[0], material->m_vec_textures.size());

				const auto slot = mesh->GetMaterialSlot(i);
				context->DrawIndexedTriangleList(slot.num_indices, slot.start_index, 0);
			}
		}

		//At the end for apply correctly AlphaBlending
		for (auto t : m_terrains)
		{
			auto transform = t->GetEntity()->GetTransform();
			transform->GetWorldMatrix(constantData.world);
			constantData.terrain.size = t->GetSize();
			constantData.terrain.heightMapSize = static_cast<float>(t->GetHeightMap()->GetTexture()->GetSize().Width());

			context->SetVertexBuffer(t->m_mesh_vb);
			context->SetIndexBuffer(t->m_mesh_ib);

			if (!m_wire_frame)
				m_render->SetCullMode(CullMode::Back);
			t->UpdateData(&constantData, sizeof(ConstantData));
			context->SetConstantBuffer(t->m_cb);

			context->SetVertexShader(t->m_vs);
			context->SetPixelShader(t->m_ps);

			Texture2DPtr terrainTextures[3];
			terrainTextures[0] = t->GetHeightMap()->GetTexture();
			terrainTextures[1] = t->GetGroundMap()->GetTexture();
			terrainTextures[2] = t->GetWallMap()->GetTexture();
			context->SetTexture(terrainTextures, 3);

			context->DrawIndexedTriangleList(t->m_mesh_ib->GetSizeIndexList(), 0, 0);
		}

		for (auto w : m_waters)
		{
			auto transform = w->GetEntity()->GetTransform();
			transform->GetWorldMatrix(constantData.world);
			constantData.water.size = w->GetSize();
			constantData.water.heightMapSize = static_cast<float>(w->GetWaveHeightMap()->GetTexture()->GetSize().Width());

			context->SetVertexBuffer(w->m_mesh_vb);
			context->SetIndexBuffer(w->m_mesh_ib);

			if (!m_wire_frame)
				m_render->SetCullMode(CullMode::Back);
			w->UpdateData(&constantData, sizeof(ConstantData));
			context->SetConstantBuffer(w->m_cb);

			context->SetVertexShader(w->m_vs);
			context->SetPixelShader(w->m_ps);

			Texture2DPtr waterTextures[1];
			waterTextures[0] = w->GetWaveHeightMap()->GetTexture();
			context->SetTexture(waterTextures, 1);

			context->DrawIndexedTriangleList(w->m_mesh_ib->GetSizeIndexList(), 0, 0);
		}

		//Rendering UI components
		//-----------------------

		for (auto t : m_texts)
		{
			auto transform = t->GetEntity()->GetTransform();
			auto pos = transform->GetPosition();
			auto color = t->GetColor();
			auto font = t->GetFont()->GetFont();

			font->m_batch->Begin();
			font->m_font->DrawString(font->m_batch.get(), t->GetText(), DirectX::XMFLOAT2(pos.x, pos.y), DirectX::XMVECTORF32({color.x, color.y, color.z, color.w}));
			font->m_batch->End();
		}

		swap_chain->Present(true);
	}

	void GraphicsEngine::AddComponent(Component* component)
	{
		if (auto c = dynamic_cast<MeshComponent*>(component))
			m_meshes.emplace(c);
		else if (auto camera = dynamic_cast<CameraComponent*>(component))
			m_cameras.emplace(camera);
		else if (auto light = dynamic_cast<LightComponent*>(component))
			m_lights.emplace(light);
		else if (auto terrain = dynamic_cast<TerrainComponent*>(component))
			m_terrains.emplace(terrain);
		else if (auto water = dynamic_cast<WaterComponent*>(component))
			m_waters.emplace(water);
		else if (auto fog = dynamic_cast<FogComponent*>(component))
			m_fogs.emplace(fog);
		else if (auto text = dynamic_cast<TextComponent*>(component))
			m_texts.emplace(text);
	}

	void GraphicsEngine::RemoveComponent(Component* component)
	{
		if (auto c = dynamic_cast<MeshComponent*>(component))
			m_meshes.erase(c);
		else if (auto camera = dynamic_cast<CameraComponent*>(component))
			m_cameras.erase(camera);
		else if (auto light = dynamic_cast<LightComponent*>(component))
			m_lights.erase(light);
		else if (auto terrain = dynamic_cast<TerrainComponent*>(component))
			m_terrains.erase(terrain);
		else if (auto water = dynamic_cast<WaterComponent*>(component))
			m_waters.erase(water);
		else if (auto fog = dynamic_cast<FogComponent*>(component))
			m_fogs.erase(fog);
		else if (auto text = dynamic_cast<TextComponent*>(component))
			m_texts.erase(text);
	}
}
