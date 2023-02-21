
//   Part2.c
//  Created on: Feb211, 2023
//   Author: Luciano Miles Mileeea

#include <msp430.h>
int count=0;
void gpioInit();
void timerInit();


void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();                             // Disable the GPIO power-on default high-impedance mode
    timerInit();
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM3_bits | GIE);

}

char REDLED = 0x01;

void gpioInit()

{


    P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
    P6DIR |= BIT6;                          // Set P6.6 to output direction



    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES &= ~BIT3;                         // P2.3 Low --> High edge
    P2IE |= BIT3;                           // P2.3 interrupt enabled
    P2IFG &= ~BIT3;                         // Clear P1.3 IFG

}

void timerInit()

{
    TB1CCTL0 = CCIE;                          // TBCCR0 interrupt enabled
    TB1CCR0 = 5000;
    TB1CTL = TBSSEL_1 | MC_2;                 // ACLK, continuous mode

}


#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    count ++;                                 // Add 1 to the counter each time
    P6OUT^= BIT6;                             // Turn on Green Light
    P2IFG&= ~BIT3;                            // Clear P1.3 IFG


}



// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    if (count == 1)                           // If Count Equals 1
            {
    TB1CCR0 += 5000;                          // Make Led Blink fast
    P6OUT ^= BIT6;                            // Make Led Blink
            }
    else if (count == 2)                      // If count equals 2
            {
    TB1CCR0 += 10000;                         // Make LED blink at a medium Pace
    P6OUT ^= BIT6;                            // Make the Green Led blink
                    }
    else if (count == 3)
            {
    TB1CCR0 += 15000;                         // Make the LED blink at a longer pace
    P6OUT ^= BIT6;                            // Make the Green LED blink
                    }
    else                                      // if count isnt a 1,2 or a 3
        count = 0;                            // Make Count = 0

}
