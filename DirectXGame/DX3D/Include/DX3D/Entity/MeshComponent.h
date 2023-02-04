#pragma once
#include <DX3D/Entity/Component.h>

namespace DX3D
{
	class MeshComponent final : public Component
	{
	public:
		MeshComponent();
		~MeshComponent() override;

		void SetMesh(const MeshPtr& mesh) { m_mesh = mesh; }
		const MeshPtr& GetMesh() { return m_mesh; }

		void AddMaterial(const MaterialPtr& material);
		void RemoveMaterial(uint index);

		const std::vector<MaterialPtr>& GetMaterials() { return m_materials; }

	protected:
		void OnCreateInternal() override;

	private:
		MeshPtr m_mesh;
		std::vector<MaterialPtr> m_materials;
	};
}
