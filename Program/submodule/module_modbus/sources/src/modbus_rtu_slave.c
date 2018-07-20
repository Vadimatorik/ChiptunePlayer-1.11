#include "globalincludefile.h"
#include "crc16.h"
#include "modbus_code_function.h"
#include "modbus_slave_registers.h"
#include "modbus_slave_function.h"
#include "modbus_rtu_slave.h"



//-----------------------------------------------------
// Инициализация ModBusRTU_Slave
//-----------------------------------------------------
void ModBusRTU_Slave_Init(
        // Указатель на экземпляр структуры modbus_rtu_slave
        struct modbus_rtu_slave *pModBusRTU_Slave,
        // Указатель на карту полей таблиц регистров
        struct modbus_slave_registers_map_table *pRegistersMapTable,
        // Указатель на приемопередающий буфер
        uint8_t *pRxTxBuff)
{
    // pModBusRTU_Slave, pRegistersMapTable, pHeaders and pRxTxBuff != NULL ?
    if((pModBusRTU_Slave == NULL) || (pRegistersMapTable == NULL) || (pRxTxBuff == NULL)) return;
    pModBusRTU_Slave->Registers_map.pRegistersMapTable = pRegistersMapTable;
    pModBusRTU_Slave->pRxTxBuff = pRxTxBuff;
    return;
}

//-----------------------------------------------------
// Глобальная проверка инициализации структуры struct modbus_rtu_slave
// Возвращает код ошибки
//-----------------------------------------------------
enum ERROR_INIT_MOBDUS_RTU_SLAVE ModBusRTU_Slave_Check_Debug(
        // Указатель на экземпляр структуры modbus_rtu_slave
        struct modbus_rtu_slave *pModBusRTU_Slave)
{
    // pModBusRTU_Slave != NULL ?
    if(pModBusRTU_Slave == NULL) return ERROR_modbus_rtu_slave;

    // pModBusRTU_Slave->FunctionPeriphery.*p  != NULL ?
    if( (pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Disable_Inter_Receiv_Phisic == NULL) ||
        (pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Disable_Inter_Trans_Phisic == NULL) ||
        (pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Enable_Inter_Receiv_Phisic == NULL) ||
        (pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Enable_Inter_Trans_Phisic == NULL) ||
        (pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_RTS1_RX == NULL) ||
        (pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_RTS1_TX == NULL) ||
        (pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Timer_Init == NULL) ||
        (pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Timer_Start == NULL) ||
        (pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Timer_Stop == NULL) ||
        (pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_UART_Init == NULL) ||
        (pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_UART_Write_Phisic == NULL))
        return ERROR_FunctionPeriphery;


    // pModBusRTU_Slave->Registers_map.pRegistersMapTable != NULL ?
    if(pModBusRTU_Slave->Registers_map.pRegistersMapTable == NULL)
        return ERROR_modbus_slave_registers_map_table;

    // pModBusRTU_Slave->Registers_map.pRegistersMapTable->pRegistersTable != NULL ?
    if(pModBusRTU_Slave->Registers_map.pRegistersMapTable->pRegistersTable == NULL)
        return ERROR_modbus_slave_registers_table;

    for(uint8_t counter = 0; counter < pModBusRTU_Slave->Registers_map.pRegistersMapTable->NumRegistersTable; counter++) {
        // psubarray != NULL ?
        if(pModBusRTU_Slave->Registers_map.pRegistersMapTable->pRegistersTable[counter].pRegistersArray == NULL) return ERROR_pRegistersArray;
    }

    // pModBusRTU_Slave->pRxTxBuff != NULL ?
    if(pModBusRTU_Slave->pRxTxBuff == NULL)
        return ERROR_pRxTxBuff;

    return ERROR_NO;
}

//-----------------------------------------------------
// Инициализация адреса и скорости
//-----------------------------------------------------
void ModBusRTU_Slave_Init_Addr_Speed(struct modbus_rtu_slave *pModBusRTU_Slave, uint8_t Addr, uint8_t Speed)
{
    // pModBusRTU_Slave, != NULL ?
    if(pModBusRTU_Slave == NULL) return;

    pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Disable_Inter_Receiv_Phisic();
    pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Disable_Inter_Trans_Phisic();
    // Линия RS-485 на прием
    pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_RTS1_RX();
    pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Timer_Stop();

    //----------------------------
    pModBusRTU_Slave->RxPacket = 0;             // Пакет -> рестарт
    pModBusRTU_Slave->RxByteOffset = 0;         // Указатель на начало буфера
    //----------------------------
    pModBusRTU_Slave->RxTimerBytes.Enable = 0;  // Стоп, 1.5 символа не больше
    pModBusRTU_Slave->RxTimerBytes.Value = 0;   // Текущие значение счетчика
    //----------------------------
    pModBusRTU_Slave->RxTimerFrame.Enable = 0;  // Стоп, 3.5 символа минимум
    pModBusRTU_Slave->RxTimerFrame.Value = 0;   // Текущие значение счетчика
    //----------------------------
    pModBusRTU_Slave->TxTimerBytes.Enable = 0;
    pModBusRTU_Slave->ReadyRxData = 0;          // Нет принятого пакета

    pModBusRTU_Slave->DeviceAddrSpeed.Addr = Addr;
    pModBusRTU_Slave->DeviceAddrSpeed.Speed = Speed;

    pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Timer_Init(Speed);
    pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_UART_Init(Speed);

    pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Enable_Inter_Receiv_Phisic();

    // Выключаем таймер
    pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Timer_Stop();
}

//-----------------------------------------------------
// Прерывание от периферийного модуля по окончанию передачи байта
//-----------------------------------------------------
void ModBusRTU_Slave_InterBytes_Sent(struct modbus_rtu_slave *pModBusRTU_Slave)
{
    if(pModBusRTU_Slave->Counter != pModBusRTU_Slave->ByteNumber)
    {
        // Передача байта
        pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_UART_Write_Phisic(pModBusRTU_Slave->pRxTxBuff[pModBusRTU_Slave->Counter++]);
    }
    else
    {
        // Запретить прерывание по окончанию передачи
        pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Disable_Inter_Trans_Phisic();
        pModBusRTU_Slave->Counter = 0;
        pModBusRTU_Slave->TxTimerBytes.Enable = 1;
        pModBusRTU_Slave->TxTimerBytes.Value = 0;
        // Запустить таймер
        pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Timer_Start();
    }
}

//-----------------------------------------------------
// Прерывание от периферийного модуля по приему байта
//-----------------------------------------------------
void ModBusRTU_Slave_Byte_Read(struct modbus_rtu_slave *pModBusRTU_Slave, uint8_t Data)
{
    if(pModBusRTU_Slave->RxPacket == 1)
    {
        // Выключаем таймер
        pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Timer_Stop();
        // Стоп, 1.5 символа
        pModBusRTU_Slave->RxTimerBytes.Enable = 0;
        // Проверяем значение счетчика с заранее инициализированным значение MODBUSRTUSLAVE_PERIOD_15_MAX
        // Если все Good (=) то записываем данные в буфер, разрешаем 1.5
        // Иначе херим пакет
        if(pModBusRTU_Slave->RxTimerBytes.Value <= MODBUSRTUSLAVE_PERIOD_15_MAX)
        {
            if(pModBusRTU_Slave->RxByteOffset <= SIZE_UART_BUFFER)
            {
                // Записываем байт в буфер
                pModBusRTU_Slave->pRxTxBuff[pModBusRTU_Slave->RxByteOffset++] = Data;
                // Старт таймер, 1.5 символа
                pModBusRTU_Slave->RxTimerBytes.Enable = 1;
                pModBusRTU_Slave->RxTimerBytes.Value = 0;
                // Старт таймер, 3.5 символа
                pModBusRTU_Slave->RxTimerFrame.Enable = 1;
                pModBusRTU_Slave->RxTimerFrame.Value = 0;
                // Включить таймер
                pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Timer_Start();
            }
            else
            {
                // Херим пакет.
                pModBusRTU_Slave->RxPacket = 0;
                // Стоп таймер, 1.5 символа
                pModBusRTU_Slave->RxTimerBytes.Enable = 0;
                pModBusRTU_Slave->RxTimerBytes.Value = 0;
                // Стоп таймер, 3.5 символа
                pModBusRTU_Slave->RxTimerFrame.Enable = 0;
                pModBusRTU_Slave->RxTimerFrame.Value = 0;
                // Устанавливаем флаг убитого фрейма.
                pModBusRTU_Slave->ReadyRxData = 0;
            }
        }
        else
        {
            // Херим пакет.
            pModBusRTU_Slave->RxPacket = 0;
            // Стоп таймер, 1.5 символа
            pModBusRTU_Slave->RxTimerBytes.Enable = 0;
            pModBusRTU_Slave->RxTimerBytes.Value = 0;
            // Стоп таймер, 3.5 символа
            pModBusRTU_Slave->RxTimerFrame.Enable = 0;
            pModBusRTU_Slave->RxTimerFrame.Value = 0;
            // Устанавливаем флаг убитого фрейма.
            pModBusRTU_Slave->ReadyRxData = 0;
        }
    }
    // Принят первый байт Modbus пакета
    else
    {
        // Принят перый байт
        pModBusRTU_Slave->RxPacket     = 1;
        // Указатель на начало буфера
        pModBusRTU_Slave->RxByteOffset = 0;
        // Записываем байт в буфер
        pModBusRTU_Slave->pRxTxBuff[pModBusRTU_Slave->RxByteOffset++] = Data;
        // Старт таймер, 1.5 символа
        pModBusRTU_Slave->RxTimerBytes.Enable = 1;
        pModBusRTU_Slave->RxTimerBytes.Value = 0;
        // Старт таймер, 3.5 символа
        pModBusRTU_Slave->RxTimerFrame.Enable = 1;
        pModBusRTU_Slave->RxTimerFrame.Value = 0;
        // Устанавливаем флаг убитого фрейма.
        pModBusRTU_Slave->ReadyRxData = 0;
        // Включить таймер
        pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Timer_Start();
    }
}

//-----------------------------------------------------
// Прерывание таймера. Щелкает с частотой передачи одного байта
//-----------------------------------------------------
void ModBusRTU_Slave_TimerTic(struct modbus_rtu_slave *pModBusRTU_Slave)
{
    if(pModBusRTU_Slave->RxTimerBytes.Enable == 1)
    {
        pModBusRTU_Slave->RxTimerBytes.Value++;
        if(pModBusRTU_Slave->RxTimerBytes.Value > MODBUSRTUSLAVE_PERIOD_15_MAX)
        {
            // Стоп таймер, 1.5 символа
            pModBusRTU_Slave->RxTimerBytes.Enable = 0;
        }
    }
    if(pModBusRTU_Slave->RxTimerFrame.Enable == 1)
    {
        pModBusRTU_Slave->RxTimerFrame.Value++;
        if(pModBusRTU_Slave->RxTimerFrame.Value >= MODBUSRTUSLAVE_PERIOD_35_MIN)
        {
            // Выключаем таймер
            pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Timer_Stop();
            // Стоп таймер, 3.5 символа
            pModBusRTU_Slave->RxTimerFrame.Enable = 0;
            pModBusRTU_Slave->RxTimerFrame.Value = 0;
            // Проверить адрес
            if((pModBusRTU_Slave->pRxTxBuff[0] == pModBusRTU_Slave->DeviceAddrSpeed.Addr) && (pModBusRTU_Slave->RxByteOffset > 3))
            {
                // Подсчитать контрольную сумму
                // CRC
                uint16_t crc = crc16_block(0xffff, pModBusRTU_Slave->pRxTxBuff, pModBusRTU_Slave->RxByteOffset - 2);
                if(RETURN_HIGH_AND_LOW(pModBusRTU_Slave->pRxTxBuff[pModBusRTU_Slave->RxByteOffset - 1],
                                       pModBusRTU_Slave->pRxTxBuff[pModBusRTU_Slave->RxByteOffset - 2]) == crc)
                {
                    // Устанавливаем флаг принятого фрейма.
                    pModBusRTU_Slave->ReadyRxData = 1;
                    // Принимаем первый байт
                    pModBusRTU_Slave->RxPacket = 0;
                }
                else
                {
                    // Херим пакет.
                    pModBusRTU_Slave->RxPacket = 0;
                }
            }
            else
            {
                // Херим пакет.
                pModBusRTU_Slave->RxPacket = 0;
            }
        }
    }
    // Сделано для того чтобы по прерыванию таймера линию данных установить на прием
    if(pModBusRTU_Slave->TxTimerBytes.Enable == 1)
    {
        if(pModBusRTU_Slave->TxTimerBytes.Value == MODBUSRTUSLAVE_ADD_BYTE_END)
        {
            pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Timer_Stop();
            pModBusRTU_Slave->TxTimerBytes.Enable = 0;
            pModBusRTU_Slave->TxTimerBytes.Value = 0;
            // Разрешить прерывание по приему байта
            pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Enable_Inter_Receiv_Phisic();
            pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_RTS1_RX();
        }
        pModBusRTU_Slave->TxTimerBytes.Value++;
    }
}

//-----------------------------------------------------
// Передача массива байт
//-----------------------------------------------------
void ModBusRTU_Slave_Byte_Write(struct modbus_rtu_slave *pModBusRTU_Slave, uint8_t ByteNumber)
{
    pModBusRTU_Slave->Counter = 0;
    pModBusRTU_Slave->ByteNumber = ByteNumber + 2; // + 2 bute CRC
    // Подсчитать контрольную сумму
    uint16_t crc = crc16_block(0xffff, pModBusRTU_Slave->pRxTxBuff, ByteNumber);

    *(pModBusRTU_Slave->pRxTxBuff + ByteNumber + 1) = RETURN_HIGH(crc);
    *(pModBusRTU_Slave->pRxTxBuff + ByteNumber) = RETURN_LOW(crc);

    pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_RTS1_TX();


    // Передача байта
    pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_UART_Write_Phisic(pModBusRTU_Slave->pRxTxBuff[pModBusRTU_Slave->Counter++]);

    // Разрешить прерывание по окончанию передачи байта
    pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Enable_Inter_Trans_Phisic();
}

//-----------------------------------------------------
// Служба MODBUS RTU
//-----------------------------------------------------
void ModBusRTU_Slave_Service(struct modbus_rtu_slave *pModBusRTU_Slave)
{
    // pModBusRTU_Slave != NULL ?
    if(pModBusRTU_Slave == NULL) return;

    // Выйти если нет новых данных
    if(pModBusRTU_Slave->ReadyRxData != 1) return;

    // Запретить прерывание по приему байта
    pModBusRTU_Slave->FunctionPeriphery.pModBusRTU_Slave_Disable_Inter_Receiv_Phisic();
    // Сбросить флаг - пакет принят
    pModBusRTU_Slave->ReadyRxData = 0;

    uint8_t offset = 0;

    switch(*(pModBusRTU_Slave->pRxTxBuff + 1)) {
        case MB_FC_READ_COILS: {
            offset = ModBus_Exception_Response(pModBusRTU_Slave->pRxTxBuff, MB_EX_ILLEGAL_FUNCTION);
            break;
        }
        case MB_FC_READ_INPUT_STAT: {
            offset = ModBus_Exception_Response(pModBusRTU_Slave->pRxTxBuff, MB_EX_ILLEGAL_FUNCTION);
            break;
        }
        case MB_FC_READ_REGS:
        case MB_FC_READ_INPUT_REGS: {
            offset = ModBus_0x03_Read_Registers(pModBusRTU_Slave->Registers_map.pRegistersMapTable, pModBusRTU_Slave->pRxTxBuff);
            break;
        }
        case MB_FC_WRITE_COIL: break;
        case MB_FC_WRITE_REG: {
            offset = ModBus_0x06_Write_Single_Register(pModBusRTU_Slave->Registers_map.pRegistersMapTable, pModBusRTU_Slave->pRxTxBuff);
            break;
        }
        case MB_FC_READ_EXCEP_STAT: {
            offset = ModBus_Exception_Response(pModBusRTU_Slave->pRxTxBuff, MB_EX_ILLEGAL_FUNCTION);
            break;
        }
        case MB_FC_WRITE_COILS: {
            offset = ModBus_Exception_Response(pModBusRTU_Slave->pRxTxBuff, MB_EX_ILLEGAL_FUNCTION);
            break;
        }
        case MB_FC_WRITE_REGS: {
            offset = ModBus_0x10_Write_Multiple_Registers(pModBusRTU_Slave->Registers_map.pRegistersMapTable, pModBusRTU_Slave->pRxTxBuff);
            break;
        }
        case MB_FC_MASK_WRITE_REG: {
            offset = ModBus_Exception_Response(pModBusRTU_Slave->pRxTxBuff, MB_EX_ILLEGAL_FUNCTION);
            break;
        }
        case MB_FC_READ_FIFO_QUEUE: {
            offset = ModBus_Exception_Response(pModBusRTU_Slave->pRxTxBuff, MB_EX_ILLEGAL_FUNCTION);
            break;
        }
        case MB_FC_READ_FILE_RECORD: {
            offset = ModBus_Exception_Response(pModBusRTU_Slave->pRxTxBuff, MB_EX_ILLEGAL_FUNCTION);
            break;
        }
        case MB_FC_WRITE_FILE_RECORD: {
            offset = ModBus_Exception_Response(pModBusRTU_Slave->pRxTxBuff, MB_EX_ILLEGAL_FUNCTION);
            break;
        }
        case MB_FC_DIAGNOSTIC: {
            offset = ModBus_Exception_Response(pModBusRTU_Slave->pRxTxBuff, MB_EX_ILLEGAL_FUNCTION);
            break;
        }
        case MB_FC_GET_COM_EVENT_COUN: {
            offset = ModBus_Exception_Response(pModBusRTU_Slave->pRxTxBuff, MB_EX_ILLEGAL_FUNCTION);
            break;
        }
        case MB_FC_GET_COM_EVENT_LOG: {
            offset = ModBus_Exception_Response(pModBusRTU_Slave->pRxTxBuff, MB_EX_ILLEGAL_FUNCTION);
            break;
        }
        case MB_FC_REPORT_SLAVE_ID: {
            offset = ModBus_Exception_Response(pModBusRTU_Slave->pRxTxBuff, MB_EX_ILLEGAL_FUNCTION);
            break;
        }

        default: {
            offset = ModBus_Exception_Response(pModBusRTU_Slave->pRxTxBuff, MB_EX_ILLEGAL_FUNCTION);
        }
    }
    // Write data
    ModBusRTU_Slave_Byte_Write(pModBusRTU_Slave, offset);
}
