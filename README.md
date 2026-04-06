# Code Dungeon

> 한국공학대학교 게임공학과 졸업작품 (2024.01 ~ 2024.07)  
> DirectX 12 기반 3D 액션 던전 게임

[![YouTube Demo](https://img.shields.io/badge/YouTube-Demo-red?logo=youtube)](https://www.youtube.com/watch?v=hQBBsoL_ETs&t)

---

## 1. 프로젝트 소개

**Code Dungeon**은 DirectX 12의 Low-Level API를 직접 다루며 GPU 리소스 및 렌더링 파이프라인 최적화를 목표로 개발한 3인 팀 프로젝트입니다.

대규모 맵과 고품질 셰이더 이펙트가 적용된 환경에서 프레임 저하 없이 동작하는 것을 핵심 목표로 삼았으며,  
Deferred Rendering, GPU 인스턴싱 파티클, Compute/Geometry Shader 기반 이펙트 등 다양한 렌더링 기법을 직접 구현하였습니다.

| 항목 | 내용 |
|------|------|
| 개발 기간 | 2024.01 ~ 2024.07 (7개월) |
| 장르 | 3D 액션 던전 게임 |
| 플랫폼 | Windows (PC) |
| 인원 | 3인 팀 프로젝트 |
| 소속 | 한국공학대학교 게임공학과 졸업작품 |

---

## 2. 기술 스택

| 분류 | 사용 기술 |
|------|-----------|
| Language | C++20 |
| Graphics API | DirectX 12 |
| Shader | HLSL (Vertex / Pixel / Geometry / Compute Shader) |
| Networking | Boost.Asio (비동기 TCP) |
| 3D Asset | FBX SDK |
| IDE | Visual Studio 2022 |
| Platform | Windows 10 / 11 |

---

## 3. 팀 역할 분담

| 이름 | 역할 |
|------|------|
| 박태현 (팀장) | 프레임워크 설계 및 제작, FBX 스키닝 / 애니메이션 시스템, Boost.Asio 기반 비동기 TCP 서버 |
| 이성현 | 레벨 디자인 / 몬스터 배치, 네비게이션 메시 기반 AI 경로탐색, 충돌 처리, 전투 시스템 |
| 서정원 | 렌더링 파이프라인 설계 및 최적화, 셰이더 이펙트 전체 구현, UI / 사운드 시스템 연동 |

### 서정원 상세 담당

- **렌더링 파이프라인**: Deferred Rendering 구조 설계, Pass 단위 최적화, FPS 성능 측정 및 디버깅
- **셰이더 이펙트**: Bloom, Distortion Fire, Guard 결계, Outline, Rim Light, Scene Fog, Sword Trail (CatmullRom), Render Target Effects, Motion Blur / Dissolve
- **GPU 파티클 시스템**: Compute Shader + Geometry Shader 기반 GPU 인스턴싱 구현
- **상수 버퍼 최적화**: Upload Heap → Default Heap 전환으로 렌더링 성능 향상
- **UI / 사운드**: 플레이어 행동 / 스킬 / 피격 / 상호작용 이벤트와 실시간 연동

---

## 4. 핵심 구현

### 4-1. Deferred Rendering 파이프라인

G-Buffer 작성부터 Post-Processing까지 단계를 명시적으로 분리하여 Pass 단위로 렌더링을 관리합니다.  
각 Pass는 독립된 Render Target Group을 가지며, 이전 Pass의 결과를 SRV로 넘겨받아 순차적으로 처리합니다.

```
PosNormal (Outline용 기하 정보)
    ↓
NonAlphaBlend (불투명 오브젝트 → G-Buffer 작성)
    ↓
Lights (Direction / Point 조명 계산)
    ↓
NonLight / Blend / AlphaBlend
    ↓
Distortion → MotionBlur
    ↓
HDR → HDR2 → GrayScale
    ↓
DownSample → DownSample2 → HorizontalBlur → VerticalBlur → UpSample
    ↓
Bloom → RenderEnd
    ↓
2D UI (Priority / Middle / Last) → 3D UI
```

렌더링 대상은 Pass별로 셰이더 이름을 키로 묶어 관리합니다.  
같은 셰이더를 쓰는 오브젝트는 한 번의 루프로 처리되어 상태 전환 비용을 줄입니다.

```cpp
// URenderer.h
using DRAWINGRENDERCONTAINER = UNORMAP<_wstring, LIST<SHPTR<UPawn>>>;
ARRAY<DRAWINGRENDERCONTAINER, RI_END> m_arrActiveDrawRenderList;
```

→ [`Engine/private/URenderer.cpp`](CodeDungeon/Engine/private/URenderer.cpp)

---

#### 전용 카메라 3개 운용

포스트 프로세싱 단계별로 독립된 카메라를 두어 뷰포트 크기와 투영 방식을 분리합니다.

| 카메라 | 용도 | 해상도 |
|--------|------|--------|
| `UShadowCamera` | Shadow Map 생성 | 뷰포트 × 4 (고해상도 그림자) |
| `UDefferedCamera` | 최종 Deferred 합성 | 뷰포트 × 1 |
| `USmallDefferedCamera` | Bloom 다운샘플링 | 뷰포트 × 1 (별도 RT) |

```cpp
// Shadow Camera — 4배 해상도로 그림자 선명도 확보
tDesc.stCamProj = UCamera::CAMPROJ(
    UCamera::PROJECTION_TYPE::PERSPECTIVE, ...,
    viewport.Width * 4,
    viewport.Height * 4, ...);

// Deferred / Small Deferred — Orthographic 투영으로 스크린 공간 처리
tDesc.stCamProj = UCamera::CAMPROJ(
    UCamera::PROJECTION_TYPE::ORTHOGRAPHIC, ...,
    viewport.Width, viewport.Height);
```

→ [`Engine/private/URenderer.cpp`](CodeDungeon/Engine/private/URenderer.cpp) `NativeConstruct()`

---

### 4-2. 상수 버퍼 최적화 — Upload Heap → Default Heap

DirectX 12에서 상수 버퍼를 Upload Heap으로만 관리하면 GPU가 읽을 때마다 CPU 공유 메모리를 거쳐야 합니다.  
자주 변하지 않는 데이터(조명, 월드 행렬 등)는 Default Heap으로 마이그레이션하여 GPU 접근 비용을 줄였습니다.

#### 구조

```cpp
// UShaderConstantBuffer.h
ComPtr<Dx12Resource> m_cpUploadBuffer;          // CPU 쓰기용 (항상 존재)
ComPtr<Dx12Resource> m_cpDefaultBuffer;         // GPU 읽기 최적화용 (opt-in)
std::unique_ptr<_ubyte[]> m_pPreviousBuffer;    // 변경 감지용 스냅샷
bool m_bUseDefaultBuffer;                       // Default Heap 사용 여부
```

Default Heap은 선택적으로 활성화합니다. 매 프레임 갱신이 필요한 버퍼는 Upload Heap만 사용하고,  
갱신 빈도가 낮은 버퍼만 Default Heap을 켜서 불필요한 복사 비용을 방지합니다.

#### 변경 감지 후 조건부 복사

```cpp
// UShaderConstantBuffer.cpp — PushData()
// 업로드 버퍼에 먼저 기록
::memcpy(&m_pMapBuffer[iIndex], _pBuffer, _iSize);

// 이전 데이터와 비교 — 변경된 경우에만 Default Heap으로 복사
if (m_bUseDefaultBuffer && ::memcmp(&m_pPreviousBuffer[iIndex], _pBuffer, _iSize) != 0)
{
    // COMMON → COPY_DEST 전환
    _spCommand->GetGpuCmdList()->ResourceBarrier(1,
        &CD3DX12_RESOURCE_BARRIER::Transition(m_cpDefaultBuffer.Get(),
            D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));

    // Upload → Default 복사
    _spCommand->GetGpuCmdList()->CopyResource(m_cpDefaultBuffer.Get(), m_cpUploadBuffer.Get());

    // COPY_DEST → VERTEX_AND_CONSTANT_BUFFER 전환
    _spCommand->GetGpuCmdList()->ResourceBarrier(1,
        &CD3DX12_RESOURCE_BARRIER::Transition(m_cpDefaultBuffer.Get(),
            D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

    // 스냅샷 갱신
    ::memcpy(&m_pPreviousBuffer[iIndex], _pBuffer, _iSize);
}
```

데이터가 바뀌지 않은 프레임에서는 Resource Barrier와 CopyResource 호출이 생략됩니다.  
대규모 맵에서 다수의 상수 버퍼가 매 프레임 바인딩될 때 불필요한 GPU 동기화 비용을 줄이는 효과를 가집니다.

→ [`Engine/public/UShaderConstantBuffer.h`](CodeDungeon/Engine/public/UShaderConstantBuffer.h)  
→ [`Engine/private/UShaderConstantBuffer.cpp`](CodeDungeon/Engine/private/UShaderConstantBuffer.cpp)

---

### 4-3. GPU 인스턴싱 파티클 시스템 (Compute + Geometry Shader)

CPU에서 파티클을 하나씩 업데이트하는 대신, Compute Shader로 GPU에서 병렬 처리합니다.  
파티클 데이터는 `RWStructuredBuffer`로 GPU 메모리에 상주하며, CPU는 파라미터만 전달합니다.

#### 파이프라인 구조

```
[CPU] 파라미터 설정 (위치, 방향, 수명, 속도 범위)
        ↓
[Compute Shader] 파티클 시뮬레이션 (생성 / 업데이트 / 소멸)
        ↓  RWStructuredBuffer<PARTICLE>
[Geometry Shader] 빌보드 생성 (Point → Quad 변환)
        ↓
[Pixel Shader] 텍스처 / 알파 블렌딩
```

#### Thread-Safe 파티클 생성

1024개 스레드가 동시에 실행되므로 파티클 생성 개수를 원자적으로 관리합니다.

```hlsl
// Compute2DParticle.hlsl
[numthreads(1024, 1, 1)]
void CS_Main(int3 threadIndex : SV_DispatchThreadID)
{
    // 모든 스레드가 iAddCount를 공유 메모리에 쓰고 동기화 대기
    g_SharedData[0].iAddCount = g_GrobalParticleInfo.iAddCount;
    GroupMemoryBarrierWithGroupSync();

    if (g_ParticleWritedata[threadIndex.x].iAlive == 0)
    {
        while (true)
        {
            int remaining = g_SharedData[0].iAddCount;
            if (remaining <= 0) break;

            int originalValue;
            // CAS로 여러 스레드가 동시에 같은 슬롯에 쓰는 것을 방지
            InterlockedCompareExchange(
                g_SharedData[0].iAddCount, remaining, remaining - 1, originalValue);

            if (originalValue == remaining)
            {
                g_ParticleWritedata[threadIndex.x].iAlive = 1;
                break;
            }
        }
    }
}
```

`InterlockedCompareExchange`(CAS)를 사용해 스레드 간 경합 없이 정확한 개수만큼만 파티클을 활성화합니다.

#### 노이즈 기반 랜덤 초기화

텍스처 샘플링 없이 누적 시간(`fAccTime`)과 스레드 인덱스를 시드로 난수를 생성해 초기 위치와 방향을 설정합니다.

```hlsl
float x = ((float)threadIndex.x / (float)g_GrobalParticleInfo.iMaxCount) + g_GrobalParticleInfo.fAccTime;
float r1 = Rand(float2(x, g_GrobalParticleInfo.fAccTime));
float r2 = Rand(float2(x * g_GrobalParticleInfo.fAccTime, g_GrobalParticleInfo.fAccTime));
float r3 = Rand(float2(x * g_GrobalParticleInfo.fAccTime * g_GrobalParticleInfo.fAccTime, ...));

float3 noise = { 2*r1-1, 2*r2-1, 2*r3-1 }; // [-1, 1] 범위
```

#### 이펙트 종류 (9종 Compute Shader)

파티클 종류별로 전용 Compute Shader를 두어 시뮬레이션 로직을 분리합니다.  
최대 파티클 수는 종류별로 고정하여 버퍼 오버플로우를 원천 차단합니다.

| 이펙트 | 최대 파티클 수 | 특징 |
|--------|--------------|------|
| Original (기본) | 1,024 | 방향성 / 확산 두 가지 타입 |
| Blood (피) | 412 | 충돌 후 비산 |
| Rotation (회전) | 102 | 축 기반 회전 궤도 |
| Flare / Heal / Slash / FootPrint / OpenChest / Attack | 각 전용 | 게임플레이 이벤트 연동 |

→ [`Engine/public/UParticleSystem.h`](CodeDungeon/Engine/public/UParticleSystem.h)  
→ [`Engine/private/UParticleSystem.cpp`](CodeDungeon/Engine/private/UParticleSystem.cpp)  
→ [`Shader/Compute2DParticle.hlsl`](CodeDungeon/Shader/Compute2DParticle.hlsl)

---

### 4-4. PSO 자동화 & Table Descriptor

#### PSO 자동화

DirectX 12에서 PSO(Pipeline State Object)는 셰이더, 블렌드, 래스터라이저, 뎁스 스텐실 등 파이프라인 전체 상태를 하나의 객체로 관리합니다.  
이를 매번 수동으로 구성하면 셰이더가 늘어날수록 설정 코드가 폭발적으로 증가합니다.

`SHADERDESC` 구조체 하나만 채우면 `CreateShader()`에서 PSO를 자동으로 생성하도록 설계했습니다.

```cpp
// SHADERDESC 구성 예시 (셰이더 등록 시)
SHADERDESC desc;
desc.wstrShaderName  = L"Bloom";
desc.ShaderList      = { VS_MAIN, PS_MAIN };       // 사용할 셰이더 스테이지
desc.eBlendType      = BLEND_TYPE::ONE_TO_ONE_BLEND;
desc.eRasterRizerType = RASTERIZER_TYPE::CULL_BACK;
desc.eDepthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST;
desc.iRenderTargets  = 1;
```

```cpp
// UShader.cpp — CreateShader() 내부
// SHADERDESC를 읽어 Blend / Rasterizer / DepthStencil 자동 적용
switch (m_stShaderDesc.eBlendType) {
    case BLEND_TYPE::ALPHA_BLEND:
        rt.SrcBlend  = D3D12_BLEND_SRC_ALPHA;
        rt.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; break;
    case BLEND_TYPE::ONE_TO_ONE_BLEND:
        rt.SrcBlend  = D3D12_BLEND_ONE;
        rt.DestBlend = D3D12_BLEND_ONE; break;
    ...
}
```

생성된 PSO는 셰이더 이름을 키로 정적 컨테이너에 캐싱하여, 동일한 셰이더가 여러 오브젝트에 쓰일 때 PSO를 재생성하지 않습니다.

```cpp
// UShader.h
using PIPELINECONTAINER = UNORMAP<_wstring, ComPtr<Dx12PipelineState>>;
static PIPELINECONTAINER s_m_PipeLineContainer; // 모든 셰이더 인스턴스가 공유
```

---

#### Table Descriptor — CBV / SRV / UAV 통합 바인딩

DirectX 12는 셰이더에 리소스를 바인딩할 때 `SetDescriptorHeaps`로 힙을 교체해야 합니다.  
힙 교체는 GPU 파이프라인 플러시를 유발할 수 있어 비용이 큽니다.

`UTableDescriptor`는 CBV / SRV / UAV를 하나의 GPU-visible Descriptor Heap에 연속 배치하여, 프레임 내에서 힙 교체 없이 모든 리소스를 바인딩합니다.

```cpp
// UTableDescriptor.cpp — NativeConstruct()
desc.NumDescriptors = _iCnt * (_iReigsterCnt - _iStatic); // 전체 슬롯 수 선할당
desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;   // GPU에서 직접 접근
desc.Type  = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;      // CBV/SRV/UAV 통합
```

CBV / SRV / UAV는 레지스터 번호를 그대로 오프셋으로 변환해 동일한 방식으로 핸들을 계산합니다.

```cpp
// UTableDescriptor.cpp — ComputeCpuHandle()
// CBV_REGISTER, SRV_REGISTER, UAV_REGISTER 모두 동일 함수로 처리
Handle.ptr += (m_iCurrentGroupIndex * m_iGroupSize) + (_iReg * m_iHandleSize);
```

커밋 시에는 Graphics / Compute 파이프라인을 구분하여 동일한 힙을 양쪽에 모두 사용합니다.

```cpp
void CommitGraphicTable(...)  { SetGraphicsRootDescriptorTable(...); }
void CommitComputeTable(...)  { SetComputeRootDescriptorTable(...); }
```

→ [`Engine/public/UShader.h`](CodeDungeon/Engine/public/UShader.h)  
→ [`Engine/private/UShader.cpp`](CodeDungeon/Engine/private/UShader.cpp)  
→ [`Engine/public/UTableDescriptor.h`](CodeDungeon/Engine/public/UTableDescriptor.h)  
→ [`Engine/private/UTableDescriptor.cpp`](CodeDungeon/Engine/private/UTableDescriptor.cpp)

---

### 4-5. 셰이더 이펙트

#### Bloom

밝은 영역을 추출하고 가우시안 블러를 거쳐 원본에 합산하는 다단계 Bloom을 구현했습니다.

```
HDR Pass     → 밝기 임계값(0.99) 초과 픽셀만 추출
DownSample   → 4×4 평균 샘플링으로 해상도 축소 (블러 비용 절감)
DownSample2  → 2차 다운샘플 (더 넓은 블러 반경 확보)
HorizontalBlur / VerticalBlur → 11-tap 가우시안 블러
UpSample     → 원본 해상도 복원
Bloom        → HDR + Blur×1.2 + Original 선형 합산
```

**밝기 추출 — 표준 휘도 공식 적용**

RGB를 균등하게 보지 않고, 인간 시각 감도에 맞춘 ITU-R BT.709 휘도 계수를 사용합니다.

```hlsl
// Hdr.hlsl
float brightness = dot(vDiffuseDesc.rgb, float3(0.2126f, 0.7152f, 0.0722f));
if (brightness > 0.99)
    HighColor = float4(vDiffuseDesc.rgb, 1.f);
```

**11-tap 가우시안 블러 — VS에서 텍스처 좌표 선계산**

PS에서 오프셋을 계산하면 의존 텍스처 읽기(dependent texture read)가 발생합니다.  
VS에서 11개 샘플 좌표를 미리 계산해 인터폴레이터로 넘겨 PS의 샘플링 비용을 줄였습니다.

```hlsl
// HorizontalBlur.hlsl — VS_Main
float texelSize = 1.f / TexWidth; // 640 (다운샘플된 해상도)
Out.texCoord1  = In.vTexUV + float2(texelSize * -5.0f, 0.0f);
// ... (texCoord2 ~ texCoord11, -5 ~ +5 오프셋)
Out.texCoord11 = In.vTexUV + float2(texelSize *  5.0f, 0.0f);
```

```hlsl
// HorizontalBlur.hlsl — PS_Main (정규화된 가우시안 가중치)
// weights: 1.0, 0.9, 0.55, 0.18, 0.1, 0.05 → 합산 후 normalization으로 나눔
Out.vColor += g_Texture0.Sample(g_Sampler_Clamp, In.texCoord6)  * weight0; // 중심
Out.vColor += g_Texture0.Sample(g_Sampler_Clamp, In.texCoord5)  * weight1; // ±1
Out.vColor += g_Texture0.Sample(g_Sampler_Clamp, In.texCoord4)  * weight2; // ±2
// ...
```

**최종 합산 — 선형 덧셈 방식 채택**

초기에는 감마 보정(pow 2.2) 기반의 물리적 합성을 시도했으나,  
던전 특유의 과포화된 빛 연출에는 선형 덧셈이 더 자연스러운 결과를 내어 방식을 변경했습니다.

```hlsl
// Bloom.hlsl — PS_Main
vector enhancedBlurColor = vBlurColor * 1.2f; // 블러 기여 강조
Out.vColor = vHDRColor + enhancedBlurColor + vOriginalColor;

// 이전 방식 (주석 처리됨) — 감마 보정 기반
// Out.vColor = pow(pow(HDR, 2.2) + pow(Blur, 2.2), 1/2.2)
```

→ [`Shader/Hdr.hlsl`](CodeDungeon/Shader/Hdr.hlsl)  
→ [`Shader/DownSampling.hlsl`](CodeDungeon/Shader/DownSampling.hlsl)  
→ [`Shader/HorizontalBlur.hlsl`](CodeDungeon/Shader/HorizontalBlur.hlsl)  
→ [`Shader/Bloom.hlsl`](CodeDungeon/Shader/Bloom.hlsl)
