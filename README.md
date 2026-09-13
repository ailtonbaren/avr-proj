## Máquina de Vendas Automática (Vending Machine)

- **Contexto:** Lógica financeira e mecânica de entrega de produtos.

- **Chaves (Sensores):**
  - Inserir Moeda (R$1,00)
  - Botão Produto A (R$2,00)
  - Botão Produto B (R$3,00)
  - Devolver Troco

- **LEDs (Atuadores):**
  - 4 LEDs (Barra indicando o saldo atual inserido)
  - Motor Produto A
  - Motor Produto B
  - Atuador de Troco

- **Requisito de Funcionamento:**
  - O sistema deve registrar e acumular os valores inseridos pelo usuário usando as macros de teste, refletindo o saldo atual progressivamente na barra de LEDs (ex: 3 LEDs acesos = R$ 3 inseridos).
  - Quando um produto é solicitado, o sistema deve verificar se o saldo é suficiente.
  - Se for, atua o motor correspondente brevemente para liberar o produto e subtrai o valor do saldo geral nos LEDs.
  - O usuário também pode pedir a devolução do dinheiro a qualquer momento, acionando o atuador de troco e zerando a barra.

## Requisitos
- avr-gcc
- avr-binutils
- avr-libc

## Como gerar o .hex
```bash
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -o main.elf main.c
avr-objcopy -O ihex -R .eeprom main.elf main.hex
```
