/* soc-init_options.c */

/*************************************************/
/* Copyright (c) 2011  AMANO, Kou                */
/* kamano@affrc.go.jp                            */
/*************************************************/

/* (* functions */
struct options *opt_alloc(int size){
	struct options *OPT;
	OPT = malloc(sizeof(struct options) * size);
	if(OPT == NULL){
		fprintf(stderr,"[E]failed : malloc() in opt_alloc().\n");
		exit(1);
	}
	return(OPT);
}

void print_usage(){
	printf("\nUSAGE:\n");
	printf("  mpi-soc-init -h\n");
	printf("  mpi-soc-init -o\n");
	printf("  mpi-soc-init ifh=<head of input file> ofh=<head of output file> frac={<number of clusters>|<grid size>} <FORMAT>\n");
}

void print_options(){
	printf("\nOPTIONS:\n");
	printf("  ifh=\n");
	printf("  ofh=\n");
	printf("  frac=\n");
	printf("  <FORMAT>:\n");
	printf("    Through\n");
	printf("    Diagonal\n");
	printf("    Central\n");
	printf("    Grid\n");
}

void init_options(struct options *OPT){
	OPT->ifh = NULL;
	OPT->ofh = NULL;
	OPT->str_frac = NULL;
	OPT->FORMAT = NULL;
	OPT->print_help = 0;	// -h
	OPT->print_opt = 0;	// -o
	OPT->print_optstr = 0;	// -c
	OPT->num_fracs = 0;
	OPT->fracs = NULL;
}

void get_options(int optc, char **optv, struct options *OPT){
	int i = 0;
	int tmplen = 0;
	for(i=0;i<optc;i++){
		if(strcmp("-h",optv[i]) == 0){
			OPT->print_help = 1;
		}else if(strcmp("-c",optv[i]) == 0){
			OPT->print_optstr = 1;
		}else if(strcmp("-o",optv[i]) == 0){
			OPT->print_opt = 1;
		}else if(strncmp("ifh=",optv[i],4) == 0){
			tmplen = strlen(optv[i]);
			if(tmplen > 0){
				OPT->ifh = c_calloc_vec(tmplen);
				sscanf(optv[i],"ifh=%s",OPT->ifh);
			}
		}else if(strncmp("ofh=",optv[i],4) == 0){
			tmplen = strlen(optv[i]);
			if(tmplen > 0){
				OPT->ofh = c_calloc_vec(tmplen);
				sscanf(optv[i],"ofh=%s",OPT->ofh);
			}
		}else if(strncmp("frac=",optv[i],5) == 0){
			tmplen = strlen(optv[i]);
			if(tmplen > 0){
				OPT->str_frac = c_calloc_vec(tmplen);
				sscanf(optv[i],"frac=%s",OPT->str_frac);
			}
		}else if(strncmp("Through",optv[i],6) == 0){
			tmplen = strlen(optv[i]);
			if(tmplen > 0){
				OPT->FORMAT = c_calloc_vec(tmplen);
				sscanf(optv[i],"%s",OPT->FORMAT);
			}
		}else if(strncmp("Diagonal",optv[i],8) == 0){
			tmplen = strlen(optv[i]);
			if(tmplen > 0){
				OPT->FORMAT = c_calloc_vec(tmplen);
				sscanf(optv[i],"%s",OPT->FORMAT);
			}
		}else if(strncmp("Central",optv[i],7) == 0){
			tmplen = strlen(optv[i]);
			if(tmplen > 0){
				OPT->FORMAT = c_calloc_vec(tmplen);
				sscanf(optv[i],"%s",OPT->FORMAT);
			}
		}else if(strncmp("Grid",optv[i],4) == 0){
			tmplen = strlen(optv[i]);
			if(tmplen > 0){
				OPT->FORMAT = c_calloc_vec(tmplen);
				sscanf(optv[i],"%s",OPT->FORMAT);
			}
		}
	}
}

void print_options_str(struct options OPT){
	int i;
	printf("OPTION STRING:\n");
	printf("  print_help:%d:\n",OPT.print_help);
	printf("  print_opt:%d:\n",OPT.print_opt);
	printf("  print_optstr:%d:\n",OPT.print_optstr);
	if(OPT.ifh == NULL){
		printf("  ifh:NULL:\n");
	}else{
		printf("  ifh:%s:\n",OPT.ifh);
	}
	if(OPT.ofh == NULL){
		printf("  ofh:NULL:\n");
	}else{
		printf("  ofh:%s:\n",OPT.ofh);
	}
	if(OPT.str_frac == NULL){
		printf("  str_frac:NULL:\n");
	}else{
		printf("  str_frac:%s:\n",OPT.str_frac);
	}
	printf("  num_fracs:%d:\n",OPT.num_fracs);
	printf("  fracs:\n");
	for(i=0;i<OPT.num_fracs;i++){
		printf("   %d\n",OPT.fracs[i]);
	}
	printf("  :\n");
	if(OPT.FORMAT == NULL){
		printf("  FORMAT:NULL:\n");
	}else{
		printf("  FORMAT:%s:\n",OPT.FORMAT);
	}
}

int check_options(struct options OPT){
	int err = 0;
	if(OPT.ifh == NULL){
		err=+1;
		fprintf(stderr,"no input file.\n");
	}
	if(OPT.ofh == NULL){
		err=+2;
		fprintf(stderr,"no output file.\n");
	}
	if(OPT.str_frac == NULL){
		err=+4;
		fprintf(stderr,"no fraction value.\n");
	}
	if(OPT.FORMAT == NULL){
		err=+8;
		fprintf(stderr,"no fromat.\n");
	}
	if(strcmp(OPT.ofh,OPT.ifh) == 0){
		err=+16;
		fprintf(stderr,"output file name is equivalent to input file name.\n");
	}
	return(err);
}

int analyse_options(struct options *OPT){
	int i;
	int str_frac_len = 0;
	int size = 0;
	int word_len = 0;
	int word_pos = 0;
	int pos = 0;
	char word[TMP_WORD_SIZE];
	word[0] = '\0';
	if((*OPT).str_frac == NULL){
		return(1);
	}
	str_frac_len = strlen((*OPT).str_frac);
	/* (* get size */
	for(i=0;i<str_frac_len;i++){
		if((*OPT).str_frac[i] == ','){
			size++;
		}
	}
	size++;
	OPT->num_fracs = size;
	/* *) */
	/* (* get fracs */
	OPT->fracs = i_calloc_vec((*OPT).num_fracs);
	for(i=0;i<str_frac_len;i++){
		if((*OPT).str_frac[i] == ','){
			word_pos++;
			word[word_pos] = '\0';
			sscanf(word,"%d",&(*OPT).fracs[pos]);
			pos++;
			word_pos = 0;
		}else{
			word[word_pos] = (*OPT).str_frac[i];
			word_pos++;
		}
	}
	word_pos++;
	word[word_pos] = '\0';
	sscanf(word,"%d",&(*OPT).fracs[pos]);
	/* *) */
	/**/
	return(0);
	/**/
}
/* *) */
