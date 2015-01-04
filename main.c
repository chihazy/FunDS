#include "fdss_trace.h"
#include <stdio.h>

#define MAIN_FN_SECTION ".fdss_main"
#define SECTION(x) __attribute__((section(x)))

#define ERR_INIT 1
#define ERR_READLINK 1<<1
#define ERR_BFD_OPENR 1<<2
#define ERR_CHECK_FORMAT 1<<3
#define ERR_NOSYMBOL 1<<4
#define ERR_SYMTAB_GET 1<<5

int 
SECTION(MAIN_FN_SECTION)
main(void)
{
	int ret;

	ret = fdss_trace_init();
	switch (ret) {
		case ERR_READLINK:
		case ERR_BFD_OPENR:
		case ERR_CHECK_FORMAT:
		case ERR_NOSYMBOL:
		case ERR_SYMTAB_GET:
		default:
			printf ("sth. wrong\n");
			break;
	}

	fdss_print_symbols();

	return 0;
}
