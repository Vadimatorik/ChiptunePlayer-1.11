#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_PWR_MODULE_ENABLED

#include "mc_hardware_interfaces_pwr.h"
#include "user_os.h"

struct pwrCfg {
	const PWR_PVDTypeDef			cfg;
};

class Pwr : public PwrBase {
public:
	Pwr ( const pwrCfg* const cfg, const uint32_t countCfg )
		: cfg( cfg ), countCfg( countCfg ),
		nowCfg( 0 ) {}

	BASE_RESULT	reinit			( uint32_t numberCfg = 0 );

	void		pvdEnable		( void );
	void		pvdDisable		( void );

	void		irqHandler		( void );

private:
	const pwrCfg*							const cfg;
	const uint32_t							countCfg;

	uint32_t								nowCfg;
};

#endif

#endif
