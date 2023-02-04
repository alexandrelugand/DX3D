#include <DX3D/Prerequisites.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Resource/Material.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Resource/Texture.h>

namespace DX3D
{
	Material::Material(ResourceManager* manager, const wchar_t* shader)
		: Resource(manager, L"")
	{
		std::filesystem::path shader_path = shader;
		if (shader_path.string().find('/') == std::string::npos)
		{
			shader_path = ResolvePath(L"Assets", shader);
			if (shader_path.empty())
				shader_path = ResolvePath(L"App", shader, true);
		}

		auto vertex_shader_file = (shader_path / L"VertexShader.hlsl");
		auto pixel_shader_file = (shader_path / L"PixelShader.hlsl");

		const auto render = m_manager->GetGame()->GetGraphicsEngine()->GetRenderSystem();

		m_vs = render->CreateVertexShader(vertex_shader_file.c_str(), "main");
		if (!m_vs)
		{
			DX3DException("Failed to create vertex shader in material.");
		}

		m_ps = render->CreatePixelShader(pixel_shader_file.c_str(), "main");
		if (!m_ps)
		{
			DX3DException("Failed to create pixel shader in material.");
		}
	}

	Material::Material(ResourceManager* manager, const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
		: Resource(manager, L"")
	{
		const auto render = m_manager->GetGame()->GetGraphicsEngine()->GetRenderSystem();

		m_vs = render->CreateVertexShader(vertex_shader_path, "main");
		if (!m_vs)
		{
			DX3DException("Failed to create vertex shader in material.");
		}

		m_ps = render->CreatePixelShader(pixel_shader_path, "main");
		if (!m_ps)
		{
			DX3DException("Failed to create pixel shader in material.");
		}
	}

	Material::Material(ResourceManager* manager, const MaterialPtr& material)
		: Resource(manager, L""), m_vs(material->m_vs), m_ps(material->m_ps) { }

	void Material::AddTexture(const TexturePtr& texture)
	{
		m_vec_textures.push_back(texture->m_texture);
	}

	void Material::RemoveTexture(uint index)
	{
		if (index >= m_vec_textures.size())
			return;

		m_vec_textures.erase(m_vec_textures.begin() + index);
	}

	void Material::SetData(void* data, uint size)
	{
		const auto render = m_manager->GetGame()->GetGraphicsEngine()->GetRenderSystem();

		if (!m_cb)
			m_cb = render->CreateConstantBuffer(data, size);
		else
			m_cb->Update(render->GetImmediateDeviceContext(), data);
	}
}
