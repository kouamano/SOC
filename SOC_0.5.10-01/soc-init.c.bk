/* soc-init.c */

/*************************************************/
/* Copyright (c) 2004-2013  AMANO, Kou           */
/* National Institute of Agrobiological Sciences */
/* University of Library And Information Science */
/* University of Tsukuba                         */
/* RIKEN BioResource Center                      */
/* kamano@affrc.go.jp                            */
/*************************************************/

/**TODO*********************/
/* revise scale ************/
/***************************/

#define SOC_INIT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "soc.h"
#include "files.h"
#include "soc.c"

char sample_file[FILE_NAME_LEN];
char cluster_file[FILE_NAME_LEN];
char coordinate_option[COOD_OP_LEN];
char axis_str[TMP_STRING_LEN];
long seed = 0;
int **int_rand_order_matrix = NULL;
float **dim_min_to_max_matrix = NULL;	/*N*dim*/
float **out_matrix = NULL;		/*N*dim not alloced, copy only*/
float **num_min_to_max_matrix = NULL;	/*N*num*/
float **dim_min_and_max_table = NULL;	/*2*dim*/
float **num_min_and_max_table = NULL;	/*2*num*/
float *origin_data_dist_list = NULL;
float *origin = NULL;
time_t t = 0;
char **sample_id = NULL;
float **data_matrix = NULL;
float **clst_matrix = NULL;
int data_format = 3;			/*AND 1 == 1: with matrix size, AND 2 == 2: with sample title*/
int detected_data_format = 0;		/*0:error, 1: without sample title, 2: with sample title*/
int of_format = 1;
int dim_data = 0;
int num_data = 0;
int dim_clst = 0;
int num_clst = 0;
float *centroid = NULL;
struct status *id_distance = NULL;
int ordered_output = 0;
int ordered_start = 0;
int partition = 0;
float *parted_values = NULL;
int *bins = NULL;
//int median_range = 0;
int median_count = 0;
float median_value = 0;
float max_dist = 0;
float min_dist = 0;
float scale = 0;


void message(void){
	printf("\n");
	printf("                    ");
	printf("soc-init\n\n");
	printf("        ");
	printf("initialize cluster-node position\n\n");
	printf("%s\n",version_info);
	printf("          \n");
}

void usage(void){
	printf("USAGE: \n");
	printf("  soc-init -h\n");
	printf("  soc-init if=<sample data file> of=<output data file> [frac=<number of cluster nodes> <FORMAT>]\n");
}

void options(void){
	printf("\nFORMAT: \n");
	printf("  Through\n");
	printf("  Diagonal\n");
	printf("  random=Order\n");
	printf("  random=Value\n");
	printf("  node=Central\n");
	printf("  node=outLying\n");
	printf("  node=Intensive\n");
	printf("  node=Power\n");
	printf("  node=diStant\n");
	printf("  node=Median\n");
	printf("  node=Median,<n> (Under construction)\n");
	printf("  Grid=<tensor size>\n");
	printf("  Axis-mean=<axis1,axis2,...>\n");
	printf("  aXis-intensive=<ratio>\n");
	printf("  scale=<scale for max-min>\n");
}

int get_options(int arg_start, int arg_stop, char **arg_s){
	int i = 0;
	int s = 0;
	s = 0;
	for(i=arg_start;i<arg_stop;i++){
		if(strncmp(arg_s[i],"if=",3) == 0){
			sscanf(arg_s[i],"if=%s",sample_file);
			if((s&2) == 0){
				s = s + 2;
			}
		}else if(strncmp(arg_s[i],"of=",3) == 0){
			sscanf(arg_s[i],"of=%s",cluster_file);
			if((s&4) == 0){
				s = s + 4;
			}
		}else if(strncmp(arg_s[i],"frac=",5) == 0){
			sscanf(arg_s[i],"frac=%d",&num_clst);
			if((s&8) == 0){
				s = s + 8;
			}
		}else if(strncmp(arg_s[i],"random=",7) == 0){
			sscanf(arg_s[i],"%s",coordinate_option);
			ordered_output = 0;
			if((s&16) == 0){
				s = s + 16;
			}
		}else if(strcmp(arg_s[i],"Diagonal") == 0){
			sscanf(arg_s[i],"%s",coordinate_option);
			ordered_output = 0;
			if((s&16) == 0){
				s = s + 16;
			}
		}else if(strncmp(arg_s[i],"node=",5) == 0){
			sscanf(arg_s[i],"%s",coordinate_option);
			ordered_output = 1;
			if((s&16) == 0){
				s = s + 16;
			}
		}else if(strncmp(arg_s[i],"Through",7) == 0){
			sscanf(arg_s[i],"%s",coordinate_option);
			ordered_output = 0;
			if((s&16) == 0){
				s = s + 16;
			}
		}else if(strncmp(arg_s[i],"Grid=",5) == 0){
			sscanf(arg_s[i],"%s",coordinate_option);
			ordered_output = 0;
			if((s&8) == 0){
				s = s + 8;
			}
			if((s&16) == 0){
				s = s + 16;
			}
		}else if(strncmp(arg_s[i],"Axis-mean=",10) == 0){
			sscanf(arg_s[i],"%s",coordinate_option);
			ordered_output = 0;
			if((s&8) == 0){
				s = s + 8;
			}
			if((s&16) == 0){
				s = s + 16;
			}
		}else if(strncmp(arg_s[i],"aXis-intensive=",15) == 0){
			sscanf(arg_s[i],"%s",coordinate_option);
			ordered_output = 0;
			if((s&16) == 0){
				s = s + 16;
			}
		}else if(strncmp(arg_s[i],"seed=",5) == 0){
			sscanf(arg_s[i],"seed=%ld",&seed);
			if((s&32) == 0){
				s = s + 32;
			}
		}else if(strncmp(arg_s[i],"if_form=",8) == 0){
			sscanf(arg_s[i],"if_form=%d,%d,%d",&data_format,&dim_data,&num_data);
			if((s&64) == 0){
				s = s + 64;
			}
		}else if(strncmp(arg_s[i],"of_form=",8) == 0){
			sscanf(arg_s[i],"of_format=%d",&of_format);
			if((s&128) == 0){
				s = s + 128;
			}
		}else if(strncmp(arg_s[i],"scale=",6) == 0){
			sscanf(arg_s[i],"scale=%f",&scale);

		}else{
			printf("unknown option.\n");
			exit(1);
		}
	}
	return(s);
}

int *get_tensor_dimANDrank(const int rank, char *str){
	int i = 0;
	int j = 0;
	int k = 0;
	int *dim = NULL;
	int local_rank = 0;
	int len = 0;
	char tmp_c[TMP_STRING_LEN];
	int tmp = 0;
	len = strlen(str);
	for(i=0;i<len;i++){
		if(str[i] == ','){
			local_rank++;
		}
	}
	local_rank++;
	if(local_rank > rank){
		printf("local_rank:%d\n",local_rank);
		printf("the tensor rank exceeded dimensions of cluster.\n");
		exit(1);
	}
	if((dim = malloc((size_t)sizeof(int)*rank)) == NULL){
		printf("failed : malloc() at %ld byte.\n",(long int)sizeof(int)*rank);
		exit(1);
	}
	for(i=0;i<len;i++){
		if(str[i] != ','){
			tmp_c[j] = str[i];
			j++;
		}else{
			tmp_c[j] = '\0';
			sscanf(tmp_c,"%d",&tmp);
			dim[k] = tmp;
			k++;
			j = 0;
		}
	}
	tmp_c[j] = '\0';
	sscanf(tmp_c,"%d",&tmp);
	dim[k] = tmp;
	k++;
	for(i=k;i<rank;i++){
		dim[i] = 1;
	}
	return(dim);
}

int **make_outer_tensor_position_matrix(int rank, int *n_elm, int *dim){
	int i = 0;
	int j = 0;
	int *prod = NULL;
	int elm = 0;
	int tmp = 0;
	float f = 0;
	int **out_mat;
	if((prod = malloc((size_t)sizeof(int)*rank)) == NULL){
		printf("failed malloc() at %ld byte.\n",(long int)sizeof(int)*rank);
		exit(1);
	}
	elm = 1;
	for(i=0;i<rank;i++){
		elm = dim[i] * elm;
	}
	out_mat = i_alloc_mat(elm,rank);
	j = 1;
	tmp = 1;
	prod[0] = 1;
	for(i=1;i<rank;i++){
		tmp = tmp * dim[i-1];
		prod[j] = tmp;
		j++;
	}
	for(i=0;i<elm;i++){
		for(j=0;j<rank;j++){
			f = (float)i/prod[j];
			out_mat[i][j] = ((int)f)%dim[j];
		}
	}
	*n_elm = elm;
	return(out_mat);
}

float *make_min_to_max_list(int fraction, float min, float max){
	int i = 0;
	int j = 0;
	int cut = 0;
	float segment_size = 1;
	float length = 1;
	float *out_list = NULL;
	cut = fraction*2;
	length = max - min;
	segment_size = length / cut;
	out_list = f_alloc_vec(fraction);
	j = 0;
	for(i=1;i<cut;i+=2){
		out_list[j] = min + segment_size * i;
		j++;
	}
	return(out_list);
}

void make_min_to_max_table(int fraction, int dim_or_num, float **min_and_max, float **_out_matrix){
	int i = 0;
	int j = 0;
	float *d = NULL;
	d = f_alloc_vec(dim_or_num);
	for(i=0;i<dim_or_num;i++){
		d[i] = (min_and_max[1][i] - min_and_max[0][i])/(float)fraction;
	}
	for(j=0;j<fraction;j++){
		for(i=0;i<dim_or_num;i++){
			_out_matrix[j][i] = min_and_max[0][i]+(j+1)*d[i]-(d[i]/2);
		}
	}
	free(d);
}



int main(int argc, char **argv){
	float **shake_dim_min_to_max_matrix = NULL;	/* N*dim */
	FILE *ifp = NULL;
	FILE *ofp = NULL;
	int with_sample_ID = 1;
	int arg_stat = 0;
	int i = 0;
	int j = 0;
	char dim_string[TMP_STRING_LEN];
	int grid_tensor_elms = 1;
	int grid_tensor_rank = 1;
	int *grid_tensor_dim = NULL;
	int **tensor_position_matrix = NULL;
	float **tensor_vars = NULL;

	/* temporary vars */
	int tmp_ptr_arr = 0;
	int tmp_ptr_word = 0;
	char *tmp_word;
	tmp_word = c_alloc_vec(SHORT_STRING_LEN);

	if(argc == 2){
		if(strcmp(argv[1],"-h") == 0){
			message();
			usage();
			options();
		}else{
			message();
			usage();
		}
	}else if(argc >= 4){
		arg_stat = get_options(1,argc,argv);
		if((arg_stat&22) != 22){
			usage();
			exit(1);
		}
		/* (* check file format */
		int max_col = 0;
		int min_col = 0;
		int lines = 0;
		if((ifp = fopen(sample_file,"r")) == NULL){
			perror(sample_file);
			exit(1);
		}
		read_ilist_from_stream(1,ifp,&dim_data);
		read_ilist_from_stream(1,ifp,&num_data);
		fgetc(ifp);
		count_column_from_stream(ifp,&lines,&max_col,&min_col);
		fclose(ifp);
		fprintf(stderr,"1st line information: dim:%d: num:%d:\n",dim_data,num_data);
		fprintf(stderr,"count: max:%d: min:%d: lines:%d:\n",max_col,min_col,lines);
		if(max_col != min_col){
			fprintf(stderr,"[EROOR] something wrong: max_col not min_col.\n");
			exit(1);
		}
		if(dim_data == max_col){
			fprintf(stderr,"dim_data == num of columns: with_sample_ID = 0.\n");
			with_sample_ID = 0;
		}else if((dim_data + 1) == max_col){
			fprintf(stderr,"dim_data + 1 == num of columns: with_sample_ID = 1.\n");
			with_sample_ID = 1;
		}
		/* *) */
		/* (* scan data, allocate mem, read data */
		if((ifp = fopen(sample_file,"r")) == NULL){
			perror(sample_file);
			exit(1);
		}
		read_ilist_from_stream(1,ifp,&dim_data);
		read_ilist_from_stream(1,ifp,&num_data);
		data_matrix = f_alloc_mat(num_data,dim_data);
		sample_id = c_alloc_mat(num_data,ID_LEN);
		if(with_sample_ID == 1){
			read_ID_ftable_from_stream(num_data,dim_data,ifp,data_matrix,sample_id);
		}else if(with_sample_ID == 0){
			read_ftable_from_stream(num_data,dim_data,ifp,data_matrix);
		}
		fclose(ifp);
		dim_clst = dim_data;
		dim_min_and_max_table = f_alloc_mat(2,dim_data);
		/* *) */
		/* (* set min_dim_list to dim_min_and_max_table[0] */
		min_list_from_matrix(dim_data,num_data,data_matrix,dim_min_and_max_table[0],0);
		/* *) */
		/* (* set max_dim_list to dim_min_and_max_table[1] */
		max_list_from_matrix(dim_data,num_data,data_matrix,dim_min_and_max_table[1],0);
		/* *) */
		/* (* scaling 
		if(scale != 0){
			for(i=0;i<2;i++){
				for(j=0;j<dim_data;j++){
					dim_min_and_max_table[i][j] = dim_min_and_max_table[i][j]*scale;
				}
			}
		}
		*) */
		/* (* set ticks to dim_min_to_max_matrix */
		dim_min_to_max_matrix = f_alloc_mat(num_clst,dim_data);
		make_min_to_max_table(num_clst,dim_data,dim_min_and_max_table,dim_min_to_max_matrix);
		/* *) */
		/* (* change operation by coordinate_option */
		if((strcmp(coordinate_option,"Diagonal") == 0)||(strcmp(coordinate_option,"-D") == 0)){
			/* (* copy pointer for write to file */
			out_matrix = dim_min_to_max_matrix;
			/* *) */


		}else if(strcmp(coordinate_option,"random=Order") == 0){
			/* (* generate random order */
			int_rand_order_matrix = i_alloc_mat(dim_data,num_clst);
			t = time(NULL);
			for(i=0;i<dim_data;i++){
				srand(t+33*i);
				mk_int_rand_order_list(0,num_clst-1,int_rand_order_matrix[i],t+pow(5,i)+(13*i)+seed);
			}
			/* *) */
			/* (* set shake_dim_min_to_max_matrix from random_order & min_max_matrix */
			shake_dim_min_to_max_matrix = f_alloc_mat(num_clst,dim_data);
			for(j=0;j<num_clst;j++){
				for(i=0;i<dim_data;i++){
					shake_dim_min_to_max_matrix[j][i] = dim_min_to_max_matrix[int_rand_order_matrix[i][j]][i];
				}
			}
			/* *) */
			/* (* copy pointer for write to file */
			out_matrix = shake_dim_min_to_max_matrix;
			/* *) */


		}else if(strcmp(coordinate_option,"random=Value") == 0){
			t = time(NULL);
			dim_min_to_max_matrix = f_alloc_mat(num_clst,dim_data);
			mk_rand_coordinate(num_clst,dim_data,dim_min_and_max_table,dim_min_to_max_matrix,t+seed);
			out_matrix = dim_min_to_max_matrix;


		}else if(strcmp(coordinate_option,"node=Central") == 0){
			/* check num */
			if(num_data < num_clst){
				printf("%d,%d\n",num_data,num_clst);
				fprintf(stderr,"Warn : number of clusters exceeded number of samples.\n");
				exit(1);
			}

			/* init id_distance */
			id_distance = status_alloc_list(num_data);
			for(i=0;i<num_data;i++){
				id_distance[i].pos = i;
				id_distance[i].value = 0;
			}

			/* set centroid */
			centroid = f_calloc_vec(dim_data);
			for(i=0;i<num_data;i++){
				for(j=0;j<dim_data;j++){
					centroid[j] += data_matrix[i][j];
				}
			}
			for(j=0;j<dim_data;j++){
				centroid[j] = centroid[j]/num_data;
			}

			/* set id_distance */
			for(i=0;i<num_data;i++){
				id_distance[i].value = euc_dist(dim_data,centroid,data_matrix[i]);
			}

			/* sort id_distance by distance */
			qsort(id_distance,num_data,sizeof(struct status),(int(*)(const void*, const void*))cmp_dist);
			out_matrix = data_matrix;


		}else if(strcmp(coordinate_option,"node=diStant") == 0){
			if(num_data < num_clst){
				fprintf(stderr,"Warn : number of clusters exceeded number of samples.\n");
				exit(1);
			}
			origin = f_calloc_vec(dim_data);
			origin_data_dist_list = f_alloc_vec(num_data);
			for(i=0;i<num_data;i++){
				origin_data_dist_list[i] = euc_dist(dim_data,origin,data_matrix[i]);
			}
			id_distance = status_alloc_list(num_data);
			for(i=0;i<num_data;i++){
				id_distance[i].pos = i;
				id_distance[i].value = origin_data_dist_list[i];
			}
			qsort(id_distance,num_data,sizeof(struct status),(int(*)(const void*, const void*))alt_cmp_dist);
			out_matrix = data_matrix;


		}else if(strcmp(coordinate_option,"node=Power") == 0){
			if(num_data < num_clst){
				fprintf(stderr,"Warn : number of clusters exceeded number of samples.\n");
				exit(1);
			}
			id_distance = status_alloc_list(num_data);
			for(i=0;i<num_data;i++){
				id_distance[i].pos = i;
				id_distance[i].value = 0;
			}
			for(i=0;i<num_data;i++){
				for(j=0;j<dim_data;j++){
					id_distance[i].value = id_distance[i].value + data_matrix[i][j];
				}
			}
			qsort(id_distance,num_data,sizeof(struct status),(int(*)(const void*, const void*))alt_cmp_dist);
			out_matrix = data_matrix;


		}else if(strcmp(coordinate_option,"node=outLying") == 0){
			if(num_data < num_clst){
				fprintf(stderr,"Warn : number of clusters exceeded number of samples.\n");
				exit(1);
			}
			id_distance = status_alloc_list(num_data);
			for(i=0;i<num_data;i++){
				id_distance[i].pos = i;
				id_distance[i].value = 0;
			}

			centroid = f_calloc_vec(dim_data);
			for(i=0;i<num_data;i++){
				for(j=0;j<dim_data;j++){
					centroid[j] += data_matrix[i][j];
				}
			}
			for(j=0;j<dim_data;j++){
				centroid[j] = centroid[j]/num_data;
			}
			for(i=0;i<num_data;i++){
				id_distance[i].value = euc_dist(dim_data,centroid,data_matrix[i]);
			}

			qsort(id_distance,num_data,sizeof(struct status),(int(*)(const void*, const void*))alt_cmp_dist);
			out_matrix = data_matrix;


		}else if(strcmp(coordinate_option,"node=Intensive") == 0){
			if(num_data < num_clst){
				fprintf(stderr,"Warn : number of cluster exceeded number of samples.\n");
				exit(1);
			}
			id_distance = status_alloc_list(num_data);
			for(i=0;i<num_data;i++){
				id_distance[i].pos = i;
				id_distance[i].value = 0;
			}
			for(i=0;i<num_data;i++){
				for(j=0;j<=i;j++){
					id_distance[i].value += euc_dist(dim_data,data_matrix[i],data_matrix[j]);
				}
			}
			for(i=0;i<num_data;i++){
				for(j=num_data-1;j>i;j--){
					id_distance[i].value += euc_dist(dim_data,data_matrix[j],data_matrix[i]);
				}
			}
			qsort(id_distance,num_data,sizeof(struct status),(int(*)(const void*, const void*))cmp_dist);
			out_matrix = data_matrix;


		}else if(strncmp(coordinate_option,"node=Median",11) == 0){
			//fprintf(stderr,"HOGE!!\n");
			double delta = 0;
			int data_ptr = 0;
			int bin_ptr = 0;
			int total_bin_count = 0;
			//int tmp = 0;
			/* check num */
			if(num_data < num_clst){
				printf("%d,%d\n",num_data,num_clst);
				fprintf(stderr,"Warn : number of clusters exceeded number of samples.\n");
				exit(1);
			}

			/* init id_distance */
			id_distance = status_alloc_list(num_data);
			for(i=0;i<num_data;i++){
				id_distance[i].pos = i;
				id_distance[i].value = 0;
			}

			/* set centroid */
			centroid = f_calloc_vec(dim_data);
			for(i=0;i<num_data;i++){
				for(j=0;j<dim_data;j++){
					centroid[j] += data_matrix[i][j];
				}
			}
			for(j=0;j<dim_data;j++){
				centroid[j] = centroid[j]/num_data;
			}

			/* set id_distance */
			for(i=0;i<num_data;i++){
				id_distance[i].value = euc_dist(dim_data,centroid,data_matrix[i]);
			}
			

			/* sort id_distance by distance */
			qsort(id_distance,num_data,sizeof(struct status),(int(*)(const void*, const void*))cmp_dist);
			max_dist = id_distance[num_data-1].value;
			min_dist = id_distance[0].value;
			fprintf(stderr,"max_dist:%f:\n",max_dist);
			fprintf(stderr,"min_dist:%f:\n",min_dist);
			out_matrix = data_matrix;

			/* set partition */
			partition = (int)num_data/num_clst;
			fprintf(stderr,"partition:%d:\n",partition);
			parted_values = f_calloc_vec(partition);
			delta = (max_dist - min_dist)/partition;
			fprintf(stderr,"delta:%f:\n",delta);
			for(i=0;i<partition;i++){
				parted_values[i] = min_dist + (i * delta); 
				//fprintf(stderr,"%f\n",parted_values[i]);
			}

			/* (* UNDER CHECK */
			bins = i_calloc_vec(partition);
			data_ptr = 0;
			bin_ptr = 0;
			total_bin_count = 0;
			//median_range = 0;
			median_count = 0;
			median_value = 0;
			ordered_start = 0;
			for(i=0;i<num_data;i++){
				if(bin_ptr >= partition){
					break;
				}
				if(id_distance[i].value <= parted_values[bin_ptr+1]){
					total_bin_count++;
					bins[bin_ptr]++;
				}else{
					if(bins[bin_ptr] > median_count){
						median_count = bins[bin_ptr];
						median_value = parted_values[bin_ptr];
						ordered_start = total_bin_count - bins[bin_ptr];
					}
					bin_ptr++;
				}
			}
			//printf(":::%d:::\n",num_data);
			//printf(":::%d:::\n",num_data - total_bin_count);
			bins[bin_ptr-1] = (num_data - total_bin_count);

			/* test 
			for(i=0;i<partition;i++){
				fprintf(stderr,"%f\n",parted_values[i]);
				fprintf(stderr,"    %d\n",bins[i]);
				tmp = tmp + bins[i];
				fprintf(stderr,"      %d\n",tmp);
			}
			fprintf(stderr,"start:%d:\n",ordered_start);
			 */
			/* *) */


		}else if(strcmp(coordinate_option,"Through") == 0){
			num_clst = num_data;
			out_matrix = data_matrix;


		}else if(strncmp(coordinate_option,"Grid=",5) == 0){
			sscanf(coordinate_option,"Grid=%s",dim_string);
			grid_tensor_rank = dim_data;
			grid_tensor_dim = get_tensor_dimANDrank(grid_tensor_rank,dim_string);
			tensor_position_matrix = make_outer_tensor_position_matrix(dim_data,&grid_tensor_elms,grid_tensor_dim);
			num_clst = grid_tensor_elms;
			if((tensor_vars = malloc((size_t)sizeof(float*)*grid_tensor_rank)) == NULL){
				printf("failed : malloc() at %ld byte.\n",(long int)sizeof(float*)*grid_tensor_rank);
				exit(1);
			}
			for(i=0;i<grid_tensor_rank;i++){
				tensor_vars[i] = make_min_to_max_list(grid_tensor_dim[i],dim_min_and_max_table[0][i],dim_min_and_max_table[1][i]);
			}
			out_matrix = f_alloc_mat(grid_tensor_elms,grid_tensor_rank);
			for(i=0;i<grid_tensor_elms;i++){
				for(j=0;j<dim_data;j++){
					out_matrix[i][j] = tensor_vars[j][tensor_position_matrix[i][j]];
				}
			}


		}else if(strncmp(coordinate_option,"Axis-mean=",10) == 0){
			int axis_str_len = 0;
			int axis_list_len = 0;
			int *axis_list = NULL;
			double *axis_v_list = NULL;
			sscanf(coordinate_option,"Axis-mean=%s",axis_str);
			axis_str_len = strlen(axis_str);
			for(i=0;i<axis_str_len;i++){
				if(axis_str[i] == ','){
					axis_list_len++;
				}
			}
			axis_list_len++;
			axis_list = i_alloc_vec(axis_list_len);
			axis_v_list = d_calloc_vec(axis_list_len);
			for(i=0;i<axis_str_len;i++){
				if(axis_str[i] == ','){
					tmp_word[tmp_ptr_word] = '\0';
					sscanf(tmp_word,"%d",&axis_list[tmp_ptr_arr]);
					tmp_ptr_arr++;
					tmp_ptr_word = 0;
				}else{
					tmp_word[tmp_ptr_word] = axis_str[i];
					tmp_ptr_word++;
				}
			}
			tmp_word[tmp_ptr_word] = '\0';
			sscanf(tmp_word,"%d",&axis_list[tmp_ptr_arr]);
			tmp_ptr_arr++;
			tmp_ptr_word = 0;
			/* num_clst */
			num_clst = axis_list_len;
			/* out_matrix alloc */
			out_matrix = f_calloc_mat(num_clst,dim_data);
			/* calculation */
			for(i=0;i<num_data;i++){
				for(j=0;j<axis_list_len;j++){
					axis_v_list[j] += data_matrix[i][axis_list[j]];
				}
			}
			for(j=0;j<axis_list_len;j++){
				axis_v_list[j] = axis_v_list[j]/num_data;
				out_matrix[j][axis_list[j]] = (float)axis_v_list[j];
			}

		}else if(strncmp(coordinate_option,"aXis-intensive=",15) == 0){
			/* declear and allocation */
			float axis_ratio = 1;
			struct st_ax_and_mean *ax_mean;
			if(((ax_mean = malloc(sizeof(struct st_ax_and_mean) * dim_data)) == NULL)){ fprintf(stderr, "[E] failed: malloc().\n"); exit(0); }
			out_matrix = f_calloc_mat(dim_data,dim_data);
			/* ratio */
			sscanf(coordinate_option,"aXis-intensive=%f",&axis_ratio);
			mat_TO_st_ax_and_mean(dim_data, num_data, data_matrix, ax_mean);
			qsort(ax_mean,dim_data,sizeof(struct st_ax_and_mean),(int(*)(const void*, const void*))cmp_ax_and_mean);
			for(j=0;j<dim_data;j++){
				out_matrix[j][ax_mean[j].ax] = axis_ratio * ax_mean[j].mean;
			}
	
	
		}else{
			options();
			exit(1);
		}
		/* *) */
		/* (* open file and write shake_dim_min_to_max_matrix */
		ofp = fopen(cluster_file,"w");
		if(ofp == NULL){
			perror(cluster_file);
			exit(1);
		}
		if(ordered_output == 0){
			fprintf(ofp,"%d %d\n",dim_data,num_clst);
			for(j=0;j<num_clst;j++){
				for(i=0;i<dim_data;i++){
					fprintf(ofp,"%f ",out_matrix[j][i]);
				}
				fprintf(ofp,"\n");
			}
			fclose(ofp);
		}else{
			fprintf(ofp,"%d %d\n",dim_data,num_clst);
			for(j=ordered_start;j<num_clst+ordered_start;j++){
				for(i=0;i<dim_data;i++){
					fprintf(ofp,"%f ",out_matrix[id_distance[j].pos][i]);
				}
				fprintf(ofp,"\n");
			}
			fclose(ofp);
		}
		/* *) */

	}else{
		message();
		usage();
	}
	return(0);
}
