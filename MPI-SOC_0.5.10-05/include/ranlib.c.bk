/* ranlib.c */

/*************************************************/
/* Copyright (c) 2003-2005 AMANO Kou             */
/* University of Tsukuba                         */
/* University of Liblary And Information Science */
/* National Institute of Agrobilogical Sciences  */
/* kamano@affrc.go.jp                            */
/*************************************************/

float mk_rand(float minimum, float maximum, int seed){
	float out;
	float rate;
	rate = (maximum-minimum)/RAND_MAX;
	srand(seed);
	out = rand()*rate+minimum;
	return(out);
}

void mk_rand_matrix(int num, int dim, float **outmatrix, float div, int seed){
	int l;
	int m;
	for(l=0;l<num;l++){
		srand(seed+pow(2,l));
		for(m=0;m<dim;m++){
			outmatrix[l][m] = rand()/div;
		}
	}
}

void mk_int_rand_order_list(int minimum, int maximum, int *list, long seed){
	int i;
	int j;
	int loop = maximum-minimum;
	srand(seed);
	list[0] = rand()%(maximum-minimum+1)+minimum;
	for(i=1;i<=loop;i++){
		list[i] = rand()%(maximum-minimum+1)+minimum;
		for(j=0;j<i;j++){
			if(list[j] == list[i]){
				i = i-1;
				break;
			}
		}
	}
}

void mk_rand_coordinate(int frac, int dim, float **min_max_table, float **out_matrix, long seed){
	int i;
	int j;
	time_t t;
	/*
	for(j=0;j<2;j++){
		for(i=0;i<dim;i++){
			printf("%f \n",min_max_table[j][i]);
		}
		printf("\n");
	}
	*/
	t = time(NULL);
	srand(seed+t);
	seed = rand();
	for(i=0;i<frac;i++){
		srand(seed+pow(i,2)+t);
		seed = rand()*i;
		for(j=0;j<dim;j++){
			srand(seed+pow(j,2));
			out_matrix[i][j] = mk_rand(min_max_table[0][j],min_max_table[1][j],t+i+seed);
			seed = seed*i+pow(j,2);
		}
	}
	/*
	for(i=0;i<frac;i++){
		for(j=0;j<dim;j++){
			printf("%f ",out_matrix[i][j]);
		}
		printf("\n");
	}
	*/
}
