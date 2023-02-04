#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Resource/Resource.h>

namespace DX3D
{
	class Material : public Resource
	{
	public:
		Material(ResourceManager* manager, const wchar_t* shader);
		Material(ResourceManager* manager, const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
		Material(ResourceManager* manager, const MaterialPtr& material);

		void AddTexture(const TexturePtr& texture);
		void RemoveTexture(uint index);

		void SetData(void* data, uint size);

		CullMode GetCullMode() const { return m_cull_mode; }
		void SetCullMode(const CullMode& mode) { m_cull_mode = mode; }

	private:
		VertexShaderPtr m_vs = nullptr;
		PixelShaderPtr m_ps = nullptr;
		ConstantBufferPtr m_cb = nullptr;

		std::vector<Texture2DPtr> m_vec_textures;
		CullMode m_cull_mode = CullMode::Back;

		friend class GraphicsEngine;
	};
}
