#include "Shader.hpp"
#include <iostream>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

bool Shader::Load(const wchar_t* file, const char* entry, const char* target)
{
    UINT flags = 0;

#ifdef _DEBUG
    flags = D3DCOMPILE_DEBUG;
#endif
    Microsoft::WRL::ComPtr<ID3DBlob> errors;

    HRESULT hr = D3DCompileFromFile(
        file,
        nullptr,
        nullptr,
        entry,
        target,
        flags,
        0,
        &m_shaderblob,
        &errors
    );

    if (FAILED(hr))
    {
        if (errors)
        {
            std::cout << "[SHADER]" << std::endl;
            std::cout << (char*)errors->GetBufferPointer() << std::endl;
        }

        return false;
    }

    return true;
}

D3D12_SHADER_BYTECODE Shader::GetShaderByteCode()
{
    D3D12_SHADER_BYTECODE code = {};

    code.pShaderBytecode = m_shaderblob->GetBufferPointer();
    code.BytecodeLength = m_shaderblob->GetBufferSize();

    return code;
}
