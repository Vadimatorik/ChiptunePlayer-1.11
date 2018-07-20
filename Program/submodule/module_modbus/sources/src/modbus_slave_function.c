#include "globalincludefile.h"
#include "modbus_code_function.h"
#include "modbus_slave_registers.h"
#include "modbus_slave_function.h"



//------------------------------------------------------
// Чтение значений из нескольких регистров хранения
//------------------------------------------------------
uint8_t ModBus_0x03_Read_Registers(struct modbus_slave_registers_map_table *pRegistersMapTable, uint8_t *pRxTxBuff)
{
	// pRegistersMapTable and pRxTxBuff != NULL ?
	if((pRegistersMapTable == NULL) || (pRxTxBuff == NULL)) return 0;
	///	Чтение регистров
	//	Запрос:
	//	+---0---+----1----+-------------------2-------------------+-------------------3-------------------+-----------------------4---------------------+----------------------5----------------------+---6---+---7---+
	//	| Адрес | Функция | Начальный номер регистра старший байт | Начальный номер регистра младший байт | Количество регистров в блоке (старший байт) | Количество регистров в блоке (младший байт) | CRC_L | CRC_H |
	//	+-------+---------+---------------------------------------+---------------------------------------+---------------------------------------------+---------------------------------------------+-------+-------+
	//	Ответ:
	//	+---0---+----1----+---------------2----------------+-------------3------------+-------------4------------+---5---+---6---+
	//	| Адрес | Функция | Количество байт — регистры * 2 | Ррегистра (старший байт) | Ррегистра (младший байт) | CRC_L | CRC_H |
	//	+-------+---------+--------------------------------+--------------------------+--------------------------+-------+-------+

	// Количество регистров в блоке
	uint16_t number_registers = MODBUS_SLAVE_FUNCTION_NUMBER_REGISTERS(pRxTxBuff);
	// Для начала проверим не выходит ли значение за диапазон
	// 0x03 — Величина, содержащаяся в поле данных запроса, является недопустимой величиной
	if(number_registers >= 126) return ModBus_Exception_Response(pRxTxBuff, MB_EX_ILLEGAL_VALUE);

	// Адрес
	uint16_t starting_register_number = MODBUS_SLAVE_FUNCTION_STARTING_REGISTER_NUMBER(pRxTxBuff);
	*(pRxTxBuff + 2 )	= number_registers * 2;

	for(uint8_t counter = 0; counter < number_registers; counter++) {
		MODBUS_SLAVE_FUNCTION_REGISTER_SET(pRxTxBuff, counter, ReadModBusReg(pRegistersMapTable, starting_register_number + counter));
	}

	// return offset
	return (number_registers * 2) + 3;
}

//------------------------------------------------------
// Чтение значений из нескольких регистров ввода
//------------------------------------------------------
uint8_t ModBus_0x04_Read_Input_Registers(struct modbus_slave_registers_map_table *pRegistersMapTable, uint8_t *pRxTxBuff)
{
	// pRegistersMapTable and pRxTxBuff != NULL ?
	if((pRegistersMapTable == NULL) || (pRxTxBuff == NULL)) return 0;
	///	Чтение регистров
	//	Запрос:
	//	+---0---+----1----+-------------------2-------------------+-------------------3-------------------+-----------------------4---------------------+----------------------5----------------------+---6---+---7---+
	//	| Адрес | Функция | Начальный номер регистра старший байт | Начальный номер регистра младший байт | Количество регистров в блоке (старший байт) | Количество регистров в блоке (младший байт) | CRC_L | CRC_H |
	//	+-------+---------+---------------------------------------+---------------------------------------+---------------------------------------------+---------------------------------------------+-------+-------+
	//	Ответ:
	//	+---0---+----1----+---------------2----------------+-------------3------------+-------------4------------+---5---+---6---+
	//	| Адрес | Функция | Количество байт — регистры * 2 | Ррегистра (старший байт) | Ррегистра (младший байт) | CRC_L | CRC_H |
	//	+-------+---------+--------------------------------+--------------------------+--------------------------+-------+-------+

	// Количество регистров в блоке
	uint16_t number_registers = MODBUS_SLAVE_FUNCTION_NUMBER_REGISTERS(pRxTxBuff);
	// Для начала проверим не выходит ли значение за диапазон
	// 0x03 — Величина, содержащаяся в поле данных запроса, является недопустимой величиной
	if(number_registers >= 126) return ModBus_Exception_Response(pRxTxBuff, MB_EX_ILLEGAL_VALUE);
	// Адрес
	uint16_t starting_register_number = MODBUS_SLAVE_FUNCTION_STARTING_REGISTER_NUMBER(pRxTxBuff);

	*(pRxTxBuff + 2 )	= number_registers * 2;

	for(uint8_t counter = 0; counter < number_registers; counter++) {
		MODBUS_SLAVE_FUNCTION_REGISTER_SET(pRxTxBuff, counter, ReadModBusReg(pRegistersMapTable, starting_register_number + counter));
	}

	// return offset
	return (number_registers * 2) + 3;
}

//------------------------------------------------------
// Запись значения в один регистр хранения
//------------------------------------------------------
uint8_t ModBus_0x06_Write_Single_Register(struct modbus_slave_registers_map_table *pRegistersMapTable, uint8_t *pRxTxBuff)
{
	 // pRegistersMapTable and pRxTxBuff != NULL ?
	if((pRegistersMapTable == NULL) || (pRxTxBuff == NULL)) return 0;

	//	Запрос:
	//	+---0---+----1----+---------------2---------------+--------------3----------------+---------------4------------------+---------------5------------------+---6---+---7---+
	//	| Адрес | Функция | Адрес регистра (старший байт) | Адрес регистра (младший байт) | Значение регистра (старший байт) | Значение регистра (младший байт) | CRC_L | CRC_H |
	//	+-------+---------+-------------------------------+-------------------------------+----------------------------------+----------------------------------+-------+-------+

	uint16_t starting_register_number = MODBUS_SLAVE_FUNCTION_STARTING_REGISTER_NUMBER(pRxTxBuff);
	uint16_t value_reg = MODBUS_SLAVE_FUNCTION_NUMBER_REGISTERS(pRxTxBuff);

	WriteModBusReg(pRegistersMapTable, value_reg, starting_register_number);

	value_reg = ReadModBusReg(pRegistersMapTable, starting_register_number);

	*(pRxTxBuff + 4) = RETURN_H(value_reg);
	*(pRxTxBuff + 5) = RETURN_L(value_reg);

	// return offset
	return 6;
}

//------------------------------------------------------
// Запись значений в несколько регистров хранения
//------------------------------------------------------
uint8_t ModBus_0x10_Write_Multiple_Registers(struct modbus_slave_registers_map_table *pRegistersMapTable, uint8_t *pRxTxBuff)
{
	// pRegistersMapTable and pRxTxBuff != NULL ?
	if((pRegistersMapTable == NULL) || (pRxTxBuff == NULL)) return 0;
	///	Запись регистров
	//	Запрос:
	//	+---0---+----1----+-------------------2-------------------+-------------------3-------------------+-----------------------4---------------------+----------------------5----------------------+----------------6---------------+-------------7------------+--------------8-----------+---9---+---10--+
	//	| Адрес | Функция | Начальный номер регистра старший байт | Начальный номер регистра младший байт | Количество регистров в блоке (старший байт) | Количество регистров в блоке (младший байт) | Количество байт — регистры * 2 | Ррегистра (старший байт) | Ррегистра (младший байт) | CRC_L | CRC_H |
	//	+-------+---------+---------------------------------------+---------------------------------------+---------------------------------------------+---------------------------------------------+--------------------------------+--------------------------+--------------------------+-------+-------+
	//	Ответ:
	//	+---0---+----1----+-------------------2-------------------+-------------------3-------------------+-----------------------4---------------------+----------------------5----------------------+---6---+---7---+
	//	| Адрес | Функция | Начальный номер регистра старший байт | Начальный номер регистра младший байт | Количество регистров в блоке (старший байт) | Количество регистров в блоке (младший байт) | CRC_L | CRC_H |
	//	+-------+---------+---------------------------------------+---------------------------------------+---------------------------------------------+---------------------------------------------+-------+-------+

	// Количество регистров в блоке
	uint16_t number_registers = MODBUS_SLAVE_FUNCTION_NUMBER_REGISTERS(pRxTxBuff);
	// Для начала проверим не выходит ли значение за диапазон
	// 0x03 — Величина, содержащаяся в поле данных запроса, является недопустимой величиной
	if(number_registers >= 126) return ModBus_Exception_Response(pRxTxBuff, MB_EX_ILLEGAL_VALUE);
	// Адрес
	uint16_t starting_register_number = MODBUS_SLAVE_FUNCTION_STARTING_REGISTER_NUMBER(pRxTxBuff);

	for(uint8_t counter = 0; counter < number_registers; counter++) {
		WriteModBusReg(pRegistersMapTable, MODBUS_SLAVE_FUNCTION_REGISTER_GET(pRxTxBuff, counter), starting_register_number + counter);
	}

	// return offset
	return 6;
}

// Ответить исключением
//------------------------------------------------------
uint8_t ModBus_Exception_Response(uint8_t *pRxTxBuff, uint8_t Exception)
{
	// pRxTxBuff and pRxTxBuff != NULL ?
	if(pRxTxBuff == NULL) return 0;
	*(pRxTxBuff + 1 ) = Exception; *(pRxTxBuff + 2 ) = Exception;

	// return offset
	return 3;
}




/*
// Чтение значений из нескольких регистров флагов.
//------------------------------------------------------
uint8_t ModBus_0x01_Read_Coils(void);
// Чтение значений из нескольких дискретных входов
//------------------------------------------------------
uint8_t ModBus_0x02_Read_Input_Status(void);
// Запись значения одного флага
//------------------------------------------------------
uint8_t ModBus_0x05_Write_Single_Coil(void);
// Запись значений в несколько регистров флагов
//------------------------------------------------------
uint8_t ModBus_0x0F_Write_Multiple_Coils(void);
*/
