#include "dac.h"

#ifdef HAL_DAC_MODULE_ENABLED

Dac::Dac( const DacCfg* const cfg, const uint32_t countCfg ) :
	cfg( cfg ), countCfg( countCfg ) {
	this->dac.Instance							= DAC;
	this->dacCh.DAC_Trigger						= DAC_TRIGGER_NONE;
}

BASE_RESULT Dac::reinit ( uint32_t numberCfg ) {
	if ( numberCfg >= this->countCfg )	return BASE_RESULT::INPUT_VALUE_ERROR;

	/// Заполнение HAL-структуры.
	this->dacCh.DAC_OutputBuffer				= cfg[ numberCfg ].buffer;

	this->clkDisable();
	this->clkEnable();

	if ( HAL_DAC_DeInit( &this->dac ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	if ( HAL_DAC_Init( &this->dac ) != HAL_OK )
		return BASE_RESULT::ERROR_INIT;

	if ( HAL_DAC_ConfigChannel( &this->dac, &this->dacCh, DAC_CHANNEL_1 ) != HAL_OK)
		return BASE_RESULT::ERROR_INIT;

	if ( HAL_DAC_ConfigChannel( &this->dac, &this->dacCh, DAC_CHANNEL_2 ) != HAL_OK)
		return BASE_RESULT::ERROR_INIT;

	HAL_DAC_Start(  &this->dac, DAC_CHANNEL_1 );
	HAL_DAC_Start(  &this->dac, DAC_CHANNEL_2 );

	HAL_DAC_SetValue( &this->dac, DAC_CHANNEL_1,	DAC_ALIGN_12B_R, this->cfg[ numberCfg ].defaultValue );
	HAL_DAC_SetValue( &this->dac, DAC_CHANNEL_2,	DAC_ALIGN_12B_R, this->cfg[ numberCfg ].defaultValue );

	return BASE_RESULT::OK;
}

BASE_RESULT	Dac::setValue ( const uint32_t ch, const uint32_t value ) {
	if ( ch > 1 )			return BASE_RESULT::INPUT_VALUE_ERROR;
	if ( value > 0xFFF )	return BASE_RESULT::INPUT_VALUE_ERROR;

	if ( this->dac.State == HAL_DAC_STATE_RESET )	return BASE_RESULT::ERROR_INIT;

	if ( ch == 0 ) {
		HAL_DAC_SetValue( &this->dac, DAC_CHANNEL_1,	DAC_ALIGN_12B_R, value );
	} else {
		HAL_DAC_SetValue( &this->dac, DAC_CHANNEL_2,	DAC_ALIGN_12B_R, value );
	}

	return BASE_RESULT::OK;
}

void Dac::clkEnable ( void ) {
	__HAL_RCC_DAC_CLK_ENABLE();
}

void Dac::clkDisable ( void ) {
	__HAL_RCC_DAC_CLK_DISABLE();
}

#endif
