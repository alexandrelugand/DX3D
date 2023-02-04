#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class Resource
	{
	public:
		Resource(ResourceManager* manager, const wchar_t* full_path);
		virtual ~Resource();

	protected:
		ResourceManager* m_manager = nullptr;
		std::wstring m_full_path;
	};
}
