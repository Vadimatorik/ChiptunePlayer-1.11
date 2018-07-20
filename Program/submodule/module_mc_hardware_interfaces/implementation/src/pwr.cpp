#include "pwr.h"

#ifdef HAL_PWR_MODULE_ENABLED

BASE_RESULT Pwr::reinit ( uint32_t numberCfg ) {
	if ( numberCfg >= this->countCfg )
		return BASE_RESULT::INPUT_VALUE_ERROR;

	__HAL_RCC_PWR_CLK_ENABLE();
	this->nowCfg	=	numberCfg;

	HAL_PWR_ConfigPVD( ( PWR_PVDTypeDef* )&this->cfg[ this->nowCfg ].cfg );

	return BASE_RESULT::OK;
}

void Pwr::pvdEnable ( void ) {
	HAL_PWR_EnablePVD();
}

void Pwr::pvdDisable ( void ) {
	HAL_PWR_DisablePVD();
}

void Pwr::irqHandler ( void ) {
	HAL_PWR_PVD_IRQHandler();
}

#endif
