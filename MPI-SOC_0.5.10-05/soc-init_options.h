/* soc-init_options.h */

/*************************************************/
/* Copyright (c) 2011  AMANO, Kou                */
/* kamano@affrc.go.jp                            */
/*************************************************/

/* (* structure */
struct options {
	char *ifh;
	char *ofh;
	char *str_frac;
	char *FORMAT;
	int print_help;		// -h
	int print_opt;		// -o
	int print_optstr;	//-c
	int num_fracs;
	int *fracs;
};
/* *) */

/* (* prototypes */
struct options *opt_alloc();
void print_usage();
void print_options();
void init_options();
void get_options();
void print_options_str();
int check_options();
int analyse_options();
/* *) */

