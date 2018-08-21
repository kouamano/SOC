/* soc-dsort.c */

/*************************************************/
/* Copyright (c) 2003-2005 AMANO Kou             */
/* University of Tsukuba                         */
/* University of Library And Information Science */
/* National Institute of Agrobilogical Sciences  */
/* kamano@affrc.go.jp                            */
/*************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>

#include "soc.h"
#include "files.h"
#include "soc.c"

int main(int argc, char **argv){
	FILE *ifp = NULL;
	char sample_file[FILE_NAME_LEN];
	int dim_data = 0;
	int num_data = 0;
	float **data_matrix = NULL;
	char **data_id = NULL;
	int format = 0;
	struct node_dist_status *idANDdistance = NULL;
	float **d_table = NULL;
	int i = 0;
	int j = 0;
	if(argc == 2){
		sscanf(argv[1],"%s",sample_file);
		format = ck_data_format(sample_file);
		if(format == 0){
			printf("check file format.\n");
			exit(1);
		}else if(format == 1){
			if((ifp = fopen(sample_file,"r")) == NULL){
				perror(sample_file);
				exit(1);
			}
			read_ilist_from_stream(1,ifp,&dim_data);
			read_ilist_from_stream(1,ifp,&num_data);
			data_matrix = f_alloc_mat(num_data,dim_data);
			read_ftable_from_stream(num_data,dim_data,ifp,data_matrix);
			fclose(ifp);
			idANDdistance = node_dist_alloc_list(num_data);
			for(i=0;i<num_data;i++){
				sprintf(idANDdistance[i].id,"%d",i);
				idANDdistance[i].dist = 0;
			}
		}else if(format == 2){
			if((ifp = fopen(sample_file,"r")) == NULL){
				perror(sample_file);
				exit(1);
			}
			read_ilist_from_stream(1,ifp,&dim_data);
			read_ilist_from_stream(1,ifp,&num_data);
			data_matrix = f_alloc_mat(num_data,dim_data);
			data_id = c_alloc_mat(num_data,ID_LEN);
			read_ID_ftable_from_stream(num_data,dim_data,ifp,data_matrix,data_id);
			fclose(ifp);
			idANDdistance = node_dist_alloc_list(num_data); 
			for(i=0;i<num_data;i++){
				sprintf(idANDdistance[i].id,"%s",data_id[i]);
				idANDdistance[i].dist = 0;
			}
		}
	}else{
		printf("%s",version_info);
		printf("type file name.\n");
		exit(1);
	}
	/* d_table = f_clear_alloc_matrix(num_data,num_data); */
	d_table = f_calloc_mat(num_data,num_data);
	euc_dist_sqtable(num_data,dim_data,data_matrix,d_table);
	for(i=0;i<num_data;i++){
		for(j=0;j<num_data;j++){
			idANDdistance[i].dist += d_table[i][j];
		}
	}
	qsort(idANDdistance,num_data,sizeof(struct node_dist_status),(int(*)(const void*, const void*))cmp_node_dist);
	for(i=0;i<num_data;i++){
		printf("%s ",idANDdistance[i].id);
		printf("%f\n",idANDdistance[i].dist);
	}
	return(0);
}
