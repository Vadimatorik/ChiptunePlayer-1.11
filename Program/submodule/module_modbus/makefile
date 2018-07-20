ifndef MODULE_MODBUS_OPTIMIZATION
	MODULE_MODBUS_OPTIMIZATION = -g3 -O0
endif

MODULE_MODBUS_C_FILE				:= $(shell find module_modbus/sources/modbus/ -maxdepth 10 -type f -name "*.c" )
MODULE_MODBUS_DIR					:= $(shell find module_modbus/sources/modbus/ -maxdepth 10 -type d -name "*" )
MODULE_MODBUS_S_FILE				+= $(STARTUPE_S_NAME)

MODULE_MODBUS_PATH					:= $(addprefix -I, $(MODULE_MODBUS_DIR))

MODULE_MODBUS_OBJ_FILE				:= $(addprefix build/obj/, $(MODULE_MODBUS_C_FILE))
MODULE_MODBUS_OBJ_FILE				:= $(patsubst %.c, %.o, $(MODULE_MODBUS_OBJ_FILE))

build/obj/module_modbus/sources/modbus/%.o:	module_modbus/sources/modbus/%.c
	@echo [CC] $<
	@mkdir -p $(dir $@)
	@$(CC) $(C_FLAGS) $(DEFINE_PROJ) $(USER_CFG_PATH) $(MODULE_MODBUS_PATH) $(MODULE_MODBUS_OPTIMIZATION) -c $< -o $@

# Добавляем к общим переменным проекта.
PROJECT_PATH			+= $(MODULE_MODBUS_PATH)
PROJECT_OBJ_FILE		+= $(MODULE_MODBUS_OBJ_FILE)
