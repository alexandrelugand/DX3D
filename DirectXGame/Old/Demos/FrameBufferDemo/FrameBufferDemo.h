#pragma once
#include <vector>

#include <App/FrameBufferDemo/MiniGame.h>

#include <GameEngine/GraphicsEngine/Prerequisites.h>
#include <GameEngine/InputSystem/IInputListener.h>
#include <GameEngine/Math/Matrix4x4.h>
#include <GameEngine/Math/Vector3D.h>
#include <GameEngine/WindowingSystem/Window.h>

namespace Ge
{
	class FrameBufferDemo : public Window, public IInputListener
	{
	public:
		FrameBufferDemo() = default;
		~FrameBufferDemo() override = default;

		// Inherited via Window
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;
		void OnFocus() override;
		void OnKillFocus() override;
		void OnSize() override;

		// Inherited via InputListener
		void OnKeyDown(int key) override;
		void OnKeyUp(int key) override;
		void OnMouseMove(const Point& mouse_pos) override;

		void OnLeftMouseDown(const Point& mouse_pos) override;
		void OnLeftMouseUp(const Point& mouse_pos) override;
		void OnRightMouseDown(const Point& mouse_pos) override;
		void OnRightMouseUp(const Point& mouse_pos) override;

		void Render();
		void Update();
		void UpdateModel(Vector3D position, Vector3D rotation, Vector3D scale, const std::vector<MaterialPtr>& list_materials);
		void UpdateThirdPersonCamera();
		void UpdateSkyBox();
		void UpdateLight();
		void DrawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials/* MaterialPtr* list_materials, unsigned int size_list_materials*/);
		void UpdateViewportProjection();

	private:
		SwapChainPtr m_swap_chain;

		TexturePtr m_sphere_tex;
		TexturePtr m_sphere_normal_tex;
		MeshPtr m_sphere_mesh;
		MaterialPtr m_sphere_mat;

		MeshPtr m_monitor_mesh;
		MaterialPtr m_monitor_mat;
		MaterialPtr m_screen_mat;

		TexturePtr m_sky_tex;
		MeshPtr m_sky_mesh;
		MaterialPtr m_sky_mat;

		long m_old_delta;
		long m_new_delta;
		float m_delta_time;

		float m_current_cam_distance;
		float m_cam_distance = 2.4f;
		Vector3D m_current_cam_rot;
		Vector3D m_cam_rot;
		Vector3D m_cam_pos;

		float m_delta_mouse_x = 0.0f, m_delta_mouse_y = 0.0f;

		Matrix4x4 m_world_cam;
		Matrix4x4 m_view_cam;
		Matrix4x4 m_proj_cam;

		float m_time = 0.0f;

		Matrix4x4 m_light_rot_matrix;
		bool m_play_state = false;
		bool m_fullscreen_state = false;
		std::vector<MaterialPtr> m_list_materials;
		MiniGame m_mini_game;
	};
}
