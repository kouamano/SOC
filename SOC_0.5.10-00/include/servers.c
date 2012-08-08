/* servers.c */

/*************************************************/
/* Copyright (c) 2003-2005 AMANO Kou             */
/* University of Tsukuba                         */
/* University of Liblary And Information Science */
/* National Institute of Agrobilogical Sciences  */
/* kamano@affrc.go.jp                            */
/*************************************************/

int start_maple(int port){
	char com[4096];
	char statement0[] = "echo '";
	char statement1[] = "server :=  proc(sid)\nlocal s, res;\nuse Sockets in\ninterface(prettyprint=0);\ns := Read(sid,60);\nres := parse(s,statement);\nWrite( sid, sprintf( \"%q\", res) );\nWrite( sid, sprintf( \"\\n\" ) );\nend use\nend proc:\n\n";
	char statement3[] = "Sockets:-Serve( ";
	char statement4[] = ", server );";
	char statement5[] = "' | maple -t 2>/dev/null &\n";
	sprintf(com,"%s%s%s%d%s%s",statement0,statement1,statement3,port,statement4,statement5);
	//printf("%s",com);
	system(com);
	return(0);
}
