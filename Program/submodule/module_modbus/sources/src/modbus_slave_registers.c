#include "globalincludefile.h"
#include "modbus_code_function.h"
#include "modbus_slave_registers.h"
#include "modbus_slave_function.h"



//-----------------------------------------------------
// Инициализирует карту таблиц регистров
//-----------------------------------------------------
void ModBus_Slave_Init_Registers_Map_Table(
    // Уникальная карта регистров для каждого экземпляра modbus_slave
    struct modbus_slave_registers_map_table *pRegistersMapTable,
    // Указатель на массив таблиц регистров
    struct modbus_slave_registers_table *pRegistersTable,
    // Число таблиц регистров
    uint8_t NumRegistersTable)
{
    pRegistersMapTable->pRegistersTable = pRegistersTable;
    pRegistersMapTable->NumRegistersTable = NumRegistersTable;
}

//-----------------------------------------------------
// Инициализировать таблицу регистров
//-----------------------------------------------------
void ModBus_Slave_Init_Registers_Table(
    // Таблицы регистров
    struct modbus_slave_registers_table  *pRegistersTable,
    // Указатель на массив ячеек регистров
    uint16_t *pRegistersArray,
    // Доступ к регистрам
    uint8_t  ACCESS,
    // Стартовый адрес
    uint16_t START_ADDR,
    // Размер массива
    uint16_t SIZE_ARR)
{
    // pRegistersTable and pRegistersArray != NULL ?
    if((pRegistersTable == NULL) || (pRegistersArray == NULL)) return;

    pRegistersTable->ACCESS = ACCESS;
    pRegistersTable->START_ADDR = START_ADDR;
    pRegistersTable->SIZE_ARR = SIZE_ARR;
    pRegistersTable->pRegistersArray = pRegistersArray;
}

//-----------------------------------------------------
// Функция записи одного регистра ModBus с проверкой возможности записи
// *pRegmap - экземпляр уникальной карты регистров с которым ведется работа в конкретном случае (их же может быть не один штук)
// DataReg - собственно значение полученного по сети записываемого или запрошенного по сети читаемого регистра ModBus
// Address - ModBus адрес этого регистра (не путать с локальным адресом ячейки памяти)
//------------------------------------------------------
void WriteModBusReg(struct modbus_slave_registers_map_table *pRegistersMapTable, uint16_t DataReg, uint16_t Address)
{
    // pRegistersMapTable != NULL ?
    if(pRegistersMapTable == NULL) return;

    uint8_t i;
    // Перебераем подмассивы
    for(i = 0; i < pRegistersMapTable->NumRegistersTable; i++) {
        // Проверяем в какой подмассив попадаем
        if ((Address >= pRegistersMapTable->pRegistersTable[i].START_ADDR) &&
                (Address <= MODBUS_SLAVE_REGISTERS_FINISH_ADDR(pRegistersMapTable->pRegistersTable[i].START_ADDR, pRegistersMapTable->pRegistersTable[i].SIZE_ARR))) {
            // Теперь проверяем спецификатор доступа (туда писать то разрешено вообще али нет) применив нотацию Йоды
            if (ACCESS_REG_RW == pRegistersMapTable->pRegistersTable[i].ACCESS) {
                // И если все хорошо, (Оружие убери! Плохого не сделаю. Знать хочу - зачем ты здесь? - как завещал Йода)
                // Определяем в какое место в локальном подмассиве надо писать и пишем
                *(pRegistersMapTable->pRegistersTable[i].pRegistersArray + Address - pRegistersMapTable->pRegistersTable[i].START_ADDR) = DataReg;
            } // А ежели регистр попадает в пустышку али доступный только для чтения, ни чего не делаем
        }
    }
}

//-----------------------------------------------------
// Функция поиска заданного регистра ModBus и последующего чтения
// *pRegmap - экземпляр уникальной карты регистров с которым ведется работа в конкретном случае
// Address - ModBus адрес этого регистра (не путать с локальным адресом ячейки памяти)
// возвращаемое значение - требуемое значение регистра ModBus
//------------------------------------------------------
uint16_t ReadModBusReg(struct modbus_slave_registers_map_table *pRegistersMapTable, uint16_t Address)
{
    // pRegistersMapTable != NULL ?
    if(pRegistersMapTable == NULL) return 0;

    uint8_t i;
    // Перебераем подмассивы
    for(i = 0; i < pRegistersMapTable->NumRegistersTable; i++) {
        // Проверяем в какой подмассив попадаем
        if ((Address >= pRegistersMapTable->pRegistersTable[i].START_ADDR) && (Address <= MODBUS_SLAVE_REGISTERS_FINISH_ADDR(pRegistersMapTable->pRegistersTable[i].START_ADDR, pRegistersMapTable->pRegistersTable[i].SIZE_ARR))) {
            // Определяем из какого места в локальном подмассиве надо прочитать и четаем
            return *(pRegistersMapTable->pRegistersTable[i].pRegistersArray+ Address - pRegistersMapTable->pRegistersTable[i].START_ADDR);
        }
    }
    // Если попали в пустышку, то читаем ее как ноль
    return 0;
}
