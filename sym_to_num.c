#include <stdio.h> 
#include <string.h> 
#include "sym_to_num.h"

#define MAX_INT 4294967295;
#define ONE_BYTE_MAX 127
#define TWO_BYTE_MAX 2047
#define THREE_BYTE_MAX 65535



/* A function recieves address of a string, address of a buffer, and  */
/* number, which equals number of byte in the string. The function */ 
/* copies the string in the buffer. */  
static void copy_in_buf(char *source, unsigned char *target, int quant_byt)  
{
	int el_num = 0;
	while (el_num < quant_byt)
	{
		target[el_num] = source[el_num];
		el_num++;
	}
	target[el_num] = '\0';
}

/* A function recieves a byte and a number, which equals the number of*/
/* radixes (radix must be > 2). The function copies the mentioned number */
/* of radixes of the byte and returns the radixes in char. */
static char copy_b_func(const char value, int bit_quant)
 {
        unsigned char copy_val;
	int count;
        char origin_mask = 0b00000001;
	char sum_mask = origin_mask;
	char work_mask = origin_mask;
	for (count=1; count<bit_quant; count++)
	{
		sum_mask = origin_mask;
		sum_mask <<= count;
		work_mask |= sum_mask;
	} 
        copy_val = (value & work_mask);
        return copy_val;
 }

/* A function sets the mentioned bit 'positive' radixes in the mentioned */
/* char. The radixes are numerated with the end of the byte, not with the */
/* beginning, because the function meant to set unicode "unsignificant" */
/* sequences (110*.****, 10**.**** and etc) and to transport the "significant"*/
/* bits from the beginnig of one byte to the end of another byte. */  
static void set_b_func(const char mask, unsigned char *source, int quant)
 {
          unsigned char trans_mask;
          trans_mask = mask;
          trans_mask <<= 8-quant;
          *(source) |= trans_mask;
 }

/* It is one of main function of the programm, it translate unicode code */
/* to the code of the symbol in the UCS. */
static void origin_code (unsigned char *str, int num)
{
	int i, pred;
	unsigned char copy_bits;
/* The unnotional bits becomes zeroes. */
	i = 0;
	str[i] <<= num;
	str[i] >>= num;
	i++;
	for (;i < num; i++)
	{
		str[i] <<= 2;
		str[i] >>= 2;
	} 
/* Moving the notional bits. */ 
	i--;
	for (pred=i-1;i > 0; i--, pred--)
	{
		if ( i == 2 || num == 2) /* the symb is 2-byte */
			{
			copy_bits = copy_b_func(str[pred],2);
			set_b_func(copy_bits, &(str[i]),2);	
			str[pred]>>=2;
			}
		if ( i == 1 && num == 3) /* the symb is 3-byte */
			{
			copy_bits = copy_b_func(str[pred],4);
			set_b_func(copy_bits, &(str[i]),4);	
			str[pred]>>=4;
			}
	}		
}

/* A function turns positions of bytes. It is necessary, because ordering */
/* of the bytes in string and in the integer is contrary. */
static int code_to_code(unsigned char *str, int buf_length)
{
	int ret_val;
	int count;
	int *iptr;
	char *cptr;
	buf_length-=1;
	ret_val = 0;
	iptr = &ret_val;
	cptr = (char*)iptr;
	for (count=0; buf_length >= 0; buf_length--, count++)
        {
		*(cptr+count)=*(str+buf_length);
        }
	return ret_val;
}

int unisym_to_unicode (char *argv, enum how_many quant_byt)
{
	unsigned char buf[4];
	int code;	

	copy_in_buf(argv, buf, quant_byt);
	origin_code (buf, quant_byt);
	code=code_to_code(buf, quant_byt);
	return code;
}

int set_zero (int number, int rad)
{
        int work_mask;
        int origin_mask=MAX_INT;
        work_mask = ~(origin_mask << rad);
        return  number &=  work_mask;
}

static int move_bits(int *number, int or_rad, int res_rad)
{
        int origin_mask = MAX_INT;
        int work_mask, copy_bits;
        work_mask = origin_mask << or_rad;
        copy_bits = *number & work_mask;
        *number = set_zero(*number, or_rad);
        copy_bits<<=res_rad-or_rad;
        (*number)|=copy_bits;
        return *number;
}

static void mask_in_int (int or_mask, int *number, int rad)
{
        unsigned int work_mask;
        work_mask = or_mask;
        work_mask<<=rad;
        *number |= work_mask; /* set '1's */
}

static void trans_ord_of_bytes(int *iptr, char *str, int byt_quant)
{
        char *or_ptr;
        char *dest_ptr;
        int i,j;
        or_ptr=(char*)iptr;
        dest_ptr=str;
        for (i=0, j=byt_quant-1; j>=0; i++, j--)
        { *(dest_ptr+i)=*(or_ptr+j); }
	*(dest_ptr+i) = '\0';
}

enum how_many How_Many_in_int (int number)
{
	if ((number>=0) && (number<=ONE_BYTE_MAX))
		return one_byte_symb;
	if ((number>ONE_BYTE_MAX) && (number<=TWO_BYTE_MAX))
		return two_byte_symb;
	if ((number>TWO_BYTE_MAX) && (number<=THREE_BYTE_MAX))
		return three_byte_symb;
}

void unicode_to_unisym (int number,char *str, enum how_many quant)
{
	move_bits(&number, 6, 8);
        mask_in_int(2, &number, 6);

	if (quant==two_byte_symb) 
	{
		mask_in_int(3, &number, 14);
		trans_ord_of_bytes(&number, str, quant);
	} 
	
	if (quant==three_byte_symb) 
	{
		move_bits(&number, 14, 16);
        	mask_in_int(2, &number, 14);
        	mask_in_int(7, &number, 21);
		trans_ord_of_bytes(&number, str, quant);
	} 
}



/*	int symb_num = 1;*/
/*	unsigned char buf[4];*/
/*	quant_byt = 0;*/
/*	enum how_many_bytes quant_byt;*/
/*	int code;*/
/*	int i;*/

