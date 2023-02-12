#include <DX3D/Graphics/Font2D.h>
#include <DX3D/Graphics/RenderSystem.h>

namespace DX3D
{
	Font2D::Font2D(RenderSystem* system, const wchar_t* fullPath)
		: m_system(system)
	{
		m_font = std::make_unique<SpriteFont>(system->m_d3d_device.Get(), fullPath);
		if (!m_font)
		{
			DX3DException("Failed to create sprite font. Creation of Font2D failed");
		}

		m_batch = std::make_unique<SpriteBatch>(system->m_imm_context.Get());
		if (!m_batch)
		{
			DX3DException("Failed to create sprite batch. Creation of Font2D failed");
		}
	}

	Font2D::~Font2D() {}

	Rect Font2D::GetBounds(const wchar_t* text)
	{
		const auto rc = m_font->MeasureDrawBounds(text, DirectX::XMFLOAT2(0, 0));
		return Rect(rc);
	}
}
