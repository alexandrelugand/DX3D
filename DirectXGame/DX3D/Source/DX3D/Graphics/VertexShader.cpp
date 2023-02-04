#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/ShaderInclude.h>
#include <DX3D/Graphics/VertexShader.h>

namespace DX3D
{
	VertexShader::VertexShader(RenderSystem* render_system, const wchar_t* full_path, const char* entry_point)
		: m_render(render_system)
	{
		ComPtr<ID3DBlob> blob;
		ComPtr<ID3DBlob> error_blob;
		ShaderInclude shader_include("Assets/Shaders");

		if (FAILED(D3DCompileFromFile(full_path, nullptr, &shader_include, entry_point, "vs_5_0", 0, 0, &blob, &error_blob)))
		{
			if (error_blob)
			{
				DX3DException("Failed to create vertex shader (" << std::filesystem::path(full_path).string() << "): " << static_cast<char*>(error_blob->GetBufferPointer()));
			}
		}

		if (!blob)
		{
			DX3DException("Failed to create vertex shader (" << std::filesystem::path(full_path).string() << "): no byte code");
		}

		if (FAILED(m_render->m_d3d_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_shader)))
		{
			DX3DException("Failed to create vertex shader (" << std::filesystem::path(full_path).string() << ")");
		}
	}
}
