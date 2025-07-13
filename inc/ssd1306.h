#ifndef SSD1306_H
#define SSD1306_H

#include "hardware/i2c.h"

// Configuração do display
#define SSD1306_HEIGHT 64
#define SSD1306_WIDTH 128

typedef struct {
    uint8_t buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
    uint8_t address;
    i2c_inst_t *i2c;
} ssd1306_t;

// Funções
void ssd1306_init(ssd1306_t *p, uint8_t address, i2c_inst_t *i2c);
void ssd1306_update(ssd1306_t *p);
void ssd1306_clear(ssd1306_t *p);
void ssd1306_draw_pixel(ssd1306_t *p, uint8_t x, uint8_t y, bool color);
void ssd1306_draw_text(ssd1306_t *p, uint8_t x, uint8_t y, const char *text);

#endif // SSD1306_H
