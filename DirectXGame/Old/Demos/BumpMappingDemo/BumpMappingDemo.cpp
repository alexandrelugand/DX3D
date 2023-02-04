#include <Windows.h>

#include <App/Constant.h>
#include <App/BumpMappingDemo/BumpMappingDemo.h>

#include <GameEngine/GraphicsEngine/GraphicsEngine.h>
#include <GameEngine/GraphicsEngine/RenderSystem/RenderSystem.h>
#include <GameEngine/GraphicsEngine/RenderSystem/DeviceContext/DeviceContext.h>
#include <GameEngine/GraphicsEngine/RenderSystem/SwapChain/SwapChain.h>
#include <GameEngine/GraphicsEngine/ResourceManager/MeshManager/Mesh.h>
#include <GameEngine/GraphicsEngine/Material/Material.h>
#include <GameEngine/GraphicsEngine/ResourceManager/MeshManager/MeshManager.h>
#include <GameEngine/GraphicsEngine/ResourceManager/TextureManager/TextureManager.h>
#include <GameEngine/InputSystem/InputSystem.h>
#include <GameEngine/Math/Utils.h>

namespace Ge
{
	void BumpMappingDemo::Render()
	{
		//CLEAR THE RENDER TARGET 
		GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->ClearRenderTargetColor(m_swap_chain, 0, 0.3f, 0.4f, 1);

		//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
		const Rect rect = GetClientWindowRect();
		GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->SetViewportSize(rect.Width(), rect.Height());

		m_list_materials.clear();
		m_list_materials.push_back(m_sphere_mat);
		UpdateModel(Vector3D(0, 0, 0), Vector3D(), Vector3D(1, 1, 1), m_list_materials);
		DrawMesh(m_sphere_mesh, m_list_materials);

		//RENDER SKYBOX/SPHERE
		m_list_materials.clear();
		m_list_materials.push_back(m_sky_mat);
		DrawMesh(m_sky_mesh, m_list_materials);

		GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->ClearDepthStencil(m_swap_chain);

		m_swap_chain->Present(true);

		m_old_delta = m_new_delta;
		m_new_delta = GetTickCount();

		m_delta_time = 0.0166666666f;
		m_time += m_delta_time;
	}

	void BumpMappingDemo::Update()
	{
		UpdateThirdPersonCamera();
		UpdateLight();
		UpdateSkyBox();
	}

	void BumpMappingDemo::UpdateModel(Vector3D position, Vector3D rotation, Vector3D scale, const std::vector<MaterialPtr>& list_materials)
	{
		Constant ct;

		Matrix4x4 temp;
		ct.world.SetIdentity();

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

		ct.light_position = m_light_rot_matrix.GetTranslation();
		ct.light_radius = 0.0f;
		ct.light_direction = m_light_rot_matrix.GetZDirection();
		ct.time = m_time;

		for (unsigned int m = 0; m < list_materials.size(); m++)
		{
			list_materials[m]->SetData(&ct, sizeof(Constant));
		}
	}

	void BumpMappingDemo::UpdateThirdPersonCamera()
	{
		Matrix4x4 world_cam, temp;
		world_cam.SetIdentity();

		m_cam_rot.m_x += m_delta_mouse_y * 0.001f;
		m_cam_rot.m_y += m_delta_mouse_x * 0.001f;


		if (m_cam_rot.m_x >= 1.57f)
			m_cam_rot.m_x = 1.57f;
		else if (m_cam_rot.m_x <= -1.57f)
			m_cam_rot.m_x = -1.57f;

		m_current_cam_rot = Vector3D::Lerp(m_current_cam_rot, m_cam_rot, 3.0f * m_delta_time);

		temp.SetIdentity();
		temp.SetRotationX(m_current_cam_rot.m_x);
		world_cam *= temp;

		temp.SetIdentity();
		temp.SetRotationY(m_current_cam_rot.m_y);
		world_cam *= temp;

		m_cam_distance = 2.0f;
		m_current_cam_distance = Lerp(m_current_cam_distance, m_cam_distance, 2.0f * m_delta_time);

		m_cam_pos = Vector3D();
		Vector3D new_pos = m_cam_pos + world_cam.GetZDirection() * (-m_current_cam_distance);

		world_cam.SetTranslation(new_pos);
		m_world_cam = world_cam;

		world_cam.Inverse();
		m_view_cam = world_cam;

		UpdateViewportProjection();
	}

	void BumpMappingDemo::UpdateSkyBox()
	{
		Constant ct;

		ct.world.SetIdentity();
		ct.world.SetScale(Vector3D(4000.0f, 4000.0f, 4000.0f));
		ct.world.SetTranslation(m_world_cam.GetTranslation());
		ct.view = m_view_cam;
		ct.projection = m_proj_cam;

		m_sky_mat->SetData(&ct, sizeof(Constant));
	}

	void BumpMappingDemo::UpdateLight()
	{
		Matrix4x4 temp;
		m_light_rot_matrix.SetIdentity();

		temp.SetIdentity();
		temp.SetRotationX(-0.707f);
		m_light_rot_matrix *= temp;

		temp.SetIdentity();
		temp.SetRotationY(0.707f);
		m_light_rot_matrix *= temp;
	}


	void BumpMappingDemo::DrawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials)
	{
		for (unsigned int m = 0; m < list_materials.size(); m++)
		{
			if (m == list_materials.size()) break;

			MaterialSlot mat = mesh->GetMaterialSlot(m);

			GraphicsEngine::Get()->SetMaterial(list_materials[m]);
			//SET THE VERTICES OF THE TRIANGLE TO DRAW
			GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->SetVertexBuffer(mesh->GetVertexBuffer());
			//SET THE INDICES OF THE TRIANGLE TO DRAW
			GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->SetIndexBuffer(mesh->GetIndexBuffer());
			// FINALLY DRAW THE TRIANGLE
			GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->DrawIndexedTriangleList(mat.num_indices, 0, mat.start_index);
		}
	}

	void BumpMappingDemo::UpdateViewportProjection()
	{
		const Rect rect = GetClientWindowRect();
		int width = rect.Width();
		int height = rect.Height();

		m_proj_cam.SetPerspectiveFovLH(1.57f, (static_cast<float>(width) / static_cast<float>(height)), 0.1f, 5000.0f);
	}

	void BumpMappingDemo::OnCreate()
	{
		Window::OnCreate();

		InputSystem::Get()->AddListener(this);

		m_play_state = true;
		InputSystem::Get()->ShowCursor(false);

		const Rect rect = GetClientWindowRect();
		m_swap_chain = GraphicsEngine::Get()->GetRenderSystem()->CreateSwapChain(m_hwnd, rect.Width(), rect.Height());

		m_sky_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTexture(L"stars_map");
		m_sky_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMesh(L"sphere_hq");

		m_sphere_mesh = GraphicsEngine::Get()->GetMeshManager()->CreateMesh(L"sphere_hq");

		m_sphere_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTexture(L"brick_d");
		m_sphere_normal_tex = GraphicsEngine::Get()->GetTextureManager()->CreateTexture(L"brick_n");
		m_sphere_mat = GraphicsEngine::Get()->CreateMaterial(L"DirLightBumpMapping");
		m_sphere_mat->AddTexture(m_sphere_tex);
		m_sphere_mat->AddTexture(m_sphere_normal_tex);

		m_sky_mat = GraphicsEngine::Get()->CreateMaterial(L"SkyBox");
		m_sky_mat->AddTexture(m_sky_tex);
		m_sky_mat->SetCullMode(CULL_MODE_FRONT);


		m_world_cam.SetTranslation(Vector3D(0, 0, -2));

		m_list_materials.reserve(32);
	}

	void BumpMappingDemo::OnUpdate()
	{
		Window::OnUpdate();
		InputSystem::Get()->Update();

		Update();
		Render();

		m_delta_mouse_x = 0;
		m_delta_mouse_y = 0;
	}

	void BumpMappingDemo::OnDestroy()
	{
		Window::OnDestroy();
		m_swap_chain->SetFullScreen(false, 1, 1);
	}

	void BumpMappingDemo::OnFocus()
	{
		InputSystem::Get()->AddListener(this);
	}

	void BumpMappingDemo::OnKillFocus()
	{
		InputSystem::Get()->RemoveListener(this);
	}

	void BumpMappingDemo::OnSize()
	{
		const Rect rect = GetClientWindowRect();
		m_swap_chain->Resize(rect.Width(), rect.Height());
		Update();
		Render();
	}

	void BumpMappingDemo::OnKeyDown(int key)
	{
		if (key == VK_ESCAPE)
		{
			Close();
		}
	}

	void BumpMappingDemo::OnKeyUp(int key)
	{
		if (key == 'P')
		{
			if (m_play_state)
			{
				m_play_state = false;
				InputSystem::Get()->ShowCursor(!m_play_state);
			}
		}
		else if (key == 'F')
		{
			m_fullscreen_state = (m_fullscreen_state) ? false : true;
			const Rect size_screen = GetSizeScreen();

			m_swap_chain->SetFullScreen(m_fullscreen_state, size_screen.Width(), size_screen.Height());
		}
	}

	void BumpMappingDemo::OnMouseMove(const Point& mouse_pos)
	{
		if (!m_play_state) return;

		const Rect win_size = GetClientWindowRect();

		int width = win_size.Width();
		int height = win_size.Height();

		m_delta_mouse_x = mouse_pos.m_x - (win_size.m_left + width / 2.0f);
		m_delta_mouse_y = mouse_pos.m_y - (win_size.m_top + height / 2.0f);

		InputSystem::Get()->SetCursorPosition(Point(win_size.m_left + static_cast<int>(width / 2.0f), win_size.m_top + static_cast<int>(height / 2.0f)));
	}

	void BumpMappingDemo::OnLeftMouseDown(const Point& mouse_pos)
	{
		if (!m_play_state)
		{
			m_play_state = true;
			InputSystem::Get()->ShowCursor(!m_play_state);
		}
	}

	void BumpMappingDemo::OnLeftMouseUp(const Point& mouse_pos)
	{
	}

	void BumpMappingDemo::OnRightMouseDown(const Point& mouse_pos)
	{
	}

	void BumpMappingDemo::OnRightMouseUp(const Point& mouse_pos)
	{
	}
}
