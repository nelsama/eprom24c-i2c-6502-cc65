/**
 * EPROM24C_MIN.C - Versión mínima para ahorrar espacio
 * Solo incluye las funciones esenciales
 * 
 * IMPORTANTE: Llamar eprom_init() antes de usar otras funciones
 */

#include "eprom24c.h"

static eprom_config_t current_eprom;

static const uint16_t eprom_sizes[] = {
    128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768
};

static const uint8_t eprom_page_sizes[] = {
    8, 8, 16, 16, 16, 32, 32, 64, 64
};

uint8_t eprom_init(eprom_type_t type, uint8_t i2c_address) {
    current_eprom.type = type;
    current_eprom.i2c_address = i2c_address;
    current_eprom.size_bytes = eprom_sizes[type];
    current_eprom.page_size = eprom_page_sizes[type];
    
    if (type <= EPROM_24C16) {
        current_eprom.addr_bytes = 1;
    } else {
        current_eprom.addr_bytes = 2;
    }
    
    i2c_init();
    
    return EPROM_SUCCESS;
}

uint8_t eprom_read_byte(uint16_t address, uint8_t *data) {
    uint8_t result;
    
    if (address >= current_eprom.size_bytes) {
        return EPROM_ERROR_ADDRESS;
    }
    
    result = i2c_read(current_eprom.i2c_address, address, 
                      current_eprom.addr_bytes, data, 1);
    
    if (result != 1) {
        return EPROM_ERROR_I2C;
    }
    
    return EPROM_SUCCESS;
}

uint8_t eprom_write_byte(uint16_t address, uint8_t data) {
    uint8_t result;
    
    if (address >= current_eprom.size_bytes) {
        return EPROM_ERROR_ADDRESS;
    }
    
    result = i2c_write(current_eprom.i2c_address, address, 
                       current_eprom.addr_bytes, &data, 1);
    
    if (result != 1) {
        return EPROM_ERROR_I2C;
    }
    
    eprom_wait_ready();
    
    return EPROM_SUCCESS;
}

uint8_t eprom_is_ready(void) {
    uint8_t ready;
    ready = i2c_start(current_eprom.i2c_address, I2C_WRITE);
    i2c_stop();
    return ready;
}

void eprom_wait_ready(void) {
    uint8_t timeout = 100;
    uint16_t i;
    
    while (timeout > 0 && !eprom_is_ready()) {
        for (i = 0; i < 1000; i++) {
        }
        timeout--;
    }
}
