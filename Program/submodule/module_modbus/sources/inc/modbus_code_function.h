#ifndef MODBUS_CODE_FUNCTION
#define MODBUS_CODE_FUNCTION



#ifdef __cplusplus
extern "C" {
#endif



#define RETURN_HIGH(a)              (((a)>>8)&0xff)
#define RETURN_LOW(a)               ((a)&0xff)
#define RETURN_HIGH_AND_LOW(a, b)   (((a) << 8) | (b))

// Function Codes
enum MB_FC {
    MB_FC_READ_COILS        = 0x01, // Read Coils (Output) Status 0xxxx
    MB_FC_READ_INPUT_STAT   = 0x02, // Read Input Status (Discrete Inputs) 1xxxx
    MB_FC_READ_REGS         = 0x03, // Read Holding Registers 4xxxx
    MB_FC_READ_INPUT_REGS   = 0x04, // Read Input Registers 3xxxx
    MB_FC_WRITE_COIL        = 0x05, // Write Single Coil (Output) 0xxxx
    MB_FC_WRITE_REG         = 0x06, // Preset Single Register 4xxxx
    MB_FC_READ_EXCEP_STAT   = 0x07, // Чтение сигналов состояния (Read Exception Status)
    MB_FC_WRITE_COILS       = 0x0F, // Write Multiple Coils (Outputs) 0xxxx
    MB_FC_WRITE_REGS        = 0x10, // Write block of contiguous registers 4xxxx
    MB_FC_MASK_WRITE_REG    = 0x16, // Запись в один регистр хранения с использованием маски «И» и маски «ИЛИ» (Mask Write Register).
    MB_FC_READ_FIFO_QUEUE   = 0x18, // Чтение данных из очереди (Read FIFO Queue)
    MB_FC_READ_FILE_RECORD  = 0x14, // Чтение из файла (Read File Record)
    MB_FC_WRITE_FILE_RECORD = 0x15, // апись в файл (Write File Record)
    MB_FC_DIAGNOSTIC        = 0x08, // Диагностика (Diagnostic)
    MB_FC_GET_COM_EVENT_COUN= 0x0B, // Чтение счетчика событий (Get Com Event Counter)
    MB_FC_GET_COM_EVENT_LOG = 0x0C, // Чтение журнала событий (Get Com Event Log)
    MB_FC_REPORT_SLAVE_ID   = 0x11, // Чтение информации об устройстве (Report Slave ID)
};

// Exception Codes
enum MB_EX {
    MB_EX_ILLEGAL_FUNCTION = 0x01, // Function Code not Supported
    MB_EX_ILLEGAL_ADDRESS  = 0x02, // Output Address not exists
    MB_EX_ILLEGAL_VALUE    = 0x03, // Output Value not in Range
    MB_EX_SLAVE_FAILURE    = 0x04, // Slave Deive Fails to process request
};



#ifdef __cplusplus
}
#endif //cpp



#endif // MODBUS_CODE_FUNCTION
