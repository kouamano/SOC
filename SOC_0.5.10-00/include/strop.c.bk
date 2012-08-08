/*
char *strrev(char *str){
	int i = 0;
	int len = 0;
	char *p;
	len = strlen(str);
	p = malloc(sizeof(char) * len);
	for(i=0;i<len;i++){
		p[len-i-1] = str[i];
	}
	return(p);
}
*/
char *strrev(char *str){
	int i = 0;
	int len = 0;
	int times = 0;
	char tmp;
	len = strlen(str);
	times = (int)len/2;
	for(i=0;i<times;i++){
		tmp = str[i];
		str[i] = str[len-i-1];
		str[len-i-1] = tmp;
	}
	return(str);
}
