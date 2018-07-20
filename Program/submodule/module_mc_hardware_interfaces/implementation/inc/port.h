#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_GPIO_MODULE_ENABLED

#include "mc_hardware_interfaces_port.h"

#include "pin.h"

class GlobalPort : public GlobalPortBase {
public:
	GlobalPort( const pinCfg* const cfg, uint32_t countCfg ) : cfg( cfg ), countCfg( countCfg ) {}
	BASE_RESULT	reinitAllPorts		( void );

private:
	const pinCfg*				const cfg;
	const uint32_t				countCfg;
};

#endif

#endif
