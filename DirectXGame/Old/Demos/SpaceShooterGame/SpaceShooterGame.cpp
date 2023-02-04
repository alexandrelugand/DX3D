// ReSharper disable CppClangTidyClangDiagnosticUndef
#include <ctime>

#include <App/Constant.h>
#include <App/SpaceShooterGame/SpaceShooterGame.h>

#include <GameEngine/Utils.h>
#include <GameEngine/GraphicsEngine/GraphicsEngine.h>
#include <GameEngine/GraphicsEngine/Material/Material.h>
#include <GameEngine/GraphicsEngine/RenderSystem/RenderSystem.h>
#include <GameEngine/GraphicsEngine/RenderSystem/DeviceContext/DeviceContext.h>
#include <GameEngine/GraphicsEngine/ResourceManager/MeshManager/Mesh.h>
#include <GameEngine/GraphicsEngine/ResourceManager/MeshManager/MeshManager.h>
#include <GameEngine/GraphicsEngine/ResourceManager/TextureManager/TextureManager.h>
#include <GameEngine/InputSystem/InputSystem.h>
#include <GameEngine/Math/Utils.h>

namespace Ge
{
	void SpaceShooterGame::OnCreate()
	{
		Window::OnCreate();

		InputSystem::Get()->AddListener(this);

		m_play_state = true;
		InputSystem::Get()->ShowCursor(false);

		const auto rect = GetClientWindowRect();
		mp_swap_chain = GraphicsEngine::Get()->GetRenderSystem()->CreateSwapChain(m_hwnd, rect.Width(), rect.Height());

		srand(static_cast<unsigned>(time(nullptr)));

		for (unsigned int i = 0; i < 200; i++)
		{
			//Calculate a position between -2000 / 2000 values
			m_asteroids_pos[i] = Vector3D(static_cast<float>(rand() % 4000 - 2000), static_cast<float>(rand() % 4000 - 2000), static_cast<float>(rand() % 4000 - 2000));

			//Calculate a rotation between 0 / 2 * PI values
			m_asteroids_rot[i] = Vector3D(rand() % 628 / 100.0f, rand() % 628 / 100.0f, rand() % 628 / 100.0f);

			//Calculate a scale between 1 / 10 values
			float scale = static_cast<float>(rand() % 20 + 6);
			m_asteroids_scale[i] = Vector3D(scale, scale, scale);
		}

		mp_sky_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTexture(L"stars_map");
		mp_sky_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMesh(L"sphere_hq");
		mp_sky_material = GraphicsEngine::Get()->CreateMaterial(L"SkyBox");
		mp_sky_material->AddTexture(mp_sky_tex);
		mp_sky_material->SetCullMode(CULL_MODE_FRONT);

		mp_spaceship_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTexture(L"spaceship");
		mp_spaceship_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMesh(L"spaceship");
		mp_spaceship_material = GraphicsEngine::Get()->CreateMaterial(L"DirectionalLight");
		mp_spaceship_material->AddTexture(mp_spaceship_tex);

		mp_asteroid_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTexture(L"asteroid");
		mp_asteroid_normal_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTexture(L"asteroid_n");
		mp_asteroid_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMesh(L"asteroid");
		mp_asteroid_material = GraphicsEngine::Get()->CreateMaterial(L"SpaceShooterGame\\Shaders\\DirLightBumpMapping");
		mp_asteroid_material->AddTexture(mp_asteroid_tex);
		mp_asteroid_material->AddTexture(mp_asteroid_normal_tex);

		//Place initial camera position
		m_world_cam.SetTranslation(Vector3D(0, 0, -2));

		m_materials.reserve(32);
	}

	void SpaceShooterGame::OnUpdate()
	{
		Window::OnUpdate();

		//Update input and raise events
		InputSystem::Get()->Update();

		//Compute transform matrices
		Update();

		//Render the scene
		Render();

		//Reset mouse delta values
		m_delta_mouse_x = 0.0;
		m_delta_mouse_y = 0.0;
	}

	void SpaceShooterGame::Render()
	{
		//Clear the render target
		GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->ClearRenderTargetColor(this->mp_swap_chain, 0, 0.3f, 0.4f, 1);

		//Set viewport of render target in which we have to draw
		const auto rect = GetClientWindowRect();
		GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->SetViewportSize(rect.Width(), rect.Height());

		//Render spaceship
		m_materials.clear();
		m_materials.push_back(mp_spaceship_material);
		UpdateModel(m_current_spaceship_position, m_current_spaceship_rotation, Vector3D(1, 1, 1), m_materials);
		DrawMesh(mp_spaceship_mesh, m_materials);

		//Render asteroids
		m_materials.clear();
		m_materials.push_back(mp_asteroid_material);
		for (unsigned int i = 0; i < 200; i++)
		{
			UpdateModel(m_asteroids_pos[i], m_asteroids_rot[i], m_asteroids_scale[i], m_materials);
			DrawMesh(mp_asteroid_mesh, m_materials);
		}

		//Render skybox
		m_materials.clear();
		m_materials.push_back(mp_sky_material);
		DrawMesh(mp_sky_mesh, m_materials);

		mp_swap_chain->Present(true);

		m_old_delta = m_new_delta;
		m_new_delta = GetTickCount();

		//m_delta_time = m_old_delta ? (m_new_delta - m_old_delta) / 1000.0f : 0;
		m_delta_time = 1.0f / 60.0f;
		m_time += m_delta_time;
	}

	void SpaceShooterGame::Update()
	{
		UpdateSpaceShip();
		UpdateThirdPersonCamera();
		UpdateViewportProjection();
		UpdateLight();
		UpdateSkyBox();
	}

	void SpaceShooterGame::UpdateCamera()
	{
		//Initialize camera matrix
		Matrix4x4 world_cam, temp;
		world_cam.SetIdentity();

		m_camera_rotation.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
		m_camera_rotation.m_y += m_delta_mouse_x * m_delta_time * 0.1f;

		//Define rotation on X axis of camera
		temp.SetIdentity();
		temp.SetRotationX(m_camera_rotation.m_x);
		world_cam *= temp;

		//Define rotation on Y axis of camera
		temp.SetIdentity();
		temp.SetRotationY(m_camera_rotation.m_y);
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
	}

	void SpaceShooterGame::UpdateThirdPersonCamera()
	{
		//Initialize camera matrix
		Matrix4x4 world_cam, temp;
		world_cam.SetIdentity();

		m_camera_rotation.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
		m_camera_rotation.m_y += m_delta_mouse_x * m_delta_time * 0.1f;

		if (m_camera_rotation.m_x >= 1.57f) //Max 90°
			m_camera_rotation.m_x = 1.57f;
		else if (m_camera_rotation.m_x <= -1.57f) //Max -90°
			m_camera_rotation.m_x = -1.57f;

		m_current_camera_rotation = Vector3D::Lerp(m_current_camera_rotation, m_camera_rotation, 3.0f * m_delta_time);

		//Define rotation on X axis of camera
		temp.SetIdentity();
		temp.SetRotationX(m_current_camera_rotation.m_x);
		world_cam *= temp;

		//Define rotation on Y axis of camera
		temp.SetIdentity();
		temp.SetRotationY(m_current_camera_rotation.m_y);
		world_cam *= temp;

		if (m_forward)
		{
			if (m_turbo_mode)
			{
				if (m_forward > 0.0f)
					m_camera_distance = 25.0f;
				else
					m_camera_distance = 9.0f;
			}
			else
			{
				if (m_forward > 0.0f)
					m_camera_distance = 16.0f;
				else
					m_camera_distance = 9.0f;
			}
		}
		else
		{
			m_camera_distance = 14.0f;
		}

		m_current_camera_distance = Lerp(m_current_camera_distance, m_camera_distance, 2.0f * m_delta_time);
		m_camera_position = m_current_spaceship_position;

		//Define camera position
		Vector3D new_pos = m_camera_position + world_cam.GetZDirection() * -m_current_camera_distance;
		new_pos = new_pos + world_cam.GetYDirection() * 5.0f; //Move camera a little bit upper

		world_cam.SetTranslation(new_pos);

		//World camera matrix
		m_world_cam = world_cam;

		//Calculate view camera matrix
		world_cam.Inverse();
		m_view_cam = world_cam;
	}

	void SpaceShooterGame::UpdateViewportProjection()
	{
		const auto rect = GetClientWindowRect();

		//Calculate projection camera matrix
		float fov = 1.57f;
		float aspect = static_cast<float>(rect.Width()) / static_cast<float>(rect.Height());
		float znear = 0.1f;
		float zfar = 5000.0f;

		m_proj_cam.SetPerspectiveFovLH(fov, aspect, znear, zfar);
	}

	void SpaceShooterGame::UpdateModel(const Vector3D& position, const Vector3D& rotation, const Vector3D& scale, const std::vector<MaterialPtr>& materials)
	{
		Constant ct;
		ct.world.SetIdentity();

		Matrix4x4 temp;
		temp.SetIdentity();
		temp.SetScale(scale);
		ct.world *= temp;

		temp.SetIdentity();
		temp.SetRotationX(rotation.m_x);
		ct.world *= temp;

		temp.SetIdentity();
		temp.SetRotationY(rotation.m_y);
		ct.world *= temp;

		temp.SetIdentity();
		temp.SetRotationZ(rotation.m_z);
		ct.world *= temp;

		temp.SetIdentity();
		temp.SetTranslation(position);
		ct.world *= temp;

		ct.view = m_view_cam;
		ct.projection = m_proj_cam;
		ct.camera_position = m_world_cam.GetTranslation();
		ct.light_direction = m_light_rot_matrix.GetZDirection();
		ct.light_position = m_light_position;
		ct.light_radius = 0.0f;

		ct.time = m_time;

		for (size_t m = 0; m < materials.size(); m++)
		{
			materials[m]->SetData(&ct, sizeof(Constant));
		}
	}

	void SpaceShooterGame::UpdateSkyBox()
	{
		Constant ct;

		//Initialize world matrix
		ct.world.SetIdentity();
		ct.world.SetScale(Vector3D(4000.0f, 4000.0f, 4000.0f));
		ct.world.SetTranslation(m_world_cam.GetTranslation()); //Place center of sky box at same position than camera
		ct.view = m_view_cam;
		ct.projection = m_proj_cam;

		mp_sky_material->SetData(&ct, sizeof(Constant));
	}

	void SpaceShooterGame::UpdateLight()
	{
		//Define direct light rotation
		m_light_rot_matrix.SetIdentity();

		Matrix4x4 temp;
		temp.SetIdentity();
		temp.SetRotationX(-0.707f); //Rotate to 45° on X axis
		m_light_rot_matrix *= temp;

		temp.SetIdentity();
		temp.SetRotationY(0.707f); //Rotate to 45° on Y axis
		m_light_rot_matrix *= temp;
	}

	void SpaceShooterGame::UpdateSpaceShip()
	{
		//Initialize spaceship matrix
		Matrix4x4 world_model, temp;
		world_model.SetIdentity();

		m_spaceship_rotation.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
		m_spaceship_rotation.m_y += m_delta_mouse_x * m_delta_time * 0.1f;

		if (m_spaceship_rotation.m_x >= 1.57f) //Max 90°
			m_spaceship_rotation.m_x = 1.57f;
		else if (m_spaceship_rotation.m_x <= -1.57f) //Max -90°
			m_spaceship_rotation.m_x = -1.57f;

		m_current_spaceship_rotation = Vector3D::Lerp(m_current_spaceship_rotation, m_spaceship_rotation, 5.0f * m_delta_time);

		//Define rotation on X axis of spaceship
		temp.SetIdentity();
		temp.SetRotationX(m_current_spaceship_rotation.m_x);
		world_model *= temp;

		//Define rotation on Y axis of spaceship
		temp.SetIdentity();
		temp.SetRotationY(m_current_spaceship_rotation.m_y);
		world_model *= temp;

		m_spaceship_speed = 125.0f;
		if (m_turbo_mode)
			m_spaceship_speed = 305.0f;

		m_spaceship_position = m_spaceship_position + world_model.GetZDirection() * m_forward * m_spaceship_speed * m_delta_time;
		m_current_spaceship_position = Vector3D::Lerp(m_current_spaceship_position, m_spaceship_position, 3.0f * m_delta_time);
	}

	void SpaceShooterGame::DrawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials)
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

	void SpaceShooterGame::OnDestroy()
	{
		Window::OnDestroy();
		mp_swap_chain->SetFullScreen(false, 1, 1);
	}

	void SpaceShooterGame::OnFocus()
	{
		InputSystem::Get()->AddListener(this);
	}

	void SpaceShooterGame::OnKillFocus()
	{
		InputSystem::Get()->RemoveListener(this);
	}

	void SpaceShooterGame::OnSize()
	{
		auto rec = GetClientWindowRect();
		mp_swap_chain->Resize(rec.Width(), rec.Height());

		Update();
		Render(); //Resize immediately the screen
	}

	void SpaceShooterGame::OnKeyDown(int key)
	{
		if (key == 'Z')
		{
			m_forward = 1.0f;
		}
		else if (key == 'S')
		{
			m_forward = -1.0f;
		}
		else if (key == 'Q' && m_rightward == 0.0f)
		{
			m_rightward = -1.0f;
		}
		else if (key == 'D' && m_rightward == 0.0f)
		{
			m_rightward = 1.0f;
		}
		else if (key == VK_SPACE) //Space
		{
			m_upward = 1.0f;
		}
		else if (key == VK_CONTROL) //Left ctrl
		{
			m_upward = -1.0f;
		}
		else if (key == VK_SHIFT) //Left shift
		{
			m_turbo_mode = true;
		}
		else if (key == VK_ESCAPE) //Escape
		{
			Close();
		}
	}

	void SpaceShooterGame::OnKeyUp(int key)
	{
		m_forward = 0.0f;
		m_rightward = 0.0f;
		m_upward = 0.0f;

		if (key == 'P')
		{
			m_play_state = !m_play_state;
			InputSystem::Get()->ShowCursor(!m_play_state);
		}
		else if (key == 'F')
		{
			m_full_screen = m_full_screen ? false : true;
			const Rect rec = GetSizeScreen();
			mp_swap_chain->SetFullScreen(m_full_screen, rec.Width(), rec.Height());
		}
		else if (key == VK_SHIFT) //Left shift
		{
			m_turbo_mode = false;
		}
	}

	void SpaceShooterGame::OnMouseMove(const Point& mouse_pos)
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

	void SpaceShooterGame::OnLeftMouseDown(const Point& mouse_pos)
	{
		m_play_state = !m_play_state;
		InputSystem::Get()->ShowCursor(!m_play_state);
	}

	void SpaceShooterGame::OnLeftMouseUp(const Point& mouse_pos)
	{
	}

	void SpaceShooterGame::OnRightMouseDown(const Point& mouse_pos)
	{
	}

	void SpaceShooterGame::OnRightMouseUp(const Point& mouse_pos)
	{
	}
}
