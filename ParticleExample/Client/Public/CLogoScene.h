#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CLOGOSCENE_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CLOGOSCENE_H

#include "UScene.h"

BEGIN(Engine)

END

BEGIN(Client)

class CLogoScene final  : public UScene {
public:
	CLogoScene(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(CLogoScene)
	DESTRUCTOR(CLogoScene)
public:
	// UScene을(를) 통해 상속됨
	virtual void Free() override;
	virtual HRESULT LoadSceneData() override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void LateTick(const _double& _dTimeDelta) override;

private:

};

END

#endif // _PTH_FRAMEWORK_CLIENT_PUBLIC_CLOGOSCENE_H