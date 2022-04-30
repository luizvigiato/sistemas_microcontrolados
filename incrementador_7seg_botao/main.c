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
    // MCLK = DCOCLK ~ 8 MHz
    // ACLK = LFXT1CLK = 32768 Hz
    // SMCLK = DCOCLK / 125 KHz

    DCOCTL = CALDCO_1MHZ; // Freq. Calibrada de 1 MHz
    BCSCTL1 = CALBC1_1MHZ;
    BCSCTL2 = DIVS0 + DIVS1; // Fator divisao = 8 para SMCLK
    BCSCTL3 = XCAP0 + XCAP1; // Capacitor do cristal ~12.5 pF
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
    P1DIR = BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT0;
    P1OUT = ZERO; // Todas as saídas da Porta 1 em nível baixo
//    P1REN = BIT7;
//    P1IE = BIT7;
//    P1IFG = 0;
//    P1IES = BIT7;


    /**
     * P2.0 -> entrada botao - ENTRADA COM RESISTOR DE PULL-UP E INTERRUPCAO POR BORDAD E DESCIDA
     * P2.x -> Todas em saida -> Baixo
     */
    P2DIR = BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;
    P2OUT = BIT0;

    P2REN = BIT0;
    P2IE = BIT0;
    P2IFG = 0;
    P2IES = BIT0;
}

void ini_Timer0(void){
    /* Configuracoes iniciais do Timer0 para o Debouncer de S2
     *
     * CONTADOR
     * - Fonte de Clock: SMCLK ~ 1 MHz
     * - Fdiv clock: 1
     * - Modo cont.: Inicialmente >>>> PARADO!
     * - Int. cont.: desabilitada
     *
     * MODULO 0
     * - Modo: Comparacao (default)
     * - Interrupcao: Habilitada
     * - TA0CCR0 = 200ms * 1MHz
     *
     */
    TA0CTL = TASSEL1;
    TA0CCTL0 = CCIE;
    TA0CCR0 = 25000;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void RTI_do_M0_do_Timer0(void){
    /**
     * Incrementa numero de contagem
     * Chama funcao de mudanca do display
     */
    TA0CTL &= ~MC0;
    P2IE = BIT0;
    P2IFG = 0;
    numero++;

    altera_led();
}

#pragma vector = PORT2_VECTOR
__interrupt void RTI_da_P2_Botao(void){
    /**
     * Desabilita interrupcao da porta
     * Inicia contagem pra debounce
     */
    TA0CTL |= MC0;
    P2IE = 0;
    P2IFG = 0;
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
