#include <unistd.h>
#include <bfd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "fdss_trace.h"

///what is linux default max path?
#define PATH_MAX 256
#define LSHIFT(x) ((x) << 1)
#define ERR_INIT 1
#define FDSS_NO_TRACE __attribute__((no_instrument_function))

static fdss_symbol_t *fdss_symbols;
static int nr_symbols;

static unsigned long
FDSS_NO_TRACE
find_mcount_call(unsigned long entry_addr)
{
}

static inline int
FDSS_NO_TRACE
intcmp (unsigned long a, unsigned long b)
{
	return a - b;
}

int 
FDSS_NO_TRACE
fdss_symbols_cmp_mcount (const void *a, const void *b)
{
	return intcmp (((fdss_symbol_t *)a)->mcount, ((fdss_symbol_t *)b)->mcount);
}

int 
FDSS_NO_TRACE
fdss_symbols_cmp_name (const void *a, const void *b)
{
	return strcmp (((fdss_symbol_t *)a)->name, ((fdss_symbol_t *)b)->name);
}

int 
FDSS_NO_TRACE
fdss_symbols_cmp_addr(const void *a, const void *b)
{
	return intcmp (((fdss_symbol_t *)a)->addr, ((fdss_symbol_t *)b)->addr);
}

int 
FDSS_NO_TRACE
fdss_trace_init(void)
{
	static char path[PATH_MAX];
	int ret, max_symtab_size, symcount, i;
	bfd *abfd;
	asymbol **symtab;

	ret = readlink("/proc/self/exe", path, sizeof(path));
	if (ret < 0) 
		goto err_readlink;

	abfd = bfd_openr(path, NULL);
	if (abfd == 0) 
		goto err_bfd_openr;
	if (!bfd_check_format(abfd, bfd_object)) 
		goto err_check_format;
	if (!(bfd_get_file_flags(abfd) & HAS_SYMS))
		goto err_nosymbol;
	
	max_symtab_size = bfd_get_symtab_upper_bound(abfd);
	if (max_symtab_size < 0)
		goto err_symtab_get;
	symtab = malloc(max_symtab_size);
	symcount = bfd_canonicalize_symtab(abfd, symtab);

	fdss_symbols = calloc (sizeof (fdss_symbol_t) , symcount);
	for (i = 0; i < symcount; ++i) {
		asymbol *sym = symtab[i];
		unsigned long ip, addr;
		const char *name;
		const char *section;

		addr = bfd_asymbol_value(sym);
		name = bfd_asymbol_name(sym);
		section = bfd_get_section_name(abfd, bfd_get_section(sym));

		/*
		 * XXX:use switch to check sym->flags
		 * add variable breakpoint, make it event-driven
		 * sym->flags & BSF_GLOBAL
		 */
		if (addr == 0 || !(sym->flags & BSF_FUNCTION))
			/* sym is not a function */
			continue;

		ip = find_mcount_call(addr);

		fdss_symbols[nr_symbols].addr=addr;
		fdss_symbols[nr_symbols].name=strdup(name);
		fdss_symbols[nr_symbols].section=strdup(section);
		fdss_symbols[nr_symbols].mcount=ip;
		nr_symbols++;
	}

	qsort(fdss_symbols, nr_symbols, sizeof(fdss_symbol_t), fdss_symbols_cmp_mcount);

	free(symtab);
	bfd_close(abfd);

	return 0;

err_readlink:
	ret = ERR_INIT;
err_bfd_openr:
	LSHIFT(ret);
err_check_format:
	LSHIFT(ret);
err_nosymbol:
	LSHIFT(ret);
err_symtab_get:
	LSHIFT(ret);

	return ret;
}

void 
FDSS_NO_TRACE
fdss_print_symbols(void)
{
	int i;
	printf ("%10s NAME %10s ADDRRESS %10s SECTION\n");
	for (i = 0; i < nr_symbols; ++i) {
		printf ("%s %x %s\n", fdss_symbols[i].name, fdss_symbols[i].addr, fdss_symbols[i].section);
	}
}

int 
FDSS_NO_TRACE
fdss_trace_enable_function(char *name)
{
	
}

void
FDSS_NO_TRACE
fdss_trace_disable_function(char *name)
{
}

void 
FDSS_NO_TRACE
fdss_trace_disable_all(void)
{
}

int
FDSS_NO_TRACE
fdss_trace_enable_all(void)
{

}

/* 
 * XXX: use better ds
 */
unsigned long fdss_name_to_addr(char *name)
{
	int i;
	for (i = 0; i < nr_symbols; ++i)
		if (!strcmp(fdss_symbols[i].name, name))
			return fdss_symbols[i].addr;

	return 0;
}
