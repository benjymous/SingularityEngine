#ifndef _INT_H
#define _INT_H

#define clock(x) (tick)

extern unsigned char tick;

extern int elapsed();
extern int wait(int wframes);
extern void setup_int(void);

#endif
