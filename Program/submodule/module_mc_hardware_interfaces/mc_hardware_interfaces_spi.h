#pragma once

#ifdef __cplusplus

#include "mc_hardware_interfaces_base.h"

/*!
 * Класс предназначен для использования SPI в режиме мастера с разрядностью
 * пакета 8 бит.
 */
class SpiMaster8BitBase {
public:
	/*!
	 * Сбрасывает текущие настройки SPI и инициализирует его заново.
	 * После инициализации SPI остается отключенным.
	 *
	 * Замечание: управление тактовым сигналом аппаратного
	 * модуля осуществляется внутри метода автоматически.
	 *
	 * \return		{	BASE_RESULT::OK					-	инициализация прошла успешно.
	 *					BASE_RESULT::INPUT_VALUE_ERROR	-	несуществующий номер конфигурации.
	 *					BASE_RESULT::ERROR_INIT			-	ошибка инициализации.
	 */
	virtual	BASE_RESULT		reinit		( uint32_t numberCfg = 0 )						= 0;

	/*!
	 * Запускает SPI.
	 *
	 * \return		{	BASE_RESULT::OK					-	передача прошла успешно.
	 *					BASE_RESULT::ERROR_INIT			-	SPI не был инициализирован ранее.	}
	 */
	virtual	BASE_RESULT		on			( void )										= 0;

	/// Останавливает SPI.
	virtual	void			off			( void )										= 0;

	/*!
	 * Передает массив по SPI игнорируя приходящие байты.
	 *
	 * Замечание: входной прием не ведется (входные данные просто теряются).
	 *
	 * \param[in]	txArray		-	указатель на массив, который требуется передать
	 *								по spi. Указатель на 0-й передаваемый байт
	 *								( байт, с которого будет начата передача ).
	 * \param[in]	length		-	длина посылки.
	 *								Условие; length != 0 и length >= 1!
	 * \param[in]	timeoutMs	-	если посылка не была передана в течении этого времени
	 *								( в миллисекундах ), то будет
	 *								возвращена соответствующая ошибка.
	 * \param[in]	stepMode	-	направление движения по массиву.
	 *
	 * \return		{	BASE_RESULT::OK					-	передача прошла успешно.
	 *					BASE_RESULT::ERROR_INIT			-	SPI не был инициализирован ранее.
	 *					BASE_RESULT::LENGTH_ERROR		-	length вне диапазона.
	 *					BASE_RESULT::INPUT_VALUE_ERROR	-	txArray == nullptr.
	 *					BASE_RESULT::TIME_OUT			-	время ожидания истекло.	}
	 */
	virtual BASE_RESULT	tx	(	const uint8_t*		const txArray,
								const uint16_t		length		=	1,
								const uint32_t		timeoutMs	=	100	)				= 0;

	/*!
	 * Передает массив по SPI и забирает ответ.
	 *
	 * Замечание: txArray может быть равен rxArray. Тогда
	 * принятые данные перезапишут массив исходящих данных!
	 *
	 * \param[in]	txArray		-	указатель на массив, который требуется передать
	 *								по spi. Указатель на 0-й передаваемый байт
	 *								( байт, с которого будет начата передача )
	 * \param[out]	rxArray		-	указатель на массив, в который будет
	 *								осуществляться прием по spi.
	 *								Указатель на 0-й принемаемый байт
	 *								( первый пришедший байт ).
	 * \param[in]	length		-	длина посылки.
	 *								Условие; length != 0 и length >= 1.
	 * \param[in]	timeoutMs	-	если посылка не была передана в течении этого времени
	 *								( в миллисекундах ), то будет
	 *								возвращена соответствующая ошибка.
	 * \param[in]	stepMode	-	направление движения по массиву.
	 *
	 * \return		{	BASE_RESULT::OK					-	передача прошла успешно.
	 *					BASE_RESULT::ERROR_INIT			-	SPI не был инициализирован ранее.
	 *					BASE_RESULT::LENGTH_ERROR		-	length вне диапазона.
	 *					BASE_RESULT::INPUT_VALUE_ERROR	-	txArray == nullptr или rxArray == nullptr.
	 *					BASE_RESULT::TIME_OUT			-	время ожидания истекло.	}
	 */
	virtual BASE_RESULT tx	(	const uint8_t*		const txArray,
								uint8_t*			rxArray,
								const uint16_t		length		=	1,
								const uint32_t		timeoutMs	=	100	)				= 0;

	/*!
	 * Передает один элемент count раз.
	 *
	 * Замечание: txArray может быть равен rxArray. Тогда
	 * принятые данные перезапишут массив исходящих данных!
	 *
	 * \param[in]	txByte		-	переменная, которая будет передаваться
	 *								count раз.
	 * \param[in]	count		-	количество транзакций (сколько раз передать байт).
	 *								Условие: ount != 0 и count >= 1.
	 * \param[in]	timeoutMs	-	если посылка не была передана в течении этого времени
	 *								( в миллисекундах ), то будет
	 *								возвращена соответствующая ошибка.
	 *
	 * \return		{	BASE_RESULT::OK					-	передача прошла успешно.
	 *					BASE_RESULT::ERROR_INIT			-	SPI не был инициализирован ранее.
	 *					BASE_RESULT::LENGTH_ERROR		-	length вне диапазона.
	 *					BASE_RESULT::TIME_OUT			-	время ожидания истекло.	}
	 */
	virtual BASE_RESULT	txOneItem (	const uint8_t	txByte,
									const uint16_t	count		=	1,
									const uint32_t	timeoutMs	=	100	)				= 0;

	/*!
	 * Принимает данные их SPI.
	 *
	 * \param[out]	rxArray		-	указатель на массив, в который будет
	 *								осуществляться прием по spi.
	 *								Указатель на 0-й принимаемый байт
	 *								( первый пришедший байт ).
	 * \param[in]	length		-	длина посылки.
	 *								Условие; length != 0 и length >= 1.
	 * \param[in]	timeoutMs	-	если посылка не была передана в течении этого времени
	 *								( в миллисекундах ), то будет
	 *								возвращена соответствующая ошибка.
	 * \param[in]	stepMode	-	направление движения по массиву.
	 * \param[in]	outValue	-	значение, которое будет отправляться по TX
	 *								( если будет использоваться ).
	 * \return		{	BASE_RESULT::OK					-	передача прошла успешно.
	 *					BASE_RESULT::ERROR_INIT			-	SPI не был инициализирован ранее.
	 *					BASE_RESULT::LENGTH_ERROR		-	length вне диапазона.
	 *					BASE_RESULT::INPUT_VALUE_ERROR	-	txArray == nullptr или rxArray == nullptr.
	 *					BASE_RESULT::TIME_OUT			-	время ожидания истекло.	}
	 */
	virtual	BASE_RESULT	rx	(	uint8_t*			rxArray,
								const uint16_t		length		=	1,
								const uint32_t		timeoutMs	=	100,
								const uint8_t		outValue	=	0xFF )				= 0;

	/*!
	 * Устанавливает делитель SPI.
	 * \param[in]	prescalerNumber		-	номер конфигурации делителя.
	 *
	 * \return		{	BASE_RESULT::OK					-	делитель установлен.
	 *					BASE_RESULT::ERROR_INIT			-	SPI не был инициализирован ранее.
	 *					BASE_RESULT::INPUT_VALUE_ERROR	-	несуществующий номер конфигурации.	}
	 */
	virtual BASE_RESULT	setPrescaler	(	uint32_t prescalerNumber	=	0	)		= 0;
};

#endif

