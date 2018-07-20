#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_GPIO_MODULE_ENABLED

#include "mc_hardware_interfaces_pin.h"

#define PIN_COUNT(x) sizeof(x)/sizeof(x[0])

struct pinCfg {
	const GPIO_TypeDef*		 const GPIOx;
	const GPIO_InitTypeDef	init;
};

class Pin : public PinBase {
public:
	Pin( const pinCfg* const cfg ) : cfg( cfg ) {}

	void	init			( void );	// Перед инициализацией включается тактирование портов.

	void	set				( void );
	void	reset			( void );
	void	toggle			( void );

	void	set				( bool state );
	void	set				( int state );
	void	set				( uint8_t state );

	bool	read			( void );

protected:
	const pinCfg*			const cfg;

};

class PinMultifunc : public Pin, public PinMultifuncBase {
public:
	PinMultifunc( const pinCfg* const cfg, uint32_t countCfg ) : Pin( cfg ), countCfg( countCfg ) {}
	bool	reinit			( uint32_t numberCfg );

protected:
	const uint32_t		countCfg;
};

class PinMultifuncIt : public PinMultifunc, public PinMultifuncItBase {
public:
	/// exitPin - GPIO_PIN_x.
	PinMultifuncIt( const pinCfg* const cfg, uint32_t countCfg, uint32_t exitPin	) : PinMultifunc( cfg, countCfg ), exitPin( exitPin ) {}

	bool	checkIt			( void );
	void	clearIt			( void );

private:
	const uint32_t			exitPin;
};

void gpioClkEn (  const GPIO_TypeDef* GPIOx );

#endif

#endif
