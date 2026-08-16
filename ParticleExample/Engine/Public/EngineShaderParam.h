
#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_ENGINESHADERPARAM_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_ENGINESHADERPARAM_H

namespace Engine {

#pragma region VIEWPROJ


	typedef struct tagViewProjMatrix {

		_float4x4 mViewMatrix{ _float4x4::Identity };
		_float4x4 mProjectionMatrix{ _float4x4::Identity };
		_float4x4 mPrevViewMatrix{ _float4x4::Identity };
		_float		   fCamFar{ 0.f };
		_float3	  vCamPadding{};
	}VIEWPROJMATRIX;

	typedef struct tagViewProjParam {
		ARRAY<VIEWPROJMATRIX, MAX_CAM_SIZE>	arrViewProjParams{};
		_bool																			isObjectMotionBlur{ false };
		_bool																			isViewProjBoolPadding1{ false };
		_bool																			isViewProjBoolPadding2{ false };
		_bool																			isViewProjBoolPadding3{ false };
		// Padding Value
		_float3																		vViewProjPaddings{ 0.f, 0.f, 0.f };
	}VIEWPROJPARAM;

#pragma endregion VIEWPROJ

#pragma region GROBADATA

	struct GROBALINFO
	{
		float		  fDeltaTime{ 0 };
		_float3     vPadding;
	};

	struct GROBALPARAM
	{
		GROBALINFO		stGrobalInfo;
	};

#pragma endregion GROBALDATA

#pragma region TRANSFORM 

	typedef struct tagTransformMatrix
	{
		_float4x4 mWorldMatrix{ _float4x4::Identity };
		_float4x4 mPrevWorldMatrix{ _float4x4::Identity };
		_uint		iCamIndex{ 0 };
		_int3		  iPadding;
	}TRANSFORMPARAM;

#pragma endregion TRANSFORM

#pragma region MATERIAL 



#pragma endregion MATERIAL 

#pragma region LIGHT 

	struct LIGHTCOLOR
	{
		LIGHTCOLOR() = default;
		LIGHTCOLOR(const _float4& _vDiffuse, const _float4& _vAmbient, const _float4& _vSpecular) : 
			vDiffuse(_vDiffuse), vAmbient(_vAmbient), vSpecular(_vSpecular)
		{}

		_float4      vDiffuse{ 0.f, 0.f, 0.f, 0.f };
		_float4      vAmbient{ 0.f, 0.f, 0.f, 0.f };
		_float4      vSpecular{ 0.f, 0.f, 0.f, 0.f };
	};

	struct RIMLIGHT
	{
		RIMLIGHT() = default;
		RIMLIGHT(const _float _fRimLightPower, const _float _fRimMin, const _float _fRimMax)
			: fRimLightPower(_fRimLightPower), fRimMin(_fRimMin), fRimMax(_fRimMax)
		{}

		// RimLight
		_float			fRimLightPower = 10.f;
		_float			fRimMin = 0.f;
		_float			fRimMax = 0.3f;
	};

	struct LIGHTPOWER
	{
		LIGHTPOWER() = default;
		LIGHTPOWER(const _float _ffLightPower, const _float _fSpecularPower)
			: fLightPower(_ffLightPower), fSpecularPower(_fSpecularPower)
		{}

		_float         fLightPower{ 0 };
		_float         fSpecularPower{ 0 };
	};

	struct LIGHTCONTROL
	{
		LIGHTCONTROL() = default;
		LIGHTCONTROL(const RIMLIGHT& _stRimLight, const _float3& _vAttenuation = { 1.f, 0.1f, 0.01f }) :
			fRimLightPower{_stRimLight.fRimLightPower}, fRimMin{ _stRimLight.fRimMin },
			fRimMax{ _stRimLight.fRimMax }, vAttenuation{ _vAttenuation }
		{}

		// ============= 4================
		_float			fShadowDepth = 1.f;
		// RimLight
		_float			fRimLightPower = 10.f;
		_float			fRimMin = 0.f;
		_float			fRimMax = 0.3f;
		// ============= 8	================
		_float3		vAttenuation{ 1.f, 0.1f, 0.01f };
		_float			fPadding2{ 0.f };
		// Shadow 
		_float4		vShadowColor = _float4(0.5f, 0.5f, 0.5f, 1.f);
	};

	typedef struct tagLightParam
	{
		_float3					vCamPos = _float3(0.f, 0.f, 0.f);
		_float						 fCamFar{ 0.f };

		_float4x4				mScreenWorldMatrix{ _float4x4::Identity };
		_float4x4				mScreenViewmatrix{ _float4x4::Identity };
		_float4x4				mScreenProjMatrix{ _float4x4::Identity };
		_float4x4				mViewMatrix{ _float4x4::Identity };
		_float4x4				mProjMatrix{ _float4x4::Identity };
		_float4x4				mViewMatrixInv{ _float4x4::Identity };
		_float4x4				mProjMatrixInv{ _float4x4::Identity };
	}LIGHTPARAM;

	typedef struct tagLightInfo
	{
		tagLightInfo() = default;
		tagLightInfo(const LIGHTTYPE& _eLightInfo, const _float4& _vDiffuse, const _float4& _vAmbient, const _float4& _vSpecular,
			const _float3& _vDirection, const _float3& _vPosition, const _float& _fRangle, const _float& _fAngle = 0.f,
			const _float _fLightPower = 1.f, const _float _fSpecularPowValue = 1.f)
			: eLightType(_eLightInfo), vDiffuse(_vDiffuse), vAmbient(_vAmbient), vSpecular(_vSpecular)
			, vDirection(_vDirection), vPosition(_vPosition),
			fRange(_fRangle), fAngle(_fAngle), fLightPower(_fLightPower), fSpecularPowValue{_fSpecularPowValue }
		{}
		_float4				vDiffuse = _float4(0.f, 0.f, 0.f, 0.f);
		_float4				vAmbient = _float4(0.f, 0.f, 0.f, 0.f);
		_float4				vSpecular = _float4(0.f, 0.f, 0.f, 0.f);
		_float4				vDirection = _float4(0.f, 0.f, 0.f, 0.f);
		_float4				vPosition = _float4(0.f, 0.f, 0.f, 0.f);
		// 4 =====================
		LIGHTTYPE		eLightType = LIGHTTYPE::LIGHT_END;
		_float					fRange = 0.f;
		_float					fAngle{ 0.f };
		_float					fLightPower{ 1.f };
		// 4 =====================
		_float					fSpecularPowValue{ 1.f };
		_float3       vPadding;

	}LIGHTINFO;

	typedef struct tagLightConstrolParam
	{
		LIGHTINFO								tLightInfo{ };
		LIGHTCONTROL					tLightControl{};
	}LIGHTCONTROLPARAM;
#pragma endregion LIGHT

#pragma region ANIMATIONPARAM

	typedef struct tagAnimationParam
	{
		_bool		isOutLineExist = false;
		_bool		isNomralExist = true;
		_bool		isAnimPadding = false;
		_bool		isAnimPadding2 = false;
	}ANIMATIONPARAM;

#pragma endregion ANIMATIONPARAM

#pragma region BONEPARAM

	struct SOCKETMATRIXPARAM
	{
		_float4x4 SocketMatrix;
	};

	typedef struct tagBoneMatrixParam
	{
		ARRAY<_float4x4, MAX_BONE_SIZE>		arrBoneMatrix;
	}BONEMATRIXPARAM;

#pragma endregion BONEPARAM

#pragma region PARTICLE

	struct GROBALPARTICLEINFO
	{
		GROBALPARTICLEINFO() : 	iMaxCount{0}, iAddCount{0},
		iFrameNumber{0}, fDeltaTime{0}, fAccTime{0},
		fMinLifeTime{0}, fMaxLifeTime{0},	fMinSpeed{0},
		 fMaxSpeed{0}, fStartScaleParticle{0}, fEndScaleParticle{0}, fPadding{0}
		{}

		_uint		iMaxCount;
		_uint		iAddCount;
		_uint		iFrameNumber;
		_float		fDeltaTime;
		// ==============
		_float		fAccTime;
		_float		fMinLifeTime;
		_float		fMaxLifeTime;
		_float		fMinSpeed;
		// ===============
		_float		 fMaxSpeed;
		_float		fStartScaleParticle;
		_float		fEndScaleParticle;
		_float		fPadding;
	};

	struct PARTICLE
	{
		PARTICLE() : vWorldPos{ 0, 0, 0 }, fCurTime{ 0 }, vWorldDir{ 0, 0, 0 }, fLifeTime{ 0 }, iAlive{ 0 },	padding{ 0, 0, 0 }
		{}
		_float3		vWorldPos;
		_float			fCurTime;
		// ===============
		_float3		vWorldDir;
		_float			fLifeTime;
		// ==============
		_uint			iAlive;
		_float3		padding;
	};


	typedef struct tagParticleParam
	{
		GROBALPARTICLEINFO stGrobalParticleInfo{};
	}PARTICLEPARAM;

	struct ANIMATEDPARTICLEPARAM
	{
		_float4x4						mAnimateMatrix{};
	};
;

	struct COMPUTESHADERINFO
	{
		_uint iAddCount{ 0 };
		_int3 padding{};
	};

#pragma endregion PARTICLE

#pragma region WATER

	struct WATERIFNODESC
	{
		_float2 vWaterNormalMapOffset{}; // 물 표면의 노말 맵 이동 오프셋
		_float fWaveAmplitude{ 0.f }; // 파고
		_float fWaveFrequency{ 0.f }; // 파동 빈도
		_float fWaveSpeed{ 0.f }; // 파동 속도

		_float Padding;
	};

	struct WATEROBJECTPARAM
	{
		WATERIFNODESC   waterStruct;
		_float4x4				   g_mTextureAnimation;
	};

#pragma endregion WATER

#pragma region DEBUGGING

	typedef struct tagDebuggingParam
	{
		_float4			vDebugging{ 0.f, 0.f, 0.f, 0.f };
	}DEBUGGINPARAM;

#pragma endregion DEBUGGING


#pragma region DEAULTOBJECT

	struct NORMALCUBEPARAM
	{
		_float4	g_vNormalCubeColor{ 1.f, 1.f, 1.f, 1.f };
		_int			g_iNormalCubeTextureBind{ 0 };
		_int3		g_iNormalCubePadding{ 0, 0, 0 };
	};

#pragma endregion  DEFAULTOBEJCT
}

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_ENGINESHADERPARAM_H