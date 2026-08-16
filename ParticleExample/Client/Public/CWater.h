#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CWATER_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CWATER_H

#include "UPawn.h"

BEGIN(Engine)
class UVIBufferCube;
class UTexGroup;
class UShaderConstantBuffer;
END

BEGIN(Client)

class CWater final : public UPawn {
public:
	struct BINDTEXTURENAME
	{
		_wstring		wstrDiffuseTextureName{ L"" };
		_wstring		wstrDetailTexture01Name{ L"" };
		_wstring		wstrDetailTexture02Name{ L"" };
	};
public:
	CWater(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CWater(const CWater& _rhs);
	DESTRUCTOR(CWater)
public:
	CLONE_MACRO(CWater, "CWater::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy);
private:
	SHPTR<UVIBufferCube>						m_spVIBuffer;
	SHPTR<UTexGroup>								m_spWaterTexGroup;
	constexpr static _uint								WATEROBJECTPARAM_SIZE{ sizeof(WATEROBJECTPARAM) };
	WATEROBJECTPARAM							m_WaterParam;

	SHPTR<UShaderConstantBuffer>		m_spWaterShaderBuffer;
	BINDTEXTURENAME								m_BindTextureName;
};

END

#endif // _PTH_FRAMEWORK_CLIENT_PUBLIC_CWATER_H