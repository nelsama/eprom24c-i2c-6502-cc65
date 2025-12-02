# Librería EPROM 24C para 6502

## Descripción

Librería para memorias EEPROM serie 24C (24C01 a 24C256). Utiliza la librería I2C genérica para la comunicación.

## Archivos

- `eprom24c.h` - Definiciones y prototipos
- `eprom24c.c` - Implementación

## Dependencias

- `../i2c/i2c.h` - Librería I2C genérica (requerida)

## Tipos de EEPROM Soportados

```c
typedef enum {
    EPROM_24C01,    // 128 bytes, 1 byte addr
    EPROM_24C02,    // 256 bytes, 1 byte addr  
    EPROM_24C04,    // 512 bytes, 1 byte addr
    EPROM_24C08,    // 1KB, 1 byte addr
    EPROM_24C16,    // 2KB, 1 byte addr
    EPROM_24C32,    // 4KB, 2 bytes addr
    EPROM_24C64,    // 8KB, 2 bytes addr
    EPROM_24C128,   // 16KB, 2 bytes addr
    EPROM_24C256    // 32KB, 2 bytes addr
} eprom_type_t;
```

## Funciones

### Inicialización
```c
uint8_t eprom_init(eprom_type_t type, uint8_t i2c_address);
```
Configura tipo de EEPROM y dirección I2C. Llama `i2c_init()` internamente.

**Parámetros:**
- `type`: Tipo de EEPROM (ej: `EPROM_24C64`)
- `i2c_address`: Dirección I2C (típicamente `0x50`)

**Retorno:** `EPROM_SUCCESS` (0) siempre

### Lectura
```c
uint8_t eprom_read_byte(uint16_t address, uint8_t *data);
```
Lee un byte de la dirección especificada.

**Parámetros:**
- `address`: Dirección de memoria
- `data`: Puntero donde guardar el byte leído

**Retorno:** `EPROM_SUCCESS` (0) o código de error

### Escritura
```c
uint8_t eprom_write_byte(uint16_t address, uint8_t data);
```
Escribe un byte. Espera automáticamente a que termine la escritura.

**Parámetros:**
- `address`: Dirección de memoria
- `data`: Byte a escribir

**Retorno:** `EPROM_SUCCESS` (0) o código de error

### Utilidades
```c
uint8_t eprom_is_ready(void);    // Retorna 1 si EEPROM lista, 0 si ocupada
void eprom_wait_ready(void);     // Espera hasta que EEPROM esté lista
```

## Códigos de Retorno

| Código | Nombre | Descripción |
|--------|--------|-------------|
| 0x00 | `EPROM_SUCCESS` | Operación exitosa |
| 0x10 | `EPROM_ERROR_I2C` | Error de comunicación I2C |
| 0x11 | `EPROM_ERROR_ADDRESS` | Dirección fuera de rango |
| 0x12 | `EPROM_ERROR_SIZE` | Error de tamaño |
| 0x13 | `EPROM_ERROR_PAGE_SIZE` | Error de tamaño de página |
| 0x14 | `EPROM_ERROR_NOT_READY` | EEPROM no lista |

## Ejemplo de Uso

```c
#include "../libs/eprom24c/eprom24c.h"

int main(void) {
    uint8_t result;
    uint8_t data;
    
    // Inicializar EPROM 24C64 en dirección 0x50
    eprom_init(EPROM_24C64, 0x50);
    
    // Verificar que responde
    if (eprom_is_ready()) {
        // EEPROM presente y lista
    }
    
    // Leer byte de dirección 0x0000
    result = eprom_read_byte(0x0000, &data);
    if (result == EPROM_SUCCESS) {
        // data contiene el valor leído
    }
    
    // Escribir byte (espera automática)
    result = eprom_write_byte(0x0000, 0x77);
    if (result == EPROM_SUCCESS) {
        // Escritura completada
    }
    
    return 0;
}
```

## Notas Importantes

1. **Llamar `eprom_init(type, addr)` al inicio** - Configura tipo y dirección
2. `eprom_write_byte()` ya incluye espera automática
3. El tipo determina automáticamente si usa 1 o 2 bytes de dirección

## Compatibilidad

- ✅ cc65 compiler
- ✅ C89 estándar
- ✅ Usa librería I2C polling
- ✅ 6502/65C02