#pragma once

#ifdef __cplusplus

#include "mc_hardware_interfaces_base.h"

class WdtBase {
public:
	/*!
	 * Сбрасывает текущие настройки WDT и инициализирует его заново.
	 * После инициализации WDT автоматически запускается.
	 *
	 * \param[in]	numberCfg		-	выбранный режим работы.
	 *
	 * \return		{	BASE_RESULT::OK					-	инициализация прошла успешно.
	 *					BASE_RESULT::INPUT_VALUE_ERROR	-	несуществующий номер конфигурации.
	 *					BASE_RESULT::ERROR_INIT			-	ошибка инициализации.
	 */
	virtual	BASE_RESULT	reinit			( uint32_t numberCfg = 0 )					= 0;

	/*!
	 * Метод сбрасывает WDT на рабочее значение.
	 */
	virtual	void	reset			( void )										= 0;

	/*!
	 * Метод сбрасывает WDT на сервисное значение.
	 */
	virtual	void	resetService	( void )										= 0;

};

#endif
