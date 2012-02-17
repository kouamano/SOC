/* soc.c */

/*******************************************************/
/* Copyright 2004-2010 AMANO Kou <kamano@affrc.go.jp>  */
/* National Institute of Agrobiological Sciences       */
/* University of Library and Information Science       */
/* University of Tsukuba                               */
/* RIKEN BioResource Center                            */
/*******************************************************/

/* version information,
 * diffinitions of structure,
 * structure operations,
 * input/output functions.
 */

/* (*version infomation */
static char version_info[128] =	{"        VER. 0.5.9-00  /  2010.08.30 JST       "};
#ifdef SOC_LM
static char title[128] =	{"        SOC - Self Organizing Clustering       "};
static char author[128] =	{"                  by AMANO Kou                 "};
static char affiliation[128] =	{"       @  NIAS / ULIS / U.Tsukuba / RIKEN      "};
static char head[128] =		{"               "};
static char bar[128] =		{"-----------------------------------------------"};
#endif
/* *) */

/* (* define list */
#define SHORT_STRING_LEN 32
#define MIDLE_STRING_LEN 256
#define LONG_STRING_LEN 1000000
#define STRING_LEN 2056
#define TMP_STRING_LEN 2048
#define FILE_NAME_LEN 2048
#define ID_LEN 1024
/*#define ID_LIST_ALLOC 1000000*/
#ifndef BRK_PRINT
#define BRK_PRINT 1             /* bracket option : mathematica - 1, lisp - 2 */
#endif
#if BRK_PRINT == 1
#define BRK_S "{"
#define BRK_E "}"
#define DELIM ","
#elif BRK_PRINT == 2
#define BRK_S "'("
#define BRK_E ")"
#define DELIM " "
#endif
#define COOD_OP_LEN 4096
#define CL_DELIM '/'
#define CL_DELIM_STR "/"
/* *) */

/* (*for soc-csort, soc-dsort, soc-smalldsort, soc-smallestdsort, soc-init soc-lm */
#ifndef SOC_SPLIT
struct status{
	int pos;
	float value;
	float arranged_value;
};

struct status *status_alloc_list(long int m){
        struct status *l;
        l = malloc((size_t)sizeof(struct status)*m);
        if(l == NULL){
                fprintf(stderr,"[E]failed : status_alloc_list().\n");
                exit(1);
        }
        return(l);
}

struct node_dist_status {
	char id[ID_LEN];
	float dist;
};

struct node_dist_status *node_dist_alloc_list(long int m){
	struct node_dist_status *l;
	l = malloc((size_t)sizeof(struct node_dist_status)*m);
	if(l == NULL){
		fprintf(stderr,"[E]failed : node_dist_alloc_list().\n");
		exit(1);
	}
	return(l);
}

int cmp_dist(struct status *x, struct status *y){
	if((*x).value < (*y).value){
		return(-1);
	}else if((*x).value > (*y).value){
		return(1);
	}else{
		return(0);
	}
}

int alt_cmp_dist(struct status *x, struct status *y){
	if((*x).value < (*y).value){
		return(1);
	}else if((*x).value > (*y).value){
		return(-1);
	}else{
		return(0);
	}
}

int cmp_node_dist(struct node_dist_status *x, struct node_dist_status *y){
	if((*x).dist < (*y).dist){
		return(-1);
	}else if((*x).dist > (*y).dist){
		return(1);
	}else{
		return(0);
	}
}

void count_column_from_stream(FILE *IFP, int *_lines, int *_maxcol, int *_mincol){
	int c = 0;
	//int i = 0;
	int IN_TERM = 0;
	int lines = 0;
	int max_col = 0;
	int min_col = 0;
	int columns = 0;
	while((c = fgetc(IFP)) != '\n'){
                if((c == ' ') || (c == '\t')){
                        IN_TERM = 0;
                }else{
                        IN_TERM++;
                }
                if(IN_TERM == 1){
                        columns++;
                }
	}
        IN_TERM = 0;
        lines++;
        max_col = columns;
        min_col = columns;
        columns = 0;
	while((c = fgetc(IFP)) != EOF){
                if(c == '\n'){
                        max_col = max(max_col,columns);
                        min_col = min(min_col,columns);
                        columns = 0;
                        lines++;
                        IN_TERM=0;
                }
                else if((c == ' ') || (c == '\t')){
                        IN_TERM=0;
                }
                else{
                        IN_TERM++;
                }
                if(IN_TERM == 1){
                        columns++;
                }
	}
	*_lines = lines;
	*_maxcol = max_col;
	*_mincol = min_col;
}
#endif
/* *) */

/* (* for soc-init */
#ifdef SOC_INIT
struct st_ax_and_mean {
	int ax;
	float mean;
};

void mat_TO_st_ax_and_mean(int dim, int num, float **mat, struct st_ax_and_mean *ax_mean){
	int i;
	int j;
	for(j=0;j<dim;j++){
		ax_mean[j].ax = j;
		ax_mean[j].mean = 0;
	}
	for(i=0;i<num;i++){
		for(j=0;j<dim;j++){
			ax_mean[j].mean =  ax_mean[j].mean + mat[i][j];
		}
	}
	for(j=0;j<dim;j++){
		ax_mean[j].mean = ax_mean[j].mean/num;
	}
}

int cmp_ax_and_mean(struct st_ax_and_mean *x, struct st_ax_and_mean *y){
	if((*x).mean < (*y).mean){
		return(1);
	}else if((*x).mean > (*y).mean){
		return(-1);
	}else{
		return(0);
	}
}
#endif
/* *) */

/* (*for soc-lm */
#ifdef SOC_LM
struct clst_status{
	//int num_data_node;
	int num_smpl_node;
	int num_arranged_smpl_node;
	//int *id_data_node;
	int *id_smpl_node;
	int *id_arranged_smpl_node;
	float sum_dist;
	float ave_dist;
};

struct clst_status *clst_status_alloc_list(long int m){
        struct clst_status *l;
        l = malloc((size_t)sizeof(struct clst_status)*m);
        if(l == NULL){
                fprintf(stderr,"[E]failed : clst_status_alloc_list().\n");
                exit(1);
        }
        return(l);
}

void f_max_status_list(int len, float *list, struct status *attrb_status, int pos_strct, int set_mode){
        int l;
        int pos_max;
        float f_max;
        pos_max = 0;
        f_max = list[0];
        for(l=1;l<len;l++){
                if(f_max <= list[l]){
                        pos_max = l;
                        f_max = list[l];
                }else{

		}
        }
	attrb_status[pos_strct].pos = pos_max;
	if((set_mode&1) == 1){
		attrb_status[pos_strct].value = f_max;
	}
	if((set_mode&2) == 2){
		attrb_status[pos_strct].arranged_value = f_max;
	}
}

void f_min_status_list(int len, float *list, struct status *attrb_status, int pos_strct, int set_mode){
        int l;
        int pos_min;
        float f_min;
        pos_min = 0;
        f_min = list[0];
        for(l=1;l<len;l++){
                if(f_min >= list[l]){
                        pos_min = l;
                        f_min = list[l];
                }else{

		}
        }
	attrb_status[pos_strct].pos = pos_min;
	if((set_mode&1) == 1){
		attrb_status[pos_strct].value = f_min;
	}
	if((set_mode&2) == 2){
		attrb_status[pos_strct].arranged_value = f_min;
	}
}

void move_write_matrix(int num, int dim, float **src_matrix, float **ref_matrix, float rate){
	int l;
	int m;
	for(l=0;l<num;l++){
		for(m=0;m<dim;m++){
			src_matrix[l][m] = src_matrix[l][m] + rate*(ref_matrix[l][m]);
		}
	}
}

void merge_list(int dim, float *list1, float *list2, float *listout){
	int l;
	for(l=0;l<dim;l++){
		listout[l] = (list1[l] + list2[l])/2;
	}
}
/*
void mk_flag_list(int num, int num_flag, int *flaged_list, int *out_list){
	int l;
	int m;
	for(l=0;l<num;l++){
		out_list[l] = 0;
	}
	for(l=0;l<num;l++){
		for(m=0;m<num_flag;m++){
			if(l == flaged_list[m]){
				out_list[l] = 1;
			}
		}
	}
}
*/
#endif
/* *) */

/* (*file operations */
#ifndef SOC_SPLIT
int ck_data_format(char *file_name){
	FILE *in;
	int dim = -1;
	int num = -1;
	int elm = 0;
	int i = 0;
	signed char c;
	char *tmp_str;
	int alloc_tmp_str_len = 0;
	int int_tmp1 = 0;
	int int_tmp2 = 0;
	int len = 0;
	int in_ELM = 0;
	int in_SP = 0;
	int in_COUNT = 0;

	/* (* temporary line allocation */
	in = fopen(file_name,"r");
	if(in == NULL){
		perror(file_name);
		exit(1);
	}
	while((c = fgetc(in)) != '\n'){
		int_tmp1++;
	}
	fprintf(stderr,"first line length:%d:\n",int_tmp1);
	while((c = fgetc(in)) != '\n'){
		int_tmp2++;
	}
	fprintf(stderr,"2nd line length:%d:\n",int_tmp2);
	fclose(in);
	alloc_tmp_str_len = 2*max(int_tmp1,int_tmp2);
	fprintf(stderr,"temporary alloc line length:%d:\n",alloc_tmp_str_len);
	if((tmp_str = malloc(sizeof(char)*alloc_tmp_str_len)) == NULL){
		fprintf(stderr,"failed malloc for tmp_str.\n");
		exit(1);
	}
	/* *) */

	in = fopen(file_name,"r");
	if(in == NULL){
		perror(file_name);
		exit(1);
	}
	read_ilist_from_stream(1,in,&dim);
	read_ilist_from_stream(1,in,&num);
	//fprintf(stderr,"-----%c-----",c);
	while((c = fgetc(in)) != '\n');
	while((c = fgetc(in)) != '\n'){
		tmp_str[i] = c;
		i++;
	}
	tmp_str[i] = '\0';
	fclose(in);
	len = strlen(tmp_str);
	i = 0;
	for(i=0;i<len;i++){
		if(tmp_str[i] == ' '){
			in_SP++;
			in_ELM = 0;
			in_COUNT = 0;
			//printf("[PS]");
		}
		if(tmp_str[i] == '\t'){
			in_SP++;
			in_ELM = 0;
			in_COUNT = 0;
			//printf("[TB]");
		}
		if((tmp_str[i] != ' ')&&(tmp_str[i] != '\t')){
			in_ELM++;
			in_SP = 0;
			//printf("[CH]");
		}
		if((in_ELM == 1)&&(in_COUNT == 0)){
			elm++;
			in_COUNT = 1;
			//printf("[CT]");
		}
	}
	if(dim+1 == elm){
		return(2);	/*with sample title*/
	}else if(dim == elm){
		return(1);	/*without sample title*/
	}else{
		return(0);	/*error*/
	}
}
#endif
/* *) */
