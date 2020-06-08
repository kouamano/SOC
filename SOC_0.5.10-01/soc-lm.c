/* soc-lm.c */

/**********************************************************/
/* Copyright (c) 2004-2009 AMANO Kou <kamano@affrc.go.jp> */
/* University of Library And Information Science          */
/* National Institute of Agrobilogical Sciences           */
/* RIKEN BioResource Center                               */
/* University of Tsukuba                                  */
/**********************************************************/

#define SOC_LM
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <complex.h>

#include "soc.h"
#include "files.h"
#include "soc.c"



/* (*function pointers */
void (*dist_triangle_func)();
void (*dist_table_func)();
float (*distance_func)();
/* *) */



/* (*argument values */
static char sample_file[FILE_NAME_LEN];		/*sample file name*/
static char cluster_file[FILE_NAME_LEN];	/*cluster file name*/
static char output_file[FILE_NAME_LEN];		/*output file name*/
static char report_file[FILE_NAME_LEN];		/*report file name*/
static char log_file[FILE_NAME_LEN];		/*log file name*/
static int arg_dim_smpl = -1;
static int arg_num_smpl = -1;
static int arg_dim_clst = -1;
static int arg_num_clst = -1;
static int sample_title = 0;			/*0:auto, 1:no title, 2:with title*/
static int with_sample_mat_size = 2;		/*1:not in file, 2:in file*/
static int smpl_data_format = 0;		/*0:error, 1:without sample title, 2:with sample title*/
static int cluster_title = 0;			/*0:auto, 1:no title, 2:with title*/
static int with_cluster_mat_size = 2;		/*1:not in file, 2:in file*/
static int clst_data_format = 0;		/*0:error, 1:without sample title, 2:with sample title*/
static int clst_expand = 0;
static float clst_expand_ratio = 5;
static int loop = 100;				/*default*/



static int dist_func = 0;			/* 0;euclidian distance, 1:cosine distance */



static int grouping_mode = 0;			/*0:E-distance 1:E/R-distance*/
static float grouping_radius_index = 0;		/*index for radius*/
static float grouping_member_index = 0;		/*index for num of members*/
static float grouping_density_index = 0;	/*index for density of cluster members*/
static float grouping_radius_ratio = 0;		/*index for radius*/



static float err_correct_rate = 0.33;		/*default*/



static int generate = 0;			/*default*/
static float r_generate = 10;			/*default*/
static int generate_suppress_clst_num = 60;	/*default*/
static int generate_interval = 1;		/*default*/



static int move = 0;				/*default*/
static float move_rate = 0.3;			/*default*/
static float r_move_suppress = 5;		/*default*/
static int move_interval = 1;			/*default*/



static int is_unified = 0;			/*default*/
static float r_unified = 5;			/*default*/
static int unified_suppress_clst_num = 1;	/*default*/
static int unified_interval = 1;		/*default*/


static int delete = 0;				/*default*/
static float delete_threshold = 0;		/*default*/
static int delete_interval = 0;			/*default*/


static int cutoff = 0;				/*default*/
static float cutoff_value = 1.4;		/*default*/
static int cutoff_iteration = 40;		/*default*/
 

 
static int cutoff_gen = 0;			/*default*/
static float cutoff_ol_th = 2;			/*default*/
static float cutoff_gen_th = 1;			/*default*/
static int cutoff_interval = 2;			/*default*/



static int check_cl = 0;
static int check_cl_interval = 15;



static int monitor = 16+32+64;			/*default*/
static int err_check = 0;			/*default*/
static float err_threshold = 0.0000001;		/*default*/
static char of_type[SHORT_STRING_LEN] = "AC";	/*default*/
static int option_status = 0;			/*default*/
static int tmp_int = 0;				/*default*/
static float tmp_float = 0;			/*default*/



char message_loop_enter[MIDLE_STRING_LEN];
char message_loop_exit[MIDLE_STRING_LEN];
/* *) */



/* (*help messages */
void message(void){
	printf("\n");
	printf("\n");
	printf("%s%s\n",head,bar);
	printf("%s%s\n",head,title);
	printf("%s%s\n",head,bar);
	printf("\n");
	printf("%s%s\n",head,author);
	printf("%s%s\n",head,affiliation);
	printf("\n");
	printf("%s%s\n",head,bar);
	printf("%s%s\n",head,version_info);
	printf("%s        Compiled: %s ; %s\n",head,compiled_time,compiled_date);
	printf("%s%s\n\n",head,bar);
}

void usage(void){
	printf("USAGE: \n");
	printf("  soc-lm --help\n");
	printf("  soc-lm --options\n");
	printf("  soc-lm --check <args...>\n");
	printf("  soc-lm sf=<sample file> [-s{i|I}] cf=<cluster file> [-c{i|I}] [loop=<loop>] [rate=<learning rate>] [unify=<n1>,<n2>,<n3>,<n4>] [generate=<n1>,<n2>,<n3>,<n4>] [cutoff=<n1>,<n2>,<n3>] [cutoff-gen=<n1>,<n2>,<n3>,<n4>] [delete=<n1>,<n2>,<n3>] [move=<n1>,<n2>,<n3>,<n4>] [grouping=<n1>,<n2>] [lf=<log file>] [of=<output file>] [of_type=<output file type>] [rf=<report file>] [monitor=<level>] dist_func=<distance func>\n");
	printf("\nOTHER COMMANDS:\n");
	printf("  soc-init\n");
	printf("  soc-split\n");
}

void print_options(void){
	printf("OPTIONS: \n");
	printf("  --check : \n");
	printf("    print parameters.\n");
	printf("  sf=<sample file> : \n");
	printf("    sample file name.\n");
	printf("  cf=<cluster file> : \n");
	printf("    cluster file name.\n");
	printf("  -sI : with sample ID.\n");
	printf("  -si : without sample ID.\n");
	printf("  -cI : with cluster ID.\n");
	printf("  -ci : without cluster ID.\n");
	printf("  -sM : with sample matrix size.\n");
	printf("  -sm<column size>,<sample size> : specify sample matrix size.\n");
	printf("  -cM : with cluster matrix size.\n");
	printf("  -cm<column size>,<sample size> : specify cluster matrix size.\n");
	printf("  loop=<loop> : \n");
	printf("    set loop iterrations.\n");
	printf("  check-cl=<n1>,<n2> : \n");
	printf("    break if clst_members == clst_members_prev.\n");
	printf("    n1 on (1) or off (0).\n");
	printf("    n2 interval.\n");
	printf("  err_check=<n1>,<n2> : \n");
	printf("    to break learning-loop.\n");
	printf("    n1 : on (1) or off (0).\n");
	printf("    n2 : threshold.\n");
	printf("  rate=<learning rate> : \n");
	printf("    cluster move rate.\n");
	printf("  clst_expand_ratio=<expand ratio> : \n");
	printf("    expand the number of clusters for cluster generate.\n");
	printf("  unify=<n1>,<n2>,<n3>,<n4> : \n");
	printf("    n1 : turn on or off the option.\n");
	printf("    n1==0 : off.\n");
	printf("    n1==1 : use each absolute distance of pair of cluster-nodes.\n");
	printf("    n1==2 : use ratio (unify if [ distance(the pair) < ratio * add(two of cluster radius) ].\n");
	printf("    n1==3 : (unify if [ distance(the pair) < th + add(two of cluster radius) ].\n");
	printf("    n2 : distance or ratio.\n");
	printf("    n3 : suppression number of clusters (min).\n");
	printf("    n4 : interval.\n");
	printf("  delete=<n1>,<n2>,<n3> : [!!UNDER CONSTRUCTION!!]\n");
	printf("    n1 : turn on or off the option.\n");
	printf("    n1==1 : delete if [clst-clst distance < threshold].\n");
	printf("    n1==2 : delete if [clst-clst distance < average of clst_radius * threshold].\n");
	printf("    n2 : threshold.\n");
	printf("    n3 : interval.\n");
	printf("  generate=<n1>,<n2>,<n3>,<n4> : \n");
	printf("    n1 : turn on or off the option.\n");
	printf("    n1==0 : off.\n");
	printf("    n1==1 : compare the given absolute distance and min-min distance between the sample and its cluster-node.\n");
	printf("    n1==2 : compare the given ratio and avarage distance ratio (distance to each sample / cluster radius).\n");
	printf("    n1==4 : compare the given number and number of cluster members.\n");
	printf("    n1==8 : compare the given absolute distance and max-max distance between the sample and its cluster-node.\n");
	printf("    n2 : radius distance, ratio or number of members.\n");
	printf("    n3 : suppression number of clusters (max).\n");
	printf("    n4 : interval.\n");
	printf("  cutoff=<n1>,<n2>,<n3> : \n");
	printf("    n1==0 : off.\n");
	printf("    n1==1 : cutoff when distance > cutoff_value * cluster radius.\n");
	printf("    n2 : cutoff_value.\n");
	printf("    n3 : cutoff when loop > cutoff_iteration.\n");
	printf("  cutoff-gen=<n1>,<n2>,<n3>,<n4> : [!!UNDER CONSTRUCTION!!]\n");
	printf("    n1==0 : off.\n");
	printf("    n1==1 : cutoff if (@earch cluster) [ distance > cutoff_ol_th * cluster radius].\n");
	printf("          : generate if [ distance > cutoff_gen_th].\n");
	printf("    n1==2 : cutoff if (@earch cluster) [ distance > cutoff_ol_th * cluster radius].\n");
	printf("          : generate if [ distance > cutoff_gen_th * average of all cluster radius].\n");
	printf("    n2 : cutoff_ol_th.\n");
	printf("    n3 : cutoff_gen_th.\n");
	printf("    n4 : cutoff_interval.\n");
	printf("  move=<n1>,<n2>,<n3>,<n4> : \n");
	printf("    n1 : turn on or off the option.\n");
	printf("    n1==0 : off.\n");
	printf("    n1==1 : move to the centroid.\n");
	printf("    n1==2 : move to the nearest sample.\n");
	printf("    n1==4 : move to the farthest sample.\n");
	printf("    n2 : move rate.\n");
	printf("    n3 : suppression radius.\n");
	printf("    n4 : interval.\n");
	printf("  grouping=<n1>,<n2> : !!warn to use!!\n");
	printf("    n1 : grouping mode.\n");
	printf("    n2 : grouping parameter.\n");
	printf("    n1==0 : normal.\n");
	printf("    n1==1 : new distance = distance * pow(radius,n2).\n");
	printf("    n1==2 : new distance = distance * pow(num of members,n2).\n");
	printf("    n1==4 : new distance = distance * pow((num of members / radius),n2).\n");
	printf("    n1==8 : new distance = distance - (radius * n2).\n");
	printf("  of_type=<output file type> : \n");
	printf("    C : cluster nodes.\n");
	printf("    A : all sample nodes.\n");
	printf("    AC : all sample nodes and cluster nodes.\n");
	printf("  of=<output file> : \n");
	printf("    output file name.\n");
	printf("  rf=<report file> : \n");
	printf("    report file name.\n");
	printf("  lf=<log file> : \n");
	printf("    log file name.\n");
	printf("  monitor=<level> : \n");
	printf("    print values.\n");
	printf("      0 : monitor off.\n");
	printf("    1<= : print result. (even number: only last loop.)\n");
	printf("      1 : progress monitor on.\n");
	printf("      2 : cluster coordinate.\n");
	printf("      4 : cluster - cluster distance table.\n");
	printf("      8 : cluster - sample distance table.\n");
	printf("     16 : cluster node information.\n");
	printf("     32 : cluster - reference node distance.\n");
	printf("     64 : error information.\n");
	printf("    128 : cluster status.\n");
	printf("    256 : cluster moving information.\n");
	printf("    default : 16+32+64 = 112.\n");
	printf("  dist_func=<func> : \n");
	printf("    0 : euclidian.\n");
	printf("    1 : cosine.\n");
	printf("    2 : neg. affinity 1-(inner[A,B]^2 / inner[A,A] / inner[B,B]).\n");
	printf("    3 : neg. weighted affinity 1-Weighted(inner[A,B]^2 / inner[A,A] / inner[B,B]).\n");
}
/* *) */



/* (*argument analysis */
int get_options(int arg_start, int arg_end, char **arg_s){
	int i = 0;
	int s = 0;
	char *sptr=NULL;
	s = 0;
		for(i=arg_start;i<arg_end;i++){
			if(strcmp(arg_s[i],"--check") == 0){
				if((s&1) == 0){
					s = s + 1;
				}
			}else if(strncmp(arg_s[i],"sf=",3) == 0){
				sscanf(arg_s[i],"sf=%s",sample_file);
				if((s&2) == 0){
					s = s + 2;
				}
			}else if(strncmp(arg_s[i],"cf=",3) == 0){
				sscanf(arg_s[i],"cf=%s",cluster_file);
				if((s&4) == 0){
					s = s + 4;
				}
			}else if(strncmp(arg_s[i],"loop=",5) == 0){
				sscanf(arg_s[i],"loop=%d",&loop);
			}else if(strncmp(arg_s[i],"rate=",5) == 0){
				sscanf(arg_s[i],"rate=%f",&err_correct_rate);
			}else if(strncmp(arg_s[i],"unify=",6) == 0){
				sscanf(arg_s[i],"unify=%d,%f,%d,%d",&is_unified,&r_unified,&unified_suppress_clst_num,&unified_interval);
				if((s&32) == 0){
					s = s + 32;
				}
			}else if(strncmp(arg_s[i],"move=",5) == 0){
				sscanf(arg_s[i],"move=%d,%f,%f,%d",&move,&move_rate,&r_move_suppress,&move_interval);
				if((s&64) == 0){
					s = s + 64;
				}
			}else if(strncmp(arg_s[i],"of_type=",8) == 0){
				sscanf(arg_s[i],"of_type=%s",of_type);
				if((s&128) == 0){
					s = s + 128;
				}
			}else if(strncmp(arg_s[i],"of=",3) == 0){
				sscanf(arg_s[i],"of=%s",output_file);
				if((s&256) == 0){
					s = s + 256;
				}
			}else if(strncmp(arg_s[i],"rf=",3) == 0){
				sscanf(arg_s[i],"rf=%s",report_file);
				if((s&2048) == 0){
					s = s + 2048;
				}
			}else if(strncmp(arg_s[i],"monitor=",8) == 0){
				sscanf(arg_s[i],"monitor=%d",&monitor);
				if((s&512) == 0){
					s = s + 512;
				}
			}else if(strncmp(arg_s[i],"err_check=",10) == 0){
				sscanf(arg_s[i],"err_check=%d,%f",&err_check,&err_threshold);
				if((s&1024) == 0){
					s = s + 1024;
				}
			}else if(strncmp(arg_s[i],"clst_expand_ratio=",12) == 0){
				clst_expand++;
				sscanf(arg_s[i],"clst_expand_ratio=%f",&clst_expand_ratio);
			}else if(strncmp(arg_s[i],"grouping=",9) == 0){
				sscanf(arg_s[i],"grouping=%d,%f",&tmp_int,&tmp_float);
				if(((tmp_int&1) == 1)&&((grouping_mode&1) == 0)){
					grouping_mode = grouping_mode + tmp_int;
					grouping_radius_index = tmp_float;
				}
				if(((tmp_int&2) == 2)&&((grouping_mode&2) == 0)){
					grouping_mode = grouping_mode + tmp_int;
					grouping_member_index = tmp_float;
				}
				if(((tmp_int&4) == 4)&&((grouping_mode&4) == 0)){
					grouping_mode = grouping_mode + tmp_int;
					grouping_density_index = tmp_float;
				}
				if(((tmp_int&8) == 8)&&((grouping_mode&8) == 0)){
					grouping_mode = grouping_mode + tmp_int;
					grouping_radius_ratio = tmp_float;
				}
			}else if(strcmp(arg_s[i],"-si") == 0){
				sample_title = 1;
			}else if(strcmp(arg_s[i],"-sI") == 0){
				sample_title = 2;
			}else if(strcmp(arg_s[i],"-ci") == 0){
				cluster_title = 1;
			}else if(strcmp(arg_s[i],"-cI") == 0){
				cluster_title = 2;
			}else if(strncmp(arg_s[i],"-sm",3) == 0){
				with_sample_mat_size = 1;
				if((sptr = strchr(sample_file,'m')) != NULL){
					sscanf(sptr+1,"%d,%d",&arg_dim_smpl,&arg_num_smpl);
					*sptr = '\0';
				}
			}else if(strcmp(arg_s[i],"-sM") == 0){
				with_sample_mat_size = 2;
			}else if(strncmp(arg_s[i],"-cm",3) == 0){
				with_cluster_mat_size = 1;
				if((sptr = strchr(cluster_file,'m')) != NULL){
					sscanf(sptr+1,"%d,%d",&arg_dim_clst,&arg_num_clst);
					*sptr = '\0';
				}
			}else if(strcmp(arg_s[i],"-cM") == 0){
				with_cluster_mat_size = 2;
			}else if(strncmp(arg_s[i],"generate=",9) == 0){
				clst_expand++;
				sscanf(arg_s[i],"generate=%d,%f,%d,%d",&generate,&r_generate,&generate_suppress_clst_num,&generate_interval);
			}else if(strncmp(arg_s[i],"cutoff=",7) == 0){
				sscanf(arg_s[i],"cutoff=%d,%f,%d",&cutoff,&cutoff_value,&cutoff_iteration);
			}else if(strncmp(arg_s[i],"cutoff-gen=",11) == 0){
				sscanf(arg_s[i],"cutoff-gen=%d,%f,%f,%d",&cutoff_gen,&cutoff_ol_th,&cutoff_gen_th,&cutoff_interval);
				clst_expand++;
			}else if(strncmp(arg_s[i],"delete=",7) == 0){
				sscanf(arg_s[i],"delete=%d,%f,%d",&delete,&delete_threshold,&delete_interval);
			}else if(strncmp(arg_s[i],"check-cl=",9) == 0){
				sscanf(arg_s[i],"check-cl=%d,%d",&check_cl,&check_cl_interval);
			}else if(strncmp(arg_s[i],"lf=",3) == 0){
				sscanf(arg_s[i],"lf=%s",log_file);
			}else if(strncmp(arg_s[i],"dist_func=",10) == 0){
				sscanf(arg_s[i],"dist_func=%d",&dist_func);
			}else{
				fprintf(stderr,"Unknown option: %s.\n",arg_s[i]);
				exit(1);
			}
		}
	return(s);
}

void print_check_opt(int _dim_smpl, int _num_smpl, int _dim_clst, int _num_clst){
	printf("sample file :%s:\n",sample_file);
	printf("sample title in file (0:auto, 1:not in file, 2:in file):%d:\n",sample_title);
	printf("sample size in file (0:auto, 1:not in file, 2:in file):%d:\n",with_sample_mat_size);
	printf("detected sample data format (0:error, 1:without sample title, 2:with sample title):%d:\n",smpl_data_format);
	printf("sample file argument dim :%d:\n",arg_dim_smpl);
	printf("sample file argument num :%d:\n",arg_num_smpl);
	printf("sample title :%d:\n",sample_title);
	printf("sample dimension :%d:\n",_dim_smpl);
	printf("sample number :%d:\n",_num_smpl);

	printf("cluster file :%s:\n",cluster_file);
	printf("cluster title in file (0:auto, 1:not in file, 2:in file):%d:\n",cluster_title);
	printf("cluster size in file (0:auto, 1:not in file, 2:in file):%d:\n",with_cluster_mat_size);
	printf("detected cluster data format (0:error, 1:without sample title, 2:with sample title):%d:\n",clst_data_format);
	printf("cluster file argument dim :%d:\n",arg_dim_clst);
	printf("cluster file argument num :%d:\n",arg_num_clst);
	printf("cluster title :%d:\n",cluster_title);
	printf("cluster dimension :%d:\n",_dim_clst);
	printf("cluster number :%d:\n",_num_clst);
	printf("cluster expand ratio :%f:\n",clst_expand_ratio);

	printf("loop :%d:\n",loop);
	printf("err_correct_rate :%f:\n",err_correct_rate);

	printf("is_unified :%d:\n",is_unified);
	printf("r_unified :%f:\n",r_unified);
	printf("unified_suppress_clst_num :%d:\n",unified_suppress_clst_num);
	printf("unified_interval :%d:\n",unified_interval);

	printf("generate :%d:\n",generate);
	printf("r_generate :%f:\n",r_generate);
	printf("generate_suppress_clst_num :%d:\n",generate_suppress_clst_num);
	printf("generate_interval :%d:\n",generate_interval);

	printf("cutoff :%d:\n",cutoff);
	printf("cutoff_value :%f:\n",cutoff_value);
	printf("cutoff_iteration :%d:\n",cutoff_iteration);

	printf("cutoff_gen :%d:\n",cutoff_gen);
	printf("cutoff_ol_th :%f:\n",cutoff_ol_th);
	printf("cutoff_gen_th :%f:\n",cutoff_gen_th);
	printf("cutoff_interval :%d:\n",cutoff_interval);

	printf("delete :%d:\n",delete);
	printf("delete_threshold :%f:\n",delete_threshold);
	printf("delete_interval :%d:\n",delete_interval);

	printf("grouping_mode :%d:\n",grouping_mode);
	printf("grouping_radius_index :%f:\n",grouping_radius_index);
	printf("grouping_member_index :%f:\n",grouping_member_index);
	printf("grouping_density_index :%f:\n",grouping_density_index);
	printf("grouping_radius_ratio :%f:\n",grouping_radius_ratio);

	printf("move :%d:\n",move);
	printf("move_rate :%f:\n",move_rate);
	printf("r_move_suppress :%f:\n",r_move_suppress);
	printf("move_interval :%d:\n",move_interval);

	printf("ckeck_cl :%d:\n",check_cl);
	printf("ckeck_cl_interval :%d:\n",check_cl_interval);

	printf("err_ckeck :%d:\n",err_check);
	printf("err_threshold :%1.12f:\n",err_threshold);

	printf("output file type :%s:\n",of_type);
	printf("output file :%s:\n",output_file);
	printf("report file :%s:\n",report_file);
	printf("monitor :%d:\n",monitor);
	printf("dist_func :%d:\n",dist_func);
}
/* *) */



#include "learning-loop-monitor.c"



int main(int argc, char **argv){
	FILE *ofp = NULL;
	FILE *rfp = NULL;
	FILE *lfp = NULL;
	FILE *ifp = NULL;
	int lf_open = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int dim_smpl = -1;			/*stable*/
	int dim_clst = -1;			/*stable*/
	int num_smpl = -1;			/*stable*/
	int num_clst = -1;			/*stable*/
	int MAX_num_clst = 0;			/*stable*/
	int allocated_num_clst = 0;		/*rewrite*/
	int assigned_num_clst = 0;		/*rewrite*/
	char **sample_id = NULL;		/*stable*/
	float **smpl_matrix = NULL;		/*stable*/
	double *smpl_ave_vec = NULL;		/*rewrite*/
	double *smpl_sd_vec = NULL;		/*rewrite*/
	char **cluster_id = NULL;		/*stable*/
	float **clst_matrix = NULL;		/*rewrite*/
	double *clst_ave_vec = NULL;		/*rewrite*/
	double *clst_sd_vec = NULL;		/*rewrite*/
	float *average_coordinate_data = NULL;	/*stable*/
	float total_sum_dist = 0;		/*rewrite*/
	float **dif_table = NULL;		/*rewrite*/
	float **clst_smpl_dist_table = NULL;	/*rewrite*/
	float **clst_clst_dist_table = NULL;	/*rewirte*/
	float **sum_ref_smpl_list = NULL;	/*NOT USE sum of refernce data (each clst)*/
	float **ave_ref_smpl_list = NULL;	/*center of reference data (each clst)*/
	struct status *attrb_list_min = NULL;		/*rewrite*/
	struct status *attrb_list_max = NULL;		/*rewrite*/
	struct clst_status *clst_status_list = NULL;	/*rewrite*/
	struct clst_status *clst_status_list_prev = NULL;	/*rewrite*/
	int num_of_output_clsts = 0;		/*for output_file*/
	int *output_clsts = NULL;
	int loop_times = 0;

	if(argc == 1){
		message();
		usage();
	}else if((argc == 2)&&(strcmp(argv[1],"--options") == 0)){
		print_options();
	}else if((argc > 2)){
		fprintf(stderr,"\n");
		option_status = get_options(1,argc,argv);
		/* {* check file format, read data */
		/* (* read sample and malloc smpl_matrix, sample_id */
		fprintf(stderr,"checking sample file format ...\n");
		if(((arg_dim_smpl<0)||(arg_num_smpl<0))&&(sample_title==0)){
			smpl_data_format = ck_data_format(sample_file);
			fprintf(stderr,"sample data format:%d:\n",smpl_data_format);
			if(smpl_data_format == 0){
				fprintf(stderr,"[ERROR] check file format.\n");
				exit(1);
			}else if(smpl_data_format == 1){
				if((ifp = fopen(sample_file,"r")) == NULL){
					perror(sample_file);
					exit(1);
				}
				read_ilist_from_stream(1,ifp,&dim_smpl);
				fprintf(stderr,"sample dimensions:%d:\n",dim_smpl);
				read_ilist_from_stream(1,ifp,&num_smpl);
				fprintf(stderr,"number of samples:%d:\n",num_smpl);
				smpl_matrix = f_alloc_mat(num_smpl,dim_smpl);
				sample_id = c_alloc_mat(num_smpl,ID_LEN);
				for(i=0;i<num_smpl;i++){
					sprintf(sample_id[i],"%d",i);
				}
				read_ftable_from_stream(num_smpl,dim_smpl,ifp,smpl_matrix);
				fclose(ifp);
			}else if(smpl_data_format == 2){
				if((ifp = fopen(sample_file,"r")) == NULL){
					perror(sample_file);
					exit(1);
				}
				read_ilist_from_stream(1,ifp,&dim_smpl);
				fprintf(stderr,"sample dimensions:%d:\n",dim_smpl);
				read_ilist_from_stream(1,ifp,&num_smpl);
				fprintf(stderr,"number of samples:%d:\n",num_smpl);
				smpl_matrix = f_alloc_mat(num_smpl,dim_smpl);
				sample_id = c_alloc_mat(num_smpl,ID_LEN);
				read_ID_ftable_from_stream(num_smpl,dim_smpl,ifp,smpl_matrix,sample_id);
				fclose(ifp);
			}
		}else if(((arg_dim_smpl<0)||(arg_num_smpl<0))&&(sample_title==1)){
			if((ifp = fopen(sample_file,"r")) == NULL){
				perror(sample_file);
				exit(1);
			}
			read_ilist_from_stream(1,ifp,&dim_smpl);
			fprintf(stderr,"sample dimensions:%d:\n",dim_smpl);
			read_ilist_from_stream(1,ifp,&num_smpl);
			fprintf(stderr,"number of samples:%d:\n",num_smpl);
			smpl_matrix = f_alloc_mat(num_smpl,dim_smpl);
			sample_id = c_alloc_mat(num_smpl,ID_LEN);
			for(i=0;i<num_smpl;i++){
				sprintf(sample_id[i],"%d",i);
			}
			read_ftable_from_stream(num_smpl,dim_smpl,ifp,smpl_matrix);
			fclose(ifp);
			smpl_data_format = 1;
		}else if(((arg_dim_smpl<0)||(arg_num_smpl<0))&&(sample_title==2)){
			if((ifp = fopen(sample_file,"r")) == NULL){
				perror(sample_file);
				exit(1);
			}
			read_ilist_from_stream(1,ifp,&dim_smpl);
			fprintf(stderr,"sample dimensions:%d:\n",dim_smpl);
			read_ilist_from_stream(1,ifp,&num_smpl);
			fprintf(stderr,"number of samples:%d:\n",num_smpl);
			smpl_matrix = f_alloc_mat(num_smpl,dim_smpl);
			sample_id = c_alloc_mat(num_smpl,ID_LEN);
			read_ID_ftable_from_stream(num_smpl,dim_smpl,ifp,smpl_matrix,sample_id);
			fclose(ifp);
			smpl_data_format = 2;
		}else if((arg_dim_smpl>0)&&(arg_num_smpl>0)&&(sample_title==0)){
			fprintf(stderr,"[ERROR] you must specify data format.\n");
			exit(0);
		}else if((arg_dim_smpl>0)&&(arg_num_smpl>0)&&(sample_title==1)){
			if((ifp = fopen(sample_file,"r")) == NULL){
				perror(sample_file);
				exit(1);
			}
			dim_smpl = arg_dim_smpl;
			fprintf(stderr,"sample dimensions:%d:\n",dim_smpl);
			num_smpl = arg_num_smpl;
			fprintf(stderr,"number of samples:%d:\n",num_smpl);
			smpl_matrix = f_alloc_mat(num_smpl,dim_smpl);
			sample_id = c_alloc_mat(num_smpl,ID_LEN);
			for(i=0;i<num_smpl;i++){
				sprintf(sample_id[i],"%d",i);
			}
			read_ftable_from_stream(num_smpl,dim_smpl,ifp,smpl_matrix);
			fclose(ifp);
			smpl_data_format = 1;
		}else if((arg_dim_smpl>0)&&(arg_num_smpl>0)&&(sample_title==2)){
			if((ifp = fopen(sample_file,"r")) == NULL){
				perror(sample_file);
				exit(1);
			}
			dim_smpl = arg_dim_smpl;
			fprintf(stderr,"sample dimensions:%d:\n",dim_smpl);
			num_smpl = arg_num_smpl;
			fprintf(stderr,"number of samples:%d:\n",num_smpl);
			smpl_matrix = f_alloc_mat(num_smpl,dim_smpl);
			sample_id = c_alloc_mat(num_smpl,ID_LEN);
			read_ID_ftable_from_stream(num_smpl,dim_smpl,ifp,smpl_matrix,sample_id);
			fclose(ifp);
			smpl_data_format = 2;
		}
		fprintf(stderr," ... done.\n\n");
		/* *) */
		/* (* determinate clst_expand_ratio */
		if(clst_expand == 0){
			clst_expand_ratio = 1.2;
		}
		/* *) */
		/* *( read cluster and malloc clst_matrix, cluster_id */
		fprintf(stderr,"checking cluster file format ...\n");
		if(((arg_dim_clst<0)||(arg_num_clst<0))&&(cluster_title==0)){
			clst_data_format = ck_data_format(cluster_file);
			fprintf(stderr,"cluster data format:%d:\n",clst_data_format);
			if(clst_data_format == 0){
				fprintf(stderr,"[ERROR] check file format.\n");
				exit(1);
			}else if(clst_data_format == 1){
				if((ifp = fopen(cluster_file,"r")) == NULL){
					perror(cluster_file);
					exit(1);
				}
				read_ilist_from_stream(1,ifp,&dim_clst);
				fprintf(stderr,"cluster dimensions:%d:\n",dim_clst);
				read_ilist_from_stream(1,ifp,&num_clst);
				fprintf(stderr,"number of clusters:%d:\n",num_clst);
					MAX_num_clst = (int)((float)num_clst * clst_expand_ratio);
					clst_matrix = f_alloc_mat(MAX_num_clst,dim_clst);
				read_ftable_from_stream(num_clst,dim_clst,ifp,clst_matrix);
				fclose(ifp);
			}else if(clst_data_format == 2){
				if((ifp = fopen(cluster_file,"r")) == NULL){
					perror(cluster_file);
					exit(1);
				}
				read_ilist_from_stream(1,ifp,&dim_clst);
				fprintf(stderr,"cluster dimensions:%d:\n",dim_clst);
				read_ilist_from_stream(1,ifp,&num_clst);
				fprintf(stderr,"number of clusters:%d:\n",num_clst);
					MAX_num_clst = (int)((float)num_clst * clst_expand_ratio);
					clst_matrix = f_alloc_mat(MAX_num_clst,dim_clst);
					cluster_id = c_alloc_mat(MAX_num_clst,ID_LEN);
				read_ID_ftable_from_stream(num_clst,dim_clst,ifp,clst_matrix,cluster_id);
				fclose(ifp);
			}
		}else if(((arg_dim_clst<0)||(arg_num_clst<0))&&(cluster_title==1)){
			if((ifp = fopen(cluster_file,"r")) == NULL){
				perror(cluster_file);
				exit(1);
			}
			read_ilist_from_stream(1,ifp,&dim_clst);
			fprintf(stderr,"cluster dimensions:%d:\n",dim_clst);
			read_ilist_from_stream(1,ifp,&num_clst);
			fprintf(stderr,"number of clusters:%d:\n",num_clst);
				MAX_num_clst = (int)((float)num_clst * clst_expand_ratio);
				clst_matrix = f_alloc_mat(MAX_num_clst,dim_clst);
			read_ftable_from_stream(num_clst,dim_clst,ifp,clst_matrix);
			fclose(ifp);
			clst_data_format = 1;
		}else if(((arg_dim_clst<0)||(arg_num_clst<0))&&(cluster_title==2)){
			if((ifp = fopen(cluster_file,"r")) == NULL){
				perror(cluster_file);
				exit(1);
			}
			read_ilist_from_stream(1,ifp,&dim_clst);
			fprintf(stderr,"cluster dimensions:%d:\n",dim_clst);
			read_ilist_from_stream(1,ifp,&num_clst);
			fprintf(stderr,"number of clusters:%d:\n",num_clst);
				MAX_num_clst = (int)((float)num_clst * clst_expand_ratio);
				clst_matrix = f_alloc_mat(MAX_num_clst,dim_clst);
				cluster_id = c_alloc_mat(MAX_num_clst,ID_LEN);
			read_ID_ftable_from_stream(num_clst,dim_clst,ifp,clst_matrix,cluster_id);
			fclose(ifp);
			clst_data_format = 2;
		}else if((arg_dim_clst>0)&&(arg_num_clst>0)&&(cluster_title==0)){
			fprintf(stderr,"[ERROR] you must specify data format.\n");
			exit(1);
		}else if((arg_dim_clst>0)&&(arg_num_clst>0)&&(cluster_title==1)){
			if((ifp = fopen(cluster_file,"r")) == NULL){
				perror(cluster_file);
				exit(1);
			}
			dim_clst = arg_dim_clst;
			fprintf(stderr," cluster dimensions:%d:\n",dim_clst);
			num_clst = arg_num_clst;
			fprintf(stderr,"number of clusters:%d:\n",num_clst);
				MAX_num_clst = (int)((float)num_clst * clst_expand_ratio);
				clst_matrix = f_alloc_mat(MAX_num_clst,dim_clst);
			read_ftable_from_stream(num_clst,dim_clst,ifp,clst_matrix);
			fclose(ifp);
			clst_data_format = 1;
		}else if((arg_dim_clst>0)&&(arg_num_clst>0)&&(cluster_title==2)){
			if((ifp = fopen(cluster_file,"r")) == NULL){
				perror(cluster_file);
				exit(1);
			}
			dim_clst = arg_dim_clst;
			fprintf(stderr,"cluster dimensions:%d:\n",dim_clst);
			num_clst = arg_num_clst;
			fprintf(stderr,"number of clusters:%d:\n",num_clst);
				MAX_num_clst = (int)((float)num_clst * clst_expand_ratio);
				clst_matrix = f_alloc_mat(MAX_num_clst,dim_clst);
				cluster_id = c_alloc_mat(MAX_num_clst,ID_LEN);
			read_ID_ftable_from_stream(num_clst,dim_clst,ifp,clst_matrix,cluster_id);
			fclose(ifp);
			clst_data_format = 2;

		}
		if((dim_smpl != dim_clst)){
			fprintf(stderr,"check : ");
			fprintf(stderr,"dim_smpl[%d]!=dim_clst[%d].\n",dim_smpl,dim_clst);
			exit(1);
		}
		fprintf(stderr," ... done.\n\n");
		/* *) */
		/* *} */
		if((option_status&1) == 1){
			print_check_opt(dim_smpl,num_smpl,dim_clst,num_clst);
			exit(0);
		}
		/* (* other memory allocations */
			MAX_num_clst = (int)((float)num_clst * clst_expand_ratio);
		dif_table = f_alloc_mat(MAX_num_clst,dim_clst);
		clst_smpl_dist_table = f_alloc_mat(num_smpl,MAX_num_clst);
		clst_clst_dist_table = f_alloc_mat(MAX_num_clst,MAX_num_clst);
		ave_ref_smpl_list = f_alloc_mat(MAX_num_clst,dim_clst);
		attrb_list_min = status_alloc_list(num_smpl);
		attrb_list_max = status_alloc_list(num_smpl);
		clst_status_list = clst_status_alloc_list(MAX_num_clst);
		if((cutoff == 0)&&(cutoff_gen == 0)){
			for(i=0;i<MAX_num_clst;i++){
				#ifdef ID_LIST_ALLOC
				clst_status_list[i].id_smpl_node = i_alloc_vec(ID_LIST_ALLOC);
				#else
				clst_status_list[i].id_smpl_node = i_alloc_vec(num_smpl);
				#endif
			}
		}else{
			for(i=0;i<MAX_num_clst;i++){
				#ifdef ID_LIST_ALLOC
				clst_status_list[i].id_smpl_node = i_alloc_vec(ID_LIST_ALLOC);
				clst_status_list[i].id_arranged_smpl_node = i_alloc_vec(ID_LIST_ALLOC);
				#else
				clst_status_list[i].id_smpl_node = i_alloc_vec(num_smpl);
				clst_status_list[i].id_arranged_smpl_node = i_alloc_vec(num_smpl);
				#endif
			}
		}
		if(check_cl == 1){
		clst_status_list_prev = clst_status_alloc_list(MAX_num_clst);
		if((cutoff == 0)&&(cutoff_gen == 0)){
			for(i=0;i<MAX_num_clst;i++){
				#ifdef ID_LIST_ALLOC
				clst_status_list_prev[i].id_smpl_node = i_alloc_vec(ID_LIST_ALLOC);
				#else
				clst_status_list_prev[i].id_smpl_node = i_alloc_vec(num_smpl);
				#endif
			}
		}else{
			for(i=0;i<MAX_num_clst;i++){
				#ifdef ID_LIST_ALLOC
				clst_status_list_prev[i].id_smpl_node = i_alloc_vec(ID_LIST_ALLOC);
				clst_status_list_prev[i].id_arranged_smpl_node = i_alloc_vec(ID_LIST_ALLOC);
				#else
				clst_status_list_prev[i].id_smpl_node = i_alloc_vec(num_smpl);
				clst_status_list_prev[i].id_arranged_smpl_node = i_alloc_vec(num_smpl);
				#endif
			}
		}
		}
		average_coordinate_data = f_alloc_vec(dim_smpl);
		/* *) */
		/* (* calculate avarage */
		for(j=0;j<dim_smpl;j++){
			average_coordinate_data[j] = 0;
		}
		for(i=0;i<num_smpl;i++){
			for(j=0;j<dim_smpl;j++){
				average_coordinate_data[j] += smpl_matrix[i][j];
			}
		}
		for(j=0;j<dim_clst;j++){
			average_coordinate_data[j] = average_coordinate_data[j]/num_smpl;
		}
		/* *) */
		/* (* open log file */
		if(strlen(log_file) > 0){
			lfp = fopen(log_file,"w");
			lf_open = 1;
		}else{
			lfp = stdout;
			lf_open = 0;
		}
		/* *) */
		/* (*print pre-information */
		if(monitor >= 2){
			fprintf(lfp,BRK_S " " BRK_S "\"center of data:\"" DELIM BRK_S);
			fprintf(lfp,"%f",average_coordinate_data[0]);
			for(j=1;j<dim_clst;j++){
				fprintf(lfp,DELIM "%f",average_coordinate_data[j]);
			}
			fprintf(lfp,BRK_E BRK_E);
		}
		/* *) */
		/* (* learning operation */
		/* condition */
		if(dist_func == 0){
			strcpy(message_loop_enter,"entering euclidian learning loop.\n");
			strcpy(message_loop_exit,"exiting euclidian learning loop.\n");
			dist_triangle_func = euc_dist_triangle;		/* assign functions */
			dist_table_func = euc_dist_table;		/* assign functions */
			distance_func = euc_dist;			/* assign functions */
		}else if(dist_func == 1){
			strcpy(message_loop_enter,"entering cosine learning loop.\n");
			strcpy(message_loop_exit,"exiting cosine learning loop.\n");
			dist_triangle_func = cos_dist_triangle;		/* assign functions */
			dist_table_func = cos_dist_table;		/* assign functions */
			distance_func = cos_dist;			/* assign functions */
		}else if(dist_func == 2){
			strcpy(message_loop_enter,"entering naf learning loop.\n");
			strcpy(message_loop_exit,"exiting naf learning loop.\n");
			dist_triangle_func = naf_dist_triangle;		/* assign functions */
			dist_table_func = naf_dist_table;		/* assign functions */
			distance_func = naf_dist;			/* assign functions */
		}else if(dist_func == 3){
			strcpy(message_loop_enter,"entering nwaf learning loop.\n");
			strcpy(message_loop_exit,"exiting nwaf learning loop.\n");
			dist_triangle_func = nwaf_dist_triangle;	/* assign functions */
			dist_table_func = nwaf_dist_table;		/* assign functions */
			distance_func = nwaf_dist;			/* assign functions */
		}

		/* loop */
		fprintf(stderr,"%s",message_loop_enter);
		/*inittialize-loop*/
		learning_loop_monitor(0,monitor,1,err_correct_rate,err_check,err_threshold,0,check_cl_interval,0,r_generate,generate_suppress_clst_num,generate_interval,MAX_num_clst,0,r_unified,unified_interval,unified_suppress_clst_num,0,delete_threshold,delete_interval,0,cutoff_value,cutoff_iteration,0,cutoff_ol_th,cutoff_gen_th,cutoff_interval,grouping_mode,grouping_radius_index,grouping_member_index,grouping_density_index,grouping_radius_ratio,0,move_rate,move_interval,r_move_suppress,dim_clst,num_clst,clst_matrix,dim_smpl,num_smpl,smpl_matrix,dif_table,clst_smpl_dist_table,clst_clst_dist_table,sum_ref_smpl_list,ave_ref_smpl_list,average_coordinate_data,&allocated_num_clst,&assigned_num_clst,&total_sum_dist,attrb_list_min,attrb_list_max,clst_status_list,clst_status_list_prev,sample_id,&loop_times,smpl_data_format,lfp);
		/*iterations*/
		learning_loop_monitor(1,monitor,loop,err_correct_rate,err_check,err_threshold,check_cl,check_cl_interval,generate,r_generate,generate_suppress_clst_num,generate_interval,MAX_num_clst,is_unified,r_unified,unified_interval,unified_suppress_clst_num,delete,delete_threshold,delete_interval,cutoff,cutoff_value,cutoff_iteration,cutoff_gen,cutoff_ol_th,cutoff_gen_th,cutoff_interval,grouping_mode,grouping_radius_index,grouping_member_index,grouping_density_index,grouping_radius_ratio,move,move_rate,move_interval,r_move_suppress,dim_clst,num_clst,clst_matrix,dim_smpl,num_smpl,smpl_matrix,dif_table,clst_smpl_dist_table,clst_clst_dist_table,sum_ref_smpl_list,ave_ref_smpl_list,average_coordinate_data,&allocated_num_clst,&assigned_num_clst,&total_sum_dist,attrb_list_min,attrb_list_max,clst_status_list,clst_status_list_prev,sample_id,&loop_times,smpl_data_format,lfp);
		/*finishing-loop*/
		assigned_num_clst = allocated_num_clst;
		for(i=0;i<allocated_num_clst;i++){
			if(clst_status_list[i].num_smpl_node == 0){
				assigned_num_clst--;
			}
		}
		fprintf(stderr,"%s",message_loop_exit);
		/* *) */



		/* (*print result */
		fprintf(stderr,"\nprinting result ...\n");
		if(monitor >= 2){
			fprintf(lfp,DELIM "\n\n" BRK_S "\"loop times:\"" DELIM "%d" BRK_E,loop_times);
			fprintf(lfp,DELIM "\n\n" BRK_S "\"RESULT::\"");
			fprintf(lfp,DELIM "\n\n" BRK_S "\"number of allocated clusters:\"" DELIM "%d" DELIM "%d" BRK_E,assigned_num_clst,allocated_num_clst);
		}
		if((monitor&2) == 2){
			fprintf(lfp,DELIM "\n\n" BRK_S "\"cluster dimension and number:\"" DELIM BRK_S "%d" DELIM "%d" BRK_E BRK_E,dim_clst,num_clst);
			fprintf(lfp,DELIM "\n\n" BRK_S "\"cluster coordimate:\"");
			for(i=0;i<allocated_num_clst;i++){
				fprintf(lfp,DELIM "\n" BRK_S "%f",clst_matrix[i][0]);
				for(j=1;j<dim_clst;j++){
					fprintf(lfp,DELIM "%f",clst_matrix[i][j]);
				}
				fprintf(lfp,BRK_E);
			}
			fprintf(lfp,BRK_E);
		}
		if((monitor&4) == 4){
			fprintf(lfp,DELIM "\n\n" BRK_S "\"clst - clst distance:\"");
			for(i=1;i<allocated_num_clst;i++){
				fprintf(lfp,DELIM "\n" BRK_S "%f",clst_clst_dist_table[i][0]);
				for(j=1;j<i;j++){
					fprintf(lfp,DELIM "%f",clst_clst_dist_table[i][j]);
				}
				fprintf(lfp,BRK_E);
			}
			fprintf(lfp,BRK_E);
		}
		if((monitor&8) == 8){
			fprintf(lfp,DELIM "\n\n" BRK_S "\"clst - data distance:\"");
			for(j=0;j<num_smpl;j++){
				fprintf(lfp,DELIM "\n" BRK_S "%f",clst_smpl_dist_table[j][0]);
				for(i=1;i<allocated_num_clst;i++){
					fprintf(lfp,DELIM "%f",clst_smpl_dist_table[j][i]);
				}
				fprintf(lfp,BRK_E);
			}
			fprintf(lfp,BRK_E);
		}
		if((monitor&16) == 16){
			fprintf(lfp,DELIM "\n\n" BRK_S "\"number of referencing data-nodes:\"");
			fprintf(lfp,DELIM "\n" BRK_S "\"clst_status_list[%d].num_smpl_node:\"" DELIM "%d" BRK_E,0,clst_status_list[0].num_smpl_node);
			for(i=1;i<allocated_num_clst;i++){
				fprintf(lfp,DELIM "\n" BRK_S "\"clst_status_list[%d].num_smpl_node:\"" DELIM "%d" BRK_E,i,clst_status_list[i].num_smpl_node);
			}
			fprintf(lfp,BRK_E DELIM "\n\n" BRK_S "\"sample number:\"");
			if(smpl_data_format == 1){
				for(i=0;i<allocated_num_clst;i++){
					fprintf(lfp,DELIM "\n" BRK_S "\"clst_status_list[%d].id_smpl_node[]:\"",i);
					for(j=0;j<clst_status_list[i].num_smpl_node;j++){
						fprintf(lfp,DELIM "%d",clst_status_list[i].id_smpl_node[j]);
					}
					fprintf(lfp,BRK_E);
				}
			}else if(smpl_data_format == 2){
				for(i=0;i<allocated_num_clst;i++){
					fprintf(lfp,DELIM "\n" BRK_S "\"clst_status_list[%d].id_smpl_node[]:\"",i);
					for(j=0;j<clst_status_list[i].num_smpl_node;j++){
						fprintf(lfp,DELIM "\"%s\"",sample_id[clst_status_list[i].id_smpl_node[j]]);
					}
					fprintf(lfp,BRK_E);
				}
			}
			fprintf(lfp,BRK_E);
		}
		if((monitor&32) == 32){
			total_sum_dist = 0;
			fprintf(lfp,DELIM "\n\n" BRK_S "\"distance of cluster - referencing data {id,sum,ave}:\"");
			for(i=0;i<allocated_num_clst;i++){
				total_sum_dist += clst_status_list[i].sum_dist;
				fprintf(lfp,DELIM "\n" BRK_S "%d" DELIM "%f" DELIM "%f" BRK_E,i,clst_status_list[i].sum_dist,clst_status_list[i].ave_dist);
			}
			fprintf(lfp,DELIM "\n" BRK_S "\"Total,Ave_clst,Ave_data:\"" DELIM "%f" DELIM "%f" DELIM "%f" BRK_E BRK_E,total_sum_dist,total_sum_dist/assigned_num_clst,total_sum_dist/num_smpl);
		}
		if((monitor&64) == 64){
			fprintf(lfp,DELIM "\n\n" BRK_S "\"error:\"");
			for(j=0;j<allocated_num_clst;j++){
				fprintf(lfp,DELIM "\n" BRK_S "%4.10f",dif_table[j][0]);
				for(i=1;i<dim_clst;i++){
					fprintf(lfp,DELIM "%4.10f",dif_table[j][i]);
				}
				fprintf(lfp,BRK_E);
			}
			fprintf(lfp,BRK_E);
		}
		if(monitor >= 2){
			fprintf(lfp,BRK_E BRK_E "\n");
		}
		if(lf_open == 1){
			fclose(lfp);
		}
		fprintf(stderr," ... done.\n");
		/* *) */



		/* (* write to output file */
		if((option_status&256) == 256){
			fprintf(stderr,"\nwriting output file ...\n");
			if(strcmp(of_type,"C") == 0){
				ofp = fopen(output_file,"w");
				if(ofp == NULL){
					perror(output_file);
					exit(1);
				}
				fprintf(ofp,"%d %d\n",dim_clst,assigned_num_clst);
				for(i=0;i<allocated_num_clst;i++){
					if(clst_status_list[i].num_smpl_node != 0){
						fprintf(ofp,"%d ",i);
						for(j=0;j<dim_clst;j++){
							fprintf(ofp,"%f ",clst_matrix[i][j]);
						}
						fprintf(ofp,"\n");
					}
				}
				fclose(ofp);
			}else if(strcmp(of_type,"A") == 0){
				ofp = fopen(output_file,"w");
				if(ofp == NULL){
					perror(output_file);
					exit(1);
				}
				fprintf(ofp,"<%d>",allocated_num_clst);
				for(i=0;i<allocated_num_clst;i++){
					fprintf(ofp,"\n%d %d\n",dim_clst,clst_status_list[i].num_smpl_node);
					for(j=0;j<clst_status_list[i].num_smpl_node;j++){
						/*fprintf(ofp,"%d ",clst_status_list[i].id_smpl_node[j]);*/
						fprintf(ofp,"%s ",sample_id[clst_status_list[i].id_smpl_node[j]]);
						for(k=0;k<dim_smpl;k++){
							fprintf(ofp,"%f ",smpl_matrix[clst_status_list[i].id_smpl_node[j]][k]);
						}
						fprintf(ofp,"\n");
					}
					fprintf(ofp,CL_DELIM_STR);
				}
				fprintf(ofp,"\n");
				fclose(ofp);
			}else if(strcmp(of_type,"AC") == 0){
				ofp = fopen(output_file,"w");
				if(ofp == NULL){
					perror(output_file);
					exit(1);
				}
				fprintf(ofp,"<%d>",allocated_num_clst);
				for(i=0;i<allocated_num_clst;i++){
					fprintf(ofp,"\n%d %d\n",dim_clst,clst_status_list[i].num_smpl_node);
					for(j=0;j<clst_status_list[i].num_smpl_node;j++){
						/*fprintf(ofp,"%d ",clst_status_list[i].id_smpl_node[j]);*/
						fprintf(ofp,"%s ",sample_id[clst_status_list[i].id_smpl_node[j]]);
						for(k=0;k<dim_smpl;k++){
							fprintf(ofp,"%f ",smpl_matrix[clst_status_list[i].id_smpl_node[j]][k]);
						}
						fprintf(ofp,"\n");
					}
					fprintf(ofp,CL_DELIM_STR);
				}
				fprintf(ofp,"\n");
				fprintf(ofp,"%d %d\n",dim_clst,assigned_num_clst);
				for(i=0;i<allocated_num_clst;i++){
					if(clst_status_list[i].num_smpl_node != 0){
						fprintf(ofp,"%d ",i+num_smpl);
						for(j=0;j<dim_clst;j++){
							fprintf(ofp,"%f ",clst_matrix[i][j]);
						}
						fprintf(ofp,"\n");
					}
				}
				fclose(ofp);
			}
			fprintf(stderr," ... done.\n");
		}
		/* *) */



		/* (* write to report file */
		if((option_status&2048) == 2048){
			fprintf(stderr,"\nwriting report file ...\n");
			fprintf(stderr,"[WARN] report file option may contain bug.\n");
			clst_ave_vec = d_calloc_vec(dim_clst);
			clst_sd_vec = d_calloc_vec(dim_clst);
			smpl_ave_vec = d_calloc_vec(dim_smpl);
			smpl_sd_vec = d_calloc_vec(dim_smpl);
			num_of_output_clsts = assigned_num_clst;
			output_clsts = i_alloc_vec(num_of_output_clsts);
			j = 0;
			for(i=0;i<allocated_num_clst;i++){
				if(clst_status_list[i].num_smpl_node != 0){
					output_clsts[j] = i;
					j++;
				}
			}
			/* (* open file */
			rfp = fopen(report_file,"w");
			if(rfp == NULL){
				perror(report_file);
				exit(1);
			}
			/* *) */
			/* (* number of cluster */
			fprintf(rfp,"<%d>\n",assigned_num_clst);
			/* *) */
			/* (* all node calc */
			/* SUM */
			for(i=0;i<num_smpl;i++){
				for(j=0;j<dim_smpl;j++){
					smpl_ave_vec[j] += smpl_matrix[i][j];
				}
			}
			/* MEAN */
			for(j=0;j<dim_smpl;j++){
				smpl_ave_vec[j] = smpl_ave_vec[j]/num_smpl;
			}
			/* *) */
			/* (* sample nodes */
			for(i=0;i<num_of_output_clsts;i++){
				fprintf(rfp,"%d %d\n",dim_clst,clst_status_list[output_clsts[i]].num_smpl_node);
				if((smpl_data_format == 1)||(smpl_data_format == 2)){
					for(j=0;j<clst_status_list[output_clsts[i]].num_smpl_node;j++){
						for(k=0;k<dim_clst;k++){
							clst_ave_vec[k] += smpl_matrix[clst_status_list[output_clsts[i]].id_smpl_node[j]][k];
						}
					}
					fprintf(rfp,"SUM ");
					for(k=0;k<dim_clst;k++){
						fprintf(rfp,"%f ",clst_ave_vec[k]);
					}
					fprintf(rfp,"\nMEAN ");
					for(k=0;k<dim_clst;k++){
						fprintf(rfp,"%f ",clst_ave_vec[k] = clst_ave_vec[k]/clst_status_list[output_clsts[i]].num_smpl_node);
					}
					fprintf(rfp,"\nSD ");
					for(j=0;j<clst_status_list[output_clsts[i]].num_smpl_node;j++){
						for(k=0;k<dim_clst;k++){
							//clst_sd_vec[k] += pow((smpl_matrix[clst_status_list[output_clsts[i]].id_smpl_node[j]][k] - clst_ave_vec[k]),2);
							clst_sd_vec[k] += ((smpl_matrix[clst_status_list[output_clsts[i]].id_smpl_node[j]][k] - clst_ave_vec[k]) * (smpl_matrix[clst_status_list[output_clsts[i]].id_smpl_node[j]][k] - clst_ave_vec[k]));
						}
					}
					for(k=0;k<dim_clst;k++){
						fprintf(rfp,"%f ",sqrt(clst_sd_vec[k]/(clst_status_list[output_clsts[i]].num_smpl_node - 1)));
					}
					for(k=0;k<dim_clst;k++){
						clst_ave_vec[k] = 0;
					}
					for(k=0;k<dim_clst;k++){
						clst_sd_vec[k] = 0;
					}
				}
				fprintf(rfp,"\n%s\n",CL_DELIM_STR);
			}
			/* *) */
			/* (* all node print */
			fprintf(rfp,"%d %d\n",dim_smpl,num_smpl);
			fprintf(rfp,"SUM ");
			for(j=0;j<dim_smpl;j++){
				fprintf(rfp,"%f ",smpl_ave_vec[j]*num_smpl);
			}
			fprintf(rfp,"\nMEAN ");
			for(j=0;j<dim_smpl;j++){
				fprintf(rfp,"%f ",smpl_ave_vec[j]);
			}
			fprintf(rfp,"\nSD ");
			for(i=0;i<num_smpl;i++){
				for(j=0;j<dim_smpl;j++){
					smpl_sd_vec[j] += ((smpl_matrix[i][j] - smpl_ave_vec[j]) * (smpl_matrix[i][j] - smpl_ave_vec[j]));
				}
			}
			for(j=0;j<dim_smpl;j++){
				fprintf(rfp,"%f ",sqrt(smpl_sd_vec[j]/(num_smpl - 1)));
			}
			/* *) */
			fprintf(rfp,"\n");
			/* (* close file */
			fclose(rfp);
			/* *) */
			fprintf(stderr," ... done.\n");
		}
		/* *) */
	}else{
		usage();
	}
	return(0);
}
