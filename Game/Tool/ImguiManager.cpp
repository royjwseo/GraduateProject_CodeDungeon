#include "ImguiManager.h"
#include "Engine.h"
#include "GridObject.h"

ImguiManager::ImguiManager()
{
    m_xmf3PickedWorldPos = XMFLOAT3(0.f, 0.f, 0.f);
    m_pPickingManager = make_shared<PickingManager>();
    m_bPickingEnabled = false;
}

ImguiManager::~ImguiManager()
{
}

void ImguiManager::DisplayWindow(const shared_ptr<CPlayer>& pPlayer, const shared_ptr<CScene>& pScene, const shared_ptr<CCamera>& pCamera)
{
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Mini Engine");

    //마우스 좌표 반환
    m_pPickingManager->CalculateScreenMouseCoords();
    ImVec2 MousePosition = m_pPickingManager->GetMousePos();
    ImGui::Text("Mouse Position: (%.1f, %.1f)", MousePosition.x, MousePosition.y);

    if (ImGui::Checkbox("Enable Picking", &m_bPickingEnabled)) {}
    if (m_bPickingEnabled)
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            Ray ray;
            ray = m_pPickingManager->CreateWorldRayFromScreenCoords(MousePosition.x, MousePosition.y, pCamera);
            RayHitResult Result;
            Result = m_pPickingManager->ReturnScreenToWorldRayClosestHitResult(ray, pScene, pCamera);
            m_xmf3PickedWorldPos = Result.hitPosition;

        }
        ImGui::Text("World Position: (%.1f, %.1f, %.1f)", m_xmf3PickedWorldPos.x, m_xmf3PickedWorldPos.y, m_xmf3PickedWorldPos.z);
    }  

    if (pPlayer)
    {
        XMFLOAT3 playerPos = pPlayer->GetPosition();
        ImGui::Text("Player Position: (%.1f, %.1f, %.1f)", playerPos.x, playerPos.y, playerPos.z);

        XMFLOAT3 playerLook = pPlayer->GetLook();
        ImGui::Text("Player Look: (%.1f, %.1f, %.1f)", playerLook.x, playerLook.y, playerLook.z);
    }
    if (pCamera)
    {
        XMFLOAT3 cameraPos = pCamera->GetCameraPosition();
        ImGui::Text("Camera Position: (%.1f, %.1f, %.1f)", cameraPos.x, cameraPos.y, cameraPos.z);

        XMFLOAT3 cameraLook = pCamera->GetCameraLook();
        ImGui::Text("Camera Look: (%.1f, %.1f, %.1f)", cameraLook.x, cameraLook.y, cameraLook.z);
    }

    ImGui::End();
}
