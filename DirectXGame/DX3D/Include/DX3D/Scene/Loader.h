#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Scene/ILoader.h>
#include <DX3D/Scene/ISceneLoaderVisitor.h>

namespace DX3D
{
	template <class T>
	class Loader : public ILoader
	{
	public:
		Loader(T* item, const json& json) : m_item(item), m_json(json) {}

		void Accept(ISceneLoaderVisitor* visitor) override;

	protected:
		T* m_item;
		const json& m_json;

		friend class SceneLoaderVisitor;
	};

	template <class T>
	class PtrLoader : public ILoader
	{
	public:
		PtrLoader(T item, const json& json) : m_item(item), m_json(json) {}

		void Accept(ISceneLoaderVisitor* visitor) override;

	protected:
		T m_item;
		const json& m_json;

		friend class SceneLoaderVisitor;
	};

	template <class T>
	void Loader<T>::Accept(ISceneLoaderVisitor* visitor)
	{
		visitor->Visit(this);
	}

	template <class T>
	void PtrLoader<T>::Accept(ISceneLoaderVisitor* visitor)
	{
		visitor->Visit(this);
	}
}
