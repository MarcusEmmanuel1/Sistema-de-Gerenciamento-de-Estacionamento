#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

//Define a interface I2C utilizada
#define I2C_PORT          i2c1
//Define os pinos do I2C: GPI 26 para SDA e GPI 27 para SCL
#define I2C_SDA_PIN       26      
#define I2C_SCL_PIN       27      
//Endereço do LCD na interface I2C (pode ser alterado para 0x3F se necessário)
#define LCD_I2C_ADDRESS   0x27    

//Parâmetros do LCD 16x2:
#define LCD_CLEAR_DISPLAY 0x01          // Comando para limpar a tela
#define LCD_ENTRY_MODE_SET 0x04         // Comando para configurar o modo de entrada
#define LCD_DISPLAY_CONTROL 0x08        // Comando para controlar o display (ligar/desligar, cursor, etc)
#define LCD_FUNCTION_SET  0x20         // Comando para definir a função (modo 4 bits, número de linhas, etc)
#define LCD_SET_DDRAM_ADDR 0x80         // Comando para definir a posição do cursor

// Flags e parâmetros de configuração:
#define LCD_DISPLAY_ON    0x04        // Liga o display
#define LCD_CURSOR_OFF    0x00        // Desliga o cursor
#define LCD_BLINK_OFF     0x00        // Desliga o piscar do cursor
#define LCD_ENTRY_LEFT    0x02        // Define a direção do texto (da esquerda para a direita)
#define LCD_4BITMODE      0x00        // Define o modo de comunicação de 4 bits
#define LCD_2LINE         0x08        // Configura o LCD para 2 linhas
#define LCD_5x8DOTS       0x00        // Define o tamanho da fonte (5x8 pontos)

// Controle do backlight e bit de enable:
#define LCD_BACKLIGHT     0x08        // Liga o backlight do LCD
#define ENABLE_BIT        0x04        // Bit de habilitação usado para sinalizar o LCD

// Define os pinos dos botões: 
#define BUTTON_A  18       //Botão A (GP 18) para adicionar veículo
#define BUTTON_B  15       //Botão B (GP 15) para remover veículo
#define MAX_CAPACITY 10 //Capacidade máxima do estacionamento

// Define os pinos para os LEDs (usando canais verde e vermelho de um LED RGB, por exemplo):
#define LED_VERDE 20       
#define LED_VERMELHO 21    

int qtdveiculos = 0;  //armazena o número atual de veículos no estacionamento

// Função para enviar um byte via I2C para o LCD
void lcd_escreve_byte(uint8_t valor) {
    i2c_write_blocking(I2C_PORT, LCD_I2C_ADDRESS, &valor, 1, false);
    sleep_us(100);
}

// Função para enviar um comando ao LCD
void lcd_envia_comando(uint8_t cmd) {
    // Separa o comando em dois nibbles (4 bits cada)
    uint8_t nibble_alto = (cmd & 0xF0) | LCD_BACKLIGHT;
    uint8_t nibble_baixo = ((cmd << 4) & 0xF0) | LCD_BACKLIGHT;
    // Envia o nibble alto com o bit de enable acionado e depois desacionado
    lcd_escreve_byte(nibble_alto | ENABLE_BIT);
    lcd_escreve_byte(nibble_alto);
    // Faz o mesmo com o nibble baixo
    lcd_escreve_byte(nibble_baixo | ENABLE_BIT);
    lcd_escreve_byte(nibble_baixo);
    sleep_ms(2); // Aguarda 2 ms para o comando processar
}

// Função para enviar um dado (caractere) ao LCD
void lcd_envia_dado(uint8_t dado) {
    uint8_t nibble_alto = (dado & 0xF0) | LCD_BACKLIGHT | 0x01;
    uint8_t nibble_baixo = ((dado << 4) & 0xF0) | LCD_BACKLIGHT | 0x01;
    lcd_escreve_byte(nibble_alto | ENABLE_BIT);
    lcd_escreve_byte(nibble_alto);
    lcd_escreve_byte(nibble_baixo | ENABLE_BIT);
    lcd_escreve_byte(nibble_baixo);
    sleep_us(50);
}

// Função para inicializar o LCD
void lcd_inicializa() {
    sleep_ms(50); // Aguarda o LCD ligar
    // Envia comandos iniciais para configurar o LCD em modo 4 bits
    lcd_envia_comando(0x03);
    lcd_envia_comando(0x03);
    lcd_envia_comando(0x03);
    lcd_envia_comando(0x02);
    // Configura o LCD com o número de linhas, modo de bits e fonte
    lcd_envia_comando(LCD_FUNCTION_SET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);
    // Liga o display, desliga o cursor e o piscar do cursor
    lcd_envia_comando(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF);
    // Limpa o display
    lcd_envia_comando(LCD_CLEAR_DISPLAY);
    // Configura o modo de entrada do texto (da esquerda para a direita)
    lcd_envia_comando(LCD_ENTRY_MODE_SET | LCD_ENTRY_LEFT);
}

// Função para definir a posição do cursor no LCD
void lcd_define_cursor(uint8_t linha, uint8_t coluna) {
    // Calcula o endereço DDRAM baseado na linha e coluna
    uint8_t endereco = coluna + (linha * 0x40);
    lcd_envia_comando(LCD_SET_DDRAM_ADDR | endereco);
}

// Função para imprimir uma string no LCD
void lcd_imprime(const char *texto) {
    while (*texto) {
        lcd_envia_dado(*texto++);
    }
}

// Função para atualizar o display com as informações do estacionamento
void atualiza_display() {
    char buf[17]; // Buffer para armazenar a string (16 caracteres + terminador)
    // Limpa o display
    lcd_envia_comando(LCD_CLEAR_DISPLAY);
    sleep_ms(2);
    // Define o cursor na linha 0, coluna 0 e imprime o status das vagas
    lcd_define_cursor(0, 0);
    sprintf(buf, "Vagas: %d/%d", qtdveiculos, MAX_CAPACITY);
    lcd_imprime(buf);
    // Na linha 1, exibe uma mensagem de status
    lcd_define_cursor(1, 0);
    if (qtdveiculos == MAX_CAPACITY) {
        lcd_imprime("Cheio!");
    } else if (qtdveiculos == 0) {
        lcd_imprime("Vazio!");
    } else {
        lcd_imprime("Vagas disp.");
    }
}

int main() {
    stdio_init_all();
    // Mensagem de cabeçalho no terminal serial
    printf("*************************************************************\n");
    printf("| Projeto Final: Sistema de Gerenciamento de Estacionamento |\n");
    printf("|                Aluno: Marcus Emmanuel                     |\n");
    printf("*************************************************************\n");

    // Inicializa a interface I2C para o LCD
    i2c_init(I2C_PORT, 100 * 1000); // Configura I2C para 100 kHz
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Inicializa o LCD e atualiza o display com as informações iniciais
    lcd_inicializa();
    atualiza_display();

    // Configura os botões (usando pull-up interno)
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Inicializa o led
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_put(LED_VERDE, 0); // Inicialmente apagado

    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_put(LED_VERMELHO, 0); // Inicialmente apagado

    while (true) {
        // Verifica se o botão A foi pressionado
        if (!gpio_get(BUTTON_A)) { // Sinal LOW indica botão pressionado
            if (qtdveiculos < MAX_CAPACITY) {
                qtdveiculos++;
                atualiza_display();
                //pisca o LED verde para indicar adição
                gpio_put(LED_VERDE, 1);
                sleep_ms(100);
                gpio_put(LED_VERDE, 0);
                printf("Veículo adicionado. Total: %d\n", qtdveiculos);
            }
            sleep_ms(300); // Tempo para debounce
        }

        // Verifica se o botão Bfoi pressionado
        if (!gpio_get(BUTTON_B)) {
            if (qtdveiculos > 0) {
                qtdveiculos--;
                atualiza_display();
                //pisca o LED vermelho para indicar remoção.
                gpio_put(LED_VERMELHO, 1);
                sleep_ms(100);
                gpio_put(LED_VERMELHO, 0);
                printf("Veículo removido. Total: %d\n", qtdveiculos);
            }
            sleep_ms(300); // Tempo para debounce
        }

        // Se o estacionamento estiver cheio, pisca sem parar o LED vermelho
        if (qtdveiculos == MAX_CAPACITY) {
            static bool estadoVermelho = false;
            estadoVermelho = !estadoVermelho;
            gpio_put(LED_VERMELHO, estadoVermelho);
            gpio_put(LED_VERDE, 0);
            sleep_ms(250);    // Garante que o LED verde esteja apagado quando a condição não for atendida
        }
        // Se o estacionamento estiver vazio, pisca sem parar o LED verde
        else if (qtdveiculos == 0) {
            static bool estadoVerde = false;
            estadoVerde = !estadoVerde;
            gpio_put(LED_VERDE, estadoVerde);
            gpio_put(LED_VERMELHO, 0);        //mantem o LED vermelho apagado quando não a condição não for atendida
            sleep_ms(250);
        }
        // Se houver veículos, os LEDs permanecem apagados
        else {
            gpio_put(LED_VERMELHO, 0);
            gpio_put(LED_VERDE, 0);
            sleep_ms(50);
        }
    }

    return 0;
}