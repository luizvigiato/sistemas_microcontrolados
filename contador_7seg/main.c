#include <msp430g2553.h>

/**
 * Exercicios de semafaros
 * Luiz Henrique A. Vigiato
 * 2022-04-21
 */

#define seg_a BIT0
#define seg_b BIT1
#define seg_c BIT2
#define seg_d BIT3
#define seg_e BIT4
#define seg_f BIT5
#define seg_g BIT6
#define seg_p BIT7

#define ZERO (seg_a + seg_b + seg_c + seg_d + seg_e + seg_f)
#define UM (seg_b + seg_c)
#define DOIS (seg_a + seg_b + seg_g + seg_e + seg_d)
#define TRES (seg_a + seg_b + seg_c + seg_d + seg_g)
#define QUATRO (seg_b + seg_c + seg_f + seg_g)
#define CINCO (seg_a + seg_c + seg_d + seg_g + seg_f)
#define SEIS (seg_a + seg_g + seg_c + seg_d + seg_e + seg_f)
#define SETE (seg_a + seg_b + seg_c)
#define OITO (seg_a + seg_b + seg_c + seg_d + seg_e + seg_f + seg_g)
#define NOVE (seg_a + seg_b + seg_c + seg_d + seg_f + seg_g)

void ini_Timer0(void);
void ini_P1_P2(void);
void config_ini(void);
void altera_led(void);

    char numero;

void main(){
    numero = 0;
    ini_P1_P2();
    ini_Timer0();
    config_ini();
    do{
    // Loop infinito
    }while(1);
}

void config_ini(void){
    WDTCTL = WDTPW | WDTHOLD;
    // Para o contador do watchdog timer
    // Configuracoes do BCS
    // MCLK = DCOCLK ~ 16 MHz
    // ACLK = LFXT1CLK = 32768 Hz
    // SMCLK = DCOCLK / 8 ~ 2 MHz

    DCOCTL = CALDCO_16MHZ; // Freq. Calibrada de 16 MHz
    BCSCTL1 = CALBC1_16MHZ;
    BCSCTL2 = DIVS0 + DIVS1; // Fator divisao = 8 para SMCLK
    BCSCTL3 = XCAP0 + XCAP1; // Capacitor do cristal ~12.5 pF
    while (BCSCTL3 & LFXT1OF)
        ;
    __enable_interrupt(); // seta o bit GIE - permite geracao de interrupcoes
}

void ini_P1_P2(void){
    /**
     * P1.x -> Todas sao saidas
     * 7 Segmentos
     * A -> P1.0
     * B -> P1.1
     * C -> P1.2
     * D -> P1.3
     * E -> P1.4
     * F -> P1.5
     * G -> P1.6
     * P1.7 saida -> Baixa
     */
    P1DIR = 0xFF;
    P1OUT = ZERO; // Todas as saídas da Porta 1 em nível baixo
    /**
     * P2.0 -> entrada botao
     * P2.x -> Todas em saida -> Baixo
     */
    P2DIR = 0xFF;
    P2OUT = 0;
}

void ini_Timer0(void){
    /* Configuracoes iniciais do Timer0 para o Debouncer de S2
     *
     * CONTADOR
     * - Fonte de Clock: SMCLK ~ 2 MHz
     * - Fdiv clock: 1
     * - Modo cont.: Inicialmente >>>> PARADO!
     * - Int. cont.: desabilitada
     *
     * MODULO 0
     * - Modo: Comparacao (default)
     * - Interrupcao: Habilitada
     * - TA0CCR0 = 5ms * 125kHz
     *
     */
    TA0CTL = TASSEL0 + MC0;
    TA0CCTL0 = CCIE;
    TA0CCR0 = 32767;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void RTI_do_M0_do_Timer0(void){
    /**
     * Incrementa numero de contagem
     * Chama funcao de mudanca do display
     */
    numero++;
    altera_led();
}

void altera_led(void){
    if(numero > 9){
        numero = 0;
    }
    switch (numero){
    case 0:
        P1OUT = ZERO;
        break;
    case 1:
        P1OUT = UM;
        break;
    case 2:
        P1OUT = DOIS;
        break;
    case 3:
        P1OUT = TRES;
        break;
    case 4:
        P1OUT = QUATRO;
        break;
    case 5:
        P1OUT = CINCO;
        break;
    case 6:
        P1OUT = SEIS;
        break;
    case 7:
        P1OUT = SETE;
        break;
    case 8:
        P1OUT = OITO;
        break;
    case 9:
        P1OUT = NOVE;
        break;
    default:
        P1OUT = 0x00;
        break;
    }
}
