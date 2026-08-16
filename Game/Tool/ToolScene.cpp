#include "ToolScene.h"
#include "Engine.h"
#include "Texture.h"
#include "CbvSrvUavDescriptorHeap.h"
#include "GridObject.h"
#include "NavigationCell.h"

bool ToolScene::ProcessInput(UCHAR* pKeysBuffer)
{
	return false;
}

void ToolScene::BuildObjects(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, const ComPtr<ID3D12RootSignature>& _RootSignature)
{
	CreateCbvSrvDescriptorHeaps(0, 200);

	XMFLOAT3 xmf3Scale(8.0f, 1.0f, 8.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.0f, 0.5f);
	shared_ptr<GridObject> pToolGrid = make_shared<GridObject>(DEVICE.Get(), CMDLIST.Get(), GRAPHICS_ROOT_SIGNATURE, 50, 50, xmf3Scale, xmf4Color);
	m_ppObjects.push_back(static_pointer_cast<CGameObject>(pToolGrid));

	shared_ptr<CGameObject> pTestModel = make_shared<CGameObject>();
	shared_ptr<CTexturedModelShader> pShader = make_shared<CTexturedModelShader>(_Device, _RootSignature, L"Shader/cso/StandardVS.cso", L"Shader/cso/StandardPS.cso");
	pTestModel->LoadGeometryFromFile(_Device.Get(), _CommandList.Get(), _RootSignature.Get(), "Model/MainTank.bin", pShader);
	shared_ptr<BoundingBoxObject> pBobObject = make_shared<BoundingBoxObject>(_Device, _CommandList, _RootSignature, pTestModel->GetAABBCenter(), pTestModel->GetAABBExtents());
	pTestModel->SetChild(pBobObject);
	pTestModel->SetType(MOB);
	pTestModel->SetShader(pShader);
	pTestModel->Rotate(0.f, 180.f, 0.f);
	m_ppObjects.push_back(pTestModel);

}


