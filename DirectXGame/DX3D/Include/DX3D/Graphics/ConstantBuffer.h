#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class ConstantBuffer
	{
	public:
		ConstantBuffer(RenderSystem* render_system, void* buffer, uint size_buffer);

		void Update(const DeviceContextPtr& context, void* buffer);

	private:
		ComPtr<ID3D11Buffer> m_buffer = nullptr;
		RenderSystem* m_render = nullptr;

		friend class DeviceContext;
	};
}
