#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_WWDG_MODULE_ENABLED

#include "mc_hardware_interfaces_wdt.h"
#include "user_os.h"

struct wdtCfg {
	const uint8_t	taskPrio;				// Приоритет задачи, сбрасывающий wdt.
	const uint32_t	runTimeMs;				// Время перезагрузки по сторожевому таймеру.
											// при номинальном режиме работы системы.
	const uint32_t	startupTimeMs;			// Время перезагрузки по сторожевому таймеру при запуске системы.
	const uint32_t	serviceTimeMs;			// Время перезагрузки по сторожевому таймеру
											// во время сервисных операций (например, стирание и перезапись flash).
};

class Wdt : public WdtBase {
public:
	Wdt ( const wdtCfg* const cfg, const uint32_t countCfg )
		: cfg( cfg ), countCfg( countCfg ),
		nowCfg( 0 ) {}

	BASE_RESULT		reinit			( uint32_t numberCfg = 0 );
	void			reset			( void );
	void			resetService	( void );

private:
	const wdtCfg*							const cfg;
	const uint32_t							countCfg;

	uint32_t								nowCfg;

	static void task ( void* obj );

	USER_OS_STATIC_STACK_TYPE				taskStack[ 64 ] = { 0 };
	USER_OS_STATIC_TASK_STRUCT_TYPE			taskStruct;

	uint8_t									reboot;
};

#endif

#endif
