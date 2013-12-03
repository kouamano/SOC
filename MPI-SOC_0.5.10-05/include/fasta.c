/* fasta.c */

/* NEEDS:  */
/* fasta.h */

/* PROCEDURES:                  */
/* sprint_bin()                 */
/* fasta_alloc()                */
/* read_fasta_from_fp()         */
/* read_fastaArray_from_fp()    */
/* mkoligonuc()                 */
/* count_pattern()              */
/* count_pattern_proportional() */

int sprint_bin(int bin, char str[(sizeof(int)*8)+1], int len){
	char buff[(sizeof(int)*8)+1];
	int i;
	buff[(sizeof(int)*8)] = '\0';
	for(i = 0; i < (sizeof(int)*8); i++){
		if(i != 0 && bin == 0 && i >= len){
			break;
                }
		if(bin % 2 == 0){
			buff[((sizeof(int)*8)-1)-i] = '0';
		} else {
			buff[((sizeof(int)*8)-1)-i] = '1';
                }
		bin = bin / 2;
	}
	sprintf(str, "%s", buff+((sizeof(int)*8)-1)-i+1);
	return(i);
}

struct fasta *fasta_alloc(int size_head, int size_body){
	struct fasta *fst;
	if((fst = malloc(sizeof(struct fasta)*1)) == NULL){
		fprintf(stderr,"[E]failed malloc().\n");
		exit(1);
	}
	if((fst->head = malloc(sizeof(char)*size_head)) == NULL){
		fprintf(stderr,"[E]failed malloc().\n");
		exit(1);
	}
	if((fst->body = malloc(sizeof(char)*size_body)) == NULL){
		fprintf(stderr,"[E]failed malloc().\n");
		exit(1);
	}
	return(fst);
}

int read_fasta_from_fp(FILE *IN, struct fasta *fst, int size_head, int size_body){
	signed char c;
	int in_head = 0;
	int in_body = 0;
	char *bf_head;
	char *bf_body;
	int pbfh = 0;
	int pbfb = 0;
	int pfst = 0;
	if((bf_head = malloc(sizeof(char)*size_head)) == NULL){
		fprintf(stderr,"[E]failed malloc.\n");
		exit(1);
	}
	if((bf_body = malloc(sizeof(char)*size_body)) == NULL){
		fprintf(stderr,"[E]failed malloc.\n");
		exit(1);
	}
	while((c = fgetc(IN)) != EOF){
		if((in_head == 0)&&(in_body == 0)){
			if(c == SH){
				in_head = 1;
			}
		} else if((in_head == 0)&&(in_body == 1)){
			if(c == SH){
				in_body = 0;
				in_head = 1;
				bf_body[pbfb] = '\0';
				strcpy((fst+pfst)->body,bf_body);
				pbfb = 0;
				pfst++;		/* only if in body */
			} else if(c == LB){
			} else if(c == ' '){
			} else if(c == '\t'){
			} else {
				bf_body[pbfb] = c;
				pbfb++;
			}
		} else if((in_head == 1)&&(in_body == 0)){
			if(c == LB){
				bf_head[pbfh] = '\0';
				strcpy((fst+pfst)->head,bf_head);
				pbfh = 0;
				in_head = 0;
				in_body = 1;
			} else {
				bf_head[pbfh] = c;
				pbfh++;
			}
		} else if((in_head == 1)&&(in_body == 1)){
			fprintf(stderr,"[C]internal data contradiction, check source code.\n");
		}
	}
	if((in_head == 0)&&(in_body == 1)){
		bf_body[pbfb] = '\0';
		strcpy((fst+pfst)->body,bf_body);
	} else if((in_head == 1)&&(in_body == 0)){
		bf_head[pbfh] = '\0';
		strcpy((fst+pfst)->head,bf_head);
	}
	pfst++;
	return(pfst);
};

int read_fastaArray_from_fp(FILE *IN, struct fasta *fst, int size_head, int size_body){
	signed char c;
	int in_head = 0;
	int in_body = 0;
	char *bf_head;
	char *bf_body;
	int pbfh = 0;
	int pbfb = 0;
	int pfst = 0;
	if((bf_head = malloc(sizeof(char)*size_head)) == NULL){
		fprintf(stderr,"[E]failed malloc.\n");
		exit(1);
	}
	if((bf_body = malloc(sizeof(char)*size_body)) == NULL){
		fprintf(stderr,"[E]failed malloc.\n");
		exit(1);
	}
	while((c = fgetc(IN)) != EOF){
		if((in_head == 0)&&(in_body == 0)){
			if(c == SH){
				in_head = 1;
			}
		} else if((in_head == 0)&&(in_body == 1)){
			if(c == SH){
				in_body = 0;
				in_head = 1;
				bf_body[pbfb] = '\0';
				strcpy((fst+pfst)->body,bf_body);
				pbfb = 0;
				pfst++;		/* only if in body */
			} else if(c == LB){
			} else if(c == ' '){
			} else if(c == '\t'){
			} else {
				bf_body[pbfb] = c;
				pbfb++;
			}
		} else if((in_head == 1)&&(in_body == 0)){
			if(c == LB){
				bf_head[pbfh] = '\0';
				strcpy((fst+pfst)->head,bf_head);
				pbfh = 0;
				in_head = 0;
				in_body = 1;
			} else {
				bf_head[pbfh] = c;
				pbfh++;
			}
		} else if((in_head == 1)&&(in_body == 1)){
			fprintf(stderr,"[C]internal data contradiction, check source code.\n");
		}
	}
	if((in_head == 0)&&(in_body == 1)){
		bf_body[pbfb] = '\0';
		strcpy((fst+pfst)->body,bf_body);
	} else if((in_head == 1)&&(in_body == 0)){
		bf_head[pbfh] = '\0';
		strcpy((fst+pfst)->head,bf_head);
	}
	pfst++;
	return(pfst);
};

void mkoligonuc(int let, char **sequence_array){
	int num;
	char str[(sizeof(int)*8)+1];
	int i;
	int j;
	int times;
	num = pow(4,let);
	times = DIGIT*let;
	for(i=0;i<num;i++){
		sprint_bin(i,str,times);
		for(j=0;j<let;j++){
			if(strncmp(str+(DIGIT*j),"00",2) == 0){
				sequence_array[i][j] = 'A';
			} else if(strncmp(str+(DIGIT*j),"01",2) == 0){
				sequence_array[i][j] = 'C';
			} else if(strncmp(str+(DIGIT*j),"10",2) == 0){
				sequence_array[i][j] = 'G';
			} else if(strncmp(str+(DIGIT*j),"11",2) == 0){
				sequence_array[i][j] = 'T';
			}
		}
		sequence_array[i][let] = '\0';
	}
};

int count_pattern(char *query, char *base, int gap, int direct_print){
	int c = 0;
	int i = 0;
	int q_len = 0;
	int b_len = 0;
	int loop = 0;
	int tmp_c = 0;
	q_len = strlen(query);
	b_len = strlen(base);
	for(i=0;i<b_len;i++){
		tmp_c = toupper(base[i]);
		base[i] = tmp_c;
	}
	if((loop = b_len - q_len + 1) <= 0){
		return(-1);
	}
	for(i=0;i<loop;i+=gap){
		if(strncmp(base+i,query,q_len) == 0){
			c++;
		}
	}
	if(direct_print == 1){
		printf("%d",c);
	}
	return(c);
};

float count_pattern_proportional(char *query, char *base, int gap, int direct_print, float total_propo){
	float c = 0;
	int i = 0;
	int q_len = 0;
	int b_len = 0;
	int loop = 0;
	int tmp_c = 0;
	q_len = strlen(query);
	b_len = strlen(base);
	for(i=0;i<b_len;i++){
		tmp_c = toupper(base[i]);
		base[i] = tmp_c;
	}
	if((loop = b_len - q_len + 1) <= 0){
		return(-1);
	}
	for(i=0;i<loop;i+=gap){
		if(strncmp(base+i,query,q_len) == 0){
			c++;
		}
	}
	c = c*total_propo/loop;
	if(direct_print == 1){
		printf("%f",c);
	}
	return(c);
};

