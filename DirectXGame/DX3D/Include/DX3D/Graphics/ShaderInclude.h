#pragma once

#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class ShaderInclude final : public ID3DInclude
	{
	public:
		ShaderInclude(const char* include_dir);
		virtual ~ShaderInclude();

		__declspec(nothrow) HRESULT __stdcall Open(D3D_INCLUDE_TYPE includeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override;
		__declspec(nothrow) HRESULT __stdcall Close(LPCVOID pData) override;

	private:
		std::string m_include_dir;
	};
}
