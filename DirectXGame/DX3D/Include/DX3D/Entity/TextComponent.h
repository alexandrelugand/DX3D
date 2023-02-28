#pragma once
#include <DX3D/Prerequisites.h>
#include "DX3D/Entity/Component.h"

namespace DX3D
{
	class TextComponent : public Component
	{
	public:
		TextComponent();
		~TextComponent() override;

		const FontPtr& GetFont() { return m_font; }
		void SetFont(const FontPtr& font) { m_font = font; }

		const wchar_t* GetText() { return m_text.c_str(); }
		void SetText(const wchar_t* text) { m_text = text; }

		const Vector4D& GetColor() { return m_color; }
		void SetColor(const Vector4D& color) { m_color = color; }

		Rect GetBounds();

	protected:
		void OnCreateInternal() override;

	private:
		std::wstring m_text;
		Vector4D m_color = Vector4D(1, 1, 1, 1);
		FontPtr m_font = nullptr;
	};
}
