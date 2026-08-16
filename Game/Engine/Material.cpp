#include "pch.h"
#include "Material.h"
#include "GameObject.h"
#include "Texture.h"
#include "Shader.h"

CMaterial::CMaterial()
{

}
CMaterial::~CMaterial()
{
}
void CMaterial::SetShader(std::shared_ptr<CShader> pShader)
{
	m_pShader = pShader;
}
void CMaterial::SetTexture(std::shared_ptr<CTexture> pTexture)
{
	m_pTexture = pTexture;
}
void CMaterial::ReleaseUploadBuffers()
{
	if (m_pTexture) m_pTexture->ReleaseUploadBuffers();
}
void CMaterial::UpdateShaderVariables(const ComPtr<ID3D12GraphicsCommandList>& _CommandList)
{
	if (m_pTexture) m_pTexture->UpdateShaderVariables(_CommandList);

	/*_CommandList->SetGraphicsRoot32BitConstants(1, 4, &m_xmf4AmbientColor, 16);
	_CommandList->SetGraphicsRoot32BitConstants(1, 4, &m_xmf4AlbedoColor, 20);
	_CommandList->SetGraphicsRoot32BitConstants(1, 4, &m_xmf4SpecularColor, 24);
	_CommandList->SetGraphicsRoot32BitConstants(1, 4, &m_xmf4EmissiveColor, 28);*/
	_CommandList->SetGraphicsRoot32BitConstants(1, 1, &m_nType, 16);
}

void CMaterial::ReleaseShaderVariables()
{
	if (m_pTexture) m_pTexture->ReleaseShaderVariables();
}