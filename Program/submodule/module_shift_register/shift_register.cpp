#include "shift_register.h"

#ifdef MODULE_SHIFT_REGISTER_ENABLED

void ShiftRegister::init ( void ) {
	this->cfg->st->set( !this->cfg->strobActive );					// Переводим защелку в неактивное положение.
}

// Перезаписать все на выходах.
void ShiftRegister::write ( void ) {
	if ( this->cfg->mutex != nullptr )
        USER_OS_TAKE_MUTEX( *this->cfg->mutex, portMAX_DELAY );

	this->cfg->spiObj->tx( this->cfg->dataArray, this->cfg->arraySize, 10 );

    // Защелкой делаем обновление.
    this->cfg->st->set( this->cfg->strobActive );
    this->cfg->st->set( !this->cfg->strobActive );

    if ( this->cfg->mutex != nullptr)
        USER_OS_GIVE_MUTEX( *this->cfg->mutex );					// Разрешаем использование SPI другим потокам.
}

#endif
