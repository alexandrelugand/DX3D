#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class ILoader
	{
	public:
		virtual ~ILoader() = default;
		virtual void Accept(ISceneLoaderVisitor* visitor) = 0;
	};
}
