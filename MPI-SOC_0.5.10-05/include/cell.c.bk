struct cell {
	char head[128];
	int n;
	void *body;
	struct cell *next;
};

struct list_cell {
	char head[128];
	int n;
	void *body;
};

struct cons_cell {
	char head[128];
	void *body;
	struct cell *next;
};

struct cell *cell_alloc_list(int n){
	struct cell *l;
	if((l = malloc((size_t)sizeof(struct cell)*n)) == NULL){
		printf("failed : malloc(struct cell) -- exit.\n");
		exit(1);
	}
	return(l);
}

struct list_cell *list_cell_alloc_list(int n){
	struct list_cell *l;
	if((l = malloc((size_t)sizeof(struct list_cell)*n)) == NULL){
		printf("failed : malloc(struct list_cell) -- exit.\n");
		exit(1);
	}
	return(l);
}

struct cons_cell *cons_cell_alloc_list(int n){
	struct cons_cell *l;
	if((l = malloc((size_t)sizeof(struct cons_cell)*n)) == NULL){
		printf("failed : malloc(struct list_cell) -- exit.\n");
		exit(1);
	}
	return(l);
}
