#include "pch.h"
#include "Mesh.h"
#include "Vertex.h"
#include "GridMesh.h"

GridMesh::GridMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList,
    int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color)
{
    m_nWidth = nWidth;
    m_nLength = nLength;
    m_xmf3Scale = xmf3Scale;

    m_nVertices = (nWidth) * (nLength);
    m_nStride = sizeof(CDiffusedVertex);
    m_nOffset = 0;
    m_nSlot = 0;
    m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    m_pxmf3Positions.resize(m_nVertices);

    vector<CDiffusedVertex> pVertices;
    pVertices.resize(m_nVertices);

    int i = 0;
    for (int z = 0; z < nLength; z++)
    {
        for (int x = 0; x < nWidth; x++, i++)
        {
            pVertices[i] = CDiffusedVertex(XMFLOAT3(x * m_xmf3Scale.x, 0, z * m_xmf3Scale.z), xmf4Color);
            m_pxmf3Positions[i] = XMFLOAT3(x * m_xmf3Scale.x, 0, z * m_xmf3Scale.z);
        }
    }

    m_pd3dPositionBuffer = Util::CreateBufferResource(_Device, _CommandList, pVertices.data(),
        m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
        D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);

    D3D12_VERTEX_BUFFER_VIEW positionBufferView;
    positionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
    positionBufferView.StrideInBytes = m_nStride;
    positionBufferView.SizeInBytes = m_nStride * m_nVertices;

    m_pd3dVertexBufferViews.push_back(positionBufferView);

    CreateBoundingBox();

    m_nSubMeshes = 1;
    m_pnSubSetIndices.resize(m_nSubMeshes);
    m_ppnSubSetIndices.resize(m_nSubMeshes);

    int numRectangles = (m_nWidth - 1) * (m_nLength - 1);
    int indicesPerRect = 6;
    int numIndices = numRectangles * indicesPerRect;

    m_pnSubSetIndices[0] = numIndices;
    m_ppnSubSetIndices[0].resize(numIndices);

    m_ppd3dIndexBuffers.resize(m_nSubMeshes);
    m_ppd3dIndexUploadBuffers.resize(m_nSubMeshes);
    m_pd3dIndexBufferViews.resize(m_nSubMeshes);

    int index = 0;
    for (int z = 0; z < m_nLength - 1; z++)
    {
        for (int x = 0; x < m_nWidth - 1; x++)
        {          
            m_ppnSubSetIndices[0][index++] = z * m_nWidth + x;
            m_ppnSubSetIndices[0][index++] = (z + 1) * m_nWidth + x;
            m_ppnSubSetIndices[0][index++] = z * m_nWidth + (x + 1);

            m_ppnSubSetIndices[0][index++] = (z + 1) * m_nWidth + x;
            m_ppnSubSetIndices[0][index++] = (z + 1) * m_nWidth + (x + 1);
            m_ppnSubSetIndices[0][index++] = z * m_nWidth + (x + 1);
        }
    }

    m_ppd3dIndexBuffers[0] = Util::CreateBufferResource(
        _Device, _CommandList,
        m_ppnSubSetIndices[0].data(),
        sizeof(UINT) * m_pnSubSetIndices[0],
        D3D12_HEAP_TYPE_DEFAULT,
        D3D12_RESOURCE_STATE_INDEX_BUFFER,
        &m_ppd3dIndexUploadBuffers[0]);

    D3D12_INDEX_BUFFER_VIEW indexBufferView;
    indexBufferView.BufferLocation = m_ppd3dIndexBuffers[0]->GetGPUVirtualAddress();
    indexBufferView.Format = DXGI_FORMAT_R32_UINT;
    indexBufferView.SizeInBytes = sizeof(UINT) * m_pnSubSetIndices[0];
    m_pd3dIndexBufferViews[0] = indexBufferView;
}

GridMesh::~GridMesh()
{

}

