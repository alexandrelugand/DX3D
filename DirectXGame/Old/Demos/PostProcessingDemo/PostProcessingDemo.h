#pragma once
#include <vector>

#include <GameEngine/GraphicsEngine/Prerequisites.h>
#include <GameEngine/WindowingSystem/Window.h>
#include <GameEngine/InputSystem/IInputListener.h>
#include <GameEngine/GraphicsEngine/RenderSystem/SwapChain/SwapChain.h>
#include <GameEngine/Math/Matrix4x4.h>

namespace Ge
{
	class PostProcessingDemo : public Window, public IInputListener
	{
	public:
		// Inherited via Window
		void OnCreate() final;
		void OnUpdate() final;
		void OnDestroy() final;
		void OnFocus() final;
		void OnKillFocus() final;
		void OnSize() final;

		// Inherited via InputListener
		void OnKeyDown(int key) final;
		void OnKeyUp(int key) final;
		void OnMouseMove(const Point& mouse_pos) final;

		void OnLeftMouseDown(const Point& mouse_pos) final;
		void OnLeftMouseUp(const Point& mouse_pos) final;
		void OnRightMouseDown(const Point& mouse_pos) final;
		void OnRightMouseUp(const Point& mouse_pos) final;

		void Render();
		void Update();
		void UpdateCamera();
		void UpdateThirdPersonCamera();
		void UpdateViewportProjection();
		void UpdateModel(const Vector3D& position, const Vector3D& rotation, const Vector3D& scale, const std::vector<MaterialPtr>& materials);
		void UpdateSkyBox();
		void UpdateLight();
		void UpdateSpaceShip();
		void DrawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials);

	private:
		SwapChainPtr mp_swap_chain;

		TexturePtr mp_sky_tex;
		MeshPtr mp_sky_mesh;
		MaterialPtr mp_sky_material;

		TexturePtr mp_asteroid_tex;
		TexturePtr mp_asteroid_normal_tex;
		MeshPtr mp_asteroid_mesh;
		MaterialPtr mp_asteroid_material;

		TexturePtr mp_spaceship_tex;
		MeshPtr mp_spaceship_mesh;
		MaterialPtr mp_spaceship_material;

		std::vector<MaterialPtr> m_materials;

		TexturePtr mp_render_target;
		TexturePtr mp_depth_stencil;

		MeshPtr mp_quad_mesh;
		MaterialPtr mp_post_process_material;

		long m_old_delta;
		long m_new_delta;
		float m_delta_time;

		float m_delta_mouse_x = 0.0f;
		float m_delta_mouse_y = 0.0f;

		Vector3D m_asteroids_pos[200];
		Vector3D m_asteroids_rot[200];
		Vector3D m_asteroids_scale[200];

		float m_forward = 0.0f;
		float m_rightward = 0.0f;
		float m_upward = 0.0f;

		Matrix4x4 m_world_cam;
		Matrix4x4 m_view_cam;
		Matrix4x4 m_proj_cam;
		Matrix4x4 m_light_rot_matrix;

		Vector3D m_camera_position;
		Vector3D m_current_camera_rotation;
		Vector3D m_camera_rotation;
		float m_current_camera_distance;
		float m_camera_distance = 14.0f;

		Vector3D m_current_spaceship_position;
		Vector3D m_spaceship_position;
		Vector3D m_current_spaceship_rotation;
		Vector3D m_spaceship_rotation;
		float m_spaceship_speed;

		Vector4D m_light_position;

		float m_time = 0.0f;
		bool m_play_state = false;
		bool m_full_screen = false;
		bool m_turbo_mode = false;
		float m_distortion_level = 1.0f;
	};
}
