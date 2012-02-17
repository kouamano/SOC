int print_bin(int bin){
	char	buff[32+1];
	int	i;

	buff[32] = '\0';
	for(i = 0; i < 32; i++){
		if(i != 0 && bin == 0)
			break;
		if(bin % 2 == 0)
			buff[(32-1)-i] = '0';        
		else
			buff[(32-1)-i] = '1';
		bin = bin / 2;
	}
	printf(">>>%s\n", buff+(32-1)-i+1);
}
