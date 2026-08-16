#include "pch.h"
#include "EnginePch.h"
#include "Engine.h"
#include "DDSTextureLoader12.h"
#include "WICTextureLoader12.h"

unique_ptr<Engine> gGameFrameWork = make_unique<Engine>();

UINT gnRtvDescriptorIncrementSize = 0;
UINT gnDsvDescriptorIncrementSize = 0;
UINT gnCbvSrvDescriptorIncrementSize = 0;


ComPtr<ID3D12Resource> CreateTextureResourceFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const wchar_t* pszFileName, ID3D12Resource** ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates)
{
    ComPtr<ID3D12Resource> pd3dTexture = NULL;
    std::unique_ptr<uint8_t[]> ddsData;
    std::vector<D3D12_SUBRESOURCE_DATA> vSubresources;
    DDS_ALPHA_MODE ddsAlphaMode = DDS_ALPHA_MODE_UNKNOWN;
    bool bIsCubeMap = false;

    HRESULT hResult = DirectX::LoadDDSTextureFromFileEx(pd3dDevice, pszFileName, 0, D3D12_RESOURCE_FLAG_NONE, DDS_LOADER_DEFAULT, pd3dTexture.GetAddressOf(), ddsData, vSubresources, &ddsAlphaMode, &bIsCubeMap);

    D3D12_HEAP_PROPERTIES d3dHeapPropertiesDesc;
    ::ZeroMemory(&d3dHeapPropertiesDesc, sizeof(D3D12_HEAP_PROPERTIES));
    d3dHeapPropertiesDesc.Type = D3D12_HEAP_TYPE_UPLOAD;
    d3dHeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    d3dHeapPropertiesDesc.CreationNodeMask = 1;
    d3dHeapPropertiesDesc.VisibleNodeMask = 1;

    UINT nSubResources = (UINT)vSubresources.size();
    UINT64 nBytes = GetRequiredIntermediateSize(pd3dTexture.Get(), 0, nSubResources);

    D3D12_RESOURCE_DESC d3dResourceDesc;
    ::ZeroMemory(&d3dResourceDesc, sizeof(D3D12_RESOURCE_DESC));
    d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; //Upload Heap에는 텍스쳐를 생성할 수 없음
    d3dResourceDesc.Alignment = 0;
    d3dResourceDesc.Width = nBytes;
    d3dResourceDesc.Height = 1;
    d3dResourceDesc.DepthOrArraySize = 1;
    d3dResourceDesc.MipLevels = 1;
    d3dResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    d3dResourceDesc.SampleDesc.Count = 1;
    d3dResourceDesc.SampleDesc.Quality = 0;
    d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    pd3dDevice->CreateCommittedResource(&d3dHeapPropertiesDesc, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, __uuidof(ID3D12Resource), (void**)ppd3dUploadBuffer);

    ::UpdateSubresources(pd3dCommandList, pd3dTexture.Get(), *ppd3dUploadBuffer, 0, 0, nSubResources, &vSubresources[0]);

    D3D12_RESOURCE_BARRIER d3dResourceBarrier;
    ::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
    d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    d3dResourceBarrier.Transition.pResource = pd3dTexture.Get();
    d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    d3dResourceBarrier.Transition.StateAfter = d3dResourceStates;
    d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);

    return(pd3dTexture);
}

ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType, D3D12_RESOURCE_STATES d3dResourceStates, ID3D12Resource** ppd3dUploadBuffer)
{
    ComPtr<ID3D12Resource> pd3dBuffer=nullptr;

    D3D12_HEAP_PROPERTIES d3dHeapPropertiesDesc;
    ::ZeroMemory(&d3dHeapPropertiesDesc, sizeof(D3D12_HEAP_PROPERTIES));
    d3dHeapPropertiesDesc.Type = d3dHeapType;
    d3dHeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    d3dHeapPropertiesDesc.CreationNodeMask = 1;
    d3dHeapPropertiesDesc.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC d3dResourceDesc;
    ::ZeroMemory(&d3dResourceDesc, sizeof(D3D12_RESOURCE_DESC));
    d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    d3dResourceDesc.Alignment = 0;
    d3dResourceDesc.Width = nBytes;
    d3dResourceDesc.Height = 1;
    d3dResourceDesc.DepthOrArraySize = 1;
    d3dResourceDesc.MipLevels = 1;
    d3dResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    d3dResourceDesc.SampleDesc.Count = 1;
    d3dResourceDesc.SampleDesc.Quality = 0;
    d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    D3D12_RESOURCE_STATES d3dResourceInitialStates = D3D12_RESOURCE_STATE_COPY_DEST;
    if (d3dHeapType == D3D12_HEAP_TYPE_UPLOAD) d3dResourceInitialStates = D3D12_RESOURCE_STATE_GENERIC_READ;
    else if (d3dHeapType == D3D12_HEAP_TYPE_READBACK) d3dResourceInitialStates = D3D12_RESOURCE_STATE_COPY_DEST;

    DX::ThrowIfFailed(pd3dDevice->CreateCommittedResource(&d3dHeapPropertiesDesc, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, d3dResourceInitialStates, NULL, IID_PPV_ARGS(pd3dBuffer.GetAddressOf())));

    if (pData)
    {
        switch (d3dHeapType)
        {
        case D3D12_HEAP_TYPE_DEFAULT:
        {
            if (ppd3dUploadBuffer)
            {
                d3dHeapPropertiesDesc.Type = D3D12_HEAP_TYPE_UPLOAD;
                pd3dDevice->CreateCommittedResource(&d3dHeapPropertiesDesc, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(ppd3dUploadBuffer));

                D3D12_RANGE d3dReadRange = { 0, 0 };
                UINT8* pBufferDataBegin = NULL;
                (*ppd3dUploadBuffer)->Map(0, &d3dReadRange, (void**)&pBufferDataBegin);
                memcpy(pBufferDataBegin, pData, nBytes);
                (*ppd3dUploadBuffer)->Unmap(0, NULL);

                pd3dCommandList->CopyResource(pd3dBuffer.Get(), *ppd3dUploadBuffer);

                D3D12_RESOURCE_BARRIER d3dResourceBarrier;
                ::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
                d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
                d3dResourceBarrier.Transition.pResource = pd3dBuffer.Get();
                d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
                d3dResourceBarrier.Transition.StateAfter = d3dResourceStates;
                d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);
            }
            break;
        }
        case D3D12_HEAP_TYPE_UPLOAD:
        {
            D3D12_RANGE d3dReadRange = { 0, 0 };
            UINT8* pBufferDataBegin = NULL;
            pd3dBuffer->Map(0, &d3dReadRange, (void**)&pBufferDataBegin);
            memcpy(pBufferDataBegin, pData, nBytes);
            pd3dBuffer->Unmap(0, NULL);
            break;
        }
        case D3D12_HEAP_TYPE_READBACK:
            break;
        }
    }
    return(pd3dBuffer);
}


namespace Util {
    ComPtr<ID3D12Resource> CreateBufferResource(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, void* pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType, D3D12_RESOURCE_STATES d3dResourceStates, ID3D12Resource** ppd3dUploadBuffer)
    {
        ComPtr<ID3D12Resource> pd3dBuffer = NULL;

        D3D12_HEAP_PROPERTIES d3dHeapPropertiesDesc;
        ::ZeroMemory(&d3dHeapPropertiesDesc, sizeof(D3D12_HEAP_PROPERTIES));
        d3dHeapPropertiesDesc.Type = d3dHeapType;
        d3dHeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        d3dHeapPropertiesDesc.CreationNodeMask = 1;
        d3dHeapPropertiesDesc.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC d3dResourceDesc;
        ::ZeroMemory(&d3dResourceDesc, sizeof(D3D12_RESOURCE_DESC));
        d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        d3dResourceDesc.Alignment = 0;
        d3dResourceDesc.Width = nBytes;
        d3dResourceDesc.Height = 1;
        d3dResourceDesc.DepthOrArraySize = 1;
        d3dResourceDesc.MipLevels = 1;
        d3dResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        d3dResourceDesc.SampleDesc.Count = 1;
        d3dResourceDesc.SampleDesc.Quality = 0;
        d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        D3D12_RESOURCE_STATES d3dResourceInitialStates = D3D12_RESOURCE_STATE_COMMON;
        if (d3dHeapType == D3D12_HEAP_TYPE_UPLOAD) d3dResourceInitialStates = D3D12_RESOURCE_STATE_GENERIC_READ;
        else if (d3dHeapType == D3D12_HEAP_TYPE_READBACK) d3dResourceInitialStates = D3D12_RESOURCE_STATE_COPY_DEST;

        DX::ThrowIfFailed(_Device->CreateCommittedResource(&d3dHeapPropertiesDesc, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, d3dResourceInitialStates, NULL, IID_PPV_ARGS(&pd3dBuffer)));

        if (pData)
        {
            switch (d3dHeapType)
            {
            case D3D12_HEAP_TYPE_DEFAULT:
            {
                if (ppd3dUploadBuffer)
                {
                    d3dHeapPropertiesDesc.Type = D3D12_HEAP_TYPE_UPLOAD;
                    _Device->CreateCommittedResource(&d3dHeapPropertiesDesc, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, __uuidof(ID3D12Resource), (void**)ppd3dUploadBuffer);
#ifdef _WITH_MAPPING
                    D3D12_RANGE d3dReadRange = { 0, 0 };
                    UINT8* pBufferDataBegin = NULL;
                    (*ppd3dUploadBuffer)->Map(0, &d3dReadRange, (void**)&pBufferDataBegin);
                    memcpy(pBufferDataBegin, pData, nBytes);
                    (*ppd3dUploadBuffer)->Unmap(0, NULL);

                    pd3dCommandList->CopyResource(pd3dBuffer, *ppd3dUploadBuffer);
#else
                    D3D12_SUBRESOURCE_DATA d3dSubResourceData;
                    ::ZeroMemory(&d3dSubResourceData, sizeof(D3D12_SUBRESOURCE_DATA));
                    d3dSubResourceData.pData = pData;
                    d3dSubResourceData.SlicePitch = d3dSubResourceData.RowPitch = nBytes;
                    UpdateSubresources<1>(_CommandList.Get(), pd3dBuffer.Get(), *ppd3dUploadBuffer, 0, 0, 1, &d3dSubResourceData);

#endif
                    Util::SynchronizeResourceTransition(_CommandList, pd3dBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, d3dResourceStates); //상태 전이 해준다 COPY_DEST로
                }
                break;
            }
            case D3D12_HEAP_TYPE_UPLOAD:
            {
                D3D12_RANGE d3dReadRange = { 0, 0 };
                UINT8* pBufferDataBegin = NULL;
                pd3dBuffer->Map(0, &d3dReadRange, (void**)&pBufferDataBegin);
                memcpy(pBufferDataBegin, pData, nBytes);
                pd3dBuffer->Unmap(0, NULL);
                break;
            }
            case D3D12_HEAP_TYPE_READBACK:
                break;
            }
        }
        return(pd3dBuffer);
    }

    ComPtr<ID3D12Resource> CreateBufferResource(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList,
        const void* pData, UINT sizePerData, UINT dataCount, D3D12_HEAP_TYPE d3dHeapType, D3D12_RESOURCE_STATES d3dResourceStates, ID3D12Resource** ppd3dUploadBuffer)
    {
        ComPtr<ID3D12Resource> buffer;
        const UINT bufferSize{ sizePerData * dataCount };

        // 디폴트 버퍼에 데이터를 넣을 경우 업로드 버퍼가 필요함
        if (d3dHeapType == D3D12_HEAP_TYPE_DEFAULT)
        {
            // 디폴트 버퍼 생성
            DX::ThrowIfFailed(_Device->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
                D3D12_HEAP_FLAG_NONE,
                &CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
                D3D12_RESOURCE_STATE_COPY_DEST,
                NULL,
                IID_PPV_ARGS(&buffer)));

            // 데이터 복사
            if (pData)
            {
                // 업로드 버퍼 생성
                DX::ThrowIfFailed(_Device->CreateCommittedResource(
                    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
                    D3D12_HEAP_FLAG_NONE,
                    &CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
                    D3D12_RESOURCE_STATE_GENERIC_READ,
                    NULL,
                    IID_PPV_ARGS(ppd3dUploadBuffer)));

                // 업로드 버퍼에서 디폴트 버퍼로 복사
                D3D12_SUBRESOURCE_DATA bufferData{};
                bufferData.pData = pData;
                bufferData.RowPitch = bufferSize;
                bufferData.SlicePitch = bufferData.RowPitch;
                UpdateSubresources<1>(_CommandList.Get(), buffer.Get(), *ppd3dUploadBuffer, 0, 0, 1, &bufferData);
            }

            // 버퍼 리소스 베리어 설정
            _CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(buffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, d3dResourceStates));
            return buffer;
        }

        // 업로드 버퍼에 데이터를 넣을 경우 바로 복사함
        if (d3dHeapType == D3D12_HEAP_TYPE_UPLOAD)
        {
            // 업로드 버퍼 생성
            DX::ThrowIfFailed(_Device->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
                D3D12_HEAP_FLAG_NONE,
                &CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
                D3D12_RESOURCE_STATE_GENERIC_READ,
                NULL,
                IID_PPV_ARGS(&buffer)));

            // 데이터 복사
            if (pData)
            {
                UINT8* pBufferDataBegin{ NULL };
                CD3DX12_RANGE readRange{ 0, 0 };
                DX::ThrowIfFailed(buffer->Map(0, &readRange, reinterpret_cast<void**>(&pBufferDataBegin)));
                memcpy(pBufferDataBegin, pData, bufferSize);
                buffer->Unmap(0, NULL);
            }

            return buffer;
        }

        if (d3dHeapType == D3D12_HEAP_TYPE_READBACK)
        {
            // 리드백 버퍼 생성
            DX::ThrowIfFailed(_Device->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK),
                D3D12_HEAP_FLAG_NONE,
                &CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
                D3D12_RESOURCE_STATE_COPY_DEST,
                NULL,
                IID_PPV_ARGS(&buffer)));

            // 데이터 복사
            if (pData)
            {
                UINT8* pBufferDataBegin{ NULL };
                CD3DX12_RANGE readRange{ 0, 0 };
                DX::ThrowIfFailed(buffer->Map(0, &readRange, reinterpret_cast<void**>(&pBufferDataBegin)));
                memcpy(pBufferDataBegin, pData, bufferSize);
                buffer->Unmap(0, NULL);
            }
            return buffer;
        }
        return NULL;
    }

    ComPtr<ID3D12Resource> CreateTextureResourceFromDDSFile(const ComPtr<ID3D12Device>& _Device,
        const ComPtr<ID3D12GraphicsCommandList>& _CommandList, wchar_t* pszFileName, ID3D12Resource** ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates)
    {
        ComPtr<ID3D12Resource> pd3dTexture = NULL;
        std::unique_ptr<uint8_t[]> ddsData;
        std::vector<D3D12_SUBRESOURCE_DATA> vSubresources;
        DDS_ALPHA_MODE ddsAlphaMode = DDS_ALPHA_MODE_UNKNOWN;
        bool bIsCubeMap = false;

        HRESULT hResult = DirectX::LoadDDSTextureFromFileEx(_Device.Get(), pszFileName, 0, D3D12_RESOURCE_FLAG_NONE, DDS_LOADER_DEFAULT, &pd3dTexture, ddsData, vSubresources, &ddsAlphaMode, &bIsCubeMap);

        D3D12_HEAP_PROPERTIES d3dHeapPropertiesDesc;
        ::ZeroMemory(&d3dHeapPropertiesDesc, sizeof(D3D12_HEAP_PROPERTIES));
        d3dHeapPropertiesDesc.Type = D3D12_HEAP_TYPE_UPLOAD;
        d3dHeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        d3dHeapPropertiesDesc.CreationNodeMask = 1;
        d3dHeapPropertiesDesc.VisibleNodeMask = 1;

        UINT nSubResources = (UINT)vSubresources.size();
        UINT64 nBytes = GetRequiredIntermediateSize(pd3dTexture.Get(), 0, nSubResources);

        D3D12_RESOURCE_DESC d3dResourceDesc;
        ::ZeroMemory(&d3dResourceDesc, sizeof(D3D12_RESOURCE_DESC));
        d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; //Upload Heap에는 텍스쳐를 생성할 수 없음 . 업로드 힙으로 CPU->GPU메모리로 옮기기 위한 목적이므로 텍스쳐 2D는 직접 GPU에 텍스쳐 데이터 밀어넣으므로 적합하지 않음.
        d3dResourceDesc.Alignment = 0;
        d3dResourceDesc.Width = nBytes;
        d3dResourceDesc.Height = 1;  //효율적으로 데이터 보내기 위해 연속적인 메모리 구조 1차원형태로 버퍼 만들기
        d3dResourceDesc.DepthOrArraySize = 1;
        d3dResourceDesc.MipLevels = 1;
        d3dResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        d3dResourceDesc.SampleDesc.Count = 1;
        d3dResourceDesc.SampleDesc.Quality = 0;
        d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        DX::ThrowIfFailed(_Device->CreateCommittedResource(&d3dHeapPropertiesDesc, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(ppd3dUploadBuffer)));

        ::UpdateSubresources(_CommandList.Get(), pd3dTexture.Get(), *ppd3dUploadBuffer, 0, 0, nSubResources, &vSubresources[0]);

        D3D12_RESOURCE_BARRIER d3dResourceBarrier;
        ::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
        d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        d3dResourceBarrier.Transition.pResource = pd3dTexture.Get();
        d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        d3dResourceBarrier.Transition.StateAfter = d3dResourceStates;
        d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        _CommandList->ResourceBarrier(1, &d3dResourceBarrier);

        //	delete[] pd3dSubResourceData;

        return(pd3dTexture);
    }

    ComPtr<ID3D12Resource> CreateTextureResourceFromWICFile(const ComPtr<ID3D12Device>& _Device,
        const ComPtr<ID3D12GraphicsCommandList>& _CommandList, wchar_t* pszFileName, ID3D12Resource** ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates)
    {
        ComPtr<ID3D12Resource> pd3dTexture = NULL;
        std::unique_ptr<uint8_t[]> decodedData;
        D3D12_SUBRESOURCE_DATA d3dSubresource;

        HRESULT hResult = DirectX::LoadWICTextureFromFileEx(_Device.Get(), pszFileName, 0, D3D12_RESOURCE_FLAG_NONE, WIC_LOADER_DEFAULT, &pd3dTexture, decodedData, d3dSubresource);

        D3D12_HEAP_PROPERTIES d3dHeapPropertiesDesc;
        ::ZeroMemory(&d3dHeapPropertiesDesc, sizeof(D3D12_HEAP_PROPERTIES));
        d3dHeapPropertiesDesc.Type = D3D12_HEAP_TYPE_UPLOAD;
        d3dHeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        d3dHeapPropertiesDesc.CreationNodeMask = 1;
        d3dHeapPropertiesDesc.VisibleNodeMask = 1;

        UINT64 nBytes = GetRequiredIntermediateSize(pd3dTexture.Get(), 0, 1);

        D3D12_RESOURCE_DESC d3dResourceDesc;
        ::ZeroMemory(&d3dResourceDesc, sizeof(D3D12_RESOURCE_DESC));
        d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; //Upload Heap에는 텍스쳐를 생성할 수 없음
        d3dResourceDesc.Alignment = 0;
        d3dResourceDesc.Width = nBytes;
        d3dResourceDesc.Height = 1;
        d3dResourceDesc.DepthOrArraySize = 1;
        d3dResourceDesc.MipLevels = 1;
        d3dResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        d3dResourceDesc.SampleDesc.Count = 1;
        d3dResourceDesc.SampleDesc.Quality = 0;
        d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        DX::ThrowIfFailed(_Device->CreateCommittedResource(&d3dHeapPropertiesDesc, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(ppd3dUploadBuffer)));

        ::UpdateSubresources(_CommandList.Get(), pd3dTexture.Get(), *ppd3dUploadBuffer, 0, 0, 1, &d3dSubresource);

        D3D12_RESOURCE_BARRIER d3dResourceBarrier;
        ::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
        d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        d3dResourceBarrier.Transition.pResource = pd3dTexture.Get();
        d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        d3dResourceBarrier.Transition.StateAfter = d3dResourceStates;
        d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        _CommandList->ResourceBarrier(1, &d3dResourceBarrier);

        return(pd3dTexture);
    }

    ComPtr<ID3D12Resource> CreateTexture2DResource(const ComPtr<ID3D12Device>& _Device,
        const ComPtr<ID3D12GraphicsCommandList>& _CommandList, UINT nWidth, UINT nHeight, UINT nElements, UINT nMipLevels, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE* pd3dClearValue)
    {
        ComPtr<ID3D12Resource> pd3dTexture = NULL;

        D3D12_HEAP_PROPERTIES d3dHeapPropertiesDesc;
        ::ZeroMemory(&d3dHeapPropertiesDesc, sizeof(D3D12_HEAP_PROPERTIES));
        d3dHeapPropertiesDesc.Type = D3D12_HEAP_TYPE_DEFAULT;
        d3dHeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        d3dHeapPropertiesDesc.CreationNodeMask = 1;
        d3dHeapPropertiesDesc.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC d3dTextureResourceDesc;
        ::ZeroMemory(&d3dTextureResourceDesc, sizeof(D3D12_RESOURCE_DESC));
        d3dTextureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        d3dTextureResourceDesc.Alignment = 0;
        d3dTextureResourceDesc.Width = nWidth;
        d3dTextureResourceDesc.Height = nHeight;
        d3dTextureResourceDesc.DepthOrArraySize = nElements;
        d3dTextureResourceDesc.MipLevels = nMipLevels;
        d3dTextureResourceDesc.Format = dxgiFormat;
        d3dTextureResourceDesc.SampleDesc.Count = 1;
        d3dTextureResourceDesc.SampleDesc.Quality = 0;
        d3dTextureResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        d3dTextureResourceDesc.Flags = d3dResourceFlags;

        DX::ThrowIfFailed(_Device->CreateCommittedResource(&d3dHeapPropertiesDesc, D3D12_HEAP_FLAG_NONE, &d3dTextureResourceDesc, d3dResourceStates, pd3dClearValue, IID_PPV_ARGS(&pd3dTexture)));

        return(pd3dTexture);
    }


    void SynchronizeResourceTransition(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, ComPtr<ID3D12Resource> pd3dResource, D3D12_RESOURCE_STATES d3dStateBefore, D3D12_RESOURCE_STATES d3dStateAfter)
    {
        D3D12_RESOURCE_BARRIER d3dResourceBarrier;
        d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        d3dResourceBarrier.Transition.pResource = pd3dResource.Get();
        d3dResourceBarrier.Transition.StateBefore = d3dStateBefore;
        d3dResourceBarrier.Transition.StateAfter = d3dStateAfter;
        d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        _CommandList->ResourceBarrier(1, &d3dResourceBarrier);
    }

    void WaitForGpuComplete(const ComPtr<ID3D12CommandQueue>& _CmdQueue, const ComPtr<ID3D12Fence>& _Fence, UINT64 nFenceValue, HANDLE hFenceEvent)
    {
        DX::ThrowIfFailed(_CmdQueue->Signal(_Fence.Get(), nFenceValue));

        if (_Fence->GetCompletedValue() < nFenceValue)
        {
            DX::ThrowIfFailed(_Fence->SetEventOnCompletion(nFenceValue, hFenceEvent));
            ::WaitForSingleObject(hFenceEvent, INFINITE);
        }
    }


    void ExecuteCommandList(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, const ComPtr<ID3D12CommandQueue>& _CmdQueue, const ComPtr<ID3D12Fence>& _Fence, UINT64 nFenceValue, HANDLE hFenceEvent)
    {
        DX::ThrowIfFailed(_CommandList->Close());

        ComPtr<ID3D12CommandList>ppd3dCommandLists[] = { _CommandList.Get() };
        _CmdQueue->ExecuteCommandLists(1, ppd3dCommandLists->GetAddressOf());

        WaitForGpuComplete(_CmdQueue, _Fence, nFenceValue, hFenceEvent);
    }
}