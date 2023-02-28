#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Scene/ILoader.h>
#include <DX3D/Scene/ISceneLoaderVisitor.h>

namespace DX3D
{
	class RootLoader : public ILoader
	{
	public:
		RootLoader(const json& scene);

		void Accept(ISceneLoaderVisitor* visitor) override;

	protected:
		std::string m_version;
		const json& m_json;

		friend class SceneLoaderVisitor;
	};
}
