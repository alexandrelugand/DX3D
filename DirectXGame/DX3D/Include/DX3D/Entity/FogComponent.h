#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Entity/Component.h>

namespace DX3D
{
	class FogComponent final : public Component
	{
	public:
		FogComponent();
		~FogComponent() override;

		void SetData(const Vector4D& color, float start, float end)
		{
			m_color = color;
			m_start = start;
			m_end = end;
		}

		void SetColor(const Vector4D& color) { m_color = color; }
		const Vector4D& GetColor() { return m_color; }

		void SetStart(const float start) { m_start = start; }
		float GetStart() const { return m_start; }

		void SetEnd(const float end) { m_end = end; }
		float GetEnd() const { return m_end; }

	protected:
		void OnCreateInternal() override;

	private:
		Vector4D m_color = {0.5f, 0.5f, 0.5f, 1.0f};
		float m_start = 50.0f;
		float m_end = 300.0f;
	};
}
