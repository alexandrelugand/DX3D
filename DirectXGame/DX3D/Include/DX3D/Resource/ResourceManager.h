#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class ResourceManager
	{
	public:
		ResourceManager(Game* game);
		~ResourceManager();

		template <typename T>
		std::shared_ptr<T> CreateResourceFromFile(const wchar_t* file_path)
		{
			return std::dynamic_pointer_cast<T>(CreateResourceFromFileConcrete(file_path));
		}

		Game* GetGame() const { return m_game; }

	private:
		Game* m_game = nullptr;
		std::unordered_map<std::wstring, ResourcePtr> m_map_resources;

		ResourcePtr CreateResourceFromFileConcrete(const wchar_t* file_path);
	};
}
