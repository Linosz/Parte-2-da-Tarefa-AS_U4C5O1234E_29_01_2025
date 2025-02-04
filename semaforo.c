#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos pinos dos LEDs
#define RED_LED 11
#define YELLOW_LED 12
#define GREEN_LED 13

// Variável de estado do semáforo
volatile int state = 0;

// Função de callback do temporizador
bool repeating_timer_callback(struct repeating_timer *t) {
    // Desligar todos os LEDs antes de mudar de estado
    gpio_put(RED_LED, 0);
    gpio_put(YELLOW_LED, 0);
    gpio_put(GREEN_LED, 0);

    // Alternar entre os estados do semáforo
    if (state == 0) {
        gpio_put(RED_LED, 1);  // Vermelho aceso
        state = 1;
    } else if (state == 1) {
        gpio_put(YELLOW_LED, 1);  // Amarelo aceso
        state = 2;
    } else {
        gpio_put(GREEN_LED, 1);  // Verde aceso
        state = 0;
    }

    return true;  // Continua repetindo
}

int main() {


    // Configuração dos pinos dos LEDs como saída
    gpio_init(RED_LED);
    gpio_set_dir(RED_LED, GPIO_OUT);
    gpio_init(YELLOW_LED);
    gpio_set_dir(YELLOW_LED, GPIO_OUT);
    gpio_init(GREEN_LED);
    gpio_set_dir(GREEN_LED, GPIO_OUT);

    // Criando um temporizador de 3 segundos para alternar os LEDs
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);
    
    while (1) {
          printf("Sistema funcionando...\n");
          sleep_ms(1000);
    }
    
    return 0;
}
