/*			This file to control one bit			*/

#ifndef		BIT_MATH_H
#define		BIT_MATH_H

/*			1- how to set a particular Bit 			*/
/*			0xAA  bit NUM 2							*/
#define		SET_BIT( VAR , BIT_NUM )  ( VAR |=  (1<<BIT_NUM) )

/*			1- how to Clear a particular Bit 		*/
/*			0x55  bit NUM 2							*/
#define		CLR_BIT( VAR , BIT_NUM )  ( VAR &=  ~(1<<BIT_NUM) )

/*			1- how to Toggle a particular Bit 		*/
/*			0x55  bit NUM 2							*/
#define		TOG_BIT( VAR , BIT_NUM )  ( VAR ^=  (1<<BIT_NUM) )

/*			1- how to Get a particular Bit Value (0 or 1)	*/
/*			0x55  bit NUM 2									*/
#define		GET_BIT( VAR , BIT_NUM )  ( (VAR >> BIT_NUM) & 1 )

/*			Nibble*/
#define ROT_R
#define ROT_L



#endif








