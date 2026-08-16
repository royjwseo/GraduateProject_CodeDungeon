#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UACTORMANAGER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UACTORMANAGER_H

#include "UBase.h"

BEGIN(Engine)
class UActor;
class UActorGroup;
class UResource;
class UComponent;
class URenderer;

class UActorManager : public UBase {
private:
	using PROTOTYPES = UNORMAP<_wstring, SHPTR<UActor>>;
	using CLONES = UNORMAP<_wstring, SHPTR<UActorGroup>>;
	using CLONEARR = ARRAY<CLONES, CLONE_MAX_SIZE>;
	using CLONEVECTOR = VECTOR<SHPTR<UActorGroup>>;
public:
	UActorManager();
	NO_COPY(UActorManager)
	DESTRUCTOR(UActorManager)
public:
	const CLONEARR& GetClonesArr() const { return m_arrClones; }
public:
	virtual void Free() override;

	// Tick, LateTick 
	void Tick(const _double& _dTimeDelta);
	void LateTick(const _double& _dTimeDelta);
	void Collision(const _double& _dTimeDelta);

	HRESULT ReadyActorManager(CSHPTRREF<URenderer> _spRenderer);
	HRESULT AddPrototypes(const _wstring& _wstrName, const SHPTR<UActor>& _spActor);
	void  CloneActor(const _wstring& _wstrProto, const VOIDDATAS& _stDatas = VOIDDATAS{});
	SHPTR<UActor> CloneActorAdd(const _wstring& _wstrProto, const VOIDDATAS& _stDatas = VOIDDATAS{});
	SHPTR<UActor> CloneActorAddAndNotInLayer(const _wstring& _wstrProto, const VOIDDATAS& _stDatas = VOIDDATAS{});

	void RemoveActor(const SHPTR<UActor>& _spActor);
	void ClearOnceTypeData();
private:
	SHPTR<UActor> FindActor(const _wstring& _wstrPrototypeNames);
private:
	// Containers
	PROTOTYPES				m_ProtoTypes;
	CLONEARR					m_arrClones;
	CLONEVECTOR			m_vecTickClones;
	SHPTR<URenderer>	m_spRenderer;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UACTORMANAGER_H