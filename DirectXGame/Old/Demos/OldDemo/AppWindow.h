#pragma once
#include <vector>

#include <GameEngine/GraphicsEngine/RenderSystem/ConstantBuffer/ConstantBuffer.h>
#include <GameEngine/GraphicsEngine/RenderSystem/IndexBuffer/IndexBuffer.h>
#include <GameEngine/GraphicsEngine/RenderSystem/PixelShader/PixelShader.h>
#include <GameEngine/GraphicsEngine/RenderSystem/SwapChain/SwapChain.h>
#include <GameEngine/GraphicsEngine/RenderSystem/VertexBuffer/VertexBuffer.h>
#include <GameEngine/GraphicsEngine/RenderSystem/VertexShader/VertexShader.h>
#include <GameEngine/InputSystem/IInputListener.h>
#include <GameEngine/Math/Matrix4x4.h>
#include <GameEngine/WindowingSystem/Window.h>

#define STATUE 1
//#define SUZANNE 1
//define EARTH 1
//#define HOUSE 1

namespace Ge
{
	class AppWindow : public Window, public IInputListener
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
		void UpdateModel(const Vector3D& position, const Vector3D& scale, const std::vector<MaterialPtr>& materials);
		void UpdateSkyBox();
		void UpdateLight();
		void DrawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials);

	private:
		SwapChainPtr mp_swap_chain;
		VertexBufferPtr mp_vb;
		VertexShaderPtr mp_vs;
		PixelShaderPtr mp_ps;
		PixelShaderPtr mp_sky_ps;
		ConstantBufferPtr mp_cb;
		ConstantBufferPtr mp_sky_cb;
		IndexBufferPtr mp_ib;

		TexturePtr mp_earth_tex;
		TexturePtr mp_earth_specular_tex;
		TexturePtr mp_earth_night_tex;
		TexturePtr mp_wall_tex;
		TexturePtr mp_bricks_tex;
		TexturePtr mp_clouds_tex;
		TexturePtr mp_sky_tex;
		TexturePtr mp_sand_tex;

		//House textures
		TexturePtr mp_barrel_tex;
		TexturePtr mp_brick_tex;
		TexturePtr mp_window_tex;
		TexturePtr mp_wood_tex;

		MeshPtr mp_mesh;
		MeshPtr mp_sky_mesh;
		MeshPtr mp_torus_mesh;
		MeshPtr mp_suzanne_mesh;
		MeshPtr mp_plane_mesh;
		MeshPtr mp_terrain_mesh;
		MeshPtr mp_house_mesh;

		MaterialPtr mp_mesh_material;
		MaterialPtr mp_bricks_material;
		MaterialPtr mp_earth_material;
		MaterialPtr mp_sky_material;
		MaterialPtr mp_terrain_material;


		//House materials
		MaterialPtr mp_barrel_material;
		MaterialPtr mp_brick_material;
		MaterialPtr mp_window_material;
		MaterialPtr mp_wood_material;

		std::vector<MaterialPtr> m_materials;

		long m_old_delta;
		long m_new_delta;
		float m_delta_time;

#if STATUE
		float m_rot_x = 0.39f;
		float m_rot_y = 1.57f;
#elif SUZANNE
		float m_rot_x = 0.0f;
		float m_rot_y = 3.14f;
#elif EARTH
		float m_rot_x = 0.0f;
		float m_rot_y = 0.0f;
#elif HOUSE
		float m_rot_x = 0.707f;
		float m_rot_y = -1.57f;
#else
		float m_rot_x = 0.0f;
		float m_rot_y = 0.0f;
#endif
		float m_delta_mouse_x = 0.0f;
		float m_delta_mouse_y = 0.0f;

		float m_light_rot_y = 0.0f;

		float m_scale_cube = 1;
		float m_forward = 0.0f;
		float m_rightward = 0.0f;
		float m_upward = 0.0f;

		Matrix4x4 m_world_cam;
		Matrix4x4 m_view_cam;
		Matrix4x4 m_proj_cam;
		Matrix4x4 m_light_rot_matrix;

		float m_time = 0.0f;
		Vector4D m_light_position;
		float m_light_radius = 500.0f;
		bool m_play_state = false;
		bool m_full_screen = false;
	};
}
