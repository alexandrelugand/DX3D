#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Entity/Component.h>

namespace DX3D
{
	class LightComponent final : public Component
	{
	public:
		LightComponent();
		~LightComponent() override;

		Vector4D GetColor() const { return m_color; }
		void SetColor(const Vector4D& color) { m_color = color; }

	protected:
		void OnCreateInternal() override;

	private:
		Vector4D m_color = {1.0f, 1.0f, 1.0f, 1.0f};
	};
}
