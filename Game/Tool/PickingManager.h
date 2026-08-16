#pragma once
#include "ToolDefines.h"
#include "ToolManager.h"
#include "ToolScene.h"

struct Ray
{
	XMVECTOR origin;
	XMVECTOR direction;
};

struct RayHitResult
{
	bool hit = false;
	float hitDistance = FLT_MAX;
	shared_ptr<CGameObject> hitObject = nullptr;
	XMFLOAT3 hitPosition = XMFLOAT3(0.f, 0.f, 0.f);;
};

class PickingManager : public ToolManager
{
public:
	PickingManager();
	~PickingManager();

	void CalculateScreenMouseCoords();
	const ImVec2 GetMousePos() { return m_pMousePos; }
	Ray CreateWorldRayFromScreenCoords(float screenX, float screenY, const shared_ptr<CCamera>& pCamera);
	RayHitResult ReturnScreenToWorldRayClosestHitResult(Ray pRay, const shared_ptr<CScene>& pScene, const shared_ptr<CCamera>& pCamera);
	RayHitResult RayIntersectionWithWorldObject(const Ray& ray, const shared_ptr<CGameObject>& Object, const XMFLOAT4X4& worldMatrix);

private:
	ImVec2 m_pMousePos;
};

