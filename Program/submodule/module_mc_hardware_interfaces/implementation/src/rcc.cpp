#include "rcc.h"

#ifdef HAL_RCC_MODULE_ENABLED

RCC_RESULT Rcc::setCfg ( const uint32_t numberCfg ) {
	if ( numberCfg >= this->cfgCount ) return RCC_RESULT::ERROR_CFG_NUMBER;

	HAL_RCC_DeInit();

	if ( HAL_RCC_OscConfig( ( RCC_OscInitTypeDef* )&this->cfg[ numberCfg ].osc ) != HAL_OK )
		return RCC_RESULT::ERROR_OSC_INIT;

	if ( HAL_RCC_ClockConfig( ( RCC_ClkInitTypeDef* )&this->cfg[ numberCfg ].clk, this->cfg[ numberCfg ].fLatency ) != HAL_OK )
		return  RCC_RESULT::ERROR_CLK_INIT;

	return RCC_RESULT::OK;
}

#endif
