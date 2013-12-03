/* data_read.c */

/* NEEDS: string.h */

/*************************************************/
/* Copyright (c) 2003-2007 AMANO Kou             */
/* University of Liblary And Information Science */
/* National Institute of Agrobilogical Sciences  */
/* University of Tsukuba                         */
/* RIKEN BioResource Center                      */
/* kamano@affrc.go.jp                            */
/*************************************************/

#ifndef FS
#define FS ' '
#endif
#ifndef IDSIZE
#define IDSIZE 1024
#endif
#ifndef TMP_STR_LEN
#define TMP_STR_LEN 1024
#endif
#ifndef TMP_STR_LEN_LONG
#define TMP_STR_LEN_LONG 2048
#endif

int read_ftable_from_stream(int num, int dim, FILE *stream, float **table){
	int i;
	int j;
	float ftmp;
	for(i=0;i<num;i++){
		for(j=0;j<dim;j++){
			fscanf(stream,"%f",&ftmp);
			table[i][j] = ftmp;
		}
	}
	return(0);
}

int read_ID_ftable_from_stream(int num, int dim, FILE *stream, float **table, char **IDs){
	int i;
	int j;
	float ftmp;
	char ctmp[IDSIZE];
	for(i=0;i<num;i++){
		fscanf(stream,"%s",ctmp);
		strcpy(IDs[i],ctmp);
		for(j=1;j<dim+1;j++){
			fscanf(stream,"%f",&ftmp);
			table[i][j-1] = ftmp;
		}
	}
	return(0);
}

int read_ftable_from_stdin(int num, int dim, float **table){
	int i;
	int j;
	float ftmp;
	for(i=0;i<num;i++){
		for(j=0;j<dim;j++){
			scanf("%f",&ftmp);
			table[i][j] = ftmp;
		}
	}
	return(0);
}

int read_ID_ftable_from_stdin(int num, int dim, float **table, char **IDs){
	int i;
	int j;
	float ftmp;
	char ctmp[IDSIZE];
	for(i=0;i<num;i++){
		scanf("%s",ctmp);
		strcpy(IDs[i],ctmp);
		for(j=1;j<dim+1;j++){
			scanf("%f",&ftmp);
			table[i][j-1] = ftmp;
		}
	}
	return(0);
}

int read_flist_from_stream(int num, FILE *stream, float *list){
	int i;
	float ftmp;
	for(i=0;i<num;i++){
		fscanf(stream,"%f",&ftmp);
		list[i] = ftmp;
	}
	return(0);
}

int read_flist_from_string(char *string, float *list){
	int i = 0;
	int len = 0;
	int in_term = 0;
	int in_fs = 0;
	int list_ptr = 0;
	len = strlen(string);
	/* drop last FSs */
	while(*(string+len-1) == FS){
		*(string+len) = '\0';
		len--;
	}
	sscanf(string+i,"%f",list+list_ptr);
	in_fs++;
	list_ptr++;
	for(i=0;i<len;i++){
		if(*(string+i) != FS){
			in_term++;
			in_fs = 0;
		}else{
			in_term = 0;
			in_fs++;
		}
		if(in_fs == 1){
			sscanf(string+i,"%f",list+list_ptr);
			list_ptr++;
		}
	}
	return(list_ptr);
}

int read_ilist_from_stream(int num, FILE *stream, int *list){
	int i;
	int itmp;
	for(i=0;i<num;i++){
		fscanf(stream,"%d",&itmp);
		list[i] = itmp;
	}
	return(0);
}

int read_flist_from_stdin(int num, float *list){
	int i;
	float ftmp;
	for(i=0;i<num;i++){
		scanf("%f",&ftmp);
		list[i] = ftmp;
	}
	return(0);
}

int read_ilist_from_stdin(int num, int *list){
	int i;
	int itmp;
	for(i=0;i<num;i++){
		scanf("%d",&itmp);
		list[i] = itmp;
	}
	return(0);
}

void read_similarity_table_from_stream(FILE *stream, float **sim_table){
	int x;
	int y;
	float sim;
	char line[TMP_STR_LEN_LONG];
	while(fgets(line,TMP_STR_LEN_LONG,stream) != NULL){
		sscanf(line,"%d %d %f",&x,&y,&sim);
		sim_table[x][y] = sim;
	}
}

void read_symmetrical_similarity_table_from_stream(FILE *stream, float **sim_table){
	int x;
	int y;
	float sim;
	char line[TMP_STR_LEN_LONG];
	while(fgets(line,TMP_STR_LEN_LONG,stream) != NULL){
		sscanf(line,"%d %d %f",&x,&y,&sim);
		sim_table[x][y] = sim;
		sim_table[y][x] = sim;
	}
}

