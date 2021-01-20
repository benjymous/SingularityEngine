#include <arch/zx.h>
#include <im2.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>
#include "int.h"

// timer

unsigned char tick = 0;
extern void isr();

IM2_DEFINE_ISR_8080(isr)
{
    // update the clock
    ++tick;
    //zx_border(tick%8);
}

int elapsed()
{
    return tick;
}

static int waitCount;
int wait(int wframes)
{
    waitCount = 0;
    while (tick < wframes)
    {
        waitCount++;
    }

    tick = 0;

    return waitCount;
}

#define INTERRUPT_BASE 0x6000

// http://jgmalcolm.com/z80/advanced/im2i
void setup_int(void)
{
    im2_init((void *)INTERRUPT_BASE);          // set z80's I register to point at INTERRUPT_BASE, set im2 mode
    memset((void *)INTERRUPT_BASE, 0x61, 257); // make a 257-byte im2 table at INTERRUPT_BASE containg 0x81 bytes

    // On interrupt the z80 will jump to address 0x6161

    z80_bpoke(INTERRUPT_BASE + 0x161, 0xc3);              // z80 JP instruction
    z80_wpoke(INTERRUPT_BASE + 0x162, (unsigned int)isr); // to the isr routine

    // enable interrupts
    intrinsic_ei();
}
