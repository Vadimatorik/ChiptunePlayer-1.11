#ifndef MODBUS_FUNCTION
#define MODBUS_FUNCTION



#ifdef __cplusplus
extern "C" {
#endif



// Вернуть количество регистров в блоке
#define MODBUS_SLAVE_FUNCTION_NUMBER_REGISTERS(pRX_BUFF)                ((*((pRX_BUFF) + 4) << 8) | *((pRX_BUFF) + 5))
// Вернуть начальный номер регистра
#define MODBUS_SLAVE_FUNCTION_STARTING_REGISTER_NUMBER(pRX_BUFF)        ((*((pRX_BUFF) + 2) << 8) | *((pRX_BUFF) + 3))
// Вернуть регистр
#define MODBUS_SLAVE_FUNCTION_REGISTER_GET(pRX_BUFF, OFFSET)            ((*((pRX_BUFF) + 7 + ((OFFSET) * 2)) << 8) | (*((pRX_BUFF) + 8 + ((OFFSET) * 2))))
// Записать регистр
#define MODBUS_SLAVE_FUNCTION_REGISTER_SET(pTX_BUFF, OFFSET, DATA_REG)  {*(pTX_BUFF + 3 + (OFFSET * 2)) = RETURN_H(DATA_REG); *(pTX_BUFF + 4 + (OFFSET * 2)) = RETURN_L(DATA_REG);}



// Чтение значений из нескольких регистров хранения
//------------------------------------------------------
uint8_t ModBus_0x03_Read_Registers(struct modbus_slave_registers_map_table *pRegistersMapTable, uint8_t *pRxTxBuff);
// Чтение значений из нескольких регистров ввода
//------------------------------------------------------
uint8_t ModBus_0x04_Read_Input_Registers(struct modbus_slave_registers_map_table *pRegistersMapTable, uint8_t *pRxTxBuff);
// Запись значения в один регистр хранения
//------------------------------------------------------
uint8_t ModBus_0x06_Write_Single_Register(struct modbus_slave_registers_map_table *pRegistersMapTable, uint8_t *pRxTxBuff);
// Запись значений в несколько регистров хранения
//------------------------------------------------------
uint8_t ModBus_0x10_Write_Multiple_Registers(struct modbus_slave_registers_map_table *pRegistersMapTable, uint8_t *pRxTxBuff);
// Ответить исключением
//------------------------------------------------------
uint8_t ModBus_Exception_Response(uint8_t *pRxTxBuff, uint8_t Exception);



#ifdef __cplusplus
}
#endif //cpp



#endif // MODBUS_FUNCTION

