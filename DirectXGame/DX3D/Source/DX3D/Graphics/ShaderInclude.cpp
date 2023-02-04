// ReSharper disable CppClangTidyBugproneBranchClone
// ReSharper disable CppClangTidyClangDiagnosticSwitchEnum
#include <DX3D/Graphics/ShaderInclude.h>

namespace DX3D
{
	ShaderInclude::ShaderInclude(const char* include_dir)
		: m_include_dir(include_dir) {}

	ShaderInclude::~ShaderInclude() {}

	__declspec(nothrow) HRESULT __stdcall ShaderInclude::Open(
		D3D_INCLUDE_TYPE includeType,
		LPCSTR pFileName,
		LPCVOID pParentData,
		LPCVOID* ppData,
		UINT* pBytes)
	{
		try
		{
			std::wstring finalPath;
			switch (includeType)
			{
			case D3D_INCLUDE_LOCAL: // #include "FILE"
				finalPath = ResolvePath(std::filesystem::path(m_include_dir).wstring().c_str(), std::filesystem::path(pFileName).wstring().c_str()).wstring();
				break;
			case D3D_INCLUDE_SYSTEM: // #include <FILE>
				finalPath = ResolvePath(std::filesystem::path(m_include_dir).wstring().c_str(), std::filesystem::path(pFileName).wstring().c_str()).wstring();
				break;
			default: // NOLINT(clang-diagnostic-covered-switch-default)
				assert(0);
			}

			std::ifstream ifs(finalPath);
			if (ifs.is_open())
			{
				std::stringstream sstr;
				sstr << ifs.rdbuf();
				auto str = sstr.str();

				uint buf_size = static_cast<uint>(str.size() + 1);
				auto shader_code = new char[buf_size];
				memcpy(shader_code, str.c_str(), buf_size);
				ifs.close();

				*ppData = shader_code;
				*pBytes = static_cast<uint>(str.size());
			}
			else
			{
				*ppData = nullptr;
				*pBytes = 0;
			}
			return S_OK;
		}
		catch (std::exception& ex)
		{
			DX3DError("Failed to read HLSL include files in '" << m_include_dir.c_str() << "': " << ex.what());
			return E_FAIL;
		}
	}

	__declspec(nothrow) HRESULT __stdcall ShaderInclude::Close(LPCVOID pData)
	{
		auto buf = static_cast<const char*>(pData);
		delete[] buf;
		return S_OK;
	}
}
