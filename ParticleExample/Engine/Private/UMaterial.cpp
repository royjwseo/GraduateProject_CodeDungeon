#include "EngineDefines.h"
#include "UMaterial.h"
#include "UShader.h"
#include "UTexture.h"

UMaterial::UMaterial(CSHPTRREF<UDevice> _spDevice) : 
    UComponent(_spDevice)
{
}

UMaterial::UMaterial(const UMaterial& _rhs) : 
    UComponent(_rhs)
{
}

void UMaterial::Free()
{
}

HRESULT UMaterial::NativeConstruct()
{
    return __super::NativeConstruct();
}

HRESULT UMaterial::NativeConstructClone(const VOIDDATAS& _convector)
{
    return __super::NativeConstructClone(_convector);
}

void UMaterial::AddTexturel( const SRV_REGISTER _eSrvRegister, CSHPTRREF<UTexture> _spTexture)
{
    RETURN_CHECK(nullptr == _spTexture, ;);

    const auto& iter = m_TextxureConatiner.find(_eSrvRegister);

    if (iter == m_TextxureConatiner.end())
    {
        m_TextxureConatiner.insert(std::pair<SRV_REGISTER, SHPTR<UTexture>>(_eSrvRegister, _spTexture));
    }
    else
    {
        // Change Texture 
        iter->second = _spTexture;
    }
}

void UMaterial::BindTexture(CSHPTRREF<UShader> _spShader)
{
    RETURN_CHECK(nullptr == _spShader, ;);
    // Bind SRV 
    for (auto& iter : m_TextxureConatiner)
    {
        _spShader->BindSRVBuffer(iter.first, iter.second);
    }
}
