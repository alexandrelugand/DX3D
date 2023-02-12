// ReSharper disable CppClangTidyConcurrencyMtUnsafe
// ReSharper disable CppClangTidyCertMsc51Cpp
#include <time.h>

#include "DemoGame.h"
#include "Entity/Player.h"

#include <DX3D/Entity/LightComponent.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Graphics/GraphicsEngine.h>

using namespace DX3D;

void DemoGame::OnCreate()
{
	Game::OnCreate();

	srand(static_cast<uint>(time(nullptr)));

	FontUtility::CreateFont(L"Bahnschrift", 16, L"Assets\\Fonts\\");

	const auto world = GetWorld();

#pragma region SkyBox

	auto sky_mesh = GetResourceManager()->CreateResourceFromFile<Mesh>(L"Assets/Meshes/sphere_hq.obj");
	auto sky_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/sky.jpg");
	auto sky_material = GetResourceManager()->CreateResourceFromFile<Material>(L"SkyBox");
	sky_material->AddTexture(sky_tex);
	sky_material->SetCullMode(CullMode::Front);

	auto sky = world->CreateEntity<Entity>();
	auto sky_component = sky->CreateComponent<MeshComponent>();
	sky_component->SetMesh(sky_mesh);
	sky_component->AddMaterial(sky_material);
	sky->GetTransform()->SetScale(Vector3D(1000.0f, 1000.0f, 1000.0f));

#pragma endregion

#pragma region Sponza

	auto sponza_mesh = GetResourceManager()->CreateResourceFromFile<Mesh>(L"Assets/Meshes/sponza_basic.obj");
	auto sponza_arch_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/Sponza/sponza_arch_diff.jpg");
	auto sponza_arch_material = GetResourceManager()->CreateResourceFromFile<Material>(L"SpecularLight");
	sponza_arch_material->AddTexture(sponza_arch_tex);

	auto sponza_bricks_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/Sponza/sponza_bricks_a_diff.jpg");
	auto sponza_bricks_material = GetResourceManager()->CreateResourceFromFile<Material>(L"SpecularLight");
	sponza_bricks_material->AddTexture(sponza_bricks_tex);

	auto sponza_column_a_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/Sponza/sponza_column_a_diff.jpg");
	auto sponza_column_a_material = GetResourceManager()->CreateResourceFromFile<Material>(L"SpecularLight");
	sponza_column_a_material->AddTexture(sponza_column_a_tex);

	auto sponza_column_b_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/Sponza/sponza_column_b_diff.jpg");
	auto sponza_column_b_material = GetResourceManager()->CreateResourceFromFile<Material>(L"SpecularLight");
	sponza_column_b_material->AddTexture(sponza_column_b_tex);

	auto sponza_column_c_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/Sponza/sponza_column_c_diff.jpg");
	auto sponza_column_c_material = GetResourceManager()->CreateResourceFromFile<Material>(L"SpecularLight");
	sponza_column_c_material->AddTexture(sponza_column_c_tex);

	auto sponza_flagpole_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/Sponza/sponza_flagpole_diff.jpg");
	auto sponza_flagpole_material = GetResourceManager()->CreateResourceFromFile<Material>(L"SpecularLight");
	sponza_flagpole_material->AddTexture(sponza_flagpole_tex);

	auto sponza = world->CreateEntity<Entity>();
	auto sponza_component = sponza->CreateComponent<MeshComponent>();
	sponza_component->SetMesh(sponza_mesh);
	sponza_component->AddMaterial(sponza_arch_material);
	sponza_component->AddMaterial(sponza_bricks_material);
	sponza_component->AddMaterial(sponza_column_a_material);
	sponza_component->AddMaterial(sponza_column_b_material);
	sponza_component->AddMaterial(sponza_column_c_material);
	sponza_component->AddMaterial(sponza_flagpole_material);
	sponza->GetTransform()->SetPosition(Vector3D(145.0f, 24.4f, 140.0f));
	sponza->GetTransform()->SetRotation(Vector3D(0.0f, -0.335f, 0.0f));

#pragma endregion

#pragma region Box

	auto box_mesh = GetResourceManager()->CreateResourceFromFile<Mesh>(L"Assets/Meshes/box.obj");
	auto box_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/wood.jpg");
	auto box_material = GetResourceManager()->CreateResourceFromFile<Material>(L"Base");
	box_material->AddTexture(box_tex);

	for (int x = -2; x <= 2; x++)
	{
		for (int y = -2; y <= 2; y++)
		{
			auto box = world->CreateEntity<Entity>();
			auto box_component = box->CreateComponent<MeshComponent>();
			box_component->SetMesh(box_mesh);
			box_component->AddMaterial(box_material);
			box->GetTransform()->SetPosition(Vector3D(145.0f + static_cast<float>(rand() % 20) - 10.0f, 24.4f, 155.0f + static_cast<float>(rand() % 20) - 10.0f));
			box->GetTransform()->SetRotation(Vector3D(0, (static_cast<float>(rand() % 628)) / 100.0f, 0));
		}
	}

#pragma endregion

#pragma region Statue

	auto statue_mesh = GetResourceManager()->CreateResourceFromFile<Mesh>(L"Assets/Meshes/Winged_Victory.obj");
	auto statue_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/sand.jpg");
	auto statue_material = GetResourceManager()->CreateResourceFromFile<Material>(L"SpecularLight");
	statue_material->AddTexture(statue_tex);
	auto statue = world->CreateEntity<Entity>();
	auto statue_component = statue->CreateComponent<MeshComponent>();
	statue_component->SetMesh(statue_mesh);
	statue_component->AddMaterial(statue_material);
	statue->GetTransform()->SetScale(Vector3D(0.009f, 0.009f, 0.009f));
	statue->GetTransform()->SetPosition(Vector3D(145.0f, 24.4f, 140.5f));

#pragma endregion

#pragma region House

	auto house_mesh = GetResourceManager()->CreateResourceFromFile<Mesh>(L"Assets/Meshes/house.obj");
	auto house_wood_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/house_wood.jpg");
	auto house_wood_material = GetResourceManager()->CreateResourceFromFile<Material>(L"Base");
	house_wood_material->AddTexture(house_wood_tex);

	auto house_brick_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/house_brick.jpg");
	auto house_brick_n_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/house_brick_n.jpg");
	auto house_brick_material = GetResourceManager()->CreateResourceFromFile<Material>(L"BumpMapping");
	house_brick_material->AddTexture(house_brick_tex);
	house_brick_material->AddTexture(house_brick_n_tex);

	auto house_windows_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/house_windows.jpg");
	auto house_windows_material = GetResourceManager()->CreateResourceFromFile<Material>(L"Base");
	house_windows_material->AddTexture(house_windows_tex);

	auto barrel_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/barrel.jpg");
	auto barrel_material = GetResourceManager()->CreateResourceFromFile<Material>(L"Base");
	barrel_material->AddTexture(barrel_tex);

	auto house = GetWorld()->CreateEntity<Entity>();
	auto mesh_component = house->CreateComponent<MeshComponent>();
	mesh_component->SetMesh(house_mesh);
	mesh_component->AddMaterial(barrel_material);
	mesh_component->AddMaterial(house_brick_material);
	mesh_component->AddMaterial(house_windows_material);
	mesh_component->AddMaterial(house_wood_material);
	house->GetTransform()->SetPosition(Vector3D(130.0f, 24.4f, 150.0f));
	house->GetTransform()->SetRotation(Vector3D(0.0f, -0.707f, 0.0f));

#pragma endregion

#pragma region Light

	m_light = GetWorld()->CreateEntity<Entity>();
	auto light_component = m_light->CreateComponent<LightComponent>();
	light_component->SetColor(Vector4D(0.97f, 0.79f, 0.41f, 1.0f));
	m_light->GetTransform()->SetRotation(Vector3D(-0.707f, 0.707f, 0));

#pragma endregion

#pragma region Terrain

	auto terrain = GetWorld()->CreateEntity<Entity>();
	auto terrain_component = terrain->CreateComponent<TerrainComponent>();
	auto height_map_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/height_map.png");
	auto grass_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/grass.jpg");
	auto wall_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/ground.jpg");
	terrain_component->SetHeightMap(height_map_tex);
	terrain_component->SetGroupMap(grass_tex);
	terrain_component->SetWallMap(wall_tex);
	terrain->GetTransform()->SetScale(Vector3D(1, 1, 1));

#pragma endregion

#pragma region Water

	auto water = GetWorld()->CreateEntity<Entity>();
	auto water_component = water->CreateComponent<WaterComponent>();
	auto water_height_map_tex = GetResourceManager()->CreateResourceFromFile<Texture>(L"Assets/Textures/waveHeightMap.png");
	water_component->SetWaveHeightMap(water_height_map_tex);
	water->GetTransform()->SetPosition(Vector3D(-768, 15, -768));

#pragma endregion

#pragma region Fog

	auto fog = GetWorld()->CreateEntity<Entity>();
	auto fog_component = fog->CreateComponent<FogComponent>();
	fog_component->SetData(Vector4D(1.0f, 1.0f, 1.0f, 1.0f), 250, 1100.0f);

#pragma endregion

#pragma region UI

	auto font = GetResourceManager()->CreateResourceFromFile<Font>(L"Assets/Fonts/Bahnschrift.font");
	m_infos = GetWorld()->CreateEntity<Entity>();
	m_copyright = GetWorld()->CreateEntity<Entity>();
	auto text = m_infos->CreateComponent<TextComponent>();
	text->SetFont(font);
	text->SetText(L"DX3D Game Engine");
	m_infos->GetTransform()->SetPosition(Vector3D(10, 10, 0));
	auto copyright = m_copyright->CreateComponent<TextComponent>();
	copyright->SetFont(font);
	copyright->SetText(L"LUGAND Alexandre. All right reserved");

#pragma endregion

	m_player = GetWorld()->CreateEntity<Player>();
	GetInputSystem()->LockCursor(m_locked);
}

void DemoGame::OnUpdate(float delta_time)
{
	Game::OnUpdate(delta_time);

	m_light->GetTransform()->SetRotation(Vector3D(0.707f, -3.14f, 0));
	auto client_size = GetWorld()->GetGame()->GetDisplay()->GetClientSize();
	auto text_component = m_infos->GetComponent<TextComponent>();

	auto player_pos = m_player->GetTransform()->GetPosition();
	auto player_rot = m_player->GetTransform()->GetRotation();
	auto p = 180.0f / 3.14f;

	auto infoText = std::wstringstream();
	infoText << L"DX3D Game Engine\n\n";
	infoText << L"FPS: " << static_cast<int>(1.0f / delta_time) << L"\n";
	infoText << L"Screen size: W: " << client_size.Width() << L" H: " << client_size.Height() << L"\n";
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

	text_component->SetText(infoText.str().c_str());

	auto textBounds = m_copyright->GetComponent<TextComponent>()->GetBounds();
	m_copyright->GetTransform()->SetPosition(Vector3D(
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
		GetWorld()->GetGame()->Quit();
	}
	else if (input_system->IsKeyUp(Key::F1))
	{
		m_fullscreen = !m_fullscreen;
		GetWorld()->GetGame()->GetDisplay()->SetFullScreen(m_fullscreen);
	}
}
