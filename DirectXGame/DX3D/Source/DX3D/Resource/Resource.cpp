#include <DX3D/Resource/Resource.h>

namespace DX3D
{
	Resource::Resource(ResourceManager* manager, const wchar_t* full_path)
		: m_manager(manager), m_full_path(full_path) { }

	Resource::~Resource() {}
}
