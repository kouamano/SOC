/* soc-smalldsort.c */

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
#include <time.h>
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
	float **d_triangle = NULL;
	int i = 0;
	int j = 0;
	if(argc == 2){
		sscanf(argv[1],"%s",sample_file);
		format = ck_data_format(sample_file);
		if(format == 0){
			printf("check file format.\n");
			exit(1);
		}else if(format == 1){
			ifp = fopen(sample_file,"r");
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
			ifp = fopen(sample_file,"r");
			read_ilist_from_stream(1,ifp,&dim_data);
			read_ilist_from_stream(1,ifp,&num_data);
			data_matrix = f_alloc_mat(num_data,dim_data);
			data_id = c_alloc_mat(num_data,ID_LEN);
			read_ID_ftable_from_stream(num_data,dim_data,ifp,data_matrix,data_id);
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
	//d_triangle = f_calloc_triangle(num_data);
	d_triangle = f_calloc_lower_triangle(num_data);
	euc_dist_triangle(num_data,dim_data,data_matrix,d_triangle);
	for(i=0;i<num_data;i++){
		for(j=0;j<num_data;j++){
			if(i >= j){
				idANDdistance[i].dist += d_triangle[i][j];
			}else{
				idANDdistance[i].dist += d_triangle[j][i];
			}
		}
	}
	qsort(idANDdistance,num_data,sizeof(struct node_dist_status),(int(*)(const void*, const void*))cmp_node_dist);
	for(i=0;i<num_data;i++){
		printf("%s ",idANDdistance[i].id);
		printf("%f\n",idANDdistance[i].dist);
	}
	return(0);
}
