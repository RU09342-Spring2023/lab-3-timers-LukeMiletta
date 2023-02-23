//Luciano Miles Miletta
// Completed on 2/23/2023
// Embedded Systems



#define Default 10000

#include <msp430.h>

 int falling = 0;                           //Set the falling variable
 int Count = 0;                              //Set the count variable
 int rising = 1;                             //Set the rising variable
unsigned long counttimer = 0;

void gpioInit();
void timerInit();

void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
    timerInit();

    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM3_bits | GIE);
    __no_operation();

}


void gpioInit(){


    P6OUT &= ~BIT6;                         // Turn off the Green LED
    P6DIR |= BIT6;                          // Set up the Green led direction


    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES &= ~BIT3;                         // P2.3 Low --> High edge
    P2IE |= BIT3;                           // P2.3 interrupt enabled


    P4OUT |= BIT1;                          // Configure P4.1 as pulled-up
    P4REN |= BIT1;                          // P4.1 pull-up register enable
    P4IES &= ~BIT1;                         // P4.1 Low --> High edge
    P4IE |= BIT1;                           // P4.1 interrupt enabled
}


void timerInit(){

    TB0CCTL0 |= CCIE;                       // Enable TB0 CCR0 Overflow IRQ
    TB0CCR0 = 1;                            // Have TB0CCR0 equal to a 1
    TB0CTL = TBSSEL_1 | MC_2 | ID_3;        // ACLK, continuous mode


    TB1CCTL0 |= CCIE;                       // Enable TB1 CCR0 Overflow IRQ
    TB1CCR0 = Default;                      // Have TB1CCR0 equal to the default time
    TB1CTL = TBSSEL_1 | MC_2 | ID_3;        // ACLK, continuous mode
}




#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;                         // Clear P4.1 interrupt flag
    counttimer = Default;                   // Put the timer back to default
    0;
}



#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;                            // Clear P2.3 interrupt flag

    if (rising)
    {
        rising = 0;                             // have the rising variable equal to a 0
        falling = 1;                            // have the falling variable equal to a 1
        Count = 1;                              // have the count variable equal to a 1
        counttimer = 0;                         // have the countertimer variable equal to 0
        P2IES &= ~BIT3;                         // P2.3 Low --> High edge
        P6OUT &= ~BIT6;                         // Turn off the Green LED

    }

    else if (falling)
    {
        rising = 1;                         //Have the rising variable equal to 1
        falling = 0;                        //Have the falling variable equal to 0
        Count = 0;                          // Have the count variable equal to 0
        P2IES |= BIT3;                      // P2.3 High --> Low edge
    }

}


#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void)
{
    if (Count)
        counttimer++;                      // If the button is pressed, continue to count the length
                                            // to add to time of interrupt for LED blinking
    else
    counttimer = counttimer;                // Have the counttimer equal the counttimer
    TB0CCR0 += 1;                           // Have TB0CCR0 add by one each time
}



#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
   P6OUT^=BIT6;                        // Make the  Red LED Blink
    TB1CCR0 += counttimer;                 // Increment time between interrupts
}






