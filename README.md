# Projeto de Controle de LED com Botão usando Raspberry Pi Pico

Este projeto utiliza a função `add_alarm_in_ms()` da ferramenta Pico SDK para controlar um LED RGB conectado ao Raspberry Pi Pico usando um botão. O LED alterna entre vermelho, amarelo e verde, com base no estado atual, cada vez que o botão é pressionado.

## Componentes

- Microcontrolador Raspberry Pi Pico W
- 03 LEDs (vermelho, amarelo e verde)
- 03 Resistores de 330 Ω
- Botão
- Protoboard e cabos jumper

## Conexões

- LED Vermelho: GPIO 11
- LED Verde: GPIO 13
- LED Azul: GPIO 12 (usado para ajustar a cor amarela)
- Botão: GPIO 5

## Funcionamento do Código

O código controla a cor dos LEDs em um semáforo utilizando um botão para iniciar a sequência de mudança de cor. O LED alterna entre vermelho, amarelo e verde com um atraso de 3 segundos entre cada mudança.

### Detalhes do Código

1. O botão inicia a sequência de mudança de cor dos LEDs.
2. O temporizador é ajustado para um atraso de 3 segundos (3.000ms).
3. A mudança de estado dos LEDs é implementada na função de call-back do temporizador, `turn_off_callback()`.
4. A rotina principal imprime a mensagem "Aguardando botão..." a cada 100 ms via porta serial.

### Código


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
