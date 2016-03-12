#ifndef _OUTPORT_H_
#define _OUTPORT_H_

static inline void outb(uint16_t port, byte v)
{
  asm volatile ("outb %0, %1" : : "a"(v), "Nd"(port) );
}

#endif /*_OUTPORT_H_*/
