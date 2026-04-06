# Code Dungeon

> 한국공학대학교 게임공학과 졸업작품 (2024.01 ~ 2024.07)  
> DirectX 12 기반 3D 액션 던전 게임

[![YouTube Demo](https://img.shields.io/badge/YouTube-Demo-red?logo=youtube)](https://www.youtube.com/watch?v=hQBBsoL_ETs&t)

---

## 1. 프로젝트 소개

**Code Dungeon**은 DirectX 12의 Low-Level API를 직접 다루며 GPU 리소스 및 렌더링 파이프라인 최적화를 목표로 개발한 3인 팀 프로젝트입니다.

대규모 맵과 고품질 셰이더 이펙트가 적용된 환경에서 프레임 저하 없이 동작하는 것을 핵심 목표로 삼았으며,  
Deferred Rendering, GPU 인스턴싱 파티클, Compute/Geometry Shader 기반 이펙트 등 다양한 렌더링 기법을 직접 구현하였습니다.

#### 개발 배경
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/ff7f125b-40f1-4df6-8adb-227643254880" />

팀 초기에는 팀장이 자체 제작한 프레임워크를 사용하려 했으나, 검토 결과 아래 구조적 문제가 있었습니다.

- Smart Pointer / ComPtr 미사용으로 리소스 관리 불안정
- 객체 간 의존성이 높아 캡슐화 및 독립성 훼손
- 프로젝트 규모가 커질수록 설계 한계가 명확

이를 해결하기 위해 **Factory 패턴 기반 프레임워크**를 새로 설계하였습니다.  
C++20 Concepts와 Template을 활용해 잘못된 타입 사용을 컴파일 타임에 차단하고, Smart Pointer / ComPtr을 전면 도입하여 안전한 리소스 생명주기를 보장하는 구조로 전환하였습니다.

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
| Sound | FMOD |
| 3D Asset | FBX SDK |
| IDE | Visual Studio 2022 |
| Platform | Windows 10 / 11 |

---

## 3. 팀 역할 분담
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/82538f1b-1f05-4125-b032-14e81cf23303" />

| 역할 |
|------|------|
| (팀장) | 프레임워크 설계 및 제작, FBX 스키닝 / 애니메이션 시스템, Boost.Asio 기반 비동기 TCP 서버 |
| (팀원) | 레벨 디자인 / 몬스터 배치, 네비게이션 메시 기반 AI 경로탐색, 충돌 처리, 전투 시스템 |
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
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/05831320-ab98-456f-815e-eef2cd920496" />

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

> Shadow Map Pass (`RenderShadowDepth`)는 구현 완료되어 있으나 현재 빌드에서 비활성화 상태입니다.  
> Shadow Camera는 뷰포트 4배 해상도로 생성되며, 활성화 시 고해상도 그림자가 적용됩니다.

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
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/1fbc9bbc-8244-4d8c-a33b-4cf433e1f68b" />

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

#### 성능 측정 결과

단순히 Default Heap으로 전환하는 것만으로는 성능이 개선되지 않았습니다.  
매 프레임 변하는 데이터까지 Default Heap으로 복사하면 Resource Barrier 오버헤드가 더 크기 때문입니다.  
**변하지 않는 상수 버퍼만 선택적으로 Default Heap을 적용**하는 것이 핵심입니다.

| 방식 | FPS |
|------|-----|
| 모든 버퍼 Upload Heap | 78 |
| 모든 버퍼 Default Heap (단순 전환) | 74 (오히려 감소) |
| **변하지 않는 버퍼만 Default Heap (조건부)** | **81** |

`memcmp` 변경 감지 + `m_bUseDefaultBuffer` 플래그 조합으로 버퍼 단위 선택적 적용이 가능합니다.

→ [`Engine/public/UShaderConstantBuffer.h`](CodeDungeon/Engine/public/UShaderConstantBuffer.h)  
→ [`Engine/private/UShaderConstantBuffer.cpp`](CodeDungeon/Engine/private/UShaderConstantBuffer.cpp)

---

### 4-3. GPU 인스턴싱 파티클 시스템 (Compute + Geometry Shader)
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/4192ad05-d58f-4ed9-80d8-eeeec60fdf8f" />

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
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/80e054fe-7e54-46c4-8f89-57df6874b655" />

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
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/7fbad4a6-3635-41b5-b1cc-e847a0ad5d79" />

#### Root Signature — 단일 범용 설계

모든 셰이더가 공유하는 Root Signature 하나를 설계하여 PSO마다 별도로 RS를 만들지 않습니다.  
Root Signature의 레지스터 레이아웃을 고정함으로써 어떤 셰이더든 동일한 바인딩 규칙으로 리소스를 참조할 수 있습니다.

- **정적 슬롯 (b0~b2)**: 카메라, 전역 파라미터 — Root Descriptor로 직접 바인딩 (Table 거치지 않음)
- **동적 슬롯 (b3~t15)**: 오브젝트별 CBV / SRV / UAV — Descriptor Table로 바인딩
- **Descriptor Table 크기**: 10,000 그룹 × 27개 레지스터 (CBV + SRV 합산)

오브젝트를 그릴 때마다 Current Group Index를 증가시켜 다음 슬롯으로 이동하므로, 이전 프레임 데이터를 덮어쓰지 않고 한 프레임 내에서 최대 10,000개 오브젝트를 바인딩할 수 있습니다.

---

#### PSO 자동화
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/bec24d7d-6d71-4818-950b-e082885f68b3" />


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
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/c39067ce-4986-4069-8004-3acbdb844a10" />

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
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/bc97c90f-dbd9-441d-b3a8-f5d6ba25d151" />

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

---

#### Sword Trail (CatmullRom 스플라인 기반 Procedural Mesh)
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/d67a1b0d-cd74-4954-8833-a80432d87d3e" />

검 궤적을 스플라인 곡선으로 부드럽게 표현하기 위해 메시를 CPU에서 매 프레임 직접 생성합니다.

**Procedural Mesh — Upload Heap 직접 기록**

정적 메시 대신 Upload Heap 버텍스 버퍼를 미리 할당(300 vertices)하고,  
매 프레임 검 끝점(End)과 시작점(Start)을 쌍으로 추가하며 `memcpy`로 GPU 버퍼에 직접 씁니다.

```cpp
// UVIBufferTrail.cpp — NativeConstructClone()
CreateVtxBufferWithNoData(300, sizeof(VTXDEFAULT), nullptr,
    D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    D3D12_HEAP_TYPE_UPLOAD,                         // CPU 직접 쓰기 가능
    D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

m_cpVertexGpuBuffer->Map(0, nullptr, (void**)&m_pBufferDataBegin); // 영구 매핑

// SetVertices() — 매 프레임 호출
memcpy(m_pBufferDataBegin, pVertices.data(), sizeof(VTXDEFAULT) * iVertexCount);
m_stD3DVertexBufferView.SizeInBytes = sizeof(VTXDEFAULT) * iVertexCount; // 실제 크기만 그리기
```

**CatmullRom 보간 — 4점 슬라이딩 윈도우**

단순히 점을 이어 붙이면 꺾이는 궤적이 생깁니다.  
이전 2점 + 현재 2점 총 4개의 제어점을 슬라이딩 윈도우로 추적하고,  
`XMVectorCatmullRom`으로 중간 점들을 보간해 부드러운 곡선을 만듭니다.

```cpp
// UVIBufferTrail.cpp (주석 내 원본 알고리즘)
// m_iCatmullRomIndex[4] — 슬라이딩 윈도우로 4개 제어점 추적
for (_uint i = 0; i < m_iCatmullRomCount; ++i)
{
    float fWeight = float(i + 1) / (m_iCatmullRomCount + 1); // 균등 분할 파라미터

    // 검 끝점 보간
    _float3 Pos = XMVectorCatmullRom(
        XMLoadFloat3(&m_pBufferDataBegin[m_iCatmullRomIndex[0]].vPosition), // P0 (이전이전)
        XMLoadFloat3(&m_pBufferDataBegin[m_iCatmullRomIndex[1]].vPosition), // P1 (이전)
        XMLoadFloat3(&m_pBufferDataBegin[m_iCatmullRomIndex[2]].vPosition), // P2 (현재)
        XMLoadFloat3(&m_pBufferDataBegin[m_iCatmullRomIndex[3]].vPosition), // P3 (다음)
        fWeight);

    // 검 시작점도 동일하게 보간 (index + 1)
}
// 윈도우 슬라이드
m_iCatmullRomIndex[0] = m_iCatmullRomIndex[1];
m_iCatmullRomIndex[1] = m_iCatmullRomIndex[2];
```

UV는 궤적 진행 비율로 자동 계산하여 텍스처가 궤적 전체에 고르게 늘어납니다.

```cpp
for (_uint i = 0; i < m_iCount; i += 2)
{
    m_pBufferDataBegin[i    ].vTexUV = { (float)i / m_iCount, 1.f }; // 위쪽 엣지
    m_pBufferDataBegin[i + 1].vTexUV = { (float)i / m_iCount, 0.f }; // 아래쪽 엣지
}
```

궤적은 0.3초(`m_EndTime`) 후 페이드 아웃됩니다.

**Trail 셰이더 — 노이즈 기반 발광 표현**
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/8d118b89-5276-4609-bb8b-1b70d4a6e650" />

3개 텍스처를 조합해 에너지가 흐르는 듯한 궤적을 표현합니다.

```hlsl
// Trail.hlsl
// Noise 텍스처로 발광 색상 계산 (R:G:B = 1:0.5:0.25 → 붉은-노란 그라데이션)
float ColorUV = min(1.0f, NoiseColor.r * 2.0f);
EmissiveColor.r = lerp(0.0f, 1.0f, ColorUV);
EmissiveColor.g = lerp(0.0f, 1.0f, ColorUV * 0.5f);
EmissiveColor.b = lerp(0.0f, 1.0f, ColorUV * 0.25f);
EmissiveColor.rgb *= 2.0f * NoiseColor.r; // 발광 강도

// Distortion 텍스처로 UV 왜곡 → Base 텍스처와 50% lerp
float2 distortion = g_Texture1.Sample(g_Sampler_Normal, uv).rg * 0.1f;
float4 finalColor = lerp(BaseColor, g_Texture2.Sample(g_Sampler_Normal, uv + distortion), 0.5f);
finalColor.rgb += EmissiveColor.rgb;

// 노이즈 알파 < 0.3 픽셀 제거 → 자연스러운 엣지
if (finalColor.a < 0.3) discard;

Out.vGlow = float4(finalColor.rgb, 0.5f); // Bloom Glow 버퍼에도 출력
```

→ [`Engine/public/UVIBufferTrail.h`](CodeDungeon/Engine/public/UVIBufferTrail.h)  
→ [`Engine/private/UVIBufferTrail.cpp`](CodeDungeon/Engine/private/UVIBufferTrail.cpp)  
→ [`Shader/Trail.hlsl`](CodeDungeon/Shader/Trail.hlsl)

---

#### Distortion Fire — 3중 노이즈 UV 왜곡
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/34771e78-aaee-4e9d-a244-783dcf449b86" />

텍스처 하나를 반복하는 대신, 서로 다른 스케일과 스크롤 속도를 가진 노이즈 레이어 3개를 합산하여 불규칙하고 자연스러운 불꽃 움직임을 만듭니다.

**VS — 3개 노이즈 UV 독립 스크롤**

각 레이어는 스케일(`fScales`)과 스크롤 속도(`fScrollSpeeds`)가 다르게 설정되어, 같은 노이즈 텍스처라도 서로 다른 패턴처럼 동작합니다.

```hlsl
// 2DFire.hlsl — VS_Main
// 레이어마다 다른 스케일로 UV 타일링
Out.vTexCoords1 = In.vTexCoord * fScales.x;
Out.vTexCoords2 = In.vTexCoord * fScales.y;
Out.vTexCoords3 = In.vTexCoord * fScales.z;

// 누적 시간으로 Y축 스크롤 → 불이 위로 타오르는 효과
Out.vTexCoords1.y += fGrobalDeltaTime * fScrollSpeeds.x;
Out.vTexCoords2.y += fGrobalDeltaTime * fScrollSpeeds.y;
Out.vTexCoords3.y += fGrobalDeltaTime * fScrollSpeeds.z;
```

**PS — 합산 노이즈로 UV 왜곡 후 샘플링**
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/8e97ddea-d165-4003-add1-5271b022a865" />

3개 노이즈를 [-1, 1]로 정규화한 뒤 각각 다른 왜곡 계수를 곱해 합산합니다.  
최종 왜곡 강도는 Y 위치에 따라 변화합니다 — 불 아래는 왜곡이 약하고 위로 갈수록 강해져 위로 타오르는 형태를 만듭니다.

```hlsl
// 2DFire.hlsl — PS_Main
noise1 = (g_Texture1.Sample(g_Sampler_Normal, In.vTexCoords1) - 0.5f) * 2.0f; // [-1, 1] 정규화
noise2 = (g_Texture1.Sample(g_Sampler_Normal, In.vTexCoords2) - 0.5f) * 2.0f;
noise3 = (g_Texture1.Sample(g_Sampler_Normal, In.vTexCoords3) - 0.5f) * 2.0f;

noise1.xy *= fDistortion1; // 레이어별 왜곡 방향/강도 차별화
noise2.xy *= fDistortion2;
noise3.xy *= fDistortion3;
finalNoise = noise1 + noise2 + noise3;

// Y 위치 기반 왜곡 강도 — 위로 갈수록 (1-y → 0에 가까워질수록) 왜곡 약해짐
perturb = ((1.0f - In.vTexCoord.y) * fDistortionScale) + fDistortionBias;
noiseCoords = (finalNoise.xy * perturb) + In.vTexCoord.xy; // 왜곡된 UV

fireColor = g_Texture0.Sample(g_Sampler_Clamp, noiseCoords); // 왜곡 UV로 Fire 텍스처 샘플링
fireColor.a = g_Texture2.Sample(g_Sampler_Clamp, noiseCoords); // Alpha 마스크 적용
```

**색상 증폭 및 Glow 연동**

최대 채널값으로 정규화 후 3배 증폭하여 채도 높은 불 색상을 만들고,  
밝기 임계값으로 분기하여 Bloom Glow 버퍼에 강도를 달리 출력합니다.

```hlsl
// 색상 정규화 + 3배 증폭 → 채도 강조
float3 normalizedColor = fireColor.rgb / max(fireColor.r, max(fireColor.g, fireColor.b));
float4 AmpColor = float4(saturate(normalizedColor * 3.0f) * brightness, fireColor.a);

// 밝기 구간별 Glow 출력 강도 분기
float brightness = dot(HighColor.rgb, float3(0.2126, 0.7152, 0.0722));
if      (brightness < 0.2f) Out.vGlow = 0;               // 어두운 영역 제외
else if (brightness < 0.4f) Out.vGlow = float4(fireColor.xyz,  1); // 중간 → 원본
else                        Out.vGlow = float4(AmpColor.xyz,   1); // 밝은 영역 → 증폭
```

→ [`Shader/2DFire.hlsl`](CodeDungeon/Shader/2DFire.hlsl)
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/11957b46-3b58-4ab5-a642-8082e59a6bf4" />

---

#### Guard 결계 / Rim Light
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/3b05fbf7-5189-44f5-b7c6-10aee83ef70b" />
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/e7044d41-f9cf-4545-b65d-746d29cab0e0" />

| 이펙트 | 구현 방식 |
|--------|-----------|
| **Guard 결계** | UV 스크롤 + 노이즈 기반 에너지 막 표현. 플레이어가 특정 구역 진입 시 활성화. Glow 알파 0.5로 Bloom 연동하여 발광 효과 강조, 안개 제외 처리 포함 |
| **Rim Light** | Deferred Lighting 단계와 별개로 적용. 카메라 방향과 법선의 내적이 0에 가까울수록(엣지) 강해지는 프레넬 기반 외곽 발광. 캐릭터 상태(피격/어빌리티)에 따라 색상 전환 |

→ [`Shader/Guard.hlsl`](CodeDungeon/Shader/Guard.hlsl)

---

#### Outline — 깊이 기반 외곽선 합성
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/14243c56-c3c1-429e-82d3-3f13d650ce04" />

외곽선을 그리는 방식은 두 가지를 구현하고 상황에 따라 선택합니다.

| 방식 | 동작 | 적용 상황 |
|------|------|-----------|
| Normal Extrusion | 법선 방향으로 1.25 단위 팽창 후 단색 렌더링 | 일반 외곽선 |
| Uniform Scale | 메시를 1.01배 균등 확대 | 곡면이 많은 오브젝트 |

```hlsl
// NorPos.hlsl — VS_Main
if (ifUseScale) {
    // 균등 스케일 방식 — 구형/유기적 형태에 적합
    matrix scaleMatrix = { 1.01, 0, 0, 0,  0, 1.01, 0, 0,  0, 0, 1.01, 0,  0, 0, 0, 1 };
    Out.vPosition = Compute_FinalMatrix(mul(float4(In.vPosition, 1.f), scaleMatrix));
} else {
    // 법선 팽창 방식 — 각진 메시에 적합
    In.vPosition.xyz += In.vNormal * 1.25;
    Out.vPosition = Compute_FinalMatrix(In.vPosition);
}
```

외곽선은 별도 Render Target에 기록한 뒤, 최종 합성 단계에서 깊이 조건을 검사해 붙입니다.  
**깊이 버퍼가 1.0인 픽셀(오브젝트 뒤에 아무것도 없는 영역)에서만 외곽선을 더합니다.**  
오브젝트가 벽이나 다른 오브젝트 앞에 있을 때 외곽선이 벽을 뚫고 나오지 않는 이유입니다.

```hlsl
// FinalDeffered.hlsl — PS_Main
float vDepthDesc = g_Texture4.Sample(g_Sampler_Normal, Input.vTexUV); // 깊이 버퍼
float4 outline   = g_Texture5.Sample(g_Sampler_Normal, Input.vTexUV); // 외곽선 RT

// 깊이 = 1.0 (빈 공간) + 외곽선 데이터 있는 곳만 합성
if (vDepthDesc == 1.f && outline.w > 0.f)
    Out.vColor.xyz += outline.xyz;
```

어빌리티 활성화 시에는 별도 외곽선 RT(`g_Texture9`)로 전환해 색상을 구분합니다.

→ [`Shader/NorPos.hlsl`](CodeDungeon/Shader/NorPos.hlsl)

---

#### Scene-Based Fog — G-Buffer 픽셀 단위 거리 계산
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/a28811a0-8ed3-4075-8a6a-ee9ca6abff8a" />

일반적인 버텍스 단위 안개는 정점 사이 보간 오차가 생깁니다.  
G-Buffer에 저장된 월드 공간 Position을 읽어 **픽셀 단위로 카메라 거리를 계산**하므로 정확도가 높습니다.

```hlsl
// FinalDeffered.hlsl — PS_Main (Fog 처리)
float3 vPosition = g_Texture2.Sample(g_Sampler_Normal, Input.vTexUV); // G-Buffer Position

// View Space로 변환 후 카메라까지 거리 계산
float3 vViewPixelPos = mul(float4(vPosition, 1.f), g_ViewProjInfoArr[g_CamID].mViewMatrix);
float3 vViewCamPos   = mul(float4(g_CamPosition, 1.f), g_ViewProjInfoArr[g_CamID].mViewMatrix);
float  fDist         = length(abs(vViewPixelPos - vViewCamPos));

// 선형 안개 공식 (start=30, end=180)
float FogFactor = saturate((180.f - fDist) / (180.f - 30.f));
Out.vColor = lerp(float4(0.21f, 0.21f, 0.21f, 1.f), Out.vColor, FogFactor);
```

선형 안개를 선택한 이유는 `fogStart` / `fogEnd` 두 파라미터만으로 안개 영역을 직관적으로 제어할 수 있어, 던전 구간별로 안개 범위를 명확하게 조정할 수 있기 때문입니다.

**Fog Exception — 특정 이펙트 안개 제외**
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/272fa55d-1bce-48c2-a9c8-ac2d4482c69f" />

불꽃 빌보드처럼 자체 발광 이펙트에 안개가 적용되면 색이 탁해집니다.  
Glow 버퍼의 알파값을 식별자로 사용해 해당 픽셀을 안개 계산에서 제외합니다.

```hlsl
// vGlow.a == 0.5 : Trail / Guard 등 발광 이펙트
// vGlow.a == 1.0 : Fire 등 강발광 이펙트
if (IsFogOn && vGlow.a != 0.5f)  // 발광 이펙트 제외
{
    if (vGlow.a != 1.f)           // Fire 제외
        Out.vColor = lerp(float4(0.21f, 0.21f, 0.21f, 1.f), Out.vColor, FogFactor);
}
```

별도 Pass나 마스크 텍스처 없이, 이미 기록된 Glow 버퍼 알파를 재활용하여 비용 없이 예외 처리합니다.

→ [`Shader/FinalDeffered.hlsl`](CodeDungeon/Shader/FinalDeffered.hlsl)  
→ [`Shader/2DFog.hlsl`](CodeDungeon/Shader/2DFog.hlsl)

---

#### Render Target 기반 화면 효과
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/63d49194-c7e9-4242-82a0-333687c70e4c" />

별도의 Post-Processing Pass 없이, Deferred 최종 합성 단계(`FinalDeffered.hlsl`)에서 게임 상태 플래그를 읽어 화면 전체 효과를 적용합니다.  
렌더러에서 `DRAWSHADERBUFFER` 상수 버퍼로 플래그와 경과 시간을 매 프레임 전달합니다.

| 효과 | 트리거 | 구현 방식 |
|------|--------|-----------|
| 게임 시작 | `IsGameStart` | 화면 중앙에서 원형 마스크가 확장 → 수축하며 씬 전환 |
| 피격 | `IsHit` | 화면 중앙에서 반경 0 → 750px로 피격 오버레이 확장 후 페이드 아웃 |
| 사망 | `IsDead` | 빨간 플래시 → 그레이스케일 텍스처로 1.25초 전환 |
| 어빌리티 | `IsAbility` | 어빌리티 오버레이 텍스처를 0.5초 lerp로 블렌드 |

```hlsl
// FinalDeffered.hlsl — 피격 효과 (화면 중심에서 방사형 확장)
float distanceFromOrigin = distance(float2(640, 460), Input.vPosition.xy);

// 경과 시간에 따라 피격 오버레이 반경 축소 (750 → 0.001)
float currentRadius = lerp(finalRadius, initialRadius,
    saturate((HitTime - 0.3f) / (1.8f - 0.3f)));

float distanceFactor = saturate((distanceFromOrigin - currentRadius) / (finalRadius - currentRadius));
Out.vColor = lerp(targetColor, baseColor, distanceFactor); // 중심: 오버레이 / 외곽: 원본
```

```hlsl
// 사망 효과 — 시간 기반 그레이스케일 전환
float blendFactor = saturate(DieTime / 1.25f);
Out.vColor = lerp(float4(1,0,0,1), grayscaleTexture, blendFactor);
```

→ [`Shader/FinalDeffered.hlsl`](CodeDungeon/Shader/FinalDeffered.hlsl)

---

#### 기타 이펙트
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/2007f453-f8c3-408f-ac12-8408b00315bb" />

| 이펙트 | 파일 | 구현 |
|--------|------|------|
| **Motion Blur** | `MotionBlur.hlsl` | Velocity 버퍼 방향으로 50샘플 누적 평균, 알파 임계값 필터링으로 오브젝트 경계 보존 |
| **GrayScale** | `GrayScale.hlsl` | ITU-R BT.709 휘도 변환 + 대비 1.5배 강조, 사망 전환 텍스처로 활용 |
| **Mat(용해)** | `Mat.hlsl` | 남은 시간 비율 기반 알파 임계값(0.7) discard로 오브젝트 소멸 표현 |

→ [`Shader/MotionBlur.hlsl`](CodeDungeon/Shader/MotionBlur.hlsl) · [`Shader/GrayScale.hlsl`](CodeDungeon/Shader/GrayScale.hlsl) · [`Shader/Mat.hlsl`](CodeDungeon/Shader/Mat.hlsl)

---

### 4-6. UI / 사운드 시스템
<img width="2400" height="1350" alt="image" src="https://github.com/user-attachments/assets/62d63f3f-0e22-43e1-8cd4-90ae4938afc6" />

#### UI — 3계층 렌더 레이어

`UUserInterface`는 위치·크기·렌더 순서를 담은 `UIDESC`로 생성됩니다.  
`DrawOrder` 값에 따라 Priority / Middle / Last 3개 렌더 그룹 중 하나에 등록되어, 같은 Z 위치에서도 레이어 순서가 보장됩니다.

```cpp
// CHpBarUI.cpp — LateTickActive()
if      (GetUIDesc().DrawOrder == L"Priority") AddRenderGroup(RI_2DUIPRIORITY);
else if (GetUIDesc().DrawOrder == L"Middle")   AddRenderGroup(RI_2DUIMIDDLE);
else if (GetUIDesc().DrawOrder == L"Last")     AddRenderGroup(RI_2DUILAST);
```

HP 바는 `HPDESC(MaxHp, CurHp)`를 CBV 버퍼로 셰이더에 전달하여, 셰이더 내에서 비율을 계산해 렌더링합니다.

```cpp
// CHpBarUI.cpp — RenderActive()
GetShader()->BindCBVBuffer(m_spHpBarUIBuffer, &_HpDesc, sizeof(HPDESC)); // MaxHp, CurHp 전달
```

마우스 상호작용은 `IsMouseOnRect()`로 RectTransform 범위와 마우스 위치를 비교해 처리합니다.

→ [`Engine/public/UUserInterface.h`](CodeDungeon/Engine/public/UUserInterface.h)  
→ [`CodeDungeon/Private/CHpBarUI.cpp`](CodeDungeon/CodeDungeon/Private/CHpBarUI.cpp)

---

#### 사운드 — FMOD 기반 이벤트 연동

FMOD를 직접 래핑한 `UAudioSystem`으로 BGM / SFX / 3D 사운드를 관리합니다.  
`ACTIVESOUND` 컨테이너로 현재 재생 중인 사운드를 추적하고, `REMOVESOUND`로 종료된 사운드를 매 프레임 정리합니다.

```cpp
// UAudioSystem.h — 주요 API
PlayBGM(soundName);                          // BGM 재생 (전용 채널)
PlayOnceWithManyChannels(soundName);         // 다중 채널 1회 재생 (효과음 중첩)
UpdateSound3D(soundName, pos, velocity, transform); // 3D 위치 기반 음량/팬 갱신
FadeIn(soundName, fadeDuration);             // 페이드 인
FadeOut(soundName, fadeDuration);            // 페이드 아웃
SetPitch(soundName, pitch);                  // 피치 조정 (피격 시 변조 등)
```

플레이어 행동과 연동 방식:

| 이벤트 | 사운드 처리 |
|--------|------------|
| 공격 / 스킬 발동 | `PlayOnceWithManyChannels` — 연속 입력 시 중첩 재생 |
| 피격 | `PlayOnce` + `SetPitch` — 강도에 따라 피치 조정 |
| 씬 전환 | `FadeOut` → `PlayBGM` + `FadeIn` |
| 3D 오브젝트 (횃불, 몬스터) | `UpdateSound3D` 매 프레임 위치 갱신 |

→ [`Engine/public/UAudioSystem.h`](CodeDungeon/Engine/public/UAudioSystem.h)

---

## 5. 빌드 방법

### 요구 환경

| 항목 | 버전 |
|------|------|
| OS | Windows 10 / 11 (64-bit) |
| IDE | Visual Studio 2022 (v143 toolset) |
| Windows SDK | 10.0 |
| DirectX | DirectX 12 (D3D12) |
| GPU | DirectX 12 지원 GPU |

### 포함된 서드파티 라이브러리

빌드에 필요한 라이브러리는 `Engine/ThirdPartyLib/`에 포함되어 있습니다.

| 라이브러리 | 용도 |
|-----------|------|
| DirectXTK12 | DirectX 12 유틸리티 |
| FMOD (`fmod_vc.lib`) | 사운드 시스템 |
| PTHLib | 프레임워크 공통 라이브러리 |

### 빌드 순서

```
1. CodeDungeon/CodeDungeon.sln 열기
2. 빌드 구성: x64 / Debug 또는 Release 선택
3. 솔루션 빌드 (Ctrl+Shift+B)
4. 빌드 완료 후 CodeDungeon/Bin/ 에서 실행 파일 생성 확인
```

> **주의** : 리소스 파일(모델, 텍스처)은 용량 문제로 저장소에 포함되지 않습니다.  
> 실행을 위한 리소스는 별도 문의 바랍니다.
