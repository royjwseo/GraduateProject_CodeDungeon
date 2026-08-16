#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UMATERIAL_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UMATERIAL_H

#include "UComponent.h"

BEGIN(Engine)

class UShader;
class UTexture;

class  ENGINE_DLL UMaterial : public UComponent {
public:
	using TEXTURECONTAINER = UNORMAP<SRV_REGISTER, SHPTR<UTexture>>;
public:
	UMaterial(CSHPTRREF<UDevice> _spDevice);
	UMaterial(const UMaterial& _rhs);
	DESTRUCTOR(UMaterial)
public:
	// Free
	virtual void Free() override;
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _convector) override;

	void AddTexturel(const SRV_REGISTER _eSrvRegister, CSHPTRREF<UTexture> _spTexture);
	void BindTexture(CSHPTRREF<UShader> _spShader);
private:
	// Texture Container 
	TEXTURECONTAINER		m_TextxureConatiner;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UMATERIAL_H