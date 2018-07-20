#pragma once

#ifdef MODULE_CPP_SYSTEM_CALLS_DUMMY_ENABLED

// Перечень используемых МК.
#ifdef STM32F103xB
#include "stm32f103xb.h"
#endif

#ifdef STM32F205xx
#include "stm32f205xx.h"
#endif

#ifdef STM32F207xx
#include "stm32f207xx.h"
#endif

#ifdef STM32F215xx
#include "stm32f215xx.h"
#endif

#ifdef STM32F217xx
#include "stm32f217xx.h"
#endif

#ifdef STM32F407xx
#include "stm32f407xx.h"
#endif

#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

#endif
