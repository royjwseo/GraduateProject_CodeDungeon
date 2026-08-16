#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UUSERINTERFACE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UUSERINTERFACE_H

#include "UPawn.h"

BEGIN(Engine)
class URectTransform;
class UVIBufferRect;

class ENGINE_DLL DEF_CACHE_ALGIN  UUserInterface abstract : public UPawn{
public:
	typedef struct tagUIDesc
	{
		tagUIDesc() = default;
		tagUIDesc(const _wstring& _strImgName, const _float2& _v2Pos, const _float2& _v2Size, const _float& _fZBufferOrder)
			: strImgName(_strImgName), v2Pos(_v2Pos), v2Size(_v2Size), fZBufferOrder(_fZBufferOrder)
		{}
		_float2		v2Pos;
		_float2		v2Size;
		_float			fZBufferOrder{ 0.f };
		_wstring		strImgName{ L"" };
	}UIDESC;
public:
	UUserInterface(CSHPTRREF<UDevice> _spDevice, 	const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UUserInterface(const UUserInterface& _rhs);
	DESTRUCTOR(UUserInterface)
public:
	SHPTR<URectTransform> GetRectTransform() const { return m_spRectTransform; }
	const UIDESC& GetUIDesc() const { return m_stUIDesc; }
public:
	virtual void Free() override PURE;
	virtual HRESULT NativeConstruct() override PURE;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override PURE;
	// Update Ratio
	void UpdateRatio(const _float _fRatio);
	// Update Pos
	void UpdatePos(const _float2& _vPos);
	// Upate Size
	void UpdateSize(const _float2& _vSize);
	// Mouse	On	Rect
	_bool IsMouseOnRect();
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override PURE;
	virtual void LateTickActive(const _double& _dTimeDelta) override PURE;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand , CSHPTRREF<UTableDescriptor> _spTableDescriptor) override PURE;
	// Ready 
	virtual HRESULT ReadyRectTransform(const VOIDDATAS& _vecDatas) PURE;
protected:
	// Ready Rect Transform
	HRESULT ReadyRectTransform(void* _pDesc);
private:
	SHPTR< URectTransform>		m_spRectTransform;
	UIDESC										m_stUIDesc;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UUSERINTERFACE_H