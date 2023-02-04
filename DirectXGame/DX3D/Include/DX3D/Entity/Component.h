#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class Component
	{
	public:
		Component();
		virtual ~Component();

		Entity* GetEntity() const { return m_entity; }

		void Release() const;

	protected:
		size_t m_typeId = 0;
		Entity* m_entity = nullptr;

		virtual void OnCreateInternal();

		friend class Entity;
	};
}
