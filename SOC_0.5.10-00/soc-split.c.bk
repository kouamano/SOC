/* soc-split.c */

/*************************************************/
/* Copyright (c) 2003-2006 AMANO Kou             */
/* University of Tsukuba                         */
/* University of Library And Information Science */
/* National Institute of Agrobilogical Sciences  */
/* kamano@affrc.go.jp                            */
/*************************************************/

#define SOC_SPLIT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "soc.c"
#include "soc-split.h"

/*
struct option {
	int status;
	char import_file[FILE_NAME_LEN];
	char export_file_name[FILE_NAME_LEN];
	char export_file_subname[FILE_NAME_LEN];
	int part_print;
	char cluster_ID_string[STRING_LEN];
};
*/

void print_options(struct option opt){
	printf("CURRENT OPTIONS:\n");
	printf("  status\t\t:%d:\n",opt.status);
	printf("  import file\t\t:%s:\n",opt.import_file);
	printf("  export file name\t:%s:\n",opt.export_file_name);
	printf("  export file subname\t:%s:\n",opt.export_file_subname);
	printf("  partition print\t:%d:\n",opt.part_print);
	printf("  cluster IDs\t\t:%s:\n",opt.cluster_ID_string);
}

void init_options(struct option *opt){
	opt->status = 0;
	opt->import_file[0] = '\0';
	opt->export_file_name[0] = '\0';
	opt->export_file_subname[0] = '\0';
	opt->part_print = 0;
	opt->cluster_ID_string[0] = '\0';
}

int set_default_options(struct option *opt){
	int error_status = 0;
	if((opt->export_file_name[0] == '\0')&&(opt->import_file[0] == '\0')){
		if((error_status&1) == 0){
			error_status = error_status + 1;
		}
	}else if((opt->export_file_name[0] == '\0')&&(opt->import_file[0] != '\0')){
		strcpy(opt->export_file_name,opt->import_file);
		opt->export_file_name[FILE_NAME_LEN - 1] = '\0';
	}else if((opt->export_file_name[0] != '\0')&&(opt->import_file[0] == '\0')){
		if((error_status&2) == 0){
			error_status = error_status + 2;
		}
	}
	if(opt->export_file_subname[0] == '\0'){
		strcpy(opt->export_file_subname,".cluster.");
	}
	return(error_status);
}

void print_help(){
	printf("\n");
	printf("                    ");
	printf("soc-split\n\n");
	printf("        ");
	printf("split each cluster to a file.\n\n");
	printf("%s\n",version_info);
	printf("          \n");
	printf("USAGE:\n");
	printf("  soc-split if=<file> [cl=<cluster num>,...] [EFN=<export file name>] [EFSN=<export file sub name>]\n");
	printf("DESCRIPTION:\n");
	printf("  cl=\t: spscify cluster numbers.\n");
	printf("  EFN=\t: set as input file name at default.\n");
	printf("  EFSN=\t: set as 'cluster' at default.\n");
}

int get_options(int optc, char **optv, struct option *opt){
	int status = 0;
	int i;
	for(i=0;i<optc;i++){
		if(strcmp(optv[i],"-h") == 0){
			if((status&1) == 0){
				status = status + 1;
			}
		}else if(strcmp(optv[i],"--help") == 0){
			if((status&1) == 0){
				status = status + 1;
			}
		}else if(strcmp(optv[i],"-c") == 0){
			if((status&2) == 0){
				status = status + 2;
			}
		}else if(strcmp(optv[i],"--check") == 0){
			if((status&2) == 0){
				status = status + 2;
			}
		}else if(strncmp(optv[i],"if=",3) == 0){
			sscanf(optv[i],"if=%s",opt->import_file);
		}else if(strncmp(optv[i],"cl=",3) == 0){
			opt->part_print = 1;
			sscanf(optv[i],"cl=%s",opt->cluster_ID_string);
		}else if(strncmp(optv[i],"EFN=",4) == 0){
			sscanf(optv[i],"EFN=%s",opt->export_file_name);
		}else if(strncmp(optv[i],"EFSN=",5) == 0){
			sscanf(optv[i],"EFSN=%s",opt->export_file_subname);
		}else{
			sscanf(optv[i],"%s",opt->import_file);
		}
	}
	opt->status = status;
	return(status);
}

int get_number_cluster_IDs(char *cluster_string){
	int i;
	int len = 0;
	int number_cluster_IDs = 0;
	len = strlen(cluster_string);
	for(i=0;i<len;i++){
		if(cluster_string[i] == ','){
			number_cluster_IDs++;
		}
	}
	number_cluster_IDs++;
	return(number_cluster_IDs);
}

int cluster_string_to_cluster_IDs(char *cluster_string, int *_cluster_IDs){
	int i;
	int len = 0;
	int status = 0;
	int word_pointer = 0;
	char word[SHORT_STRING_LEN];
	int cluster_pointer = 0;
	len = strlen(cluster_string);
	for(i=0;i<len;i++){
		if(cluster_string[i] == ','){
			word[word_pointer] = '\0';
			sscanf(word,"%d",_cluster_IDs+cluster_pointer);
			word_pointer = 0;
			cluster_pointer++;
		}else if((48 <= (int)cluster_string[i])&&((int)cluster_string[i] <= 57)){
			word[word_pointer] = cluster_string[i];
			word_pointer++;
		}else{
			fprintf(stderr,"something wrong with cluster number.\n");
			fprintf(stderr,"program exits.\n");
			exit(1);
		}
	}
	word[word_pointer] = '\0';
	sscanf(word,"%d",_cluster_IDs+cluster_pointer);
	return(status);
}

//int cmp_int(const void* _A, const void* _B){
int cmp_int(int* A, int* B){
	//const int A = *((int*)_A);
	//const int B = *((int*)_B);
	if(*A < *B){
		return(-1);
	}else if(*A > *B){
		return(1);
	}else{
		return(0);
	}
}

int main(int argc, char **argv){
	int i;
	struct option opt;
	FILE *in = NULL;
	FILE *out = NULL;
	int file_open = 0;
	int c;
	int ignore = 0;
	int skip = 0;
	int clst = 0;
	int char_in_clst = 0;
	int number_cluster_IDs = 0;
	int *cluster_IDs;
	int cluster_IDs_pointer = 0;
	char export_file_name[FILE_NAME_LEN];
	char export_file_subname[FILE_NAME_LEN];
	char export_file_number[FILE_NAME_LEN];
	init_options(&opt);
	get_options(argc-1,argv+1,&opt);
	set_default_options(&opt);
	if((opt.status&1) == 1){print_help();}
	if((opt.status&2) == 2){print_options(opt);}
	if(((opt.status&1) == 1)||((opt.status&2) == 2)){exit(0);}
	/* (* read import file, write export file */
	if(opt.part_print == 0){
		in = fopen(opt.import_file,"r");
		if(in == NULL){
			perror(opt.import_file);
			exit(1);
		}
		sprintf(export_file_name,"%s",opt.export_file_name);
		sprintf(export_file_subname,"%s",opt.export_file_subname);
		sprintf(export_file_number,"%06d",0);
		strcat(export_file_subname,export_file_number);
		strcat(export_file_name,export_file_subname);
		while((c = fgetc(in)) != EOF){
			if(file_open == 0){
				out = fopen(export_file_name,"w");
				file_open++;
			}
			if(out == NULL){
				perror(export_file_name);
				exit(1);
			}
			skip = 0;
			if(c == '<'){
				ignore = 1;
			}else if(c == '>'){
				ignore = 0;
				skip = 1;
			}
			if((ignore == 0)&&(skip == 0)){
				char_in_clst++;
				//if(c == '/'){
				if(c == CL_DELIM){
					fclose(out);
					file_open = 0;
					clst++;
					char_in_clst = 0;
					sprintf(export_file_name,"%s",opt.export_file_name);
					sprintf(export_file_subname,"%s",opt.export_file_subname);
					sprintf(export_file_number,"%06d",clst);
					strcat(export_file_subname,export_file_number);
					strcat(export_file_name,export_file_subname);
				}else{
					if((char_in_clst == 1)&&(c == '\n')){
					}else{
						fprintf(out,"%c",c);
					}
				}
			}
		}
		fclose(in);
	}else{
		number_cluster_IDs = get_number_cluster_IDs(opt.cluster_ID_string);
		if((cluster_IDs = malloc(sizeof(int) * number_cluster_IDs)) == NULL){
			fprintf(stderr,"failed : malloc() -- exit.\n");
			exit(1);
		}
		cluster_string_to_cluster_IDs(opt.cluster_ID_string,cluster_IDs);
		qsort(cluster_IDs,number_cluster_IDs,sizeof(int),(int(*)(const void*, const void*))cmp_int);
		in = fopen(opt.import_file,"r");
		if(in == NULL){
			perror(opt.import_file);
			exit(1);
		}
		sprintf(export_file_name,"%s",opt.export_file_name);
		sprintf(export_file_subname,"%s",opt.export_file_subname);
		sprintf(export_file_number,"%06d",0);
		strcat(export_file_subname,export_file_number);
		strcat(export_file_name,export_file_subname);
		while((c = fgetc(in)) != EOF){
			skip = 0;
			if(c == '<'){
				ignore = 1;
			}else if(c == '>'){
				ignore = 0;
				skip = 1;
			}
			if((ignore == 0)&&(skip == 0)){
				char_in_clst++;
				//if(c == '/'){
				if(c == CL_DELIM){
					if(file_open > 0){
						fclose(out);
					}
					file_open = 0;
					clst++;
					char_in_clst = 0;
					sprintf(export_file_name,"%s",opt.export_file_name);
					sprintf(export_file_subname,"%s",opt.export_file_subname);
					sprintf(export_file_number,"%06d",clst);
					strcat(export_file_subname,export_file_number);
					strcat(export_file_name,export_file_subname);
					cluster_IDs_pointer++;
				}else{
					if((char_in_clst == 1)&&(c == '\n')){
					}else{
						for(i=0;i<=cluster_IDs_pointer;i++){
							if(clst == cluster_IDs[i]){
								if(file_open == 0){
									if((out = fopen(export_file_name,"w")) == NULL){
										perror(export_file_name);
										exit(1);
									}
									file_open++;
								}
								fprintf(out,"%c",c);
							}
						}
					}
				}
			}
		}
		fclose(in);
	}
	/* *) */
	return(0);
}
