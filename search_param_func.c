#include <stdio.h>
#include <string.h>
#include "search_param_func.h"

#define SHORT_CALL_OF_HELP "-h"
#define LONG_CALL_OF_HELP "--help"
#define SHORT_INSERT_SYMB_MODE "-s"
#define LONG_INSERT_SYMB_MODE "--symbolic"
#define SHORT_INSERT_CODE_MODE "-c"
#define LONG_INSERT_CODE_MODE "--code"
#define SHORT_BIN_INP "-b"
#define LONG_BIN_INP "--binary"
#define SHORT_OX_INP "-o"
#define LONG_OX_INP "--octal"
#define SHORT_DEC_INP "-d"
#define LONG_DEC_INP "--decimal"
#define SHORT_HEX_INP "-h"
#define LONG_HEX_INP "--hexadecimal"
#define HELP_MESSAGE "Now I can't help you!"

/* A function compares two strings, recieves '0' if they aren't equal, and */
/* '1' if they are equal. */
static int str_comp (const char *str, const char *sample)
{
	int count;
	int match_quant=0;
	for (count=0; *(str+count) != '\0'; count++)
	{
		if (*(str+count) != *(sample+count))
		{
			return 0;
			break;
		}
		else 
			match_quant++;
	}
	if (match_quant==strlen(sample))
		return 1;
	else
		return 0;
};


int search_of_set_param (const char *str, enum modes_of_prog* ptr_mode)  
/* A function recieves pointer to a string and the address of mode variable, */
/* it compares the string with the parameters of the programm, if the function */
/* finds a string equals one of parameters it changes the mode variable or prints */
/* necessary text (exemple help message). If the function found the one of the */
/* parameters it returns '0', otherwise it returns '1'. */
{
	char short_help_message_str[]=SHORT_CALL_OF_HELP;
	char long_help_message_str[]=LONG_CALL_OF_HELP;
	char short_symb_mode_str[]=SHORT_INSERT_SYMB_MODE;
	char long_symb_mode_str[]=LONG_INSERT_SYMB_MODE;
	char short_code_mode_str[]=SHORT_INSERT_CODE_MODE;
	char long_code_mode_str[]=LONG_INSERT_CODE_MODE;
	int copy_mode;
	int ret_value=1;

/* Saving a value of mode variable */
	copy_mode = *ptr_mode;
/* string = "-h" or "--help"? */
	if (str_comp(str,short_help_message_str) || str_comp(str,long_help_message_str))
	{	
		printf ("%s\n", HELP_MESSAGE);
		ret_value = 0; 
	}
/* string = "-s" or "--symbolic"? */
	if (str_comp(str,short_symb_mode_str) || str_comp(str,long_symb_mode_str))
	{	
		*ptr_mode = sym_in_dec;
		ret_value = 0; 
	}
/* string = "-c" or "--code"? */
	if (str_comp(str,short_code_mode_str) || str_comp(str,long_code_mode_str))
	{
		*ptr_mode = code_in_dec;
		ret_value = 0; 
	}

	switch(*ptr_mode) {
	case sym_in_dec:
			break;
	case code_in_dec:
			break;
	default:
		printf ("Error mode");
		*ptr_mode = copy_mode;
		break;
	}
	return ret_value;
};
		
 
