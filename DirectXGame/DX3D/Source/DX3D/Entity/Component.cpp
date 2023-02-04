#include <DX3D/Entity/Component.h>
#include <DX3D/Entity/Entity.h>

namespace DX3D
{
	Component::Component() {}

	Component::~Component() {}

	void Component::Release() const
	{
		m_entity->RemoveComponent(m_typeId);
	}

	void Component::OnCreateInternal() { }
}
