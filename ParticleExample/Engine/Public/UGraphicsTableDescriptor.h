#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UGRAPHICSTABLEDESCRIPTOR_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UGRAPHICSTABLEDESCRIPTOR_H
#include "UTableDescriptor.h"


BEGIN(Engine)
class UDevice;
class UGpuCommand;


class ENGINE_DLL DEF_CACHE_ALGIN UGraphicsTableDescriptor final : public UTableDescriptor {
public:
	UGraphicsTableDescriptor();
	NO_COPY(UGraphicsTableDescriptor)
	DESTRUCTOR(UGraphicsTableDescriptor)
public:
	virtual void Free() override;
	// Set 
	virtual void CommitTable(CSHPTRREF<UCommand> _spCommand) override;
	using UTableDescriptor::GetCpuHandle;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UGRAPHICSTABLEDESCRIPTOR_H