#include "pch.h"
#include "GridObject.h"
#include "Shader.h"
#include "GridMesh.h"
#include "Material.h"

GridObject::GridObject(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList,
    const ComPtr<ID3D12RootSignature>& _RootSignature, int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color)
{
    m_nWidth = nWidth;
    m_nLength = nLength;
    m_xmf3Scale = xmf3Scale;

    shared_ptr<GridMesh> pGridMesh = make_shared<GridMesh>(_Device, _CommandList, nWidth, nLength, xmf3Scale, xmf4Color);
    SetMesh(pGridMesh);

    shared_ptr<BoundingBoxObject> pBobObject = make_shared<BoundingBoxObject>(_Device, _CommandList, _RootSignature, pGridMesh->GetAABBCenter(), pGridMesh->GetAABBExtents());
    SetChild(pBobObject);

    float fWidth = static_cast<float>(nWidth) * xmf3Scale.x * 0.5f;
    float fDepth = static_cast<float>(nLength) * xmf3Scale.z * 0.5f;
    SetPosition(XMFLOAT3(-fWidth, 0.0f, -fDepth));

    shared_ptr<CDiffusedWireFrameShader> pGridShader = make_shared<CDiffusedWireFrameShader>(_Device, _RootSignature, L"Shader/cso/DiffuseVS.cso", L"Shader/cso/DiffusePS.cso");
    SetShader(pGridShader);
    SetType(GRID);
    SetActive(true);
}

GridObject::~GridObject()
{
}

