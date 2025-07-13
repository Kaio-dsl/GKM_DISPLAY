#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

// Configuração do I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO 0x3C

// Configuração dos pinos do joystick
#define PINO_JOYSTICK_X 26  // ADC0
#define PINO_JOYSTICK_Y 27  // ADC1

int main() {
    // Inicializa o stdio (USB output opcional)
    stdio_init_all();

    // Inicializa I2C a 400 kHz
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa ADC
    adc_init();
    adc_gpio_init(PINO_JOYSTICK_X);
    adc_gpio_init(PINO_JOYSTICK_Y);

    // Inicializa display
    ssd1306_t ssd;
    ssd1306_init(&ssd, ENDERECO, I2C_PORT);
    ssd1306_clear(&ssd);
    ssd1306_update(&ssd);

    char texto[32];

    while (true) {
        // Lê ADC X
        adc_select_input(0);
        uint16_t leitura_x = adc_read();
        float tensao_x = (3.3f * leitura_x) / 4095;

        // Lê ADC Y
        adc_select_input(1);
        uint16_t leitura_y = adc_read();
        float tensao_y = (3.3f * leitura_y) / 4095;

        // Limpa display
        ssd1306_clear(&ssd);

        // Escreve tensões
        snprintf(texto, sizeof(texto), "X = %.2f V", tensao_x);
        ssd1306_draw_text(&ssd, 0, 0, texto);

        snprintf(texto, sizeof(texto), "Y = %.2f V", tensao_y);
        ssd1306_draw_text(&ssd, 0, 12, texto);

        ssd1306_draw_text(&ssd, 0, 30, "Joystick Analogico");

        // Atualiza display
        ssd1306_update(&ssd);

        sleep_ms(500);
    }
}