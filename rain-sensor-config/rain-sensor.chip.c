#include "wokwi-api.h"   // Inclui a API do Wokwi para simulação de hardware
#include <stdio.h>       // Inclui a biblioteca padrão para entrada e saída
#include <stdlib.h>      // Inclui a biblioteca para funções de alocação de memória

// Estrutura que define o estado do chip
typedef struct {
  pin_t pin_vcc;       // Pino para alimentação (VCC)
  pin_t pin_gnd;       // Pino para terra (GND)
  pin_t pin_ao;        // Pino para leitura analógica (AO)
  pin_t pin_do;        // Pino para leitura digital (DO)
  uint32_t rain_attr;  // Atributo que representa a quantidade de chuva
} chip_state_t;

// Declaração da função para o evento do timer
static void chip_timer_event(void *user_data);

// Função para inicializar o chip
void chip_init(void) {
  chip_state_t *chip = malloc(sizeof(chip_state_t)); // Aloca memória para o estado do chip

  // Inicializa os pinos do chip
  chip->pin_ao = pin_init("AO", ANALOG);  // Inicializa o pino analógico
  chip->pin_do = pin_init("DO", OUTPUT);   // Inicializa o pino digital
  chip->pin_vcc = pin_init("VCC", INPUT_PULLDOWN); // Inicializa o pino VCC com pull-down
  chip->pin_gnd = pin_init("GND", INPUT_PULLUP);   // Inicializa o pino GND com pull-up

  // Inicializa o atributo de chuva com valor inicial 0
  chip->rain_attr = attr_init("rain", 0);

  // Configura o timer para atualização periódica
  const timer_config_t timer_config = {
    .callback = chip_timer_event, // Define a função de callback do timer
    .user_data = chip,             // Passa o estado do chip como dados do usuário
  };
  timer_t timer_id = timer_init(&timer_config); // Inicializa o timer
  timer_start(timer_id, 1000, true); // Inicia o timer para disparar a cada 1000 ms (1 segundo)
}

// Função chamada quando o timer é acionado
void chip_timer_event(void *user_data) {
  chip_state_t *chip = (chip_state_t*)user_data; // Recupera o estado do chip

  // Lê o valor do atributo de chuva
  uint32_t rain = attr_read(chip->rain_attr);

  // Converte o valor de chuva para uma voltagem simulada
  float voltage = 5 * ((float)rain / 1023.0);  // Ajusta para uma faixa de 0 a 5V (simulando 3.3V)
  
  // Verifica se a alimentação (VCC) está ligada e o terra (GND) está desligado
  if (pin_read(chip->pin_vcc) && !pin_read(chip->pin_gnd)) {
    // Gera o valor analógico baseado na quantidade de chuva
    pin_dac_write(chip->pin_ao, voltage); // Escreve a voltagem no pino analógico

    // O sinal digital indica se está chovendo com base na leitura de chuva
    if (rain > 0) {
      pin_write(chip->pin_do, HIGH);  // Liga o pino digital se está chovendo
    } else {
      pin_write(chip->pin_do, LOW);   // Desliga o pino digital se não está chovendo
    }

    // Imprime os valores para fins de depuração
    printf("Chuva: %u, Voltagem: %.2fV\n", rain, voltage); // Mostra a quantidade de chuva e a voltagem gerada
  }
}
