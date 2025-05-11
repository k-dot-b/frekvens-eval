//NOP instruction
//inline assembly
#define NOP __asm__ __volatile__ ("nop\n\t")