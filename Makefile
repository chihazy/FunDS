all:
	gcc main.c fdss_trace.h fdss_trace.c -o tm -lbfd -g -pg
