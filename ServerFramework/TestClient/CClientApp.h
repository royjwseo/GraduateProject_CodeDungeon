#pragma once

#include "ACoreApp.h"

BEGIN(Core)
class ACoreInstance;
END

class CClientAdiminstor;

class CClientApp final : public ACoreApp {
public:
	CClientApp();
	NO_COPY(CClientApp)
public:
	virtual _bool Start() override;
private:
};

