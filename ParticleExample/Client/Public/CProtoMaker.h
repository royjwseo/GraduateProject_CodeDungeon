#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CPROTOMAKER_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CPROTOMAKER_H

BEGIN(Engine)
class UGameInstance;
class UDevice;
class UCommand;
class USkyBox;
END

BEGIN(Client)

class CProtoMaker {
public:
	static HRESULT CreateProtoData(CSHPTRREF<UGameInstance> _spGameInstance,
		CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand);

	static HRESULT LoadStageScene1(CSHPTRREF<UGameInstance> _spGameInstance,
		CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand);
};

END

#endif