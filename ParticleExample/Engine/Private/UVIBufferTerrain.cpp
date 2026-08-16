#include "EngineDefines.h"
#include "UVIBufferTerrain.h"
#include "UVIBuffer.h"
#include "UMethod.h"
#include "URawData.h"

UVIBufferTerrain::UVIBufferTerrain(CSHPTRREF<UDevice> _spDevice)
	: UVIBuffer(_spDevice, VISPACE_TYPE::SPACE_3D, VIINSTANCE_TYPE::SINGLE), m_stTerrainDesc({}), m_iMaxIndex{0}
{
}

UVIBufferTerrain::UVIBufferTerrain(const UVIBufferTerrain& _rhs)
	: UVIBuffer(_rhs), m_stTerrainDesc(_rhs.m_stTerrainDesc), m_iMaxIndex{ m_stTerrainDesc.iNumVerticiesX * m_stTerrainDesc.iNumVerticiesZ }
{
}

void UVIBufferTerrain::Free()
{
}

HRESULT UVIBufferTerrain::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UVIBufferTerrain::NativeConstruct(const TERRAINDESC& _stTerrainDesc, const _float& _fHeightSize )
{
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
	{
		m_stTerrainDesc = _stTerrainDesc;
		m_iMaxIndex = m_stTerrainDesc.iNumVerticiesX * m_stTerrainDesc.iNumVerticiesZ;
		const _uint VERTEX_CNT = _stTerrainDesc.iNumVerticiesX * _stTerrainDesc.iNumVerticiesZ;
		VECTOR<VTXNORMAL>		VertexNormal(VERTEX_CNT);
		VECTOR<_float3>		VertexPos(VERTEX_CNT);

		for (_uint i = 0; i < m_stTerrainDesc.iNumVerticiesZ; ++i)
		{
			for (_uint j = 0; j < m_stTerrainDesc.iNumVerticiesX; ++j)
			{
				// iIndex
				_uint iIndex = i * m_stTerrainDesc.iNumVerticiesX + j;
				// Vertex Normal
				VertexNormal[iIndex].vPosition= _float3((_float)j, 0.f, (_float)i) ;
				VertexNormal[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
				VertexNormal[iIndex].vTexUV = _float2((_float)j / (m_stTerrainDesc.iNumVerticiesX - 1.f),
					(_float)i / (m_stTerrainDesc.iNumVerticiesZ - 1.f));
			}
		}
		// Create Index
		RETURN_CHECK_FAILED(CreateIndex(VertexNormal), E_FAIL);

		for (auto& iter : VertexNormal)
			iter.vNormal.Normalize();
		// Create Vertex
		RETURN_CHECK_FAILED(CreateVtxBuffer(VERTEX_CNT, sizeof(VTXNORMAL), VertexNormal.data(),
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, VertexPos), E_FAIL);
	}

	return S_OK;
}

HRESULT UVIBufferTerrain::NativeConstruct(const _wstring& _wstrMapFilePath, const _float& _fHeightSize)
{
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
	HANDLE hFile = CreateFile(_wstrMapFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0);
	{
		_ulong dwByte{ 0 };

		BITMAPFILEHEADER BitMapFileHeader;
		BITMAPINFOHEADER BitMapInfoHeader;

		ReadFile(hFile, &BitMapFileHeader, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
		ReadFile(hFile, &BitMapInfoHeader, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

		m_stTerrainDesc.iNumVerticiesX = BitMapInfoHeader.biWidth;
		m_stTerrainDesc.iNumVerticiesZ = BitMapInfoHeader.biHeight;

		m_iMaxIndex = m_stTerrainDesc.iNumVerticiesX * m_stTerrainDesc.iNumVerticiesZ;

		{
			const _uint VERTEX_CNT{ m_stTerrainDesc.iNumVerticiesX * m_stTerrainDesc.iNumVerticiesZ };
			VECTOR<VTXNORMAL>		VertexNormal(VERTEX_CNT);
			VECTOR<_float3>		VertexPos(VERTEX_CNT);
			_ulong* pPixel = new _ulong[VERTEX_CNT];
			UMethod::ClearToMemory(pPixel);
			ReadFile(hFile, pPixel, sizeof(_ulong) * VERTEX_CNT, &dwByte, nullptr);
			// ===================================================
			for (_uint i = 0; i < m_stTerrainDesc.iNumVerticiesZ; ++i)
			{
				for (_uint j = 0; j < m_stTerrainDesc.iNumVerticiesX; ++j)
				{
					// iIndex
					_uint iIndex = i * m_stTerrainDesc.iNumVerticiesX + j;
					// Vertex Normal
					VertexNormal[iIndex].vPosition = VertexPos[iIndex] = _float3((_float)j, (pPixel[iIndex] & 0x000000ff) / _fHeightSize, (_float)i) ;
					VertexNormal[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
					VertexNormal[iIndex].vTexUV = _float2((_float)j / (m_stTerrainDesc.iNumVerticiesX - 1.f),
						(_float)i / (m_stTerrainDesc.iNumVerticiesZ - 1.f));
				}
			}
			// Create Index
			RETURN_CHECK_FAILED(CreateIndex(VertexNormal), E_FAIL);

			for (auto& iter : VertexNormal)
				iter.vNormal.Normalize();
			// Create Vertex
			RETURN_CHECK_FAILED(CreateVtxBuffer(VERTEX_CNT, sizeof(VTXNORMAL), VertexNormal.data(),
				D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, VertexPos), E_FAIL);

			Safe_Delete_Array(pPixel);
		}
		CloseHandle(hFile);
	}

	return S_OK;
}

HRESULT UVIBufferTerrain::NativeConstruct(CSHPTRREF<URawData> _spRawData, const _float& _fHeightSize)
{
	RETURN_CHECK(nullptr == _spRawData, E_FAIL);
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);

	{
		m_stTerrainDesc.iNumVerticiesX = _spRawData->GetDataWidth();
		m_stTerrainDesc.iNumVerticiesZ = _spRawData->GetDataHeight();
		m_iMaxIndex = m_stTerrainDesc.iNumVerticiesX * m_stTerrainDesc.iNumVerticiesZ;
		const _uint VERTEX_CNT{ m_stTerrainDesc.iNumVerticiesX * m_stTerrainDesc.iNumVerticiesZ };

		VECTOR<VTXNORMAL>		VertexNormal(VERTEX_CNT);
		VECTOR<_float3>		VertexPos(VERTEX_CNT);

		// ===================================================
		for (_uint i = 0; i < m_stTerrainDesc.iNumVerticiesZ; ++i)
		{
			for (_uint j = 0; j < m_stTerrainDesc.iNumVerticiesX; ++j)
			{
				// iIndex
				_uint iIndex = i * m_stTerrainDesc.iNumVerticiesX + j;
				// Vertex Normal
				VertexNormal[iIndex].vPosition = VertexPos[iIndex] = 
					_float3((_float)j, (_spRawData->GetRawImagePixel()[iIndex] & 0x000000ff) / _fHeightSize, (_float)i) ;
				VertexNormal[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
				VertexNormal[iIndex].vTexUV = _float2((_float)j / (m_stTerrainDesc.iNumVerticiesX - 1.f),
					(_float)i / (m_stTerrainDesc.iNumVerticiesZ - 1.f));
			}
		}
		// Create Index
		RETURN_CHECK_FAILED(CreateIndex(VertexNormal), E_FAIL);

		for (auto& iter : VertexNormal)
			iter.vNormal.Normalize();

		// Create Vertex
		RETURN_CHECK_FAILED(CreateVtxBuffer(VERTEX_CNT, sizeof(VTXNORMAL), VertexNormal.data(),
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, VertexPos), E_FAIL);
	}

	return S_OK;
}

HRESULT UVIBufferTerrain::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	return __super::NativeConstructClone(_vecDatas);
}

bool UVIBufferTerrain::TerrainHeightPos(_float3& _vPosition, const _float4x4& _mWorldMatirx)
{
	_float4x4 vInverseMatrix = _mWorldMatirx.Invert();

	_float3			vLocalPoint = XMVector3TransformCoord(_vPosition, vInverseMatrix);

	_uint			iIndex = (_uint)vLocalPoint.z * m_stTerrainDesc.iNumVerticiesX + (_uint)vLocalPoint.x;

	_uint			iIndices[] = {
		iIndex + m_stTerrainDesc.iNumVerticiesX,
		iIndex + m_stTerrainDesc.iNumVerticiesX + 1,
		iIndex + 1,
		iIndex
	};

	if (iIndices[0] >= m_iMaxIndex)
		return false;

	_float			fWidth = vLocalPoint.x - (*GetVertexPos())[iIndices[0]].x;
	_float			fDepth = (*GetVertexPos())[iIndices[0]].z - vLocalPoint.z;

	_float4			vPlane;

	if (fWidth > fDepth)
	{
		if (iIndices[1] >= m_iMaxIndex || iIndices[2] >= m_iMaxIndex)
			return false;

		vPlane = DirectX::XMPlaneFromPoints((*GetVertexPos())[iIndices[0]],
			(*GetVertexPos())[iIndices[1]],
			(*GetVertexPos())[iIndices[2]]);
	}
	else
	{
		if (iIndices[2] >= m_iMaxIndex || iIndices[3] >= m_iMaxIndex)
			return false;

		vPlane = DirectX::XMPlaneFromPoints((*GetVertexPos())[iIndices[0]],
			(*GetVertexPos())[iIndices[2]],
			(*GetVertexPos())[iIndices[3]]);
	}

	vLocalPoint.y = (-vPlane.x * vLocalPoint.x - vPlane.z * vLocalPoint.z - vPlane.w) / vPlane.y;
	vLocalPoint = XMVector3TransformCoord(vLocalPoint, _mWorldMatirx);
	_vPosition.y = vLocalPoint.y;
	return true;
}

HRESULT UVIBufferTerrain::CreateIndex(VECTOR<VTXNORMAL>& _vecVertex)
{
	// Create Index
	{
		const _uint INDICES_CNT = (m_stTerrainDesc.iNumVerticiesX - 1) * (m_stTerrainDesc.iNumVerticiesZ - 1) * 2;
		INDICIES32* pIndices = new INDICIES32[INDICES_CNT];
		_uint		iNumFaces = 0;
		for (_uint i = 0; i < m_stTerrainDesc.iNumVerticiesX - 1; ++i) {
			for (_uint j = 0; j < m_stTerrainDesc.iNumVerticiesZ - 1; ++j) {

				_uint iIndex = i * m_stTerrainDesc.iNumVerticiesX + j;
				_uint iIndices[] = {
					iIndex + m_stTerrainDesc.iNumVerticiesX,
					iIndex + m_stTerrainDesc.iNumVerticiesX + 1,
					iIndex + 1,
					iIndex
				};

				pIndices[iNumFaces]._1 = iIndices[0];
				pIndices[iNumFaces]._2 = iIndices[1];
				pIndices[iNumFaces]._3 = iIndices[2];
				ComputeNormal(_vecVertex, pIndices, iNumFaces);

				pIndices[iNumFaces]._1 = iIndices[0];
				pIndices[iNumFaces]._2 = iIndices[2];
				pIndices[iNumFaces]._3 = iIndices[3];
				ComputeNormal(_vecVertex, pIndices, iNumFaces);
			}
		}

		RETURN_CHECK_FAILED(CreateIndexBuffer(INDICES_CNT, sizeof(INDICIES32), pIndices, DXGI_FORMAT_R32_UINT), E_FAIL);
	}
	return S_OK;
}

HRESULT UVIBufferTerrain::ComputeNormal(VECTOR<VTXNORMAL>& _vecVertex, INDICIES32* _pIndices, _uint& _iIndex)
{
	_float3 vSour, vDest, vNormal;
	// Calculate 
	vSour = _vecVertex[_pIndices[_iIndex]._2].vPosition - _vecVertex[_pIndices[_iIndex]._1].vPosition;
	vDest = _vecVertex[_pIndices[_iIndex]._3].vPosition - _vecVertex[_pIndices[_iIndex]._2].vPosition;
	vNormal = DirectX::XMVector3Cross(vSour, vDest);
	// Normal Vertex + 
	_vecVertex[_pIndices[_iIndex]._1].vNormal += vNormal;
	_vecVertex[_pIndices[_iIndex]._2].vNormal += vNormal;
	_vecVertex[_pIndices[_iIndex]._3].vNormal += vNormal;
	++_iIndex;
	return S_OK;
}
