#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "sym_to_num.h"
#include "search_param_func.h"




int main(int argc, char **argv)
{
	enum modes_of_prog mode_var;
	enum how_many quant_byt;
	/* defauly value of mode_var */
	mode_var = sym_in_dec;
	unsigned char buf[4];
	int code = 0; 
	int code_var = 0;
	int param_count=1;
	int res_var=1;

	


	for (; param_count<=argc-1; param_count++)
	{
		res_var=search_of_set_param(*(argv+param_count), &mode_var);
		if (res_var==0)
		continue;

		quant_byt = strlen(*(argv+param_count));

		if (mode_var==sym_in_dec)
		{
			switch (quant_byt) {
			case one_byte_symb:
                        	printf("%c = %d\n", *(argv[param_count]), *(argv[param_count]));
                        	break;
        		case two_byte_symb:
				code = unisym_to_unicode (argv[param_count], quant_byt);
                        	printf ("%s = %u\n", argv[param_count], code);
                        	break;
        		case three_byte_symb:
				code = unisym_to_unicode (argv[param_count], quant_byt);
                        	printf ("%s = %d\n", argv[param_count], code);
                        	break;
        		default:
                        	printf("Error");
                        	break;
			}
       		 }
	
		if (mode_var==code_in_dec)
		{
			code_var=atoi(argv[param_count]);
			quant_byt=How_Many_in_int(code_var);
	
			switch (quant_byt) {
			case one_byte_symb:
					printf("%s = %c\n", argv[param_count], code_var);
					break;
			case two_byte_symb:
					unicode_to_unisym(code_var, argv[param_count], quant_byt);
					printf("%d = %s\n", code_var, argv[param_count]);
					break;
			case three_byte_symb:
					unicode_to_unisym(code_var, argv[param_count], quant_byt);
					printf("%d = %s\n", code_var, argv[param_count]);
					break;
			default:

		
			}
		}
	}
	return 0;
}

			  
