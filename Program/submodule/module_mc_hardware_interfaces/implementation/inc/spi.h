#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_SPI_MODULE_ENABLED

#include "dma.h"
#include "user_os.h"
#include "mc_hardware_interfaces_spi.h"
#include "pin.h"

struct SpiMaster8BitCfg {
	SPI_TypeDef*				SPIx;

	PinBase*					pinCs;

	uint32_t					clkPolarity;					/// SPI_Clock_Polarity.
	uint32_t					clkPhase;						/// SPI_Clock_Phase.

	uint32_t*					baudratePrescalerArray;			/// SPI_BaudRate_Prescaler
	uint32_t					numberBaudratePrescalerCfg;		/// Колличество режимов.

	/// В случае, если DMA не используется (передача и прием на прерываниях),
	/// то следует указать nullptr.
	DMA_Stream_TypeDef*			dmaTx;							/// Из мерии DMAx_Streamx.
	DMA_Stream_TypeDef*			dmaRx;							/// Из мерии DMAx_Streamx.
	uint32_t					dmaTxCh;						/// Из серии DMA_CHANNEL_x.
	uint32_t					dmaRxCh;						/// Из серии DMA_CHANNEL_x.
};

class SpiMaster8Bit : public SpiMaster8BitBase {
public:
	SpiMaster8Bit( const SpiMaster8BitCfg* const cfg, const uint32_t countCfg );

	BASE_RESULT		reinit			( uint32_t numberCfg = 0 );

	BASE_RESULT		on				( void );
	void			off				( void );

	BASE_RESULT	tx				(	const uint8_t*		const txArray,
									const uint16_t		length		=	1,
									const uint32_t		timeoutMs	=	100	);

	BASE_RESULT tx				(	const uint8_t*		const txArray,
									uint8_t*			rxArray,
									const uint16_t		length		=	1,
									const uint32_t		timeoutMs	=	100	);

	BASE_RESULT	txOneItem		(	const uint8_t		txByte,
									const uint16_t		count		=	1,
									const uint32_t		timeoutMs	=	100	);

	BASE_RESULT	rx				(	uint8_t*			rxArray,
									const uint16_t		length		=	1,
									const uint32_t		timeoutMs	=	100,
									const uint8_t		outValue	=	0xFF );

	BASE_RESULT		setPrescaler	(	uint32_t prescalerNumber		=	0	);


	void	giveSemaphore			( void );			// Отдать симафор из прерывания (внутренняя функция.
	void	irqHandler		( void );

private:
	bool	initClkSpi		( void );											// Включаем тактирование SPI и DMA (если используется).
	bool	initSpi			( void );											// Инициализируем только SPI (считается, что он уже затактирован).

	const SpiMaster8BitCfg*					const cfg;
	const uint32_t							countCfg;

	uint32_t*								baudratePrescalerArray;
	uint32_t								numberBaudratePrescalerCfg;

	SPI_HandleTypeDef						spi;

	USER_OS_STATIC_BIN_SEMAPHORE_BUFFER		sb;
	USER_OS_STATIC_BIN_SEMAPHORE			s = nullptr;

	DMA_HandleTypeDef						dmaTx;
	DMA_HandleTypeDef						dmaRx;

	PinBase*								cs;

	USER_OS_STATIC_MUTEX					m = nullptr;
	USER_OS_STATIC_MUTEX_BUFFER				mb;

};

#endif

#endif
