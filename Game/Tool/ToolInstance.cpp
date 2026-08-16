#include "ToolInstance.h"
#include "Engine.h"
#include "NavigationCell.h"

void Tool::Init(HINSTANCE hInstance, HWND hWnd)
{
	gGameFrameWork->OnCreate(hInstance, hWnd);
    gGameFrameWork->GetTimer().Reset();
    BuildToolScene();
    InitImgui(hWnd);
}

void Tool::BuildToolScene()
{
    CMDLIST->Reset(CMDALLOCATOR.Get(), NULL);

    m_pToolScene = make_shared<ToolScene>();
    m_pToolScene->BuildObjects(DEVICE.Get(), CMDLIST.Get(), GRAPHICS_ROOT_SIGNATURE);

    m_pToolScene->GetPlayer() = m_pToolPlayer = make_shared<CDebugPlayer>();
    m_pToolPlayer->InitCamera(DEVICE.Get(), CMDLIST.Get(), GRAPHICS_ROOT_SIGNATURE);
    m_pToolCamera = m_pToolPlayer->GetCamera();

    m_pToolPlayer->SetPosition(XMFLOAT3(0.f, 20.f, 0.f));

    m_pImguiManager = make_shared<ImguiManager>();
    m_pInteractionManager = make_shared <InteractionManager>();

    Util::ExecuteCommandList(CMDLIST, CMDQUEUE, FENCE, ++CMD->m_nFenceValues[RTVDSVDESCRIPTORHEAP->GetSwapChainIndex()], CMD->m_hFenceEvent);
}

void Tool::InitImgui(HWND hWnd)
{
    //Imgui 초기화
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();

    //win32, API 초기화
    //이를 위해 별도의 SRV Descriptor Heap을 생성
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = 1;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        DX::ThrowIfFailed(DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pdxgiSRVDescriptorHeapForImgui)));
    }

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX12_Init(DEVICE.Get(), 3,
        DXGI_FORMAT_R8G8B8A8_UNORM, m_pdxgiSRVDescriptorHeapForImgui.Get(),
        m_pdxgiSRVDescriptorHeapForImgui.Get()->GetCPUDescriptorHandleForHeapStart(),
        m_pdxgiSRVDescriptorHeapForImgui.Get()->GetGPUDescriptorHandleForHeapStart());
}

void Tool::Update()
{
    gGameFrameWork->RenderBegin();
    gGameFrameWork->GetTimer().Tick(0.0f);
    ProcessInput();
    AnimateObjects();
    Render();
    ImguiRender();
    gGameFrameWork->RenderEnd();
    gGameFrameWork->GetTimer().GetFrameRate(gGameFrameWork->m_pszFrameRate + 12, 37);
    ::SetWindowText(RTVDSVDESCRIPTORHEAP->m_hWnd, gGameFrameWork->m_pszFrameRate);
}

void Tool::Destroy()
{
	gGameFrameWork->OnDestroy();
    if (m_pToolScene)m_pToolScene->ReleaseObjects();
    // Cleanup
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void Tool::Render()
{
    if (m_pToolCamera)
        m_pToolCamera->SetViewportsAndScissorRects(CMDLIST.Get());
    if (m_pToolScene)
    {
        m_pToolScene->AnimateObjects(gGameFrameWork->GetTimer().GetTimeElapsed());
        m_pToolScene->Render(CMDLIST.Get(), m_pToolCamera);
    }
}

void Tool::ImguiRender()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //테스트를 위한 예제 윈도우
    //ImGui::ShowDemoWindow();

    if(m_pImguiManager) 
        m_pImguiManager->DisplayWindow(m_pToolPlayer, m_pToolScene, m_pToolCamera);


    ImGui::Render();

    CMDLIST->SetDescriptorHeaps(1, m_pdxgiSRVDescriptorHeapForImgui.GetAddressOf());
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), CMDLIST.Get());
}

LRESULT Tool::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
    switch (nMessageID)
    {
    case WM_ACTIVATE:
    {
        if (LOWORD(wParam) == WA_INACTIVE)
            gGameFrameWork->GetTimer().Stop();
        else
            gGameFrameWork->GetTimer().Start();
        break;
    }
    case WM_SIZE:
        break;
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MOUSEMOVE:
        OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
        break;
    case WM_KEYDOWN:
        OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
        break;
    case WM_KEYUP:
        break;
    }
    return(0);
}

void Tool::OnProcessingMouseMessage(HWND hwnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void Tool::OnProcessingKeyboardMessage(HWND hwnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
    switch (nMessageID)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            ::PostQuitMessage(0);
            break;
        }
        break;
    default:
        break;
    }
}

void Tool::ProcessInput()
{
    m_pInteractionManager->MovePlayerWithKeyBoard(m_pToolPlayer);
    m_pInteractionManager->PlayerZoomMoveMent(m_pToolPlayer);
    m_pInteractionManager->PlayerRotate(m_pToolPlayer);
    m_pToolPlayer->Update(gGameFrameWork->GetTimer().GetTimeElapsed());
}

void Tool::AnimateObjects()
{
    if (m_pToolScene)m_pToolScene->AnimateObjects(gGameFrameWork->GetTimer().GetTimeElapsed());
}



