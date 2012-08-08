/* math_base.c */

/*************************************************/
/* Copyright (c) 2003-2007 AMANO Kou             */
/* University of Liblary And Information Science */
/* National Institute of Agrobilogical Sciences  */
/* University of Tsukuba                         */
/* RIKEN BioResource Center                      */
/* <kamano@affrc.go.jp>,<amano@brc.riken.jp>     */
/*************************************************/

#include <math.h>
#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif
#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif
#define DELTAP 100
#define DELTAN -100


float norm(int len, float *list){
	int i;
	float nor = 0;
	for(i=0;i<len;i++){
		//nor += pow(list[i],2);
		nor += (list[i] * list[i]);
	}
	nor = sqrt(nor);
	return(nor);
}

double d_norm(int len, double *list){
	int i;
	double nor = 0;
	for(i=0;i<len;i++){
		//nor += pow(list[i],2);
		nor += (list[i] * list[i]);
	}
	nor = sqrt(nor);
	return(nor);
}

float inner(int len, float *list1, float *list2){
	int i;
	float inn = 0;
	for(i=0;i<len;i++){
		inn += (list1[i] * list2[i]);
	}
	return(inn);
}

double d_inner(int len, double *list1, double *list2){
	int i;
	double inn = 0;
	for(i=0;i<len;i++){
		inn += (list1[i] * list2[i]);
	}
	return(inn);
}

float waf(int len, float *list1, float *list2){
        int i = 0;
        float count_list1 = 0;
        float count_list2 = 0;
        float *weight = NULL;
        float *Pi = NULL;
        float ret = 0;
        for(i=0;i<len;i++){
                count_list1 += list1[i];
                count_list2 += list2[i];
        }
        weight = f_calloc_vec(len);
        for(i=0;i<len;i++){
                weight[i] = list1[i] + list2[i];
                if(weight[i] == 0){
                        ;
                }else{
                        weight[i] = 1/weight[i];
                }
        }
        Pi = f_balloc_vec(len,1);
        for(i=0;i<len;i++){
                Pi[i] = list1[i] * list2[i];
                Pi[i] = Pi[i] * weight[i];
        }
        for(i=0;i<len;i++){
                ret += Pi[i];
        }
        ret = ret * (count_list1 + count_list2) / (count_list1 * count_list2);
	free(weight);
	free(Pi);
        return(ret);
}

float waf_bf(int len, float *list1, float *list2, float *weight, float *Pi){
        int i = 0;
        float count_list1 = 0;
        float count_list2 = 0;
        float ret = 0;
        for(i=0;i<len;i++){
                count_list1 += list1[i];
                count_list2 += list2[i];
        }
        for(i=0;i<len;i++){
                weight[i] = list1[i] + list2[i];
                if(weight[i] == 0){
                        ;
                }else{
                        weight[i] = 1/weight[i];
                }
        }
        for(i=0;i<len;i++){
                Pi[i] = list1[i] * list2[i];
                Pi[i] = Pi[i] * weight[i];
        }
        for(i=0;i<len;i++){
                ret += Pi[i];
        }
        ret = ret * (count_list1 + count_list2) / (count_list1 * count_list2);
        return(ret);
}

float waf_multi(int num_vec, int size_vec, int *count_vec, float **gram_vec_vec){
        int i = 0;
        int j = 0;
        float *prod_vec = NULL;
        float *weight_vec = NULL;
        float sim = 0;
        float Sigma = 0;
        float Pi = 1;
        /* (* weight_vec */
        weight_vec = f_calloc_vec(size_vec);
        for(i=0;i<num_vec;i++){
                for(j=0;j<size_vec;j++){
                        weight_vec[j] += gram_vec_vec[i][j];
                }
        }
        for(j=0;j<size_vec;j++){
                if(weight_vec[j] != 0){
                        weight_vec[j] = 1/weight_vec[j];
                }
        }
        /* *) */
        //f_print_vec(3,weight_vec,stdout);
        /* (* prod_vec */
        prod_vec = f_balloc_vec(size_vec,1);
        for(i=0;i<num_vec;i++){
                for(j=0;j<size_vec;j++){
                        prod_vec[j] *= gram_vec_vec[i][j];
                }
        }
        /* *) */
        //f_print_vec(3,prod_vec,stdout);
        /* (* weight_vec * prod_vec */
        for(j=0;j<size_vec;j++){
                prod_vec[j] = weight_vec[j] * prod_vec[j];
        }
        /* *) */
        //f_print_vec(3,prod_vec,stdout);
        /* (* Sigma count_vec */
        for(i=0;i<num_vec;i++){
                Sigma += count_vec[i];
        }
        /* *) */
        //f_print(Sigma,stdout);
        /* (* Pi count_vec */
        for(i=0;i<num_vec;i++){
                Pi *= count_vec[i];
        }
        /* *) */
        //f_print(Pi,stdout);
        /* (* sim */
        for(j=0;j<size_vec;j++){
                sim += prod_vec[j];
        }
        sim = sim * Sigma / Pi;
        /* *) */
        //f_print(sim,stdout);
	free(prod_vec);
	free(weight_vec);
        return(sim);
}

float veccos(int len, float *list1, float *list2){
	float nor1 = 0;
	float nor2 = 0;
	float dist = 0;
	nor1 = norm(len,list1);
	nor2 = norm(len,list2);
	dist = inner(len,list1,list2);
	dist = (dist / nor1 / nor2);
	return(dist);
}

float cos_dist(int len, float *list1, float *list2){
	float nor1 = 0;
	float nor2 = 0;
	float dist = 0;
	nor1 = norm(len,list1);
	nor2 = norm(len,list2);
	dist = inner(len,list1,list2);
	dist = 1 - (dist / nor1 / nor2);
	return(dist);
}

float iaf_dist(int len, float *list1, float *list2){
	float af11 = 0;
	float af22 = 0;
	float af12 = 0;
	float dist = 0;
	af11 = inner(len,list1,list1);
	af22 = inner(len,list2,list2);
	af12 = inner(len,list1,list2);
	dist = af11 * af22 / af12 / af12;
	return(dist);
}

float naf_dist(int len, float *list1, float *list2){
	float af11 = 0;
	float af22 = 0;
	float af12 = 0;
	float dist = 0;
	af11 = inner(len,list1,list1);
	af22 = inner(len,list2,list2);
	af12 = inner(len,list1,list2);
	//dist = af11 * af22 / af12 / af12;
	dist = af12 * af12 / af11 / af22;
	dist = 1-dist;
	return(dist);
}

float nwaf_dist(int len, float *list1, float *list2){
	return(1 - waf(len,list1,list2));
}

float d_cos_dist(int len, double *list1, double *list2){
	double nor1 = 0;
	double nor2 = 0;
	double dist = 0;
	nor1 = d_norm(len,list1);
	nor2 = d_norm(len,list2);
	dist = d_inner(len,list1,list2);
	dist = 1 - (dist / nor1 / nor2);
	return(dist);
}

float sq_euc_dist(int len, float *list1, float *list2){
	int i;
	float sq_sum = 0;
	for(i=0;i<len;i++){
		//sq_sum += pow(list1[i]-list2[i],2);
		sq_sum += ((list1[i]-list2[i]) * (list1[i]-list2[i]));
	}
	return(sq_sum);
}

double d_sq_euc_dist(int len, double *list1, double *list2){
	int i;
	double sq_sum = 0;
	for(i=0;i<len;i++){
		//sq_sum += pow(list1[i]-list2[i],2);
		sq_sum += ((list1[i]-list2[i]) * (list1[i]-list2[i]));
	}
	return(sq_sum);
}

float euc_dist(int len, float *list1, float *list2){
	int i;
	float sq_sum = 0;
	for(i=0;i<len;i++){
		//sq_sum += pow(list1[i]-list2[i],2);
		sq_sum += ((list1[i]-list2[i]) * (list1[i]-list2[i]));
	}
	return(sqrt(sq_sum));
}

double d_euc_dist(int len, double *list1, double *list2){
	int i;
	double sq_sum = 0;
	for(i=0;i<len;i++){
		//sq_sum += pow(list1[i]-list2[i],2);
		sq_sum += ((list1[i]-list2[i]) * (list1[i]-list2[i]));
	}
	return(sqrt(sq_sum));
}


float synteny_dist(int len, float *list1, float *list2, int axlen, int *ax){
	//extern int *syn_negax;
	int i;
	int j;
	float r = 0;
	float cs = 0;
	float dist = 0;
	float *tmp_base_posi = NULL;
	//float *tmp_base_nega = NULL;
	float *tmp_sample = NULL;
	tmp_base_posi = f_calloc_vec(len);
	//tmp_base_nega = f_calloc_vec(len);
	tmp_sample = f_calloc_vec(len);
	r = euc_dist(len,list1,list2);
	for(i=0;i<len;i++){
		for(j=0;j<axlen;j++){
			tmp_base_posi[i] = DELTAP;
			if(i == j){
				tmp_base_posi[i] = DELTAN;
				break;
			}
		}
	}
	for(i=0;i<len;i++){
		tmp_sample[i] = list2[i] - list1[i];
	}
	cs = veccos(len,tmp_base_posi,tmp_sample);
	dist = r * (2 - cs);	/*2: parameter*/
	free(tmp_base_posi);
	return(dist);
}


void cos_dist_list(int num, int dim, float **arr, float *list, float *dist_list){
	int l;
	for(l=0;l<num;l++){
		dist_list[l] = cos_dist(dim,list,arr[l]);
	}
}

void sq_euc_dist_list(int num, int dim, float **arr, float *list, float *dist_list){
	int l;
	for(l=0;l<num;l++){
		dist_list[l] = sq_euc_dist(dim,list,arr[l]);
	}
}

void euc_dist_list(int num, int dim, float **arr, float *list, float *dist_list){
	int l;
	for(l=0;l<num;l++){
		dist_list[l] = euc_dist(dim,list,arr[l]);
	}
}

void cos_dist_sqtable(int num, int dim, float **arr, float **dist_table){
	int l;
	int m;
	for(l=0;l<num;l++){
		for(m=0;m<num;m++){
			dist_table[l][m] = cos_dist(dim,arr[l],arr[m]);
		}
	}
}

void sq_euc_dist_sqtable(int num, int dim, float **arr, float **dist_table){
	int l;
	int m;
	for(l=0;l<num;l++){
		for(m=0;m<num;m++){
			dist_table[l][m] = sq_euc_dist(dim,arr[l],arr[m]);
		}
	}
}

void euc_dist_sqtable(int num, int dim, float **arr, float **dist_table){
	int l;
	int m;
	for(l=0;l<num;l++){
		for(m=0;m<num;m++){
			dist_table[l][m] = euc_dist(dim,arr[l],arr[m]);
		}
	}
}

void euc_dist_table(int num1, float **arr1, int num2, float **arr2, int dim, float **dist_table){
	int l;
	int m;
	for(l=0;l<num1;l++){
		for(m=0;m<num2;m++){
			dist_table[l][m] = euc_dist(dim,arr1[l],arr2[m]);
		}
	}
}

void cos_dist_table(int num1, float **arr1, int num2, float **arr2, int dim, float **dist_table){
	int l;
	int m;
	for(l=0;l<num1;l++){
		for(m=0;m<num2;m++){
			dist_table[l][m] = cos_dist(dim,arr1[l],arr2[m]);
		}
	}
}

void iaf_dist_table(int num1, float **arr1, int num2, float **arr2, int dim, float **dist_table){
	int l;
	int m;
	for(l=0;l<num1;l++){
		for(m=0;m<num2;m++){
			dist_table[l][m] = iaf_dist(dim,arr1[l],arr2[m]);
		}
	}
}

void naf_dist_table(int num1, float **arr1, int num2, float **arr2, int dim, float **dist_table){
	int l;
	int m;
	for(l=0;l<num1;l++){
		for(m=0;m<num2;m++){
			dist_table[l][m] = naf_dist(dim,arr1[l],arr2[m]);
		}
	}
}

void nwaf_dist_table(int num1, float **arr1, int num2, float **arr2, int dim, float **dist_table){
	int l;
	int m;
	for(l=0;l<num1;l++){
		for(m=0;m<num2;m++){
			dist_table[l][m] = nwaf_dist(dim,arr1[l],arr2[m]);
		}
	}
}

void synteny_dist_table(int num1, float **arr1, int num2, float **arr2, int dim, float **dist_table, int axlen, int *ax){
	int l;
	int m;
	for(l=0;l<num1;l++){
		for(m=0;m<num2;m++){
			dist_table[l][m] = synteny_dist(dim,arr1[l],arr2[m],axlen,ax);
		}
	}
}

void cos_dist_triangle(int num, int dim, float **arr, float **dist_table){
	int l;
	int m;
	for(l=0;l<num;l++){
		for(m=0;m<l;m++){
			dist_table[l][m] = cos_dist(dim,arr[l],arr[m]);
		}
	}
}

void iaf_dist_triangle(int num, int dim, float **arr, float **dist_table){
	int l;
	int m;
	for(l=0;l<num;l++){
		for(m=0;m<l;m++){
			dist_table[l][m] = iaf_dist(dim,arr[l],arr[m]);
		}
	}
}

void naf_dist_triangle(int num, int dim, float **arr, float **dist_table){
	int l;
	int m;
	for(l=0;l<num;l++){
		for(m=0;m<l;m++){
			dist_table[l][m] = naf_dist(dim,arr[l],arr[m]);
		}
	}
}

void nwaf_dist_triangle(int num, int dim, float **arr, float **dist_table){
	int l;
	int m;
	for(l=0;l<num;l++){
		for(m=0;m<l;m++){
			dist_table[l][m] = nwaf_dist(dim,arr[l],arr[m]);
		}
	}
}

void sq_euc_dist_triangle(int num, int dim, float **arr, float **dist_table){
	int l;
	int m;
	for(l=0;l<num;l++){
		for(m=0;m<l;m++){
			dist_table[l][m] = sq_euc_dist(dim,arr[l],arr[m]);
		}
	}
}

void euc_dist_triangle(int num, int dim, float **arr, float **dist_table){
	int l;
	int m;
	for(l=0;l<num;l++){
		for(m=0;m<l;m++){
			dist_table[l][m] = euc_dist(dim,arr[l],arr[m]);
		}
	}
}

void synteny_dist_triangle(int num, int dim, float **arr, float **dist_table, int axlen, int *ax){
	int l;
	int m;
	for(l=0;l<num;l++){
		for(m=0;m<l;m++){
			dist_table[l][m] = synteny_dist(dim,arr[l],arr[m],axlen,ax);
		}
	}
}

void max_list(int len, float *list, float out_max){
	int l;
	out_max = list[0];
	for(l=1;l<len;l++){
		if(out_max < list[l]){
			out_max = list[l];
		}
	}
}

void min_list(int len, float *list, float out_min){
	int l;
	out_min = list[0];
	for(l=1;l<len;l++){
		if(out_min > list[l]){
			out_min = list[l];
		}
	}
}

void max_pos_list(int len, float *list, int *pos, float *out_max){
	int l;
	*out_max = list[0];
	*pos = 0;
	for(l=1;l<len;l++){
		if(*out_max < list[l]){
			*out_max = list[l];
			*pos = l;
		}
	}
}

void min_pos_list(int len, float *list, int *pos, float *out_min){
	int l;
	*out_min = list[0];
	*pos = 0;
	for(l=1;l<len;l++){
		if(*out_min > list[l]){
			*out_min = list[l];
			*pos = l;
		}
	}
}

void max_list_from_matrix(int dim, int num, float **matrix, float *out_list, int axis){
	int l;
	int m;
	if(axis == 1){
		for(l=0;l<num;l++){
			out_list[l] = matrix[l][0];
			for(m=1;m<dim;m++){
				if(out_list[l] < matrix[l][m]){
					out_list[l] = matrix[l][m];
				}
			}
		}
	}else if(axis == 0){
		for(m=0;m<dim;m++){
			out_list[m] = matrix[0][m];
			for(l=0;l<num;l++){
				if(out_list[m] < matrix[l][m]){
					out_list[m] = matrix[l][m];
				}
			}
		}
	}
}

void min_list_from_matrix(int dim, int num, float **matrix, float *out_list, int axis){
	int l;
	int m;
	if(axis == 1){
		for(l=0;l<num;l++){
			out_list[l] = matrix[l][0];
			for(m=1;m<dim;m++){
				if(out_list[l] > matrix[l][m]){
					out_list[l] = matrix[l][m];
				}
			}
		}
	}else if(axis == 0){
		for(m=0;m<dim;m++){
			out_list[m] = matrix[0][m];
			for(l=0;l<num;l++){
				if(out_list[m] > matrix[l][m]){
					out_list[m] = matrix[l][m];
				}
			}
		}
	}
}

void max_pos_matrix(int dim, int num, float **matrix, int *pos, float *out_list, int axis){
	int l;
	int m;
	if(axis == 1){
		for(l=0;l<num;l++){
			out_list[l] = matrix[l][0];
			pos[l] = 0;
			for(m=1;m<dim;m++){
				if(out_list[l] < matrix[l][m]){
					out_list[l] = matrix[l][m];
					pos[l] = m;
				}
			}
		}
	}else if(axis == 0){
		for(m=0;m<dim;m++){
			out_list[m] = matrix[0][m];
			pos[m] = 0;
			for(l=1;l<num;l++){
				if(out_list[m] < matrix[l][m]){
					out_list[m] = matrix[l][m];
					pos[m] = l;
				}
			}
		}
	}
}

void min_pos_matrix(int dim, int num, float **matrix, int *pos, float *out_list, int axis){
	int l;
	int m;
	if(axis == 1){
		for(l=0;l<num;l++){
			out_list[l] = matrix[l][0];
			pos[l] = 0;
			for(m=1;m<dim;m++){
				if(out_list[l] > matrix[l][m]){
					out_list[l] = matrix[l][m];
					pos[l] = m;
				}
			}
		}
	}else if(axis == 0){
		for(m=0;m<dim;m++){
			out_list[m] = matrix[0][m];
			pos[m] = 0;
			for(l=1;l<num;l++){
				if(out_list[m] > matrix[l][m]){
					out_list[m] = matrix[l][m];
					pos[m] = l;
				}
			}
		}
	}
}

void min_dist_table_position(int num, float **matrix, float *dist, int *position){
	int l;
	int m;
	float minimum = matrix[1][0];
	int l_pos = 1;
	int m_pos = 0;
	for(l=1;l<num;l++){
		for(m=0;m<l;m++){
			if(minimum > matrix[l][m]){
				minimum = matrix[l][m];
				l_pos = l;
				m_pos = m;
			}
		}
	}
	*dist = minimum;
	position[0] = l_pos;
	position[1] = m_pos;
}

float arithm_ave(int dim, float *list){
	int l;
	float sum = 0;
	for(l=0;l<dim;l++){
		sum += list[l];
	}
	return(sum/dim);
}

double d_arithm_ave(int dim, double *list){
	int l;
	double sum = 0;
	for(l=0;l<dim;l++){
		sum += list[l];
	}
	return(sum/dim);
}

void arithm_ave_list(int num, int dim, float **arr, float *list){
	int l;
	int m;
	for(l=0;l<num;l++){
		for(m=0;m<dim;m++){
			list[m] += arr[l][m];
		}
	}
	for(m=0;m<dim;m++){
		list[m] = list[m]/num;
	}
}

void d_arithm_ave_list(int num, int dim, double **arr, double *list){
	int l;
	int m;
	for(l=0;l<num;l++){
		for(m=0;m<dim;m++){
			list[m] += arr[l][m];
		}
	}
	for(m=0;m<dim;m++){
		list[m] = list[m]/num;
	}
}

void add_column_of_matrix(int num, int dim, int *ref_list, float **matrix, float *list){
	int l;
	int m;
	for(l=0;l<num;l++){
		for(m=0;m<dim;m++){
			list[m] += matrix[ref_list[l]][m];
		}
	}
}

void dif_2lists(int num, float *list1, float *list2, float *list_out){
	int l;
	for(l=0;l<num;l++){
		list_out[l] = list2[l] - list1[l];
	}
}

