/* list_oprations.c */
#ifndef TMP_STR_LEN
#define TMP_STR_LEN 512
#endif

int *get_tensor_dim_from_str(char *str, int *rank){
	int i;
	int j;
	int k;
	int *dim;
	int len;
	char tmp_c[TMP_STR_LEN];
	int tmp;
	*rank = 0;
	len = strlen(str);
	for(i=0;i<len;i++){
		if(str[i] == ','){
			(*rank)++;
		}
	}
	(*rank)++;
	dim = malloc(sizeof(int)*(*rank));
	if(dim == NULL){
		printf("error\n");
	}
	j = 0;
	k = 0;
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
	return(dim);
}

int create_outer_list(int rank, int *dim, int **outer_list){
	int i = 0;
	int j = 1;
	int tmp = 1;
	float f = 0;
	int elm = 1;
	long long int *prod;
        for(i=0;i<rank;i++){
                elm = dim[i] * elm;
        }
	prod = lli_alloc_vec(rank);
        j = 1;
        tmp = 1;
        prod[0] = 1;
        for(i=1;i<rank;i++){
                tmp = tmp * dim[i-1];
                prod[j] = tmp;
                j++;
        }
        for(i=0;i<elm;i++){
                f = (float)i/prod[0];
                outer_list[i][0] = ((int)f)%dim[0];
                for(j=1;j<rank;j++){
                        f = (float)i/prod[j];
                        outer_list[i][j] = ((int)f)%dim[j];
                }
        }
	return(elm);
}
