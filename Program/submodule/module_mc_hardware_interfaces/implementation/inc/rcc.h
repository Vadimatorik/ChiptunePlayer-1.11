#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_RCC_MODULE_ENABLED

#include "mc_hardware_interfaces_rcc.h"

struct rccCfg {
	RCC_OscInitTypeDef			osc;
	RCC_ClkInitTypeDef			clk;
	uint32_t					fLatency;
};

class Rcc : public RccBase {
public:
	Rcc( const rccCfg* const cfg, const uint32_t cfgCount ) : cfg( cfg ), cfgCount( cfgCount ) {}
	RCC_RESULT setCfg ( const uint32_t number_cfg_set = 0 );

private:
	const rccCfg*				const cfg;
	const uint32_t				cfgCount;
};

#endif

#endif
