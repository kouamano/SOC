struct option {
        int status;
        char import_file[FILE_NAME_LEN];
        char export_file_name[FILE_NAME_LEN];
        char export_file_subname[FILE_NAME_LEN];
        int part_print;
        char cluster_ID_string[STRING_LEN];
};

void print_options(struct option);
void init_options(struct option *);
int set_default_options(struct option *);
void print_help();
int get_options(int optc, char **, struct option *);
int get_number_cluster_IDs(char *);
int cluster_string_to_cluster_IDs(char *, int *);
int cmp_int(int*, int*);
