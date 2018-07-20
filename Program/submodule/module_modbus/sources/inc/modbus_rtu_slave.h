#ifndef MODBUS_RTU_SLAVE
#define MODBUS_RTU_SLAVE

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Размер входного/выходного буфера
#define SIZE_UART_BUFFER	255

#define MODBUSRTUSLAVE_PERIOD_15_MAX    2
#define MODBUSRTUSLAVE_PERIOD_35_MIN    4
#define MODBUSRTUSLAVE_ADD_BYTE_END     0 // 0 + 1

//-----------------------------------------------------
// Буфер передатчика/приемника
//-----------------------------------------------------
typedef struct {
    uint8_t byte[SIZE_UART_BUFFER];
}MODBUSBUFFER;

// Коды ошибок инициализации
enum ERROR_INIT_MOBDUS_RTU_SLAVE {
    ERROR_NO = 0,
    ERROR_modbus_rtu_slave = 1,
    ERROR_FunctionPeriphery = 2,
    ERROR_modbus_slave_registers_map_table = 3,
    ERROR_modbus_slave_registers_table = 4,
    ERROR_pRegistersArray = 5,
    ERROR_pRxTxBuff = 6
};

/*
 * Нужно реализовать в phisic.
 * Код функций расположен в файле phisic.c
 * Все эти функции содержат регистры контроллера
 * */
struct modbus_rtu_slave_function {
    // Инициализация UART
    void (*pModBusRTU_Slave_UART_Init)(uint8_t Speed);
    // Разрешить прерывание по окончанию передачи
    void (*pModBusRTU_Slave_Enable_Inter_Trans_Phisic)(void);
    // Запретить прерывание по окончанию передачи
    void (*pModBusRTU_Slave_Disable_Inter_Trans_Phisic)(void);
    // Разрешить прерывание по приему байта
    void (*pModBusRTU_Slave_Enable_Inter_Receiv_Phisic)(void);
    // Запретить прерывание по приему байта
    void (*pModBusRTU_Slave_Disable_Inter_Receiv_Phisic)(void);
    // Передача байта
    void (*pModBusRTU_Slave_UART_Write_Phisic)(uint8_t Data);

    // Инициализация Таймера
    void (*pModBusRTU_Slave_Timer_Init)(uint8_t Speed);
    // Запустить таймер
    void (*pModBusRTU_Slave_Timer_Start)(void);
    // Стоп таймер
    void (*pModBusRTU_Slave_Timer_Stop)(void);

    // Направление линии на прием
    void (*pModBusRTU_Slave_RTS1_RX)(void);
    // Направление линии на передачу
    void (*pModBusRTU_Slave_RTS1_TX)(void);
};

//-----------------------------------------------------
// Структура данных ModBusRTU_Slave стека
//-----------------------------------------------------
struct modbus_rtu_slave {
    //----------------------------------------------------
    // Структура адрес и скорость устройства
    //----------------------------------------------------
    struct {
        uint8_t Addr,   // Адрес устройства
                Speed;  // Скорость устройства
    }DeviceAddrSpeed;
    //----------------------------------------------------
    // Если обнаружен разрыв между символами длительностью
    // более 1,5 времени передачи одного символа, то
    // приемник должен поставить «черную метку»
    //----------------------------------------------------
    struct {
        uint8_t	Enable, // Разрешаем отсчет времени
                Value;  // Текущие значение счетчика
    }RxTimerBytes;
    //----------------------------------------------------
    // В Modbus RTU разделителем сообщений (фреймов,
    // содержащих запросы и ответы) является тишина на
    // линии в течение не менее 3,5 символов. С учетом того,
    // что стандартный символ передается УАРТом 11-ю битами
    //----------------------------------------------------
    struct {
        uint8_t	Enable, // Разрешаем отсчет времени
                Value;  // Текущие значение счетчика
    }RxTimerFrame;
    //----------------------------------------------------
    // Сделано для того чтобы по прерыванию таймера
    // линию данных установить на прием c задержкой
    //----------------------------------------------------
    struct {
        uint8_t	Enable; // Разрешаем отсчет времени
        uint8_t	Value;  // Текущие значение счетчика
    }TxTimerBytes;
    //----------------------------------------------------
    // Уникальные функции периферийного контроллера
    //----------------------------------------------------
    struct modbus_rtu_slave_function FunctionPeriphery;
    //----------------------------------------------------
    // Уникальная карта регистров
    //----------------------------------------------------
    struct {
        // Указатель на карту полей таблиц регистров
        struct modbus_slave_registers_map_table *pRegistersMapTable;
    }Registers_map;
    //----------------------------------------------------
    //
    //----------------------------------------------------
    uint8_t	RxPacket,       // Устанавливается 1-ца если принимаем следующие байты, 0 принимаем первый байт
            RxByteOffset,   // Указатель буфера
            ReadyRxData,    // Флаг принятого фрейма
            Counter,        // Счетчик переданных байт
            ByteNumber;     // Сколько нужно передать байт
    // Указатель на приемопередающий буфер
    uint8_t *pRxTxBuff;
};



// Инициализация ModBusRTU_Slave
//-----------------------------------------------------
void ModBusRTU_Slave_Init(
    // Указатель на экземпляр структуры modbus_rtu_slave
    struct modbus_rtu_slave *pModBusRTU_Slave,
    // Указатель на карту полей таблиц регистров
    struct modbus_slave_registers_map_table *pRegistersMapTable,
    // Указатель на приемопередающий буфер
    uint8_t *pRxTxBuff);

// Глобальная проверка инициализации структуры struct modbus_rtu_slave
// Возвращает код ошибки
//-----------------------------------------------------
enum ERROR_INIT_MOBDUS_RTU_SLAVE ModBusRTU_Slave_Check_Debug(
        // Указатель на экземпляр структуры modbus_rtu_slave
        struct modbus_rtu_slave *pModBusRTU_Slave);

// Инициализация адреса и скорости
//-----------------------------------------------------
void ModBusRTU_Slave_Init_Addr_Speed(struct modbus_rtu_slave *pModBusRTU_Slave, uint8_t Addr, uint8_t Speed);

// Прерывание от периферийного модуля по окончанию передачи байта
//-----------------------------------------------------
void ModBusRTU_Slave_InterBytes_Sent(struct modbus_rtu_slave *pModBusRTU_Slave);

// Прерывание от периферийного модуля по приему байта
//-----------------------------------------------------
void ModBusRTU_Slave_Byte_Read(struct modbus_rtu_slave *pModBusRTU_Slave, uint8_t Data);

// Прерывание таймера. Щелкает с частотой передачи одного байта
//-----------------------------------------------------
void ModBusRTU_Slave_TimerTic(struct modbus_rtu_slave *pModBusRTU_Slave);

// Передача массива байт
//-----------------------------------------------------
void ModBusRTU_Slave_Byte_Write(struct modbus_rtu_slave *pModBusRTU_Slave, uint8_t ByteNumber);

// Служба MODBUS RTU
//-----------------------------------------------------
void ModBusRTU_Slave_Service(struct modbus_rtu_slave *pModBusRTU_Slave);



#ifdef __cplusplus
}
#endif //cpp



#endif // MODBUS_RTU_SLAVE

