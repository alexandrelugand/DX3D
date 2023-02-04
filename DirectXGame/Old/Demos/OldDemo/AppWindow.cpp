// ReSharper disable CppClangTidyClangDiagnosticUndef
#include <iostream>

#include <App/OldDemo/AppWindow.h>
#include <App/Constant.h>

#include <GameEngine/GraphicsEngine/GraphicsEngine.h>
#include <GameEngine/GraphicsEngine/Material/Material.h>
#include <GameEngine/GraphicsEngine/RenderSystem/RenderSystem.h>
#include <GameEngine/GraphicsEngine/RenderSystem/DeviceContext/DeviceContext.h>
#include <GameEngine/GraphicsEngine/ResourceManager/MeshManager/Mesh.h>
#include <GameEngine/GraphicsEngine/ResourceManager/MeshManager/MeshManager.h>
#include <GameEngine/GraphicsEngine/ResourceManager/TextureManager/TextureManager.h>
#include <GameEngine/InputSystem/InputSystem.h>
#include <GameEngine/Math/Vertex.h>

namespace Ge
{
	void AppWindow::OnCreate()
	{
		Window::OnCreate();

		InputSystem::Get()->AddListener(this);

		m_play_state = true;
		InputSystem::Get()->ShowCursor(false);


#if STATUE
		mp_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\GameEngine\\Assets\\Meshes\\Winged_Victory.obj");
		mp_sky_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\GameEngine\\Assets\\Meshes\\sphere_hq.obj");
		mp_sky_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\GameEngine\\Assets\\Textures\\sky.jpg");
#elif SUZANNE
		mp_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\GameEngine\\Assets\\Meshes\\suzanne.obj");
		mp_sky_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\GameEngine\\Assets\\Meshes\\sphere_hq.obj");
		mp_sky_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\GameEngine\\Assets\\Textures\\sky.jpg");
#elif EARTH
		mp_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\GameEngine\\Assets\\Meshes\\sphere_hq.obj");
		mp_sky_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\GameEngine\\Assets\\Meshes\\sphere_hq.obj");
		mp_sky_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\GameEngine\\Assets\\Textures\\stars_map.jpg");

		mp_earth_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\GameEngine\\Assets\\Textures\\earth_color.jpg");
		mp_earth_specular_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\GameEngine\\Assets\\Textures\\earth_spec.jpg");
		mp_earth_night_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\GameEngine\\Assets\\Textures\\earth_night.jpg");
		mp_clouds_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\GameEngine\\Assets\\Textures\\clouds.jpg");
#elif HOUSE
		mp_sky_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\GameEngine\\Assets\\Meshes\\sphere_hq.obj");
		mp_sky_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\GameEngine\\Assets\\Textures\\sky.jpg");

		mp_terrain_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\GameEngine\\Assets\\Meshes\\terrain.obj");
		mp_sand_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\GameEngine\\Assets\\Textures\\sand.jpg");

		mp_house_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\GameEngine\\Assets\\Meshes\\house.obj");
		mp_barrel_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\GameEngine\\Assets\\Textures\\barrel.jpg");
		mp_brick_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\GameEngine\\Assets\\Textures\\house_brick.jpg");
		mp_window_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\GameEngine\\Assets\\Textures\\house_windows.jpg");
		mp_wood_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\GameEngine\\Assets\\Textures\\house_wood.jpg");
#else
		mp_sky_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\GameEngine\\Assets\\Meshes\\sphere_hq.obj");
		mp_sky_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\GameEngine\\Assets\\Textures\\sky.jpg");

		mp_terrain_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMeshFromFile(L"..\\..\\GameEngine\\Assets\\Meshes\\terrain.obj");
		mp_sand_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"..\\..\\GameEngine\\Assets\\Textures\\sand.jpg");
#endif
		const auto rect = GetClientWindowRect();
		mp_swap_chain = GraphicsEngine::Get()->GetRenderSystem()->CreateSwapChain(m_hwnd, rect.Width(), rect.Height());

#if STATUE
		m_world_cam.SetTranslation(Vector3D(-6, 7, 0));
#elif SUZANNE
		m_world_cam.SetTranslation(Vector3D(0, 0, 2));
#elif EARTH
		m_world_cam.SetTranslation(Vector3D(0, 0, -2));
#elif HOUSE
		m_world_cam.SetTranslation(Vector3D(4, 6, -2));
#else
		m_world_cam.SetTranslation(Vector3D(0, 0, -2));
#endif

#if DRAW_TRIANGLE
		////Default camera position for suzanne mesh
		//m_world_cam.SetTranslation(Vector3D(0, 0, 2));

		// Triangle List
		// V2-----V3/V4
		// |     / |
		// |   /   |
		// | /     |
		// V1/V6---V5

		// Triangle Strip
		// V2-----/V4
		// | \     |
		// |   \   |
		// |    _| |
		// V1------V3

		Vector3D position_list[] =
		{
			//X - Y - Z
			//FRONT FACE
			{Vector3D(-0.5f, -0.5f, -0.5f)},
			{Vector3D(-0.5f, 0.5f, -0.5f)},
			{Vector3D(0.5f, 0.5f, -0.5f)},
			{Vector3D(0.5f, -0.5f, -0.5f)},

			//BACK FACE
			{Vector3D(0.5f, -0.5f, 0.5f)},
			{Vector3D(0.5f, 0.5f, 0.5f)},
			{Vector3D(-0.5f, 0.5f, 0.5f)},
			{Vector3D(-0.5f, -0.5f, 0.5f)}
		};

		Vector2D texcoord_list[] =
		{
			{Vector2D(0.0f, 0.0f)},
			{Vector2D(0.0f, 1.0f)},
			{Vector2D(1.0f, 0.0f)},
			{Vector2D(1.0f, 1.0f)},
		};

		Vertex vertex_list[] =
		{
			//X - Y - Z
			//FRONT FACE
			{position_list[0], texcoord_list[1]},
			{position_list[1], texcoord_list[0]},
			{position_list[2], texcoord_list[2]},
			{position_list[3], texcoord_list[3]},

			//BACK FACE
			{position_list[4], texcoord_list[1]},
			{position_list[5], texcoord_list[0]},
			{position_list[6], texcoord_list[2]},
			{position_list[7], texcoord_list[3]},

			//TOP SIDE
			{position_list[1], texcoord_list[1]},
			{position_list[6], texcoord_list[0]},
			{position_list[5], texcoord_list[2]},
			{position_list[2], texcoord_list[3]},

			//BOTTOM SIDE
			{position_list[7], texcoord_list[1]},
			{position_list[0], texcoord_list[0]},
			{position_list[3], texcoord_list[2]},
			{position_list[4], texcoord_list[3]},

			//RIGHT SIDE
			{position_list[3], texcoord_list[1]},
			{position_list[2], texcoord_list[0]},
			{position_list[5], texcoord_list[2]},
			{position_list[4], texcoord_list[3]},

			//LEFT SIDE
			{position_list[7], texcoord_list[1]},
			{position_list[6], texcoord_list[0]},
			{position_list[1], texcoord_list[2]},
			{position_list[0], texcoord_list[3]}
		};

		UINT size_list = ARRAYSIZE(vertex_list);

		unsigned int index_list[] =
		{
			//FRONT SIDE
			0, 1, 2, //FIRST TRIANGLE
			2, 3, 0, //SECOND TRIANGLE
			//BACK SIDE
			4, 5, 6,
			6, 7, 4,
			//TOP SIDE
			8, 9, 10,
			10, 11, 8,
			//BOTTOM SIDE
			12, 13, 14,
			14, 15, 12,
			//RIGHT SIDE
			16, 17, 18,
			18, 19, 16,
			//LEFT SIDE
			20, 21, 22,
			22, 23, 20
		};
		UINT size_index_list = ARRAYSIZE(index_list);

		mp_ib = GraphicsEngine::Get()->GetRenderSystem()->CreateIndexBuffer(index_list, size_index_list);
		void* shader_byte_code = nullptr;
		size_t size_shader = 0;
		GraphicsEngine::Get()->GetRenderSystem()->CompileVertexShader(L"..\\..\\GameEngine\\App\\OldDemo\\Shaders\\VertexShader.hlsl", "main", &shader_byte_code, &size_shader);
		mp_vb = GraphicsEngine::Get()->GetRenderSystem()->CreateVertexBuffer(vertex_list, sizeof(Vertex), size_list, shader_byte_code, size_shader);

#endif

		m_materials.reserve(32);

#if EARTH
		mp_mesh_material = GraphicsEngine::Get()->CreateMaterial(L"..\\..\\GameEngine\\App\\OldDemo\\Shaders\\VertexShader.hlsl", L"..\\..\\GameEngine\\App\\OldDemo\\Shaders\\Earth\\PixelShader.hlsl");
		mp_mesh_material->AddTexture(mp_earth_tex);
		mp_mesh_material->AddTexture(mp_earth_specular_tex);
		mp_mesh_material->AddTexture(mp_clouds_tex);
		mp_mesh_material->AddTexture(mp_earth_night_tex);

#elif HOUSE
		mp_terrain_material = GraphicsEngine::Get()->CreateMaterial(L"..\\..\\GameEngine\\App\\OldDemo\\Shaders\\VertexShader.hlsl", L"..\\..\\GameEngine\\App\\OldDemo\\Shaders\\PointLight\\PixelShader.hlsl");
		mp_terrain_material->AddTexture(mp_sand_tex);

		mp_barrel_material = GraphicsEngine::Get()->CreateMaterial(mp_terrain_material);
		mp_barrel_material->AddTexture(mp_barrel_tex);
		mp_brick_material = GraphicsEngine::Get()->CreateMaterial(mp_terrain_material);
		mp_brick_material->AddTexture(mp_brick_tex);
		mp_window_material = GraphicsEngine::Get()->CreateMaterial(mp_terrain_material);
		mp_window_material->AddTexture(mp_window_tex);
		mp_wood_material = GraphicsEngine::Get()->CreateMaterial(mp_terrain_material);
		mp_wood_material->AddTexture(mp_wood_tex);
#else
		mp_mesh_material = GraphicsEngine::Get()->CreateMaterial(L"..\\..\\GameEngine\\App\\OldDemo\\Shaders\\VertexShader.hlsl", L"..\\..\\GameEngine\\App\\OldDemo\\Shaders\\PixelShader.hlsl");
#endif

		mp_sky_material = GraphicsEngine::Get()->CreateMaterial(L"..\\..\\GameEngine\\App\\OldDemo\\Shaders\\VertexShader.hlsl", L"..\\..\\GameEngine\\App\\OldDemo\\Shaders\\SkyBox\\PixelShader.hlsl");
		mp_sky_material->AddTexture(mp_sky_tex);
		mp_sky_material->SetCullMode(CULL_MODE_FRONT);
	}

	void AppWindow::OnUpdate()
	{
		Window::OnUpdate();

		//Update input and raise events
		InputSystem::Get()->Update();

		//Render the scene
		Render();

		m_delta_mouse_x = 0.0;
		m_delta_mouse_y = 0.0;
	}

	void AppWindow::Render()
	{
		//Clear the render target
		GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->ClearRenderTargetColor(this->mp_swap_chain, 0, 0.3f, 0.4f, 1);

		//Set viewport of render target in which we have to draw
		const auto rect = GetClientWindowRect();
		GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->SetViewportSize(rect.Width(), rect.Height());

		//Compute transform matrices
		Update();

		//Render model
#if HOUSE
		m_materials.clear();
		m_materials.push_back(mp_terrain_material);
		UpdateModel(Vector3D(0, 0, 0), Vector3D(1, 1, 1), m_materials);
		DrawMesh(mp_terrain_mesh, m_materials);

		m_materials.clear();
		m_materials.push_back(mp_barrel_material);
		m_materials.push_back(mp_brick_material);
		m_materials.push_back(mp_window_material);
		m_materials.push_back(mp_wood_material);

		for (float i = 0; i < 3; i++)
		{
			for (float j = 0; j < 3; j++)
			{
				UpdateModel(Vector3D(-14.0f + 14.0f * i, 0, -14.0f + 14.0f * j), Vector3D(1, 1, 1), m_materials);
				DrawMesh(mp_house_mesh, m_materials);
			}
		}
#else
		m_materials.clear();
		m_materials.push_back(mp_mesh_material);
		UpdateModel(Vector3D(0, 0, 0), Vector3D(1, 1, 1), m_materials);
		DrawMesh(mp_mesh, m_materials);
#endif

		//Render skybox / sphere
		m_materials.clear();
		m_materials.push_back(mp_sky_material);
		DrawMesh(mp_sky_mesh, m_materials);

		mp_swap_chain->Present(true);

		m_old_delta = m_new_delta;
		m_new_delta = GetTickCount();

		m_delta_time = m_old_delta ? (m_new_delta - m_old_delta) / 1000.0f : 0;
		m_time += m_delta_time;
	}

	void AppWindow::Update()
	{
		UpdateCamera();
		UpdateLight();
		UpdateSkyBox();
	}

	void AppWindow::UpdateCamera()
	{
		//Initialize camera matrix
		Matrix4x4 world_cam, temp;
		world_cam.SetIdentity();

		m_rot_x += m_delta_mouse_y * m_delta_time * 0.1f;
		m_rot_y += m_delta_mouse_x * m_delta_time * 0.1f;

		//Define rotation on X axis of camera
		temp.SetIdentity();
		temp.SetRotationX(m_rot_x);
		world_cam *= temp;

		//Define rotation on Y axis of camera
		temp.SetIdentity();
		temp.SetRotationY(m_rot_y);
		world_cam *= temp;

		//Define camera position
		Vector3D new_pos = m_world_cam.GetTranslation() + world_cam.GetZDirection() * (m_forward * 0.05f);
		new_pos = new_pos + world_cam.GetXDirection() * (m_rightward * 0.05f);
		new_pos = new_pos + world_cam.GetYDirection() * (m_upward * 0.05f);
		world_cam.SetTranslation(new_pos);

		//World camera matrix
		m_world_cam = world_cam;

		//Calculate view camera matrix
		world_cam.Inverse();
		m_view_cam = world_cam;

		const auto rect = GetClientWindowRect();

		//Calculate projection camera matrix
		float fov = 1.57f;
		float aspect = static_cast<float>(rect.Width()) / static_cast<float>(rect.Height());
		float znear = 0.1f;
		float zfar = 100.0f;

		m_proj_cam.SetPerspectiveFovLH(fov, aspect, znear, zfar);
	}

	void AppWindow::UpdateModel(const Vector3D& position, const Vector3D& scale, const std::vector<MaterialPtr>& materials)
	{
		Constant ct;

		//Initialize world matrix
		ct.world.SetIdentity();
		ct.world.SetScale(scale);
		ct.world.SetTranslation(position);

#ifdef STATUE
		ct.world.SetScale(Vector3D(0.01f, 0.01f, 0.01f));
#endif
		ct.view = m_view_cam;
		ct.projection = m_proj_cam;
		ct.camera_position = m_world_cam.GetTranslation();
		ct.light_direction = m_light_rot_matrix.GetZDirection();
		ct.light_position = m_light_position;
		ct.light_radius = m_light_radius;

		ct.time = m_time;

		for (size_t m = 0; m < materials.size(); m++)
		{
			materials[m]->SetData(&ct, sizeof(Constant));
		}
	}

	void AppWindow::UpdateSkyBox()
	{
		Constant ct;

		//Initialize world matrix
		ct.world.SetIdentity();
		ct.world.SetScale(Vector3D(100.0f, 100.0f, 100.0f));
		ct.world.SetTranslation(m_world_cam.GetTranslation()); //Place center of sky box at same position than camera
		ct.view = m_view_cam;
		ct.projection = m_proj_cam;

		mp_sky_material->SetData(&ct, sizeof(Constant));
	}

	void AppWindow::UpdateLight()
	{
		//Define direct light rotation
		m_light_rot_matrix.SetIdentity();
		m_light_rot_matrix.SetRotationY(m_light_rot_y);
#if HOUSE
		m_light_rot_y += 1.57f * m_delta_time; //45° peer seconds equals to 0.707 in radian
#else
		m_light_rot_y += 0.307f * m_delta_time; //45° peer seconds equals to 0.707 in radian
#endif
		//const float dist_from_origin = 3.0f;
		//m_light_position = Vector4D(cos(m_light_rot_y) * dist_from_origin, 2.0f, sin(m_light_rot_y) * dist_from_origin, 2.0f);
		m_light_position = Vector4D(180, 140, 70, 1.0f);
	}

	void AppWindow::DrawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials)
	{
		//Set the vertices of the triangle to draw
		GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->SetVertexBuffer(mesh->GetVertexBuffer());
		//SET THE INDICES OF THE TRIANGLE TO DRAW
		GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->SetIndexBuffer(mesh->GetIndexBuffer());

		for (size_t m = 0; m < mesh->GetNumMaterialSlots(); m++)
		{
			if (m >= materials.size())
				break;

			MaterialSlot mat_slot = mesh->GetMaterialSlot(m);

			GraphicsEngine::Get()->SetMaterial(materials[m]);

			// Finally draw the triangle
			GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->DrawIndexedTriangleList(mat_slot.num_indices, 0, mat_slot.start_index);
		}
	}

	void AppWindow::OnDestroy()
	{
		Window::OnDestroy();
		mp_swap_chain->SetFullScreen(false, 1, 1);
	}

	void AppWindow::OnFocus()
	{
		InputSystem::Get()->AddListener(this);
	}

	void AppWindow::OnKillFocus()
	{
		InputSystem::Get()->RemoveListener(this);
	}

	void AppWindow::OnSize()
	{
		auto rec = GetClientWindowRect();
		mp_swap_chain->Resize(rec.Width(), rec.Height());
		Render(); //Resize immediately the screen
	}

	void AppWindow::OnKeyDown(int key)
	{
		if (key == 'Z')
		{
			m_forward = 1.0f;
		}
		else if (key == 'S')
		{
			m_forward = -1.0f;
		}
		else if (key == 'Q')
		{
			m_rightward = -1.0f;
		}
		else if (key == 'D')
		{
			m_rightward = 1.0f;
		}
		else if (key == 0x20) //Space
		{
			m_upward = 1.0f;
		}
		else if (key == 0x11) //Left ctrl
		{
			m_upward = -1.0f;
		}
		else if (key == 'O')
		{
			m_light_radius -= 1.0f * m_delta_time;
		}
		else if (key == 'P')
		{
			m_light_radius += 1.0f * m_delta_time;
		}
		else if (key == 0x1B) //Escape
		{
			Close();
		}
	}

	void AppWindow::OnKeyUp(int key)
	{
		m_forward = 0.0f;
		m_rightward = 0.0f;
		m_upward = 0.0f;

		if (key == 'G')
		{
			m_play_state = m_play_state ? false : true;
			InputSystem::Get()->ShowCursor(!m_play_state);
		}
		else if (key == 'F')
		{
			m_full_screen = m_full_screen ? false : true;
			const Rect rec = GetSizeScreen();
			mp_swap_chain->SetFullScreen(m_full_screen, rec.Width(), rec.Height());
		}
	}

	void AppWindow::OnMouseMove(const Point& mouse_pos)
	{
		if (!m_play_state)
			return;

		const Rect& rect = GetClientWindowRect();
		const int width = rect.Width();
		const int height = rect.Height();

		m_delta_mouse_x = mouse_pos.m_x - (rect.m_left + width / 2.0f);
		m_delta_mouse_y = mouse_pos.m_y - (rect.m_top + height / 2.0f);

		InputSystem::Get()->SetCursorPosition(Point(rect.m_left + static_cast<int>(width / 2.0f), rect.m_top + static_cast<int>(height / 2.0f)));
	}

	void AppWindow::OnLeftMouseDown(const Point& mouse_pos)
	{
		m_scale_cube = 0.5f;
	}

	void AppWindow::OnLeftMouseUp(const Point& mouse_pos)
	{
		m_scale_cube = 1.0f;
	}

	void AppWindow::OnRightMouseDown(const Point& mouse_pos)
	{
		m_scale_cube = 2.0f;
	}

	void AppWindow::OnRightMouseUp(const Point& mouse_pos)
	{
		m_scale_cube = 1.0f;
	}
}
