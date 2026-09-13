#include <avr/io.h>
#include <util/delay.h>

// definição de macros para manipulação de bits
#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tgl_bit(Y, bit_x) (Y ^= (1 << bit_x))
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))

// definição dos pinos
#define MOEDA          PD0	// botão moeda
#define PRODUTO_A      PD1	// botão produto A (R$2,00)
#define PRODUTO_B      PD2	//
#define TROCO          PD3	//

#define LED_1          PB0
#define LED_2          PB1
#define LED_3          PB2
#define LED_4          PB3

#define MOTOR_A        PC0
#define MOTOR_B        PC1
#define ATUADOR_TROCO  PC2

// atualiza os LEDs de acordo com o saldo
void atualizaLeds(uint8_t saldo) {
    PORTB &= 0xF0; 			// apaga os quatro leds

// acende os LEDs de acordo com o saldo
    if (saldo == 1) {
        PORTB |= 0x01; 		// 0000 0001
    }
    else if (saldo == 2) {
        PORTB |= 0x03;		// 0000 0011
    }
    else if (saldo == 3) {
        PORTB |= 0x07;		// 0000 0111
    }
    else if (saldo == 4) {
        PORTB |= 0x0F;		// 0000 1111
    }
}

// libera o produto A
void liberarProduto_A(void) {
    // liga o motor do produto A
    set_bit(PORTC, MOTOR_A);

    // mantém o motor ligado por 2 segundos
    _delay_ms(2000);

    // desliga o motor do Produto A
    clr_bit(PORTC, MOTOR_A);
}

// libera o produto B
void liberarProduto_B(void) {
    // liga o motor do produto B
    set_bit(PORTC, MOTOR_B);

    // mantém o motor ligado por 2 segundos
    _delay_ms(2000);

    // desliga o motor do produto B
    clr_bit(PORTC, MOTOR_B);
}

// devolver o troco
void devolveTroco(void) {
    // liga o atuador
    set_bit(PORTC, ATUADOR_TROCO);

    // mantém o atuador ligado por 1.5 segundos
    _delay_ms(1500);

    // desliga o atuador
    clr_bit(PORTC, ATUADOR_TROCO);
}


int main(void) {
    // configuração da PORTB: PB0 a PB3(saídas) e PB4 a PB7(entradas)
    DDRB = 0b00001111;

    // configuração da PORTC: PC0 a PC2 (saídas)
    DDRC = 0b00000111;

	// configuração da PORTD: PD0 a PD3 (entradas para os botões)
    DDRD = 0b00000000;
	
    // ativação dos resistores de pull-up internos nos pinos PD0 a PD3
    PORTD = 0b00001111;

    // inicialização dos LEDs desligados
    PORTB = 0b00000000;

    // inicialização dos motores e atuador desligados
    PORTC = 0b00000000;

    uint8_t saldo = 0;
	
    while (1) {
        // verifica PD0
        if (!tst_bit(PIND, MOEDA)) {
            // debounce do botão
            _delay_ms(50);

            // confirma se o botão continua pressionado
            if (!tst_bit(PIND, MOEDA)) {
                if (saldo < 4) {
                    // adiciona R$1,00 à máquina
                    saldo++;
					
                    atualizaLeds(saldo);
                }
                // aguarda o botão ser solto
                while (!tst_bit(PIND, MOEDA));
            }
        }
		
		// verifica PD1
        if (!tst_bit(PIND, PRODUTO_A)) {
            // debounce do botão
            _delay_ms(50);

            // confirma se o botão continua pressionado
            if (!tst_bit(PIND, PRODUTO_A)) {
                if (saldo >= 2) {
                    // desconta R$2 do saldo
                    saldo -= 2;

                    // atualiza os LEDs
                    atualizaLeds(saldo);

                    // liga o motor do Produto A
                    liberarProduto_A();
                }
                // aguarda o botão ser solto
                while (!tst_bit(PIND, PRODUTO_A));
            }
        }
		
		// verifica PD2
        if (!tst_bit(PIND, PRODUTO_B)) {
            // debounce do botão
			_delay_ms(50);
			
            // confirma se o botão continua pressionado
            if (!tst_bit(PIND, PRODUTO_B)) {
                if (saldo >= 3) {
                    // desconta R$3 do saldo
                    saldo -= 3;

                    // atualiza os LEDs
                    atualizaLeds(saldo);

                    // liga o motor do Produto B
                    liberarProduto_B();
                }

                // aguarda o botão ser solto
                while (!tst_bit(PIND, PRODUTO_B));
            }
        }

		// verifica PD3
        if (!tst_bit(PIND, TROCO)) {
            // debounce do botão
            _delay_ms(50);

            // confirma se o botão continua pressionado
            if (!tst_bit(PIND, TROCO)) {
                // verifica se existe saldo para devolver
                if (saldo > 0) {
                    // aciona o mecanismo de troco
                    devolveTroco();

                    // zera o saldo
                    saldo = 0;

                    // apaga os LEDs
                    atualizaLeds(saldo);
                }
                // aguarda o botão ser solto
                while (!tst_bit(PIND, TROCO));
            }
        }
    }
	
    return 0;
}
