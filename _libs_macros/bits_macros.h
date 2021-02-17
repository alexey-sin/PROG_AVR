#ifndef BITS_MACROS_
#define BITS_MACROS_

#define		SetBit(reg, bit)			reg |= (1<<bit)           
#define		ClrBit(reg, bit)			reg &= (~(1<<bit))
#define		InvBit(reg, bit)			reg ^= (1<<bit)
#define		BitIsSet(reg, bit)			((reg & (1<<bit)) != 0)
#define		BitIsClr(reg, bit)		((reg & (1<<bit)) == 0)

#define TRUE 1
#define true 1
#define FALSE 0
#define false 0
#define HIGH 1
#define LOW 0

#endif//BITS_MACROS_



