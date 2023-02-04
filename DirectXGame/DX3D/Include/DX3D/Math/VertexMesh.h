#pragma once
#include <DX3D/Math/Vector2D.h>
#include <DX3D/Math/Vector3D.h>

class VertexMesh
{
public:
	VertexMesh() {}

	VertexMesh(const Vector3D& position, const Vector2D& texcoord, const Vector3D& normal, const Vector3D& tangent, const Vector3D& binormal) :
		position(position),
		texcoord(texcoord),
		normal(normal),
		tangent(tangent),
		binormal(binormal) { }

	VertexMesh(const VertexMesh& vm) :
		position(vm.position),
		texcoord(vm.texcoord),
		normal(vm.normal),
		tangent(vm.tangent),
		binormal(vm.binormal) { }

	Vector3D position;
	Vector2D texcoord;
	Vector3D normal;
	Vector3D tangent;
	Vector3D binormal;
};
