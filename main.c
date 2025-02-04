#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"

#define LED_RED 11
#define LED_GREEN 13
#define LED_BLUE 12
#define BUTTON 5

volatile bool button_pressed = false;
volatile int state = 0;

bool turn_off_callback(struct repeating_timer *t) {
    if (state == 0) {
        gpio_put(LED_RED, 1);
        gpio_put(LED_GREEN, 0);
        gpio_put(LED_BLUE, 0);
    } else if (state == 1) {
        gpio_put(LED_RED, 1);
        gpio_put(LED_GREEN, 1);
        gpio_put(LED_BLUE, 0); // Amarelo (vermelho + verde)
    } else if (state == 2) {
        gpio_put(LED_RED, 0);
        gpio_put(LED_GREEN, 1);
        gpio_put(LED_BLUE, 0);
    } else {
        gpio_put(LED_RED, 0);
        gpio_put(LED_GREEN, 0);
        gpio_put(LED_BLUE, 0);
        button_pressed = false;
        state = 0; // Reinicia a sequência
        return false;
    }
    
    state++;
    add_alarm_in_ms(3000, turn_off_callback, NULL, false);
    return true;
}

void button_callback(uint gpio, uint32_t events) {
    if (!button_pressed) {
        button_pressed = true;
        state = 0; // Garante que a sequência reinicie corretamente
        add_alarm_in_ms(3000, turn_off_callback, NULL, false);
    }
}

int main() {
    stdio_init_all();
    
    gpio_init(LED_RED);
    gpio_init(LED_GREEN);
    gpio_init(LED_BLUE);
    gpio_init(BUTTON);
    
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);
    
    gpio_set_irq_enabled_with_callback(BUTTON, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    
    while (1) {
        printf("Aguardando botão...\n");
        sleep_ms(100);
    }
}
