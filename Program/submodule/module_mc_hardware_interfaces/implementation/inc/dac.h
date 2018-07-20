#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_DAC_MODULE_ENABLED

#include "mc_hardware_interfaces_dac.h"

struct DacCfg {
	uint32_t			buffer;				// DAC_OUTPUTBUFFER_ENABLE/DAC_OUTPUTBUFFER_DISABLE.
	uint32_t			defaultValue;		// Значение, выставляемое на выводы каналов Dac после инициализации.
};

class Dac : public DacBase {
public:
	Dac( const DacCfg* const cfg, const uint32_t countCfg );

	BASE_RESULT	reinit			( uint32_t numberCfg = 0 );
	BASE_RESULT	setValue		( const uint32_t ch, const uint32_t value );

private:
	void		clkEnable		( void );
	void		clkDisable		( void );

	const DacCfg*					const cfg;
	const uint32_t					countCfg;

	DAC_HandleTypeDef				dac;
	DAC_ChannelConfTypeDef			dacCh;
};

#endif

#endif
