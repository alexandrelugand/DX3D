#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class VertexShader
	{
	public:
		VertexShader(RenderSystem* render_system, const wchar_t* full_path, const char* entry_point);

	private:
		ComPtr<ID3D11VertexShader> m_shader = nullptr;
		RenderSystem* m_render = nullptr;

		friend class RenderSystem;
		friend class DeviceContext;
	};
}
