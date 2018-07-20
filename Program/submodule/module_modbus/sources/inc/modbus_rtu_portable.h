#ifndef MODBUS_RTU_PORTABLE
#define MODBUS_RTU_PORTABLE



#ifdef __cplusplus
extern "C" {
#endif



// Частота работы МК
#define FCY                 8000000
// Частота инкрементирования счетчика таймера
#define FREQ_INCR_TIMER     1152000
// Скорости
#define SP_1200     0x00
#define SP_2400     0x01
#define SP_4800     0x02
#define SP_9600     0x03
#define SP_19200    0x04
#define SP_38400    0x05
#define SP_57600    0x06
#define SP_115200   0x07
#define SP_256000   0x08
// Количество бит передаваемых по UART
#define SP_UART_LEGGTH_BYTE 10
// Формула расчета скорости работы UART
// SP_FCY - частота тактирования модуля UART
// SP_BUD - скорость передачи данных BOD
#define SP_MACRO_UART(SP_FCY, SP_BUD) (((SP_FCY) + ((SP_BUD) / 2)) / (SP_BUD))
// Формула расчета счетчика таймера. Прерывание таймера должно срабатывать с частотой передачи байта
// SP_FREQ_INCR_TIMER - частота тактирования модуля TIMER
// SP_BUD - скорость передачи данных BOD
#define SP_MACRO_FREQ_INCR_TIMER(SP_FREQ_INCR_TIMER, SP_BUD) \
    (((SP_FREQ_INCR_TIMER) / ((SP_BUD) / (SP_UART_LEGGTH_BYTE))))

#define SP_1200_CONST_UART       SP_MACRO_UART(FCY, 1200)
#define SP_2400_CONST_UART       SP_MACRO_UART(FCY, 2400)
#define SP_4800_CONST_UART       SP_MACRO_UART(FCY, 4800)
#define SP_9600_CONST_UART       SP_MACRO_UART(FCY, 9600)
#define SP_19200_CONST_UART      SP_MACRO_UART(FCY, 19200)
#define SP_38400_CONST_UART      SP_MACRO_UART(FCY, 38400)
#define SP_57600_CONST_UART      SP_MACRO_UART(FCY, 57600)
#define SP_115200_CONST_UART     SP_MACRO_UART(FCY, 115200)
#define SP_256000_CONST_UART     SP_MACRO_UART(FCY, 256000)

#define SP_1200_CONST_TIMER         SP_MACRO_FREQ_INCR_TIMER(FREQ_INCR_TIMER, 1200)
#define SP_2400_CONST_TIMER         SP_MACRO_FREQ_INCR_TIMER(FREQ_INCR_TIMER, 2400)
#define SP_4800_CONST_TIMER         SP_MACRO_FREQ_INCR_TIMER(FREQ_INCR_TIMER, 4800)
#define SP_9600_CONST_TIMER         SP_MACRO_FREQ_INCR_TIMER(FREQ_INCR_TIMER, 9600)
#define SP_19200_CONST_TIMER        SP_MACRO_FREQ_INCR_TIMER(FREQ_INCR_TIMER, 19200)
#define SP_38400_CONST_TIMER        SP_MACRO_FREQ_INCR_TIMER(FREQ_INCR_TIMER, 38400)
#define SP_57600_CONST_TIMER        SP_MACRO_FREQ_INCR_TIMER(FREQ_INCR_TIMER, 57600)
#define SP_115200_CONST_TIMER       SP_MACRO_FREQ_INCR_TIMER(FREQ_INCR_TIMER, 115200)
#define SP_256000_CONST_TIMER       SP_MACRO_FREQ_INCR_TIMER(FREQ_INCR_TIMER, 256000)

// Массив значений делителя для UART
extern uint16_t BR_UART[];
// Массив значений делителя для таймера
extern uint16_t BR_TIMER[];

/*
// Заготовки для функций

void ModBusRTU_Slave_UART_Init();                       // Инициализация UART
void ModBusRTU_Slave_Enable_Inter_Trans_Phisic(void);   // Разрешить прерывание по окончанию передачи
void ModBusRTU_Slave_Disable_Inter_Trans_Phisic(void);  // Запретить прерывание по окончанию передачи
void ModBusRTU_Slave_Enable_Inter_Receiv_Phisic(void);  // Разрешить прерывание по приему байта
void ModBusRTU_Slave_Disable_Inter_Receiv_Phisic(void); // Запретить прерывание по приему байта
void ModBusRTU_Slave_UART_Write_Phisic(uint8_t Data);   // Передача байта

void ModBusRTU_Slave_Timer_Init(void);                  // Инициализация Таймера
void ModBusRTU_Slave_Timer_Start(void);                 // Запустить таймер
void ModBusRTU_Slave_Timer_Stop(void);                  // Стоп таймер
*/



#ifdef __cplusplus
}
#endif //cpp



#endif // MODBUS_RTU_PORTABLE

