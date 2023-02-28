#include <DX3D/Scene/RootLoader.h>

namespace DX3D
{
	RootLoader::RootLoader(const json& scene) : m_json(scene) {}

	void RootLoader::Accept(ISceneLoaderVisitor* visitor)
	{
		visitor->Visit(this);
	}
}
