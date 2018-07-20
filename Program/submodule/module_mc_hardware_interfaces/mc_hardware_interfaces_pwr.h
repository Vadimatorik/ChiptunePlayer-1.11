#pragma once

#ifdef __cplusplus

#include "mc_hardware_interfaces_base.h"

/*!
 *
 */
class PwrBase {
public:
	/*!
	 * Сбрасывает текущие настройки Pwr и инициализирует его заново
	 * согласно выбранному режиму.
	 *
	 * \param[in]	numberCfg		-	выбранный режим.
	 *
	 * \return		{	BASE_RESULT::OK					-	инициализация прошла успешно.
	 *					BASE_RESULT::INPUT_VALUE_ERROR	-	несуществующий номер конфигурации.
	 */
	virtual	BASE_RESULT	reinit			( uint32_t numberCfg = 0 )				= 0;

	/*!
	 * Запускает контроль за напряжением питания согласно конфигурации.
	 */
	virtual	void		pvdEnable		( void )								= 0;

	/*!
	 * Отключает контроль за напряжением питания согласно конфигурации.
	 */
	virtual	void		pvdDisable		( void )								= 0;
};

#endif
