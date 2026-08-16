#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_USKYBOX_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_USKYBOX_H

#include "UPawn.h"

BEGIN(Engine)
class UVIBufferSkyBox;
class UTexGroup;

class ENGINE_DLL DEF_CACHE_ALGIN USkyBox : public UPawn {
public:
	typedef struct tagSkyBoxDesc
	{
		tagSkyBoxDesc() :
			wstrSkyBoxTexPath{ L"" },
			wstrSkyBoxTextureName{ L"" },
			isFolder{ false }
		{};
		tagSkyBoxDesc(const _wstring& _wstrSkyBoxTexPath, const _wstring& _wstrSkyBoxTextureName,
			const _bool _isFolder)
			: wstrSkyBoxTexPath{ _wstrSkyBoxTexPath }, wstrSkyBoxTextureName{ _wstrSkyBoxTextureName }, isFolder{ _isFolder }
		{}

		_wstring		wstrSkyBoxTexPath;
		_wstring		wstrSkyBoxTextureName;
		_bool			isFolder;
	}SKYBOXDESC;
public:
	USkyBox(CSHPTRREF<UDevice> _spDevice, 	const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	USkyBox(const USkyBox& _rhs);
	DESTRUCTOR(USkyBox)
public:
	CSHPTRREF<UTexGroup> GetTextureGroup() const { return m_spTextureGroup; }
public:
	CLONE_MACRO(USkyBox, "USkyBox::Clone To Failed")
	virtual void Free() override ;
	virtual HRESULT NativeConstruct() override ;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override ;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override ;
	virtual void LateTickActive(const _double& _dTimeDelta) override ;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override ;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
#ifdef _USE_IMGUI
public:
	virtual void ShowObjectInfo() override;
#endif
private:
	SHPTR<UVIBufferSkyBox>	m_spVIBufferSkyBox;
	SHPTR<UTexGroup>				m_spTextureGroup;
	_wstring										m_wstrTextureName;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_USKYBOX_H

