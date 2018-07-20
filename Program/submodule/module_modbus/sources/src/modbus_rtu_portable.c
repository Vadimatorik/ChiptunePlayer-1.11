#include "globalincludefile.h"
#include "modbus_rtu_portable.h"

// Массив значений делителя для UART
uint16_t BR_UART[] = {
   SP_1200_CONST_UART,
    SP_2400_CONST_UART,
    SP_4800_CONST_UART,
    SP_9600_CONST_UART,
    SP_19200_CONST_UART,
    SP_38400_CONST_UART,
    SP_57600_CONST_UART,
    SP_115200_CONST_UART,
    SP_256000_CONST_UART
};

// Массив значений делителя для таймера
uint16_t BR_TIMER[] = {
    SP_1200_CONST_TIMER,
    SP_2400_CONST_TIMER,
    SP_4800_CONST_TIMER,
    SP_9600_CONST_TIMER,
    SP_19200_CONST_TIMER,
    SP_38400_CONST_TIMER,
    SP_57600_CONST_TIMER,
    SP_115200_CONST_TIMER,
    SP_256000_CONST_TIMER
};
