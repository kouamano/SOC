/* clients.c */

/*************************************************/
/* Copyright (c) 2003-2005 AMANO Kou             */
/* University of Tsukuba                         */
/* University of Liblary And Information Science */
/* National Institute of Agrobilogical Sciences  */
/* kamano@affrc.go.jp                            */
/*************************************************/

#ifndef BUF_LEN
#define BUF_LEN 1024
#endif
#ifndef COMM_LEN
#define COMM_LEN 4096
#endif
#ifndef OUT_LEN
#define OUT_LEN 8192
#endif

char *maple_client(char *hostname, int port, char *com, int alive, int d_print, int rec){
	int s;
	struct hostent *servhost;
	struct sockaddr_in server;
	int com_len;
	int con_stat;
	char buf[BUF_LEN];
	int read_size;
	char *ret;
	int i = 0;
	ret = malloc((size_t)sizeof(char)*OUT_LEN);
	com_len = strlen(com);
	servhost = gethostbyname(hostname);
	bzero((char *)&server,sizeof(server));
	server.sin_family = AF_INET;
	bcopy(servhost->h_addr,(char *)&server.sin_addr,servhost->h_length);
	server.sin_port = htons(port);
	if((s = socket(AF_INET,SOCK_STREAM,0)) < 0){
		if(alive != 1){
			printf("failed : generating socket.\n");
		}
		return("-1");
	}
	if((con_stat = connect(s,(struct sockaddr *)&server,sizeof(server))) == -1){
		if(alive != 1){
			printf("failed : connect().\n");
		}
		return("-1");
	}
	write(s,com,com_len);
	if(rec >= 1){
		if(d_print == 1){
			while(1){
				read_size = read(s,buf,BUF_LEN);
				if(read_size > 0){
					write(1,buf,read_size);
				}else{
					break;
				}
			}
			close(s);
			ret = "0";
		}else if(d_print == 0){
			while(1){
				read_size = read(s,buf,BUF_LEN);
				if(read_size > 0){
					strncat(ret+i,buf,read_size);
					i = i+read_size;
				}else{
					break;
				}
			}
			close(s);
			if(ret[i-1] == '\n'){
				ret[i-1] = '\0';
			}else{
				ret[i] = '\0';
			}
		}
	}else{
		close(s);
		ret = "0";
	}

	return(ret);
}
