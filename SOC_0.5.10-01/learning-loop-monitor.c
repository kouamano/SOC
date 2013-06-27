/* learning-loop-monitor.c */

/**********************************************************/
/* Copyright (c) 2003-2009 AMANO Kou <kamano@affrc.go.jp> */
/* University of Library And Information Science          */
/* National Institute of Agrobilogical Sciences           */
/* RIKEN BioResource Center                               */
/* University of Tsukuba                                  */
/**********************************************************/

void learning_loop_monitor(int _loop_offset, int _monitor, int _loop, float _err_correct_rate, int _err_ck, float _err_thrld, int _check_cl, int _check_cl_interval, int _generated, float _r_generated, int _generated_suppress_clst_num, int _generated_interval, int _max_num_clst, int _unified, float _r_unified, int _u_interval, int _u_suppress_clst_num, int _delete, float _delete_threshold, int _delete_interval, int _cutoff, float _cutoff_value, int _cutoff_iteration, int _cutoff_gen, float _cutoff_ol_th, float _cutoff_gen_th, int _cutoff_interval, int _grouping_mode, float _grouping_radius_index, float _grouping_member_index, float _grouping_density_index, float _grouping_radius_ratio, int _moved, float _moved_rate, int _moved_interval, float _r_moved_suppress, int _dim_clst, int _num_clst, float **_clst_matrix, int _dim_smpl, int _num_smpl, float **_smpl_matrix, float **_dif_table, float **_clst_smpl_dist_table, float **_clst_clst_dist_table, float **_sum_ref_smpl_list, float **_ave_ref_smpl_list, float *_ave_coordinate_smpl, int *_alloc_num_clst, int *_assigned_num_clst, float *_total_sum_dist, struct status *_attrb_list_min, struct status *_attrb_list_max, struct clst_status *_clst_status_list, struct clst_status *_clst_status_list_prev, char **_sample_id, int *_l_times, int _data_form,FILE *_lfp){
	int a = _loop_offset;
	int i = 0;
	int j = 0;
	int k = 0;
	int assigned_num_clst = 0;
	float clst_clst_min_dist = 0;		/* for clst unifying*/
	int clst_clst_min_position[2] = {0,0};	/* for clst unifying*/
	float *merged_list = NULL;
	int *flaged_list = NULL;
	float dist_clst_to_ave_smpl = 0;
	float ave_ave_radius_clst = 0;		/* for generated */
	float ave_ave_clst_num = 0;		/* for generated */
	float **arranged_clst_smpl_dist_table = NULL;
	int *min_pos_to_data_list = NULL;	/*for moving*/
	float *min_dist_to_data_list = NULL;	/*for moving*/
	int *max_pos_to_data_list = NULL;	/*for moving*/
	float *max_dist_to_data_list = NULL;	/*for moving*/
	int *outlier = NULL;			/*for cutoff_gen*/
	int *outlier_stat = NULL;		/*for cutoff_gen*/
	float **outlier_vec = NULL;		/*for cutoff_gen*/
	float **outlier_dist_tri = NULL;	/*for cutoff_gen*/
	int num_outlier = 0;			/*for cutoff_gen*/
	float ave_ave_radius_th = 0;		/*for cutoff_gen*/
	float ave_ave_radius_th_del = 0;	/*for delete*/
	float th_del = 0;			/*for delete*/
	int num_del_clst = 0;			/*for delete*/
	int *del_clst_list = NULL;		/*for delete length=num_del_clst*/
	int *del_clst_count_list = NULL;	/*for delete length=_num_clst*/
	float *total_sq_err = 0;
	float ave_total_sq_err = 0;
	float unifying_dist = 0;
	struct status generated_node;
	int check_cl_count = 0;
	merged_list = f_alloc_vec(_dim_clst);
	flaged_list = i_alloc_vec(_num_clst);
	outlier = i_alloc_vec(_num_smpl);
	del_clst_list = i_calloc_vec(_max_num_clst);
	del_clst_count_list = i_calloc_vec(_max_num_clst);
	if((_monitor&1) == 1){
		fprintf(_lfp,"\n\n" DELIM BRK_S);
	}
	if(grouping_mode >= 1){
			arranged_clst_smpl_dist_table = f_alloc_mat(_num_smpl,_num_clst);
	}



	for(a=_loop_offset;a<_loop;a++){
		(*_l_times)++;
		/* (* print _clst_matrix */
		if((_monitor >= 2)&&((_monitor&1) == 1)&&(a >= 1)){
			fprintf(_lfp,"\n" DELIM BRK_S BRK_S "\"loop[%d]::\"" BRK_E,a);
		}else if((_monitor >= 2)&&((_monitor&1) == 1)&&(a == 0)){
			fprintf(_lfp,BRK_S BRK_S "\"loop[%d]::\"" BRK_E,a);
		}
		if((_monitor&3) == 3){
			fprintf(_lfp,DELIM "\n" BRK_S "\"clst dim,num:\"" DELIM "%d" DELIM "%d" BRK_E DELIM "\n",_dim_clst,_num_clst);
			fprintf(_lfp,BRK_S "\"clst coordinate:\"");
			for(i=0;i<_num_clst;i++){
				fprintf(_lfp,DELIM "\n" BRK_S "%f",_clst_matrix[i][0]);
				for(j=1;j<_dim_clst;j++){
					fprintf(_lfp,DELIM "%f",_clst_matrix[i][j]);
				}
				fprintf(_lfp,BRK_E);
			}
			fprintf(_lfp,BRK_E);
		}
		/* *) */



		/* (* [2]. caluculate _clst_smpl_dist_table */
		dist_table_func(_num_smpl,_smpl_matrix,_num_clst,_clst_matrix,_dim_smpl,_clst_smpl_dist_table);
		/* *) */
		/* (* print _clst_smpl_dist_table */
		if((_monitor&9) == 9){
			fprintf(_lfp,DELIM "\n" BRK_S "\"clst-data distance:\"");
			for(j=0;j<_num_smpl;j++){
				fprintf(_lfp,DELIM "\n" BRK_S "%f",_clst_smpl_dist_table[j][0]);
				for(i=1;i<_num_clst;i++){
					fprintf(_lfp,DELIM "%f",_clst_smpl_dist_table[j][i]);
				}
				fprintf(_lfp,BRK_E);
			}
			fprintf(_lfp,BRK_E);
		}
		/* *) */



		/* (* [3]. find farther clst from each sample (_attrb_list_max) */
		if(_generated == 8){
			if(_grouping_mode == 0){
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_max,i,3);
				}
	
	
			}else if(_grouping_mode == 1){
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_max,i,1);
				}
				for(j=0;j<_num_smpl;j++){
					for(i=0;i<_num_clst;i++){
						if(_clst_status_list[i].ave_dist <= 0){
							arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i];
						}else{
							arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i]*pow(_clst_status_list[i].ave_dist,_grouping_radius_index);
						}
					}
				}
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_max,i,2);
				}
	
	
			}else if(_grouping_mode == 2){
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_max,i,1);
				}
				for(j=0;j<_num_smpl;j++){
					for(i=0;i<_num_clst;i++){
						if(_clst_status_list[i].num_smpl_node <= 0){
							arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i];
						}else{
							arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i]*pow(_clst_status_list[i].num_smpl_node,_grouping_member_index);
						}
					}
				}
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_max,i,2);
				}
	
	
			}else if(_grouping_mode == 3){
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_max,i,1);
				}
				for(j=0;j<_num_smpl;j++){
					for(i=0;i<_num_clst;i++){
						if(_clst_status_list[i].num_smpl_node <= 0){
							arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i];
						}else{
							arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i]*pow(_clst_status_list[i].ave_dist,_grouping_radius_index)*pow(_clst_status_list[i].num_smpl_node,_grouping_member_index);
						}
					}
				}
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_max,i,2);
				}
	
	
			}else if(_grouping_mode == 4){
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_max,i,1);
				}
				for(j=0;j<_num_smpl;j++){
					for(i=0;i<_num_clst;i++){
						if(_clst_status_list[i].num_smpl_node <= 0){
							arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i];
						}else{
							arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i]*pow(((float)_clst_status_list[i].num_smpl_node / _clst_status_list[i].ave_dist),grouping_density_index);
						}
					}
				}
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_max,i,2);
				}
	
	
			}else if(_grouping_mode == 5){
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_max,i,1);
				}
				for(j=0;j<_num_smpl;j++){
					for(i=0;i<_num_clst;i++){
						if(_clst_status_list[i].num_smpl_node <= 0){
							arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i];
						}else{
							arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i]*pow(_clst_status_list[i].ave_dist,_grouping_radius_index)*pow(((float)_clst_status_list[i].num_smpl_node / _clst_status_list[i].ave_dist),grouping_density_index);
						}
					}
				}
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_max,i,2);
				}
	
	
			}else if(_grouping_mode == 6){
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_max,i,1);
				}
				for(j=0;j<_num_smpl;j++){
					for(i=0;i<_num_clst;i++){
						if(_clst_status_list[i].num_smpl_node <= 0){
							arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i];
						}else{
							arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i]*pow(_clst_status_list[i].num_smpl_node,_grouping_member_index)*pow(((float)_clst_status_list[i].num_smpl_node / _clst_status_list[i].ave_dist),grouping_density_index);
						}
					}
				}
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_max,i,2);
				}
	
	
			}else if(_grouping_mode == 7){
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_max,i,1);
				}
				for(j=0;j<_num_smpl;j++){
					for(i=0;i<_num_clst;i++){
						if(_clst_status_list[i].num_smpl_node <= 0){
							arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i];
						}else{
							arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i]*pow(_clst_status_list[i].ave_dist,_grouping_radius_index)*pow(_clst_status_list[i].num_smpl_node,_grouping_member_index)*pow(((float)_clst_status_list[i].num_smpl_node / _clst_status_list[i].ave_dist),grouping_density_index);
						}
					}
				}
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_max,i,2);
				}

	
			}else if(_grouping_mode == 8){
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_max,i,1);
				}
				for(j=0;j<_num_smpl;j++){
					for(i=0;i<_num_clst;i++){
						arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i] - (_clst_status_list[i].ave_dist * _grouping_radius_index);
					}
				}
				for(i=0;i<_num_smpl;i++){
					f_max_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_max,i,2);
				}
			}
		}
		/* *) */



		/* (* [4]. find nearest clst from each sample (_attrb_list_min) */
		if(_grouping_mode == 0){
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_min,i,3);
			}


		}else if(_grouping_mode == 1){
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_min,i,1);
			}
			for(j=0;j<_num_smpl;j++){
				for(i=0;i<_num_clst;i++){
					if(_clst_status_list[i].ave_dist <= 0){
						arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i];
					}else{
						arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i]*pow(_clst_status_list[i].ave_dist,_grouping_radius_index);
					}
				}
			}
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_min,i,2);
			}


		}else if(_grouping_mode == 2){
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_min,i,1);
			}
			for(j=0;j<_num_smpl;j++){
				for(i=0;i<_num_clst;i++){
					if(_clst_status_list[i].num_smpl_node <= 0){
						arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i];
					}else{
						arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i]*pow(_clst_status_list[i].num_smpl_node,_grouping_member_index);
					}
				}
			}
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_min,i,2);
			}


		}else if(_grouping_mode == 3){
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_min,i,1);
			}
			for(j=0;j<_num_smpl;j++){
				for(i=0;i<_num_clst;i++){
					if(_clst_status_list[i].num_smpl_node <= 0){
						arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i];
					}else{
						arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i]*pow(_clst_status_list[i].ave_dist,_grouping_radius_index)*pow(_clst_status_list[i].num_smpl_node,_grouping_member_index);
					}
				}
			}
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_min,i,2);
			}


		}else if(_grouping_mode == 4){
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_min,i,1);
			}
			for(j=0;j<_num_smpl;j++){
				for(i=0;i<_num_clst;i++){
					if(_clst_status_list[i].num_smpl_node <= 0){
						arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i];
					}else{
						arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i]*pow(((float)_clst_status_list[i].num_smpl_node / _clst_status_list[i].ave_dist),grouping_density_index);
					}
				}
			}
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_min,i,2);
			}


		}else if(_grouping_mode == 5){
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_min,i,1);
			}
			for(j=0;j<_num_smpl;j++){
				for(i=0;i<_num_clst;i++){
					if(_clst_status_list[i].num_smpl_node <= 0){
						arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i];
					}else{
						arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i]*pow(_clst_status_list[i].ave_dist,_grouping_radius_index)*pow(((float)_clst_status_list[i].num_smpl_node / _clst_status_list[i].ave_dist),grouping_density_index);
					}
				}
			}
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_min,i,2);
			}


		}else if(_grouping_mode == 6){
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_min,i,1);
			}
			for(j=0;j<_num_smpl;j++){
				for(i=0;i<_num_clst;i++){
					if(_clst_status_list[i].num_smpl_node <= 0){
						arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i];
					}else{
						arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i]*pow(_clst_status_list[i].num_smpl_node,_grouping_member_index)*pow(((float)_clst_status_list[i].num_smpl_node / _clst_status_list[i].ave_dist),grouping_density_index);
					}
				}
			}
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_min,i,2);
			}


		}else if(_grouping_mode == 7){
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_min,i,1);
			}
			for(j=0;j<_num_smpl;j++){
				for(i=0;i<_num_clst;i++){
					if(_clst_status_list[i].num_smpl_node <= 0){
						arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i];
					}else{
						arranged_clst_smpl_dist_table[j][i] = _clst_smpl_dist_table[j][i]*pow(_clst_status_list[i].ave_dist,_grouping_radius_index)*pow(_clst_status_list[i].num_smpl_node,_grouping_member_index)*pow(((float)_clst_status_list[i].num_smpl_node / _clst_status_list[i].ave_dist),grouping_density_index);
					}
				}
			}
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_min,i,2);
			}


		}else if(_grouping_mode == 8){
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,_clst_smpl_dist_table[i],_attrb_list_min,i,1);
			}
			for(j=0;j<_num_smpl;j++){
				for(i=0;i<_num_clst;i++){
					arranged_clst_smpl_dist_table[j][i] = (_clst_smpl_dist_table[j][i] - (_clst_status_list[i].ave_dist * _grouping_radius_ratio));
				}
			}
			for(i=0;i<_num_smpl;i++){
				f_min_status_list(_num_clst,arranged_clst_smpl_dist_table[i],_attrb_list_min,i,2);
			}


		}
		/* *) */



		/* (* [5]. allocate samples to each cluster, set cluster radius and num of members, including cutoff-generation and delete-preparation */
			for(i=0;i<_num_clst;i++){
				_clst_status_list[i].num_smpl_node = 0;
				_clst_status_list[i].sum_dist = 0;
				_clst_status_list[i].ave_dist = 0;
			}
			for(j=0;j<_num_smpl;j++){
				for(i=0;i<_num_clst;i++){
					if(_attrb_list_min[j].pos == i){
						_clst_status_list[i].id_smpl_node[_clst_status_list[i].num_smpl_node] = j;
						_clst_status_list[i].num_smpl_node++;
						_clst_status_list[i].sum_dist += _attrb_list_min[j].value;
					}
				}
			}
			for(i=0;i<_num_clst;i++){
				if(_clst_status_list[i].num_smpl_node >= 1){
					_clst_status_list[i].ave_dist = (_clst_status_list[i].sum_dist) / (_clst_status_list[i].num_smpl_node);
				}
			}
		if((_cutoff == 1)&&(a >= _cutoff_iteration)){
			for(i=0;i<_num_clst;i++){
				_clst_status_list[i].num_arranged_smpl_node = 0;
			}
			for(i=0;i<_num_clst;i++){
				for(j=0;j<_clst_status_list[i].num_smpl_node;j++){
					if(_clst_smpl_dist_table[_clst_status_list[i].id_smpl_node[j]][i] <= _clst_status_list[i].ave_dist * _cutoff_value){
						_clst_status_list[i].id_arranged_smpl_node[_clst_status_list[i].num_arranged_smpl_node] = _clst_status_list[i].id_smpl_node[j];
						_clst_status_list[i].num_arranged_smpl_node++;
					}else{
						fprintf(stderr,"found outlier in cl[%d] loop[%d]!!\n",i,a);
					}
				}
			}
		}else if((_cutoff_gen == 1)&&(a%_cutoff_interval == 0)){
			num_outlier = 0;
			for(j=0;j<_num_smpl;j++){
				outlier[j] = -1;
			}
			for(i=0;i<_num_clst;i++){
				_clst_status_list[i].num_arranged_smpl_node = 0;
			}
			for(i=0;i<_num_clst;i++){
				for(j=0;j<_clst_status_list[i].num_smpl_node;j++){
					if(_clst_smpl_dist_table[_clst_status_list[i].id_smpl_node[j]][i] <= _clst_status_list[i].ave_dist * _cutoff_ol_th){
						_clst_status_list[i].id_arranged_smpl_node[_clst_status_list[i].num_arranged_smpl_node] = _clst_status_list[i].id_smpl_node[j];
						_clst_status_list[i].num_arranged_smpl_node++;
					}else{
						outlier[num_outlier] = _clst_status_list[i].id_smpl_node[j];
						num_outlier++;
					}
				}
			}
			for(j=0;j<num_outlier;j++){
				fprintf(stderr,"found outlier %d \n",outlier[j]);
			}

			/*memory allocation*/
			//outlier_dist_tri = f_calloc_triangle(num_outlier);
			outlier_dist_tri = f_calloc_lower_triangle(num_outlier);
			if((outlier_vec = malloc(sizeof(float *) * num_outlier)) == NULL){
				fprintf(stderr,"failed malloc at outlier_vec\n.");
				exit(0);
			}
			outlier_stat = i_calloc_vec(num_outlier);

			/*ave_ave_radius for generate*/
			/*
			ave_ave_radius_th = 0;
			for(i=0;i<_num_clst;i++){
				ave_ave_radius_th = ave_ave_radius_th + _clst_status_list[i].ave_dist;
			}
			ave_ave_radius_th = (ave_ave_radius_th/_num_clst * _cutoff_gen_th);
			*/

			/*calculate outlier-outlier dist*/
			for(j=0;j<num_outlier;j++){
				outlier_vec[j] = _smpl_matrix[outlier[j]];
			}
			euc_dist_triangle(num_outlier, _dim_smpl, outlier_vec, outlier_dist_tri);
			for(i=0;i<num_outlier;i++){
				for(j=0;j<i;j++){
					if(outlier_dist_tri[i][j] < _cutoff_gen_th){
						outlier_stat[i]++;
					}
				}
			}

			/*cluster generate*/
			for(i=0;i<num_outlier;i++){
				if((outlier_stat[i] == 0)&&(_num_clst < _max_num_clst)){
					for(j=0;j<_dim_smpl;j++){
						_clst_matrix[_num_clst][j] = _smpl_matrix[outlier[i]][j];
					}
					_num_clst++;
				}
			}

			/*memory free*/
			free(outlier_stat);
			free(outlier_vec);
			/*
			for(j=0;j<num_outlier;j++){
				free(outlier_dist_tri[j]);
			}
			*/
			free(outlier_dist_tri[0]);
			free(outlier_dist_tri);
		}else if((_cutoff_gen == 2)&&(a%_cutoff_interval == 0)){
			num_outlier = 0;
			for(j=0;j<_num_smpl;j++){
				outlier[j] = -1;
			}
			for(i=0;i<_num_clst;i++){
				_clst_status_list[i].num_arranged_smpl_node = 0;
			}
			for(i=0;i<_num_clst;i++){
				for(j=0;j<_clst_status_list[i].num_smpl_node;j++){
					if(_clst_smpl_dist_table[_clst_status_list[i].id_smpl_node[j]][i] <= _clst_status_list[i].ave_dist * _cutoff_ol_th){
						_clst_status_list[i].id_arranged_smpl_node[_clst_status_list[i].num_arranged_smpl_node] = _clst_status_list[i].id_smpl_node[j];
						_clst_status_list[i].num_arranged_smpl_node++;
					}else{
						outlier[num_outlier] = _clst_status_list[i].id_smpl_node[j];
						num_outlier++;
					}
				}
			}
			for(j=0;j<num_outlier;j++){
				fprintf(stderr,"found outlier %d \n",outlier[j]);
			}

			/*memory allocation*/
			//outlier_dist_tri = f_calloc_triangle(num_outlier);
			outlier_dist_tri = f_calloc_lower_triangle(num_outlier);
			if((outlier_vec = malloc(sizeof(float *) * num_outlier)) == NULL){
				fprintf(stderr,"failed malloc at outlier_vec\n.");
				exit(0);
			}
			outlier_stat = i_calloc_vec(num_outlier);

			/*ave_ave_radius for generate*/
			ave_ave_radius_th = 0;
			for(i=0;i<_num_clst;i++){
				ave_ave_radius_th = ave_ave_radius_th + _clst_status_list[i].ave_dist;
			}
			ave_ave_radius_th = (ave_ave_radius_th/_num_clst * _cutoff_ol_th * 1);
			/*calculate outlier-outlier dist*/
			for(j=0;j<num_outlier;j++){
				outlier_vec[j] = _smpl_matrix[outlier[j]];
			}
			euc_dist_triangle(num_outlier, _dim_smpl, outlier_vec, outlier_dist_tri);
			for(i=0;i<num_outlier;i++){
				for(j=0;j<i;j++){
					if(outlier_dist_tri[i][j] < ave_ave_radius_th){
						outlier_stat[i]++;
					}
				}
			}

			/*cluster generate*/
			for(i=0;i<num_outlier;i++){
				if((outlier_stat[i] == 0)&&(_num_clst < _max_num_clst)){
					for(j=0;j<_dim_smpl;j++){
						_clst_matrix[_num_clst][j] = _smpl_matrix[outlier[i]][j];
					}
					_num_clst++;
				}
			}

			/*memory free*/
			free(outlier_stat);
			free(outlier_vec);
			/*
			for(j=0;j<num_outlier;j++){
				free(outlier_dist_tri[j]);
			}
			*/
			free(outlier_dist_tri[0]);
			free(outlier_dist_tri);
		}else if(_delete > 0){
			/*ave_ave_radius for delete*/
			ave_ave_radius_th_del = 0;
			for(i=0;i<_num_clst;i++){
				ave_ave_radius_th_del = ave_ave_radius_th_del + _clst_status_list[i].ave_dist;
			}
			ave_ave_radius_th_del = (ave_ave_radius_th_del / _num_clst);
		}
		/* *) */
		/* (* print clst_status_list */
		if((_monitor&17) == 17){
			fprintf(_lfp,DELIM "\n" BRK_S "\"number of referencing data-nodes:\"");
			for(i=0;i<_num_clst;i++){
				fprintf(_lfp,DELIM "\n" BRK_S "\"_clst_status_list[%d].num_smpl_node:\"" DELIM "%d" BRK_E,i,_clst_status_list[i].num_smpl_node);
			}
			fprintf(_lfp,BRK_E DELIM "\n" BRK_S "\"data number of each cluster:\"");
			if(_data_form == 1){
				for(i=0;i<_num_clst;i++){
					fprintf(_lfp,DELIM "\n" BRK_S "\"_clst_status_list[%d].id_smpl_node[]:\"",i);
					for(j=0;j<_clst_status_list[i].num_smpl_node;j++){
						fprintf(_lfp,DELIM "%d",_clst_status_list[i].id_smpl_node[j]);
					}
					fprintf(_lfp,BRK_E);
				}
			}else if(_data_form == 2){
				for(i=0;i<_num_clst;i++){
					fprintf(_lfp,DELIM "\n" BRK_S "\"_clst_status_list[%d].id_smpl_node[]:\"",i);
					for(j=0;j<_clst_status_list[i].num_smpl_node;j++){
						fprintf(_lfp,DELIM "\"%s\"",_sample_id[_clst_status_list[i].id_smpl_node[j]]);
					}
					fprintf(_lfp,BRK_E);
				}
			}
			fprintf(_lfp,BRK_E);
		}
		/* *) */
		/* (* print cluster radius */
		if((_monitor&33) == 33){
			*_total_sum_dist = 0;
			fprintf(_lfp,DELIM "\n" BRK_S "\"dist_to_data_node_from_clst (sum,ave):\"");
			for(i=0;i<_num_clst;i++){
				*_total_sum_dist += _clst_status_list[i].sum_dist;
				fprintf(_lfp,DELIM "\n" BRK_S "%f" DELIM "%f" BRK_E,_clst_status_list[i].sum_dist,_clst_status_list[i].ave_dist);
			}
			fprintf(_lfp,DELIM "\n" BRK_S "\"Total:\"" DELIM "%f" BRK_E BRK_E,*_total_sum_dist);
		}
		/* *) */



		/* (* [7]. reference operation, including centroid detection and cutoff operation */
		if(((_cutoff == 0)&&(_cutoff_gen == 0))||(a < _cutoff_iteration)){
			for(j=0;j<_num_clst;j++){
				for(i=0;i<_dim_clst;i++){
					_ave_ref_smpl_list[j][i] = 0;
				}
			}
			for(j=0;j<_num_clst;j++){
				add_column_of_matrix(_clst_status_list[j].num_smpl_node,_dim_clst,_clst_status_list[j].id_smpl_node,_smpl_matrix,_ave_ref_smpl_list[j]);
			}
			for(j=0;j<_num_clst;j++){
				for(i=0;i<_dim_clst;i++){
					if(_clst_status_list[j].num_smpl_node > 0){
						_ave_ref_smpl_list[j][i] = (_ave_ref_smpl_list[j][i])/(_clst_status_list[j].num_smpl_node);
					}
				}
			}
		}else{
			for(j=0;j<_num_clst;j++){
				for(i=0;i<_dim_clst;i++){
					_ave_ref_smpl_list[j][i] = 0;
				}
			}
			for(j=0;j<_num_clst;j++){
				add_column_of_matrix(_clst_status_list[j].num_arranged_smpl_node,_dim_clst,_clst_status_list[j].id_arranged_smpl_node,_smpl_matrix,_ave_ref_smpl_list[j]);
			}
			for(j=0;j<_num_clst;j++){
				for(i=0;i<_dim_clst;i++){
					if(_clst_status_list[j].num_arranged_smpl_node > 0){
						_ave_ref_smpl_list[j][i] = (_ave_ref_smpl_list[j][i])/(_clst_status_list[j].num_arranged_smpl_node);
					}
				}
			}
		}
		/* *) */



		/* (* [8]. calclate errors between centroid and each clst */
		for(j=0;j<_num_clst;j++){
			for(i=0;i<_dim_clst;i++){
				_dif_table[j][i] = 0;
			}
		}
		for(j=0;j<_num_clst;j++){
			if(_clst_status_list[j].num_smpl_node > 0){
				dif_2lists(_dim_clst,_clst_matrix[j],_ave_ref_smpl_list[j],_dif_table[j]);
			}
		}
		/* *) */
		/* (* print errors */
		if((_monitor&65) == 65){
			fprintf(_lfp,DELIM "\n" BRK_S "\"error:\"");
			for(j=0;j<_num_clst;j++){
				for(i=0;i<_dim_clst;i++){
					fprintf(_lfp,DELIM "%f",_dif_table[j][i]);
				}
			}
			fprintf(_lfp,BRK_E);
		}
		/* *) */



		/* (* [9]. check errors for breaking loop */
		if(_err_ck == 1){
			assigned_num_clst = 0;
			for(i=0;i<_num_clst;i++){
				if(_clst_status_list[i].num_smpl_node != 0){
					assigned_num_clst++;
				}
			}
			total_sq_err = f_calloc_vec(_num_clst);
			for(j=0;j<_num_clst;j++){
				for(i=0;i<_dim_clst;i++){
					total_sq_err[j] += pow(_dif_table[j][i],2);
				}
			}
			for(j=0;j<_num_clst;j++){
				total_sq_err[j] = sqrt(total_sq_err[j]);
				ave_total_sq_err =+ total_sq_err[j];
			}
			/* ave_total_sq_err = ave_total_sq_err/_num_clst; */
			ave_total_sq_err = ave_total_sq_err/assigned_num_clst;
			if(ave_total_sq_err < _err_thrld){
				break;
			}
			free(total_sq_err);
		}
		/* *) */


		/* (* [10]. check cluster member */
		if((_check_cl == 1)&&(a%_check_cl_interval == 0)&&(a>_check_cl_interval)){
			check_cl_count = 0;
			for(j=0;j<_num_clst;j++){
				if(_clst_status_list[j].num_smpl_node != _clst_status_list_prev[j].num_smpl_node){
					check_cl_count++;
				}
			}
			if(check_cl_count == 0){
				for(j=0;j<_num_clst;j++){
					for(i=0;i<_clst_status_list[j].num_smpl_node;i++){
						if(_clst_status_list[j].id_smpl_node[i] != _clst_status_list_prev[j].id_smpl_node[i]){
							check_cl_count++;
						}
					}
				}
			}
			if(check_cl_count > 0){
				fprintf(stderr,"break at %d\n",a);
				break;
			}
			for(j=0;j<_num_clst;j++){
				_clst_status_list_prev[j].num_smpl_node = _clst_status_list[j].num_smpl_node;
				for(i=0;i<_clst_status_list[j].num_smpl_node;i++){
					 _clst_status_list_prev[j].id_smpl_node[i] = _clst_status_list[j].id_smpl_node[i];
				}
			}
		}
		/* *) */


		/* (* [11]. correct and write clst matrix (cluster move) */
		move_write_matrix(_num_clst,_dim_clst,_clst_matrix,_dif_table,_err_correct_rate);
		/* *) */



		/* (* [12]. caluculate _clst_clst_dist_table */
		if(((_monitor&5) == 5)||(_unified > 0)||(_delete > 0)){
			dist_triangle_func(_num_clst,_dim_clst,_clst_matrix,_clst_clst_dist_table);
		}
		/* *) */
		/* (* print clst-clst distance */
		if((_monitor&5) == 5){
			fprintf(_lfp,DELIM "\n" BRK_S "\"clst-clst distance:\"");
			for(i=1;i<_num_clst;i++){
				fprintf(_lfp,DELIM "\n" BRK_S "%f",_clst_clst_dist_table[i][0]);
				for(j=1;j<i;j++){
					fprintf(_lfp,DELIM "%f",_clst_clst_dist_table[i][j]);
				}
				fprintf(_lfp,BRK_E);
			}
			fprintf(_lfp,BRK_E);
		}
		/* *) */



		/* (* [13]. cluster delete */
		if((_delete == 1)&&(a%_delete_interval == 0)){
			th_del = _delete_threshold;
			num_del_clst = 0;
			for(i=0;i<_num_clst;i++){
				del_clst_count_list[i] = 0;
			}
			fprintf(stderr,"\n--- del th:%f: ---\n",th_del);
			for(i=1;i<_num_clst;i++){
				for(j=0;j<i;j++){
					if(_clst_clst_dist_table[i][j] < th_del){
						del_clst_count_list[i]++;
					}
				}
				if(del_clst_count_list[i] > 0){
					del_clst_list[num_del_clst] = i;
					num_del_clst++;
				}
			}
			/* print clst matrix 
			fprintf(stderr,"-----clst matrix-----\n");
			fprintf(stderr,"_num_clst :%d:\n",_num_clst);
			for(i=0;i<_num_clst;i++){
				for(j=0;j<_dim_clst;j++){
					fprintf(stderr,"%f ",_clst_matrix[i][j]);
				}
				fprintf(stderr,"\n");
			}
			fprintf(stderr,"----- -----\n");
			*/
			for(k=0;k<num_del_clst;k++){
				fprintf(stderr,"delete cluster %d\n",del_clst_list[k]);
				for(j=0;j<_dim_clst;j++){
					_clst_matrix[del_clst_list[k]][j] = _clst_matrix[_num_clst - 1][j];
				}
				_num_clst--;
			}
			fprintf(stderr,"--- --- --- ---\n");
		}else if(_delete == 2){
			ave_ave_radius_th_del = ave_ave_radius_th_del * _delete_threshold;
			num_del_clst = 0;
			for(i=0;i<_num_clst;i++){
				del_clst_count_list[i] = 0;
			}
			fprintf(stderr,"\n--- del th:%f: ---\n",ave_ave_radius_th_del);
			for(i=1;i<_num_clst;i++){
				for(j=0;j<i;j++){
					if(_clst_clst_dist_table[i][j] < ave_ave_radius_th_del){
						del_clst_count_list[i]++;
					}
				}
				if(del_clst_count_list[i] > 0){
					del_clst_list[num_del_clst] = i;
					num_del_clst++;
				}
			}
			/* print clst matrix 
			fprintf(stderr,"-----clst matrix-----\n");
			fprintf(stderr,"_num_clst :%d:\n",_num_clst);
			for(i=0;i<_num_clst;i++){
				for(j=0;j<_dim_clst;j++){
					fprintf(stderr,"%f ",_clst_matrix[i][j]);
				}
				fprintf(stderr,"\n");
			}
			fprintf(stderr,"----- -----\n");
			*/
			for(k=0;k<num_del_clst;k++){
				fprintf(stderr,"delete cluster %d\n",del_clst_list[k]);
				for(j=0;j<_dim_clst;j++){
					_clst_matrix[del_clst_list[k]][j] = _clst_matrix[_num_clst - 1][j];
				}
				_num_clst--;
			}
			fprintf(stderr,"--- --- --- ---\n");

		}
		/* *) */



		/* (* [14]. cluster unifying */
		if((_unified == 1)&&(a > 0)&&(_num_clst > _u_suppress_clst_num)){	/* _r_unified : distance */
			min_dist_table_position(_num_clst,_clst_clst_dist_table,&clst_clst_min_dist,clst_clst_min_position);
			if((_monitor&129) == 129){
				fprintf(_lfp,DELIM "\n" BRK_S "\"unified:on\"" DELIM "\n");
				fprintf(_lfp,"\"clst-clst_min_pos:\"" DELIM "%d" DELIM "%d" DELIM "\n",clst_clst_min_position[0],clst_clst_min_position[1]);
				fprintf(_lfp,"\"clst-clst_min_dist:\"" DELIM "%f",clst_clst_min_dist);
			}
			if(a%_u_interval == 0){
				if(_r_unified > clst_clst_min_dist){
					merge_list(_dim_clst,_clst_matrix[clst_clst_min_position[0]],_clst_matrix[clst_clst_min_position[1]],merged_list);
					if((_monitor&129) == 129){
						fprintf(_lfp,DELIM "\n\"merged coordinate:\"");
						for(i=0;i<_dim_clst;i++){
							fprintf(_lfp,DELIM "%f",merged_list[i]);
						}
					}
					j = 0;
					for(i=0;i<_num_clst;i++){
						if((i != clst_clst_min_position[0])&&(i != clst_clst_min_position[1])){
							_clst_matrix[j] = _clst_matrix[i];
							j++;
						}
					}
					_clst_matrix[j] = merged_list;
					for(i=j;i<_num_clst;i++){
						_clst_status_list[i].num_smpl_node = 0;
					}
					_num_clst--;
				}
				if((_monitor&129) == 129){
					fprintf(_lfp,BRK_E);
				}
			}
		}else if((_unified == 2)&&(a > 0)&&(_num_clst > _u_suppress_clst_num)){	/* _r_unified : ratio */
			min_dist_table_position(_num_clst,_clst_clst_dist_table,&clst_clst_min_dist,clst_clst_min_position);
			if((_monitor&129) == 129){
				fprintf(_lfp,DELIM "\n" BRK_S "\"unified:on\"" DELIM "\n");
				fprintf(_lfp,"\"clst-clst_min_pos:\"" DELIM "%d" DELIM "%d" DELIM "\n",clst_clst_min_position[0],clst_clst_min_position[1]);
				fprintf(_lfp,"\"clst-clst_min_dist:\"" DELIM "%f",clst_clst_min_dist);
			}
			if(a%_u_interval == 0){
				unifying_dist = _r_unified * (_clst_status_list[clst_clst_min_position[0]].ave_dist + _clst_status_list[clst_clst_min_position[1]].ave_dist);
				if(unifying_dist > clst_clst_min_dist){
					merge_list(_dim_clst,_clst_matrix[clst_clst_min_position[0]],_clst_matrix[clst_clst_min_position[1]],merged_list);
					if((_monitor&129) == 129){
						fprintf(_lfp,DELIM "\n\"merged coordinate:\"");
						for(i=0;i<_dim_clst;i++){
							fprintf(_lfp,DELIM "%f",merged_list[i]);
						}
					}
					j = 0;
					for(i=0;i<_num_clst;i++){
						if((i != clst_clst_min_position[0])&&(i != clst_clst_min_position[1])){
							_clst_matrix[j] = _clst_matrix[i];
							j++;
						}
					}
					_clst_matrix[j] = merged_list;
					for(i=j;i<_num_clst;i++){
						_clst_status_list[i].num_smpl_node = 0;
					}
					_num_clst--;
				}
				if((_monitor&129) == 129){
					fprintf(_lfp,BRK_E);
				}
			}
		}else if((_unified == 3)&&(a > 0)&&(_num_clst > _u_suppress_clst_num)){	/* _r_unified : ratio */
			min_dist_table_position(_num_clst,_clst_clst_dist_table,&clst_clst_min_dist,clst_clst_min_position);
			if((_monitor&129) == 129){
				fprintf(_lfp,DELIM "\n" BRK_S "\"unified:on\"" DELIM "\n");
				fprintf(_lfp,"\"clst-clst_min_pos:\"" DELIM "%d" DELIM "%d" DELIM "\n",clst_clst_min_position[0],clst_clst_min_position[1]);
				fprintf(_lfp,"\"clst-clst_min_dist:\"" DELIM "%f",clst_clst_min_dist);
			}
			if(a%_u_interval == 0){
				unifying_dist = (_r_unified + _clst_status_list[clst_clst_min_position[0]].ave_dist + _clst_status_list[clst_clst_min_position[1]].ave_dist);
				if(unifying_dist > clst_clst_min_dist){
					merge_list(_dim_clst,_clst_matrix[clst_clst_min_position[0]],_clst_matrix[clst_clst_min_position[1]],merged_list);
					if((_monitor&129) == 129){
						fprintf(_lfp,DELIM "\n\"merged coordinate:\"");
						for(i=0;i<_dim_clst;i++){
							fprintf(_lfp,DELIM "%f",merged_list[i]);
						}
					}
					j = 0;
					for(i=0;i<_num_clst;i++){
						if((i != clst_clst_min_position[0])&&(i != clst_clst_min_position[1])){
							_clst_matrix[j] = _clst_matrix[i];
							j++;
						}
					}
					_clst_matrix[j] = merged_list;
					for(i=j;i<_num_clst;i++){
						_clst_status_list[i].num_smpl_node = 0;
					}
					_num_clst--;
				}
				if((_monitor&129) == 129){
					fprintf(_lfp,BRK_E);
				}
			}

		}
		/* *) */



		/* (* [15]. cluster generating */
		/* TODO: if monitor == 129, print generting informations. */
		if((_generated == 1)&&(a > 0)&&(_num_clst < _max_num_clst)&&(a%_generated_interval == 0)){	/* using absolute distance */
			generated_node.pos = 0;
			generated_node.value = _attrb_list_min[0].value;
			for(i=0;i<_num_smpl;i++){
				if(generated_node.value < _attrb_list_min[i].value){
					generated_node.pos = i;
					generated_node.value = _attrb_list_min[i].value;
				}
			}
			if((_num_clst < _generated_suppress_clst_num)&&(generated_node.value > _r_generated)){
				for(i=0;i<_dim_smpl;i++){
					_clst_matrix[_num_clst][i] = _smpl_matrix[generated_node.pos][i];
				}
				_num_clst++;
			}

		}else if((_generated == 2)&&(a > 0)&&(_num_clst < _max_num_clst)&&(a%_generated_interval == 0)){	/* using radius ratio */
			for(i=0;i<_num_clst;i++){
				if(_clst_status_list[i].num_smpl_node > 0){
					ave_ave_clst_num++;
					ave_ave_radius_clst += _clst_status_list[i].ave_dist;
				}
			}
			ave_ave_radius_clst = ave_ave_radius_clst / ave_ave_clst_num;
			/* you can choose single radius or double radius */
			/* _r_generated = ave_ave_radius_clst * _r_generated; */
			_r_generated = ave_ave_radius_clst * _r_generated * 2;
			generated_node.pos = 0;
			generated_node.value = _attrb_list_min[0].value;
			for(i=0;i<_num_smpl;i++){
				if(generated_node.value < _attrb_list_min[i].value){
					generated_node.pos = i;
					generated_node.value = _attrb_list_min[i].value;
				}
			}
			if((_num_clst < _generated_suppress_clst_num)&&(generated_node.value > _r_generated)){
				for(i=0;i<_dim_smpl;i++){
					_clst_matrix[_num_clst][i] = _smpl_matrix[generated_node.pos][i];
				}
				_num_clst++;
			}

		}else if((_generated == 4)&&(a > 0)&&(_num_clst < _max_num_clst)&&(a%_generated_interval == 0)){        /* using absolute distance */
			generated_node.pos = 0;
			generated_node.value = _clst_status_list[0].num_smpl_node;
			for(i=0;i<_num_clst;i++){
				if(generated_node.value < (float)_clst_status_list[i].num_smpl_node){
					generated_node.pos = i;
					generated_node.value = _clst_status_list[i].num_smpl_node;
				}
			}
			if((_num_clst < _generated_suppress_clst_num)&&(generated_node.value > _r_generated)){
				for(i=0;i<_dim_smpl;i++){
					_clst_matrix[_num_clst][i] = _clst_matrix[generated_node.pos][i]; /* simple copy */
					/*_clst_matrix[_num_clst][i] = (_clst_matrix[generated_node.pos][i] + _smpl_matrix[_clst_status_list[generated_node.pos].id_smpl_node[0]][i])/2;*/
				}
				_num_clst++;
			}

		}else if((_generated == 8)&&(a > 0)&&(_num_clst < _max_num_clst)&&(a%_generated_interval == 0)){	/* using absolute distance */
			generated_node.pos = 0;
			generated_node.value = _attrb_list_max[0].value;
			for(i=0;i<_num_smpl;i++){
				if(generated_node.value < _attrb_list_max[i].value){
					generated_node.pos = i;
					generated_node.value = _attrb_list_max[i].value;
				}
			}
			if((_num_clst < _generated_suppress_clst_num)&&(generated_node.value > _r_generated)){
				for(i=0;i<_dim_smpl;i++){
					_clst_matrix[_num_clst][i] = _smpl_matrix[generated_node.pos][i];
				}
				_num_clst++;
			}
		}
		/* *) */



		/* (* [16]. force clster moving */
			if((_moved&1) == 1){
				if((_monitor&257) == 257){
					fprintf(_lfp,DELIM "\n" BRK_S "\"moved:on");
				}
				if((a%_moved_interval) == 0){
					for(i=0;i<_num_clst;i++){
						dist_clst_to_ave_smpl = distance_func(_dim_clst,_clst_matrix[i],_ave_coordinate_smpl);
						if((_clst_status_list[i].num_smpl_node == 0)&&((dist_clst_to_ave_smpl > _r_moved_suppress))){
							for(j=0;j<_dim_clst;j++){
								_clst_matrix[i][j] = _clst_matrix[i][j] + (_ave_coordinate_smpl[j] - _clst_matrix[i][j])*_moved_rate;
							}
							if((_monitor&257) == 257){
								fprintf(_lfp,DELIM "\nmoved node[%d] : ",i);
								fprintf(_lfp,BRK_S "%f",_clst_matrix[i][0]);
								for(j=1;j<_dim_clst;j++){
									fprintf(_lfp,DELIM "%f",_clst_matrix[i][j]);
								}
								fprintf(_lfp,BRK_E " %f",dist_clst_to_ave_smpl);
							}
						}
					}
				}
				if((_monitor&257) == 257){
					fprintf(_lfp,"\""BRK_E);
				}
			}else if((_moved&2) == 2){
				min_pos_to_data_list = i_alloc_vec(_num_clst);
				min_dist_to_data_list = f_alloc_vec(_num_clst);
				min_pos_matrix(_num_clst,_num_smpl,_clst_smpl_dist_table,min_pos_to_data_list,min_dist_to_data_list,0);
				if((_monitor&257) == 257){
					fprintf(_lfp,DELIM "\n" BRK_S "\"moved:on");
				}
				if(a%_moved_interval == 0){
					for(i=0;i<_num_clst;i++){
						if((_clst_status_list[i].num_smpl_node == 0)&&((min_dist_to_data_list[i] > _r_moved_suppress))){
							for(j=0;j<_dim_clst;j++){
								_clst_matrix[i][j] = _clst_matrix[i][j] + (_smpl_matrix[min_pos_to_data_list[i]][j] - _clst_matrix[i][j])*_moved_rate;
							}
							if((_monitor&257) == 257){
								fprintf(_lfp,DELIM "\nmoved node[%d] to " BRK_S,i);
								fprintf(_lfp,"%f",_clst_matrix[i][0]);
								for(j=1;j<_dim_clst;j++){
									fprintf(_lfp,DELIM "%f",_clst_matrix[i][j]);
								}
								fprintf(_lfp,BRK_E " -> data[%d]",min_pos_to_data_list[i]);
								fprintf(_lfp," distance : %f",min_dist_to_data_list[i]);
							}
						}
					}
				}
				if((_monitor&257) == 257){
					fprintf(_lfp,"\""BRK_E);
				}
				free(min_pos_to_data_list);
				free(min_dist_to_data_list);
			}else if((_moved&4) == 4){
				max_pos_to_data_list = i_alloc_vec(_num_clst);
				max_dist_to_data_list = f_alloc_vec(_num_clst);
				max_pos_matrix(_num_clst,_num_smpl,_clst_smpl_dist_table,max_pos_to_data_list,max_dist_to_data_list,0);
				if((_monitor&257) == 257){
					fprintf(_lfp,"\n" DELIM BRK_S "\"moved:on");
				}
				if(a%_moved_interval == 0){
					for(i=0;i<_num_clst;i++){
						if((_clst_status_list[i].num_smpl_node == 0)&&((max_dist_to_data_list[i] > _r_moved_suppress))){
							for(j=0;j<_dim_clst;j++){
								_clst_matrix[i][j] = _clst_matrix[i][j] + (_smpl_matrix[max_pos_to_data_list[i]][j] - _clst_matrix[i][j])*_moved_rate;
							}
							if((_monitor&257) == 257){
								fprintf(_lfp,DELIM "\nmoved node[%d] to " BRK_S,i);
								fprintf(_lfp,"%f",_clst_matrix[i][0]);
								for(j=1;j<_dim_clst;j++){
									fprintf(_lfp,DELIM "%f",_clst_matrix[i][j]);
								}
								fprintf(_lfp,BRK_E " -> data[%d]",max_pos_to_data_list[i]);
								fprintf(_lfp," distance : %f",max_dist_to_data_list[i]);
							}
						}
					}
				}
				if((_monitor&257) == 257){
					fprintf(_lfp,"\""BRK_E);
				}
				free(max_pos_to_data_list);
				free(max_dist_to_data_list);
			}else if(_moved > 4){
			}
		/* *) */






		if((_monitor >= 2)&&((_monitor&1) == 1)){
			fprintf(_lfp,BRK_E"\n");
		}
	}
	/*end for-loop*/




	if((_monitor&1) == 1){
		fprintf(_lfp,BRK_E);
	}
	*_alloc_num_clst = _num_clst;
	*_assigned_num_clst = assigned_num_clst;
}/*end function learning-loop*/

