#include <string.h>
#include "ssd1306.h"
#include "font.h"
#include "pico/stdlib.h"

#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0x40

static void ssd1306_command(ssd1306_t *p, uint8_t cmd) {
    uint8_t buf[2] = {SSD1306_COMMAND, cmd};
    i2c_write_blocking(p->i2c, p->address, buf, 2, false);
}

void ssd1306_init(ssd1306_t *p, uint8_t address, i2c_inst_t *i2c) {
    p->address = address;
    p->i2c = i2c;

    // Inicialização do display (comandos padrão)
    sleep_ms(100);
    ssd1306_command(p, 0xAE); // Display OFF
    ssd1306_command(p, 0xA8); // Multiplex ratio
    ssd1306_command(p, 0x3F);
    ssd1306_command(p, 0xD3); // Display offset
    ssd1306_command(p, 0x00);
    ssd1306_command(p, 0x40); // Start line
    ssd1306_command(p, 0xA1); // Segment re-map
    ssd1306_command(p, 0xC8); // COM scan dir
    ssd1306_command(p, 0xDA); // COM pins
    ssd1306_command(p, 0x12);
    ssd1306_command(p, 0x81); // Contrast
    ssd1306_command(p, 0x7F);
    ssd1306_command(p, 0xA4); // Entire display ON
    ssd1306_command(p, 0xA6); // Normal display
    ssd1306_command(p, 0xD5); // Clock divide
    ssd1306_command(p, 0x80);
    ssd1306_command(p, 0x8D); // Charge pump
    ssd1306_command(p, 0x14);
    ssd1306_command(p, 0xAF); // Display ON
    ssd1306_clear(p);
    ssd1306_update(p);
}

void ssd1306_update(ssd1306_t *p) {
    for (uint8_t page = 0; page < 8; page++) {
        ssd1306_command(p, 0xB0 + page);
        ssd1306_command(p, 0x00);
        ssd1306_command(p, 0x10);

        uint8_t buf[SSD1306_WIDTH + 1];
        buf[0] = SSD1306_DATA;
        memcpy(&buf[1], &p->buffer[SSD1306_WIDTH * page], SSD1306_WIDTH);
        i2c_write_blocking(p->i2c, p->address, buf, SSD1306_WIDTH + 1, false);
    }
}

void ssd1306_clear(ssd1306_t *p) {
    memset(p->buffer, 0, sizeof(p->buffer));
}

void ssd1306_draw_pixel(ssd1306_t *p, uint8_t x, uint8_t y, bool color) {
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
        return;
    if (color)
        p->buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y % 8));
    else
        p->buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
}

void ssd1306_draw_text(ssd1306_t *p, uint8_t x, uint8_t y, const char *text) {
    while (*text) {
        char c = *text++;
        for (uint8_t i = 0; i < 5; i++) {
            uint8_t line = font[c - 32][i];
            for (uint8_t j = 0; j < 8; j++) {
                bool pixel = line & (1 << j);
                ssd1306_draw_pixel(p, x + i, y + j, pixel);
            }
        }
        x += 6;
    }
}
