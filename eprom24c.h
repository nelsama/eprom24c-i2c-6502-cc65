/**
 * EPROM.H - Librería para memorias EEPROM serie (24Cxx)
 * 
 * Utiliza la librería I2C genérica para comunicación.
 * Compatible con diferentes tamaños de EEPROM 24Cxx.
 */

#ifndef EPROM_H
#define EPROM_H

#include <stdint.h>
#include "../i2c/i2c.h"

/* Tipos de EEPROM soportados */
typedef enum {
    EPROM_24C01,    /* 128 bytes */
    EPROM_24C02,    /* 256 bytes */
    EPROM_24C04,    /* 512 bytes */
    EPROM_24C08,    /* 1KB */
    EPROM_24C16,    /* 2KB */
    EPROM_24C32,    /* 4KB */
    EPROM_24C64,    /* 8KB */
    EPROM_24C128,   /* 16KB */
    EPROM_24C256    /* 32KB */
} eprom_type_t;

/* Configuración de EEPROM */
typedef struct {
    eprom_type_t type;          /* Tipo de EEPROM */
    uint8_t i2c_address;        /* Dirección I2C (típicamente 0x50) */
    uint16_t size_bytes;        /* Tamaño total en bytes */
    uint8_t page_size;          /* Tamaño de página para escritura */
    uint8_t addr_bytes;         /* Bytes de dirección (1 o 2) */
} eprom_config_t;

/* Códigos de retorno específicos de EPROM */
#define EPROM_SUCCESS           0x00
#define EPROM_ERROR_I2C         0x10
#define EPROM_ERROR_ADDRESS     0x11
#define EPROM_ERROR_SIZE        0x12
#define EPROM_ERROR_PAGE_SIZE   0x13
#define EPROM_ERROR_NOT_READY   0x14

/* Funciones principales */
uint8_t eprom_init(eprom_type_t type, uint8_t i2c_address);
uint8_t eprom_read_byte(uint16_t address, uint8_t *data);
uint8_t eprom_write_byte(uint16_t address, uint8_t data);
uint8_t eprom_read_bytes(uint16_t address, uint8_t *buffer, uint16_t length);
uint8_t eprom_write_bytes(uint16_t address, const uint8_t *data, uint16_t length);
uint8_t eprom_write_page(uint16_t address, const uint8_t *buffer, uint8_t length);

/* Funciones de utilidad */
uint8_t eprom_is_ready(void);
void eprom_wait_ready(void);
uint16_t eprom_get_size(void);
uint8_t eprom_get_page_size(void);

/* Funciones de testing */
uint8_t eprom_test_device(void);
uint8_t eprom_test_read_write(uint16_t address, uint8_t test_value);

#endif /* EPROM_H */