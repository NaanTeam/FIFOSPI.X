/*Author: Connor Martin*/

#include <stdio.h>
#include <stdlib.h>
#include <plib.h>


// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 10 MHz (SYSCLK / FPBDIV)
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8

#define SYS_FREQ (80000000L)

#include "FIFOSPI.h"

int SetupSPI2();
int SendSPI2(unsigned char data);
/**
 * Delays for some amount of time. Not accurate in the least sense.
 * @param ms: The amont of "time" to delay for.
 */
void DelayTime(int ms);

void readAll()
{
    char k, j;

    //Read the X Acceleration
    char ReadReg[10];
    ReadReg[0] = 0x0B;
    ReadReg[1] = 0x0E;
    ReadReg[2] = 0x00; //X
    ReadReg[3] = 0x00;
    ReadReg[4] = 0x00; //Y
    ReadReg[5] = 0x00;
    ReadReg[6] = 0x00; //Z
    ReadReg[7] = 0x00;
    ReadReg[8] = 0x00; //Temp
    ReadReg[9] = 0x00;
    FIFOSPI2_QueueSend(ReadReg, 10);
    DelayTime(10);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
}

void readOne()
{
    char k, j;

    //Read the X Acceleration
    char ReadReg[10];
    ReadReg[0] = 0x0B;
    ReadReg[1] = 0x08;
    ReadReg[2] = 0x00; //X
    FIFOSPI2_QueueSend(ReadReg, 3);
    DelayTime(10);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
}

int main(int argc, char** argv)
{
    TRISFbits.TRISF0 = 0; //LD4 on chipkit uno32

    FIFOSPI2_Setup();
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();
    
    //PORTFbits.RF0 = 1;
    //PORTDbits.RD4 = 1; //High to deselect
    //INTSetFlag(INT_SPI2TX);

    char j;
    int k;

    //Reset the device
    char reset[3];
    reset[0] = 0x0A;
    reset[1] = 0x1F;
    reset[2] = 0x52;
    FIFOSPI2_QueueSend(reset, 3);
    DelayTime(1);

    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);

    //Turns on Measurement Mode
    char ReadReg[3];
    ReadReg[0] = 0x0A;
    ReadReg[1] = 0x2D;
    ReadReg[2] = 0x02;
    FIFOSPI2_QueueSend(ReadReg, 3);
    ReadReg[0] = 0x0B;
    ReadReg[1] = 0x08;
    ReadReg[2] = 0x00;
    FIFOSPI2_QueueSend(ReadReg, 3); //Follow it with a measurement read.

    DelayTime(10);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
    k = FIFOSPI2_QueueRead(&j);
    DelayTime(15);

    while(1)
    {
        readAll();
        //readOne();
        DelayTime(10);
        
    }
    return (EXIT_SUCCESS);
}



void DelayTime(int ms)
{
    int i = 0;
    while (i < (ms*1000))
    {
        i++;
    }
}