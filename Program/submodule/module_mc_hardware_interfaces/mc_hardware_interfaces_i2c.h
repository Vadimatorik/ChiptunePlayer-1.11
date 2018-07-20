#pragma once

#ifdef __cplusplus

#include "mc_hardware_interfaces_base.h"

/*!
 * Класс переназначен для работы с I2C
 * в режиме ведущего устройства на шине.
 */
class I2cMasterBase {
public:
	/*!
	 * Сбрасывает текущие настройки I2C и инициализирует его заново.
	 * После инициализации I2C остается отключенным.
	 *
	 * Замечание: управление тактовым сигналом аппаратного
	 * модуля осуществляется внутри метода автоматически.
	 *
	 * \param[in]	numberCfg		-	выбранный режим работы.
	 *
	 * \return		{	BASE_RESULT::OK			-	инициализация прошла успешно.
	 *					BASE_RESULT::INPUT_VALUE_ERROR	-	несуществующий номер конфигурации.
	 *					BASE_RESULT::ERROR_INIT	-	в противном случае.	}
	 */
	virtual	BASE_RESULT		reinit		( uint32_t numberCfg = 0 )							= 0;

	/*!
	 * Запускает I2C.
	 *
	 * \return		{	BASE_RESULT::OK					-	передача прошла успешно.
	 *					BASE_RESULT::ERROR_INIT			-	SPI не был инициализирован ранее.	}
	 */
	virtual	BASE_RESULT		on			( void )											= 0;

	/// Останавливает I2C.
	virtual	void			off			( void )											= 0;


	virtual	BASE_RESULT		read		( const uint8_t slaveDeviceAddress,
										  uint8_t* rx, uint8_t readAddress,
										  uint16_t countByte )								= 0;

	virtual	BASE_RESULT		readDma		( const uint8_t slaveDeviceAddress,
										  uint8_t* rx, uint8_t readAddress,
										  uint16_t countByte )								= 0;

	virtual	BASE_RESULT		writeByte	( const uint8_t slaveDeviceAddress,
										  uint8_t* rx,
										  uint8_t writeAddress )							= 0;
};

#endif
