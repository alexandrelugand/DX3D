// ReSharper disable CppUnusedIncludeDirective
// ReSharper disable CppClangTidyBugproneMacroParentheses
#pragma once

#include <cassert>
#include <chrono>
#include <codecvt>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <memory>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <json.hpp>
#include <SpriteFont.h>
#include <Windows.h>
#include <wrl.h>

#include <DX3D/Math/Matrix4x4.h>
#include <DX3D/Math/Rect.h>
#include <DX3D/Math/Vector2D.h>
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Math/VertexMesh.h>

#undef CreateFont

namespace DX3D
{
	using namespace Microsoft::WRL;
	using namespace DirectX::DX11;
	using json = nlohmann::json;

	class SwapChain;
	class DeviceContext;
	class VertexBuffer;
	class IndexBuffer;
	class ConstantBuffer;
	class VertexShader;
	class PixelShader;
	class Texture2D;
	class Font2D;
	class RenderSystem;

	class Resource;
	class Texture;
	class Mesh;
	class Material;
	class Font;

	class Window;
	class Display;
	class Game;
	class GraphicsEngine;
	class InputSystem;

	class World;
	class Entity;
	class Player;
	class Component;
	class TransformComponent;
	class MeshComponent;
	class CameraComponent;
	class LightComponent;
	class TerrainComponent;
	class WaterComponent;
	class FogComponent;
	class TextComponent;

	class ResourceManager;
	class TextureManager;
	class MeshManager;

	class SceneManager;
	class ISceneLoaderVisitor;
	class RootLoader;
	template <class T>
	class Loader;
	template <class T>
	class PtrLoader;

	using SwapChainPtr = std::shared_ptr<SwapChain>;
	using DeviceContextPtr = std::shared_ptr<DeviceContext>;
	using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
	using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
	using ConstantBufferPtr = std::shared_ptr<ConstantBuffer>;
	using VertexShaderPtr = std::shared_ptr<VertexShader>;
	using PixelShaderPtr = std::shared_ptr<PixelShader>;
	using Texture2DPtr = std::shared_ptr<Texture2D>;
	using Font2DPtr = std::shared_ptr<Font2D>;
	using ResourcePtr = std::shared_ptr<Resource>;
	using TexturePtr = std::shared_ptr<Texture>;
	using MeshPtr = std::shared_ptr<Mesh>;
	using MaterialPtr = std::shared_ptr<Material>;
	using FontPtr = std::shared_ptr<Font>;
	using EntityPtr = std::unique_ptr<Entity>;
	using ComponentPtr = std::unique_ptr<Component>;

	using EntityLoader = Loader<Entity>;
	using PlayerLoader = Loader<Player>;
	using MeshComponentLoader = Loader<MeshComponent>;
	using LightComponentLoader = Loader<LightComponent>;
	using TerrainComponentLoader = Loader<TerrainComponent>;
	using WaterComponentLoader = Loader<WaterComponent>;
	using FogComponentLoader = Loader<FogComponent>;
	using CameraComponentLoader = Loader<CameraComponent>;
	using MaterialLoader = PtrLoader<MaterialPtr>;
	using TransformComponentLoader = Loader<TransformComponent>;

	using uint = unsigned int;

	enum class CameraType
	{
		Orthogonal = 0,
		Perspective
	};

	enum class CullMode
	{
		None = 0,
		Front,
		Back
	};

	enum class Key
	{
		A = 0,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		_0,
		_1,
		_2,
		_3,
		_4,
		_5,
		_6,
		_7,
		_8,
		_9,
		Escape,
		Shift,
		Ctrl,
		Space,
		Enter,
		LeftMouse,
		MiddleMouse,
		RightMouse,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		PrintScreen
	};
}

#define DX3DException(message)\
std::stringstream m;\
m << "Error: " << message << std::endl;\
throw std::runtime_error(m.str())

#define DX3DError(message)\
std::wclog << "Error - [" << GetTimestamp().c_str() << "]: " << message << std::endl

#define DX3DWarning(message)\
std::wclog << "Warning - [" << GetTimestamp().c_str() << "]: " << message << std::endl

#define DX3DInfo(message)\
std::wclog << "Info - [" << GetTimestamp().c_str() << "]: " << message << std::endl

static std::string GetTimestamp()
{
	const auto now = std::chrono::system_clock::now();
	const auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
	const auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	std::stringstream nowSs;
	tm time{};
	localtime_s(&time, &nowAsTimeT);
	nowSs << std::put_time(&time, "%d-%m-%Y %T") << '.' << std::setfill('0') << std::setw(3) << nowMs.count();
	return nowSs.str();
}

static const char* UnicodeToChar(const wchar_t* str)
{
	// ReSharper disable CppDeprecatedEntity
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str).c_str(); // NOLINT(clang-diagnostic-deprecated-declarations, clang-diagnostic-return-stack-address)
}

static std::wstring StringToUnicode(const std::string& str)
{
	return std::wstring(str.begin(), str.end());
}

static std::filesystem::path ResolvePath(const wchar_t* base_dir, const wchar_t* file, bool raise_exception = false)
{
	const std::filesystem::path base_dir_path = base_dir;
	const std::filesystem::directory_entry base_dir_entry(base_dir_path);

	for (const auto& dir_entry : std::filesystem::recursive_directory_iterator(base_dir_entry))
	{
		auto test = dir_entry.path().parent_path() / std::filesystem::path(file);
		if (dir_entry == std::filesystem::directory_entry(test))
			return dir_entry.path();
	}

	if (raise_exception)
	{
		DX3DException("Failed to resolve shader path: " << std::filesystem::path(file).string());
	}

	return std::filesystem::path();
}
