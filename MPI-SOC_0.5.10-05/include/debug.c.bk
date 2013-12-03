/* NAME:  */
/* test.c */
/* :END   */

/* COPYRIGHT:                    */
/* AMANO, Kou                    */
/* insertion code for test/debug */
/* :END                          */

/* CODE: */
void f_print(float f, FILE *dev){
	fprintf(dev,"=== start debug procedure ===\n");
	fprintf(dev,"%f ",f);
	fprintf(dev,"\n=== end debug procedure ===\n");

}

void f_print_vec(int size, float *vec, FILE *dev){
	int i;
	fprintf(dev,"=== start debug procedure ===\n");
	for(i=0;i<size;i++){
		fprintf(dev,"%f ",vec[i]);
	}
	fprintf(dev,"\n=== end debug procedure ===\n");
}

void i_print(float i, FILE *dev){
	fprintf(dev,"=== start debug procedure ===\n");
	fprintf(dev,"%d ",i);
	fprintf(dev,"\n=== end debug procedure ===\n");

}

void i_print_vec(int size, int *vec, FILE *dev){
	int i;
	fprintf(dev,"=== start debug procedure ===\n");
	for(i=0;i<size;i++){
		fprintf(dev,"%d ",vec[i]);
	}
	fprintf(dev,"\n=== end debug procedure ===\n");
}

void f_print_mat(int *size, float **vec, FILE *dev){
	int i;
	int j;
	fprintf(dev,"=== start debug procedure ===\n");
	for(i=0;i<size[0];i++){
		for(j=0;j<size[1];j++){
			fprintf(dev,"%f ",vec[i][j]);
		}
		fprintf(dev,"\n");
	}
	fprintf(dev,"\n=== end debug procedure ===\n");
}

void i_print_mat(int *size, int **vec, FILE *dev){
	int i;
	int j;
	fprintf(dev,"=== start debug procedure ===\n");
	for(i=0;i<size[0];i++){
		for(j=0;j<size[1];j++){
			fprintf(dev,"%d ",vec[i][j]);
		}
		fprintf(dev,"\n");
	}
	fprintf(dev,"\n=== end debug procedure ===\n");
}
/* :END  */
