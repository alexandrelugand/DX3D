// ReSharper disable CppClangTidyConcurrencyMtUnsafe
#include <time.h>

#include "DemoGame.h"

#include <DX3D/Game/Display.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Scene/SceneManager.h>

using namespace DX3D;

DemoGame::DemoGame(Options& options) : m_options(options) {}

void DemoGame::OnCreate()
{
	Game::OnCreate();

	srand(static_cast<uint>(time(nullptr))); // NOLINT(cert-msc51-cpp)

	FontUtility::CreateFont(L"Bahnschrift", 16, L"Assets\\Fonts\\");


	if (m_options.HasOption("-s"))
		GetSceneManager()->Load(StringToUnicode(m_options.GetOption("-s")).c_str());
	else if (m_options.HasOption("--scene"))
		GetSceneManager()->Load(StringToUnicode(m_options.GetOption("--scene")).c_str());

#pragma region UI

	const auto font = GetResourceManager()->CreateResourceFromFile<Font>(L"Assets/Fonts/Bahnschrift.font");
	m_infos = GetWorld()->CreateEntity<Entity>();
	m_version_info = GetWorld()->CreateEntity<Entity>();
	const auto text = m_infos->CreateComponent<TextComponent>();
	text->SetFont(font);
	text->SetColor(Vector4D(0.2980f, 1.0f, 0.0f, 1.0f));
	text->SetText(L"DX3D Game Engine");
	m_infos->GetTransform()->SetPosition(Vector3D(10, 10, 0));
	const auto version = m_version_info->CreateComponent<TextComponent>();
	version->SetFont(font);
	version->SetText(m_version.c_str());

#pragma endregion

	m_player = GetWorld()->GetEntity<Player>("Player");
	GetInputSystem()->LockCursor(m_locked);
}

void DemoGame::OnUpdate(float delta_time)
{
	Game::OnUpdate(delta_time);

	const auto client_size = GetDisplay()->GetClientSize();
	const auto text_component = m_infos->GetComponent<TextComponent>();


	auto infoText = std::wstringstream();
	infoText << L"DX3D Game Engine\n\n";
	infoText << L"FPS: " << static_cast<int>(1.0f / delta_time) << L"\n";
	infoText << L"Screen size: W: " << client_size.Width() << L" H: " << client_size.Height() << L"\n";

	if (m_player)
	{
		const auto player_pos = m_player->GetTransform()->GetPosition();
		const auto player_rot = m_player->GetTransform()->GetRotation();
		constexpr auto p = 180.0f / 3.14f;
		infoText << L"Player position:"
			<< L" X: " << static_cast<int>(player_pos.x)
			<< L" Y: " << static_cast<int>(player_pos.y)
			<< L" Z: " << static_cast<int>(player_pos.z)
			<< L"\n";
		infoText << L"Player rotation:"
			<< L" X: " << static_cast<int>(player_rot.x * p)
			<< L" Y: " << static_cast<int>(player_rot.y * p)
			<< L" Z: " << static_cast<int>(player_rot.z * p)
			<< L"\n";
	}

	text_component->SetText(infoText.str().c_str());

	const auto textBounds = m_version_info->GetComponent<TextComponent>()->GetBounds();
	m_version_info->GetTransform()->SetPosition(Vector3D(
		client_size.Width() - textBounds.Width() - 15.0f,
		client_size.Height() - textBounds.Height() - 10.0f,
		0
	));

	const auto input_system = GetInputSystem();
	if (input_system->IsKeyUp(Key::P))
	{
		m_locked = !m_locked;
		input_system->LockCursor(m_locked);
	}
	else if (input_system->IsKeyUp(Key::Escape))
	{
		Quit();
	}
	else if (input_system->IsKeyUp(Key::F1))
	{
		m_fullscreen = !m_fullscreen;
		GetDisplay()->SetFullScreen(m_fullscreen);
	}
	else if (input_system->IsKeyUp(Key::F2))
	{
		GetGraphicsEngine()->SetWireFrameMode(!GetGraphicsEngine()->GetWireFrameMode());
	}
}
