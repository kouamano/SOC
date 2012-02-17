/* soc-dtable.c */

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

#include "soc.h"
#include "files.h"
#include "soc.c"

int main(int argc, char **argv){
	FILE *ifp = NULL;
	int i;
	int j;
	int dim;
	int num;
	float **data = NULL;
	float **dist_table = NULL;
	char filename[FILE_NAME_LEN];
	int data_format = 0;
	char **id = NULL;
	if(argc == 2){
		if(strcmp(argv[1],"-v") == 0){
			printf("soc-dtable %s\n",version_info);
		}else if(strncmp(argv[1],"if=",3) == 0){
			sscanf(argv[1],"if=%s",filename);
			data_format = ck_data_format(filename);
			if(data_format == 1){
				if((ifp = fopen(filename,"r")) == NULL){
					perror(filename);
					exit(1);
				}
				read_ilist_from_stream(1,ifp,&dim);
				read_ilist_from_stream(1,ifp,&num);
				data = f_alloc_mat(num,dim);
				read_ftable_from_stream(num,dim,ifp,data);
				fclose(ifp);
				dist_table = f_alloc_mat(num,num);
				euc_dist_sqtable(num,dim,data,dist_table);
				printf("    n \\ n    ");
				for(i=0;i<num;i++){
					printf("[%09d] ",i);
				}
				printf("\n");
				for(i=0;i<num;i++){
					printf("[%09d] ",i);
					for(j=0;j<num;j++){
						printf("%11f ",dist_table[i][j]);
					}
					printf("\n");
				}
			}else if(data_format == 2){
				if((ifp = fopen(filename,"r")) == NULL){
					perror(filename);
					exit(1);
				}
				read_ilist_from_stream(1,ifp,&dim);
				read_ilist_from_stream(1,ifp,&num);
				data = f_alloc_mat(num,dim);
				id = c_alloc_mat(num,ID_LEN);
				read_ID_ftable_from_stream(num,dim,ifp,data,id);
				fclose(ifp);
				dist_table = f_alloc_mat(num,num);
				euc_dist_sqtable(num,dim,data,dist_table);
				printf("SquareMatrix ");
				for(i=0;i<num;i++){
					printf("[%9s] ",id[i]);
				}
				printf("\n");
				for(i=0;i<num;i++){
					printf("[%9s] ",id[i]);
					for(j=0;j<num;j++){
						printf("%11f ",dist_table[i][j]);
					}
					printf("\n");
				}
			}
		}
	}else{
		printf("\n");
		printf("          soc-dtable -- print distance table\n");
		printf("          SOC %s\n",version_info);
		printf("\n");
		printf("  USAGE : \n");
		printf("  soc-dtable if=\"input file\"\n");
		printf("  soc-dtable -v\n");
	}
	return(0);
}
