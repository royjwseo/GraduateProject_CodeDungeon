#include "pch.h"
#include "Mesh.h"
#include "Vertex.h"
#include "TextureModelMesh.h"


void TextureModelMesh::ReleaseUploadBuffers()
{
    if (m_pd3dPositionUploadBuffer) m_pd3dPositionUploadBuffer.Reset();
    if (m_pd3dNormalUploadBuffer) m_pd3dNormalUploadBuffer.Reset();
    if (m_ppd3dIndexUploadBuffers.data())
    {
        m_ppd3dIndexUploadBuffers.clear();
        vector<ComPtr<ID3D12Resource>>().swap(m_ppd3dIndexUploadBuffers);
    }
    if (m_pd3dTextureCoord0UploadBuffer)m_pd3dTextureCoord0UploadBuffer.Reset();
    if (m_pd3dTextureCoord1UploadBuffer)m_pd3dTextureCoord1UploadBuffer.Reset();
    if (m_pd3dTangentUploadBuffer)m_pd3dTangentUploadBuffer.Reset();
    if (m_pd3dBiTangentUploadBuffer)m_pd3dBiTangentUploadBuffer.Reset();
};


void TextureModelMesh::LoadMeshFromFile(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, const string& filePath)
{
    ifstream inFile(filePath, ios::binary);
    if (!inFile.is_open()) {
        // 파일 열기 실패 처리
        return;
    }

    char pstrToken[64] = { '\0' };
    BYTE nStrLength = 0;

    int nPositions = 0, nColors = 0, nNormals = 0, nTangents = 0, nBiTangents = 0, nTextureCoords = 0, nIndices = 0, nSubMeshes = 0, nSubIndices = 0;

    UINT nReads = 0;
    inFile.read(reinterpret_cast<char*>(&m_nVertices), sizeof(int));
    inFile.read(reinterpret_cast<char*>(&nStrLength), sizeof(BYTE));
    inFile.read(m_pstrMeshName, nStrLength);
    m_pstrMeshName[nStrLength] = '\0';

    while (!inFile.eof())
    {
        inFile.read(reinterpret_cast<char*>(&nStrLength), sizeof(BYTE));
        inFile.read(pstrToken, nStrLength);
        pstrToken[nStrLength] = '\0';

        if (!strcmp(pstrToken, "<Bounds>:"))
        {
            inFile.read(reinterpret_cast<char*>(&m_xmf3AABBCenter), sizeof(XMFLOAT3));
            inFile.read(reinterpret_cast<char*>(&m_xmf3AABBExtents), sizeof(XMFLOAT3));
        }
        else if (!strcmp(pstrToken, "<Positions>:"))
        {
            inFile.read(reinterpret_cast<char*>(&nPositions), sizeof(int));
            if (nPositions > 0)
            {
                m_nType |= VERTEXT_POSITION;
                m_pxmf3Positions.resize(nPositions);
                inFile.read(reinterpret_cast<char*>(m_pxmf3Positions.data()), sizeof(XMFLOAT3) * nPositions);

                m_pd3dPositionBuffer = Util::CreateBufferResource(_Device, _CommandList, m_pxmf3Positions.data(), sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);

                D3D12_VERTEX_BUFFER_VIEW m_pd3dPositionBufferView;
                m_pd3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
                m_pd3dPositionBufferView.StrideInBytes = sizeof(XMFLOAT3);
                m_pd3dPositionBufferView.SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;
                m_pd3dVertexBufferViews.emplace_back(m_pd3dPositionBufferView);
            }
        }
        if (!strcmp(pstrToken, "<Colors>:"))
        {
            inFile.read(reinterpret_cast<char*>(&nColors), sizeof(int));
            if (nColors > 0)
            {
                m_nType |= VERTEXT_COLOR;
                std::vector<XMFLOAT4> m_pxmf4Colors;
                m_pxmf4Colors.resize(nColors);
                inFile.read(reinterpret_cast<char*>(m_pxmf4Colors.data()), sizeof(XMFLOAT4) * nColors);
            }
        }
        else if (!strcmp(pstrToken, "<TextureCoords0>:"))
        {
            inFile.read(reinterpret_cast<char*>(&nTextureCoords), sizeof(int));
            if (nTextureCoords > 0)
            {
                m_nType |= VERTEXT_TEXTURE_COORD0;
                m_pxmf2TextureCoords0.resize(nTextureCoords);
                inFile.read(reinterpret_cast<char*>(m_pxmf2TextureCoords0.data()), sizeof(XMFLOAT2) * nTextureCoords);

                m_pd3dTextureCoord0Buffer = Util::CreateBufferResource(_Device, _CommandList, m_pxmf2TextureCoords0.data(), sizeof(XMFLOAT2) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dTextureCoord0UploadBuffer);

                D3D12_VERTEX_BUFFER_VIEW m_pd3dTextureCoord0BufferView;
                m_pd3dTextureCoord0BufferView.BufferLocation = m_pd3dTextureCoord0Buffer->GetGPUVirtualAddress();
                m_pd3dTextureCoord0BufferView.StrideInBytes = sizeof(XMFLOAT2);
                m_pd3dTextureCoord0BufferView.SizeInBytes = sizeof(XMFLOAT2) * m_nVertices;
                m_pd3dVertexBufferViews.push_back(m_pd3dTextureCoord0BufferView);
            }
        }
        else if (!strcmp(pstrToken, "<TextureCoords1>:"))
        {
            inFile.read(reinterpret_cast<char*>(&nTextureCoords), sizeof(int));
            if (nTextureCoords > 0)
            {
                m_nType |= VERTEXT_TEXTURE_COORD1;
                m_pxmf2TextureCoords1.resize(nTextureCoords);
                inFile.read(reinterpret_cast<char*>(m_pxmf2TextureCoords1.data()), sizeof(XMFLOAT2) * nTextureCoords);

                m_pd3dTextureCoord1Buffer = Util::CreateBufferResource(_Device, _CommandList, m_pxmf2TextureCoords1.data(), sizeof(XMFLOAT2) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dTextureCoord1UploadBuffer);

                D3D12_VERTEX_BUFFER_VIEW m_pd3dTextureCoord1BufferView;
                m_pd3dTextureCoord1BufferView.BufferLocation = m_pd3dTextureCoord1Buffer->GetGPUVirtualAddress();
                m_pd3dTextureCoord1BufferView.StrideInBytes = sizeof(XMFLOAT2);
                m_pd3dTextureCoord1BufferView.SizeInBytes = sizeof(XMFLOAT2) * m_nVertices;
                //m_pd3dVertexBufferViews.push_back(m_pd3dTextureCoord1BufferView);
            }
        }
        if (!strcmp(pstrToken, "<Normals>:"))
        {
            inFile.read(reinterpret_cast<char*>(&nNormals), sizeof(int));
            if (nNormals > 0)
            {
                m_nType |= VERTEXT_NORMAL;
                m_pxmf3Normals.resize(nNormals);
                inFile.read(reinterpret_cast<char*>(m_pxmf3Normals.data()), sizeof(XMFLOAT3) * nNormals);

                m_pd3dNormalBuffer = Util::CreateBufferResource(_Device, _CommandList, m_pxmf3Normals.data(), sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dNormalUploadBuffer);

                D3D12_VERTEX_BUFFER_VIEW m_pd3dNormalBufferView;
                m_pd3dNormalBufferView.BufferLocation = m_pd3dNormalBuffer->GetGPUVirtualAddress();
                m_pd3dNormalBufferView.StrideInBytes = sizeof(XMFLOAT3);
                m_pd3dNormalBufferView.SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;
                m_pd3dVertexBufferViews.push_back(m_pd3dNormalBufferView);
            }
        }
        else if (!strcmp(pstrToken, "<Tangents>:"))
        {
            inFile.read(reinterpret_cast<char*>(&nTangents), sizeof(int));
            if (nTangents > 0)
            {
                m_nType |= VERTEXT_TANGENT;
                m_pxmf3Tangents.resize(nTangents);
                inFile.read(reinterpret_cast<char*>(m_pxmf3Tangents.data()), sizeof(XMFLOAT3) * nTangents);
            }
        }
        if (!strcmp(pstrToken, "<BiTangents>:"))
        {
            inFile.read(reinterpret_cast<char*>(&nBiTangents), sizeof(int));
            if (nBiTangents > 0)
            {
                m_pxmf3BiTangents.resize(nBiTangents);
                inFile.read(reinterpret_cast<char*>(m_pxmf3BiTangents.data()), sizeof(XMFLOAT3) * nBiTangents);
            }
        }
        else if (!strcmp(pstrToken, "<SubMeshes>:"))
        {
            inFile.read(reinterpret_cast<char*>(&m_nSubMeshes), sizeof(int));
            if (m_nSubMeshes > 0)
            {
                m_pnSubSetIndices.resize(m_nSubMeshes);
                m_ppnSubSetIndices.resize(m_nSubMeshes);
                m_ppd3dIndexBuffers.resize(m_nSubMeshes);
                m_ppd3dIndexUploadBuffers.resize(m_nSubMeshes);
                m_pd3dIndexBufferViews.resize(m_nSubMeshes);

                for (int i = 0; i < m_nSubMeshes; ++i)
                {
                    inFile.read(reinterpret_cast<char*>(&nStrLength), sizeof(BYTE));
                    inFile.read(pstrToken, nStrLength);
                    pstrToken[nStrLength] = '\0';
                    if (!strcmp(pstrToken, "<SubMesh>:"))
                    {
                        int nIndex = 0;
                        inFile.read(reinterpret_cast<char*>(&nIndex), sizeof(int));
                        inFile.read(reinterpret_cast<char*>(&(m_pnSubSetIndices[i])), sizeof(int));
                        if (m_pnSubSetIndices[i] > 0)
                        {
                            m_ppnSubSetIndices[i].resize(m_pnSubSetIndices[i]);
                            inFile.read(reinterpret_cast<char*>(m_ppnSubSetIndices[i].data()), sizeof(UINT) * m_pnSubSetIndices[i]);

                            m_ppd3dIndexBuffers[i] = Util::CreateBufferResource(_Device, _CommandList, m_ppnSubSetIndices[i].data(), sizeof(UINT) * m_pnSubSetIndices[i], D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_ppd3dIndexUploadBuffers[i]);

                            m_pd3dIndexBufferViews[i].BufferLocation = m_ppd3dIndexBuffers[i]->GetGPUVirtualAddress();
                            m_pd3dIndexBufferViews[i].Format = DXGI_FORMAT_R32_UINT;
                            m_pd3dIndexBufferViews[i].SizeInBytes = sizeof(UINT) * m_pnSubSetIndices[i];
                        }
                    }
                }
            }
        }
        else if (!strcmp(pstrToken, "</Mesh>"))
        {
            break;
        }
    }

    inFile.close();

}


void TextureModelMesh::LoadMeshFromFile(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, FILE* pInFile)
{
    char pstrToken[64] = { '\0' };
    BYTE nStrLength = 0;
    int nPositions = 0, nColors = 0, nNormals = 0, nTangents = 0, nBiTangents = 0, nTextureCoords = 0, nIndices = 0, nSubMeshes = 0, nSubIndices = 0;

    UINT nReads = (UINT)::fread(&m_nVertices, sizeof(int), 1, pInFile);
    nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
    nReads = (UINT)::fread(m_pstrMeshName, sizeof(char), nStrLength, pInFile);
    m_pstrMeshName[nStrLength] = '\0';

    for (; ; )
    {
        nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
        nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
        pstrToken[nStrLength] = '\0';

        if (!strcmp(pstrToken, "<Bounds>:"))
        {
            nReads = (UINT)::fread(&m_xmf3AABBCenter, sizeof(XMFLOAT3), 1, pInFile);
            nReads = (UINT)::fread(&m_xmf3AABBExtents, sizeof(XMFLOAT3), 1, pInFile);
            //2024-01-18 이성현
            //AABB로 BoundingOrientedBox생성
            CreateOrientedBoxFromAABB();
        }
        else if (!strcmp(pstrToken, "<Positions>:"))
        {
            nReads = (UINT)::fread(&nPositions, sizeof(int), 1, pInFile);
            if (nPositions > 0)
            {
                m_nType |= VERTEXT_POSITION;
                m_pxmf3Positions.resize(nPositions);
                nReads = (UINT)::fread(m_pxmf3Positions.data(), sizeof(XMFLOAT3), nPositions, pInFile);
                m_pd3dPositionBuffer = Util::CreateBufferResource(_Device, _CommandList, m_pxmf3Positions.data(), sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);

                D3D12_VERTEX_BUFFER_VIEW m_pd3dPositionBufferView;
                m_pd3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
                m_pd3dPositionBufferView.StrideInBytes = sizeof(XMFLOAT3);
                m_pd3dPositionBufferView.SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;
                m_pd3dVertexBufferViews.emplace_back(m_pd3dPositionBufferView);
            }
        }
        else if (!strcmp(pstrToken, "<Colors>:"))
        {
            nReads = (UINT)::fread(&nColors, sizeof(int), 1, pInFile);
            if (nColors > 0)
            {
                m_nType |= VERTEXT_COLOR;
                std::vector<XMFLOAT4> m_pxmf4Colors;
                m_pxmf4Colors.resize(nColors);
                nReads = (UINT)::fread(m_pxmf4Colors.data(), sizeof(XMFLOAT4), nColors, pInFile);
            }
        }
        else if (!strcmp(pstrToken, "<TextureCoords0>:"))
        {
            nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
            if (nTextureCoords > 0)
            {
                m_nType |= VERTEXT_TEXTURE_COORD0;
                m_pxmf2TextureCoords0.resize(nTextureCoords);
                nReads = (UINT)::fread(m_pxmf2TextureCoords0.data(), sizeof(XMFLOAT2), nTextureCoords, pInFile);

                m_pd3dTextureCoord0Buffer = Util::CreateBufferResource(_Device, _CommandList, m_pxmf2TextureCoords0.data(), sizeof(XMFLOAT2) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dTextureCoord0UploadBuffer);

                D3D12_VERTEX_BUFFER_VIEW m_pd3dTextureCoord0BufferView;
                m_pd3dTextureCoord0BufferView.BufferLocation = m_pd3dTextureCoord0Buffer->GetGPUVirtualAddress();
                m_pd3dTextureCoord0BufferView.StrideInBytes = sizeof(XMFLOAT2);
                m_pd3dTextureCoord0BufferView.SizeInBytes = sizeof(XMFLOAT2) * m_nVertices;
                m_pd3dVertexBufferViews.push_back(m_pd3dTextureCoord0BufferView);
            }
        }
        else if (!strcmp(pstrToken, "<TextureCoords1>:"))
        {
            nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
            if (nTextureCoords > 0)
            {
                m_nType |= VERTEXT_TEXTURE_COORD1;

                m_pxmf2TextureCoords1.resize(nTextureCoords);
                nReads = (UINT)::fread(m_pxmf2TextureCoords1.data(), sizeof(XMFLOAT2), nTextureCoords, pInFile);

                m_pd3dTextureCoord1Buffer = Util::CreateBufferResource(_Device, _CommandList, m_pxmf2TextureCoords1.data(), sizeof(XMFLOAT2) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dTextureCoord1UploadBuffer);

                D3D12_VERTEX_BUFFER_VIEW m_pd3dTextureCoord1BufferView;
                m_pd3dTextureCoord1BufferView.BufferLocation = m_pd3dTextureCoord1Buffer->GetGPUVirtualAddress();
                m_pd3dTextureCoord1BufferView.StrideInBytes = sizeof(XMFLOAT2);
                m_pd3dTextureCoord1BufferView.SizeInBytes = sizeof(XMFLOAT2) * m_nVertices;
                //m_pd3dVertexBufferViews.push_back(m_pd3dTextureCoord1BufferView);
            }
        }
        else if (!strcmp(pstrToken, "<Normals>:"))
        {
            nReads = (UINT)::fread(&nNormals, sizeof(int), 1, pInFile);
            if (nNormals > 0)
            {
                m_nType |= VERTEXT_NORMAL;
                m_pxmf3Normals.resize(nNormals);
                nReads = (UINT)::fread(m_pxmf3Normals.data(), sizeof(XMFLOAT3), nNormals, pInFile);

                m_pd3dNormalBuffer = Util::CreateBufferResource(_Device,_CommandList, m_pxmf3Normals.data(), sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dNormalUploadBuffer);

                D3D12_VERTEX_BUFFER_VIEW m_pd3dNormalBufferView;
                m_pd3dNormalBufferView.BufferLocation = m_pd3dNormalBuffer->GetGPUVirtualAddress();
                m_pd3dNormalBufferView.StrideInBytes = sizeof(XMFLOAT3);
                m_pd3dNormalBufferView.SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;
                m_pd3dVertexBufferViews.push_back(m_pd3dNormalBufferView);
            }
        }
        else if (!strcmp(pstrToken, "<Tangents>:"))
        {
            nReads = (UINT)::fread(&nTangents, sizeof(int), 1, pInFile);
            if (nTangents > 0)
            {
                m_nType |= VERTEXT_TANGENT;
                m_pxmf3Tangents.resize(nTangents);
                nReads = (UINT)::fread(m_pxmf3Tangents.data(), sizeof(XMFLOAT3), nTangents, pInFile);
            }
        }
        else if (!strcmp(pstrToken, "<BiTangents>:"))
        {
            nReads = (UINT)::fread(&nBiTangents, sizeof(int), 1, pInFile);
            if (nBiTangents > 0)
            {
                m_pxmf3BiTangents.resize(nBiTangents);
                nReads = (UINT)::fread(m_pxmf3BiTangents.data(), sizeof(XMFLOAT3), nBiTangents, pInFile);
            }
        }
        else if (!strcmp(pstrToken, "<SubMeshes>:"))
        {
            nReads = (UINT)::fread(&(m_nSubMeshes), sizeof(int), 1, pInFile);
            if (m_nSubMeshes > 0)
            {
                // 서브 메쉬 인덱스 개수
                m_pnSubSetIndices.resize(m_nSubMeshes);
                m_ppnSubSetIndices.resize(m_nSubMeshes);


                // 서브 메쉬 인덱스 버퍼
                m_ppd3dIndexBuffers.resize(m_nSubMeshes);

                // 서브 메쉬 인덱스 업로드 버퍼
                m_ppd3dIndexUploadBuffers.resize(m_nSubMeshes);

                // 서브 메쉬 인덱스 버퍼 뷰
                m_pd3dIndexBufferViews.resize(m_nSubMeshes);

                for (int i = 0; i < m_nSubMeshes; i++)
                {
                    nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
                    nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
                    pstrToken[nStrLength] = '\0';
                    if (!strcmp(pstrToken, "<SubMesh>:"))
                    {
                        int nIndex = 0;
                        nReads = (UINT)::fread(&nIndex, sizeof(int), 1, pInFile);
                        nReads = (UINT)::fread(&(m_pnSubSetIndices[i]), sizeof(int), 1, pInFile);
                        if (m_pnSubSetIndices[i] > 0)
                        {
                            // 서브 메쉬들의 인덱스 들
                            m_ppnSubSetIndices[i].resize(m_pnSubSetIndices[i]);
                            nReads = (UINT)::fread(m_ppnSubSetIndices[i].data(), sizeof(UINT) * m_pnSubSetIndices[i], 1, pInFile);

                            m_ppd3dIndexBuffers[i] = Util::CreateBufferResource(_Device, _CommandList, m_ppnSubSetIndices[i].data(), sizeof(UINT) * m_pnSubSetIndices[i], D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_ppd3dIndexUploadBuffers[i]);

                            m_pd3dIndexBufferViews[i].BufferLocation = m_ppd3dIndexBuffers[i]->GetGPUVirtualAddress();
                            m_pd3dIndexBufferViews[i].Format = DXGI_FORMAT_R32_UINT;
                            m_pd3dIndexBufferViews[i].SizeInBytes = sizeof(UINT) * m_pnSubSetIndices[i];
                        }
                    }
                }
            }
        }
        else if (!strcmp(pstrToken, "</Mesh>"))
        {
            break;
        }
    }
}
