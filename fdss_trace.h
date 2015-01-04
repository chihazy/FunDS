#ifndef _FDSS_TRACE_H_
#define _FDSS_TRACE_H_

typedef struct fdss_symbol_t fdss_symbol_t;
typedef struct fdss_tracer_t fdss_tracer_t;

struct fdss_symbol_t {
	unsigned long addr;
	unsigned long mcount;
	const char *name;
	const char *section;
};

struct fdss_tracer_t {
	const char *name;
	void (*enter)();
	void (*exit)();
};


int fdss_trace_init(void);
int fdss_trace_enable_function(char *name);
int fdss_trace_enable_all(void);
void fdss_trace_disable_function(char *name);
void fdss_trace_disable_all(void);

unsigned long fdss_name_to_addr(char *name);
char *fdss_addr_to_name(unsigned long addr);

#endif
