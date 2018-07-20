#include "adc.h"

#ifdef HAL_ADC_MODULE_ENABLED

AdcOneChannel::AdcOneChannel( const AdcOneChannelCfg* const cfg, const uint32_t countCfg ) :
	cfg( cfg ), countCfg( countCfg ) {}

BASE_RESULT AdcOneChannel::reinit ( uint32_t numberCfg ) {
	if ( numberCfg >= this->countCfg )	return BASE_RESULT::INPUT_VALUE_ERROR;

	/// Заполняем HAL-структуру.
	this->adc.Instance						= this->cfg[ numberCfg ].ADCx;
	this->adc.Init.ClockPrescaler			= this->cfg[ numberCfg ].clockPrescaler;
	this->adc.Init.Resolution				= this->cfg[ numberCfg ].resolution;
	this->adc.Init.DataAlign				= this->cfg[ numberCfg ].dataAlign;
	this->adc.Init.ScanConvMode				= DISABLE;
	this->adc.Init.ContinuousConvMode		= ENABLE;
	this->adc.Init.DiscontinuousConvMode	= DISABLE;
	this->adc.Init.ExternalTrigConv			= ADC_SOFTWARE_START;
	this->adc.Init.ExternalTrigConvEdge		= ADC_EXTERNALTRIGCONVEDGE_NONE;
	this->adc.Init.NbrOfConversion			= 1;
	this->adc.Init.DMAContinuousRequests	= DISABLE;
	this->adc.Init.EOCSelection				= ADC_EOC_SEQ_CONV;

	this->channelCfg.Channel				= this->cfg[ numberCfg ].channel;
	this->channelCfg.Rank					= 1;
	this->channelCfg.SamplingTime			= this->cfg[ numberCfg ].samplingTime;

	this->clkDisable();
	this->clkEnable();

	HAL_ADC_DeInit( &this->adc );

	HAL_StatusTypeDef r;
	r = HAL_ADC_Init( &this->adc );
	if ( r != HAL_OK ) return BASE_RESULT::ERROR_INIT;

	r = HAL_ADC_ConfigChannel( &this->adc, &this->channelCfg );
	if ( r != HAL_OK ) return BASE_RESULT::ERROR_INIT;

	return BASE_RESULT::OK;
}

void AdcOneChannel::clkEnable ( void ) {
	switch( ( uint32_t )this->adc.Instance ) {
		case ADC1_BASE:	__HAL_RCC_ADC1_CLK_ENABLE(); break;
		case ADC2_BASE:	__HAL_RCC_ADC2_CLK_ENABLE(); break;
		case ADC3_BASE:	__HAL_RCC_ADC3_CLK_ENABLE(); break;
	}
}

void AdcOneChannel::clkDisable ( void ) {
	switch( ( uint32_t )this->adc.Instance ) {
		case ADC1_BASE:	__HAL_RCC_ADC1_CLK_DISABLE(); break;
		case ADC2_BASE:	__HAL_RCC_ADC2_CLK_DISABLE(); break;
		case ADC3_BASE:	__HAL_RCC_ADC3_CLK_DISABLE(); break;
	}
}

BASE_RESULT AdcOneChannel::startContinuousConversion ( void ) {
	HAL_StatusTypeDef r;
	r = HAL_ADC_Start( &this->adc );
	if ( r != HAL_OK ) return BASE_RESULT::ERROR_INIT;
	return BASE_RESULT::OK;
}

void AdcOneChannel::stopContinuousConversion ( void ) {
	HAL_ADC_Stop( &this->adc );
}

uint32_t AdcOneChannel::getMeasurement ( void ) {
	return this->adc.Instance->DR;
}

void AdcOneChannel::irqHandler ( void ) {
	this->adc.Instance->SR = 0;
}

#endif
