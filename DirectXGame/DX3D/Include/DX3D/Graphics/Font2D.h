#pragma once
#include <DX3D/Prerequisites.h>


namespace DX3D
{
	class Font2D
	{
	public:
		Font2D(RenderSystem* system, const wchar_t* fullPath);
		~Font2D();

		Rect GetBounds(const wchar_t* text);

	private:
		std::unique_ptr<SpriteFont> m_font = nullptr;
		std::unique_ptr<SpriteBatch> m_batch = nullptr;

		RenderSystem* m_system = nullptr;

		friend class GraphicsEngine;
	};
}
