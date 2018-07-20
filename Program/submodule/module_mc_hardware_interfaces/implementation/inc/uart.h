#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_UART_MODULE_ENABLED

#include "dma.h"
#include "mc_hardware_interfaces_uart.h"
#include "user_os.h"

struct uartCfg {
	USART_TypeDef*				uart;
	uint32_t					baudrate;
	uint32_t					mode;						// UART_MODE_RX/UART_MODE_TX/UART_MODE_TX_RX.
	DMA_Stream_TypeDef*			dmaTx;						// Из мерии DMAx_Streamx.
	uint32_t					dmaTxCh;					// Из серии DMA_CHANNEL_x.
};

class Uart : public UartBase {
public:
	Uart( const uartCfg* const cfg, const uint32_t countCfg );

	BASE_RESULT		reinit						( uint32_t numberCfg = 0 );

	BASE_RESULT		on							( void );
	void			off							( void );

	BASE_RESULT		tx							(	const uint8_t*		const txArray,
													const uint16_t&		length		=	1,
													const uint32_t&		timeoutMs	=	100	);

	BASE_RESULT		getByte						(	uint8_t* retrunData,
													const uint32_t&		timeoutMs	=	100	);

	BASE_RESULT		getByteWitchout				(	uint8_t* retrunData	);

	void			irqHandler					( void );

	/// Для внутреннего пользования HAL-а.
	void			giveSemaphore				( void );

private:
	bool 			clkInit						( void );
	bool			clkDeinit					( void );

	const uartCfg*								const cfg;
	const uint32_t								countCfg;

	DMA_HandleTypeDef							dmaTx;
	UART_HandleTypeDef							uart;
	USER_OS_STATIC_BIN_SEMAPHORE				s = nullptr;
	USER_OS_STATIC_MUTEX						m = nullptr;
	USER_OS_STATIC_BIN_SEMAPHORE_BUFFER			sb;
	USER_OS_STATIC_MUTEX_BUFFER					mb;
};

#endif

#endif
