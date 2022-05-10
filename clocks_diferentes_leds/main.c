#include <msp430.h> 

/*
 * main.c
 */

void config_ini(void);
void ini_P1_P2(void);
void ini_Timer1(void);
void ini_Timer0(void);


void main(void) {

	config_ini();
	ini_P1_P2();
	ini_Timer1();
	ini_Timer0();
	while(1);
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

void ini_Timer0(void){
    /* Configuracoes iniciais do Timer0 para o Debouncer de S2
     *
     * CONTADOR
     * - Fonte de Clock: Cristal = 32 768Hz
     * - Fdiv clock: 1
     * - Modo cont.: Inicialmente >>>> Ativo
     * - Int. cont.: desabilitada
     *
     * MODULO 0
     * - Modo: Comparacao (default)
     * - Interrupcao: Habilitada
     * - TA0CCR0 = 1seg
     *
     */
    TA0CTL = TASSEL0 + MC0;
    TA0CCTL0 = CCIE;
    TA0CCR0 = 32767;
}

void ini_Timer1(void){
    /* Configuracoes iniciais do Timer0 para o Debouncer de S2
     *
     * CONTADOR
     * - Fonte de Clock: 125 000 Hz
     * - Fdiv clock: 2 - ID0 -> clock aprox 62500 Hz
     * - Modo cont.: Inicialmente >>>> Ativo
     * - Int. cont.: desabilitada
     *
     * MODULO 0
     * - Modo: Comparacao (default)
     * - Interrupcao: Habilitada
     * - TA0CCR0 = 1seg
     *
     */
    TA1CTL = TASSEL1 + ID0 + MC0;
    TA1CCTL0 = CCIE;
    TA1CCR0 = 62499;
}

void ini_P1_P2(void){
	P1DIR = 0xFF;
	P1OUT = 0;
	
	P2DIR = 0xFF;
	P2OUT = 0;
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void RTI_do_M0_do_Timer0(void){
	P1OUT ^= BIT0;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void RTI_do_M0_do_Timer1(void){
	P1OUT ^= BIT6;
}
