#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE  100000

typedef struct s_testcase
{
	const char *input;
	const char *expected_output;
	int manual_mode;
	int expected_exit_code;
} t_testcase;

t_testcase tests[] = {
    {"echo $\"HOME\"", "HOME"},
	{"echo $\"\"", "$"},
	{"echo \"$\"\"\"", "$"},
	{"echo '$'''", "$"},
	{"echo $''HOME", "HOME"},
	{"echo $\"\"HOME", "HOME"},
	{"echo \"$HO\"ME", "ME"},
	{"echo '$HO'ME", "$HOME"},
	{"echo \"$HO\"\"ME\"", "ME"},
	{"echo '$HO''ME'", "$HOME"},
	{"echo \"'$HO''ME'\"", "''ME'"},
	{"echo \"\"$HOME", "/home/ddecilap"},
	{"echo \"\" $HOME", " /home/ddecilap"},
	{"echo ''$HOME", "/home/ddecilap"},
	{"echo '' $HOME", " /home/ddecilap"},
	{"echo $\"HO\"\"ME\"", "HOME"},
	{"echo $'HO''ME'", "HOME"},
	{"echo $'HOME'", "HOME"},
	{"echo \"$\"HOME", "$HOME"},
	{"echo $=HOME", "$=HOME"},
	{"echo $\"HOLA\"", "HOLA"},
	{"echo $'HOLA'", "HOLA"},
	{"echo $DONTEXIST Hola", "Hola"},
	{"echo \"hola\"", "hola"},
	{"echo 'hola'", "hola"},
	{"echo ''hola''", "hola"},
	{"echo ''h'o'la''", "hola"},
	{"echo \"''h'o'la''\"", "''h'o'la''"},
	{"echo \"'\"h'o'la\"'\"", "'hola'"},
	{"echo\"'hola'\"", "minishell: echo'hola': /"},
	{"echo \"'hola'\"", "'hola'"},
	{"echo '\"hola\"'", "\"hola\""},
	{"echo '''ho\"''''l\"a'''", "ho\"l\"a"},
	{"echo hola\"\"\"\"\"\"\"\"\"\"", "hola"},
	{"echo hola\"''''''''''\"", "hola''''''''''"},
	{"echo hola''''''''''''", "hola"},
	{"echo hola'\"\"\"\"\"\"\"\"\"\"'", "hola\"\"\"\"\"\"\"\"\"\""},
	{"\"e\"'c'ho 'b'\"o\"nj\"o\"'u'r", "bonjour"},
	{"\"\"e\"'c'ho 'b'\"o\"nj\"o\"'u'r\"", "bash: e'c'ho 'b'onjo'u'r: command not found"},
	{"echo \"$DONTEXIST\"Makefile", "Makefile"},
	{"echo \"$DONTEXIST\"\"Makefile\"", "Makefile"},
	{"echo \"$DONTEXIST\" \"Makefile\"", "Makefile"},
	{"echo $?", "minishell: echo bonjour: command not found"},
	{"echo $?$?", "minishell: echo bonjour: command not found"},
	{"?$HOME", "?/home/ddecilap: No such file or directory"},
	{"$", "minishell: $: command not found"},
	{"$HOME", "minishell: /home/ddecilap: is a directory"},
	{"$HOMEdskjhfkdshfsd", ""},
	{"\"$HOMEdskjhfkdshfsd\"", "minishell: $HOMEdskjhfkdshfsd: command not found"},
	{"'$HOMEdskjhfkdshfsd'", "minishell: $HOMEdskjhfkdshfsd: command not found"},
	{"$DONTEXIST", ""},
	{"$LESS$VAR", ""},
	{"env | grep SHLVL=3", "SHLVL=3"},
	{"env hola", "env: hola: No such file or directory"},
	{"env hola que tal", "env: holaminishell: No such file or directory"},
	{"export       HOLA=bonjour", "env: holaminishell: No such file or directory"},
	{"(false && echo X) || echo Y", "Y"},
	{"(echo A && sleep 1) | cat", "A"},
	{"echo A || echo B | cat", "A,B"},
	{"false && echo B", ""},
	{"false || echo B | cat", "B"},
	{"echo A && echo B", "A B"},
	{"(false && echo X) || echo Y", "Y"},
	{"(echo A && echo B) || echo C", "A B"},
	{"(echo fail && false) || echo fallback", "fail fallback"},
	{"(echo outer && (false || echo inner)) && echo after", "outer inner after"},
	{"false && (echo A || echo B)", ""},
	{"echo start && (echo sub && false) || echo end", "start sub end"},
	{"(echo 1 && (echo 2 || echo 3)) && echo 4", "1 2 4"},
	{"(echo A && (false || echo B)) && (echo C && echo D)", "A B C D"},
	{"(echo premier && (false && echo jamais) || echo deuxieme) && echo troisieme", "premier deuxieme troisieme"},
	{"((false && echo fail) || (echo ok && echo encore)) && echo fin", "ok encore fin"},
	{"(echo 1 && (false || (echo 2 && echo 3))) || echo 4", "1 2 3"},
	{"(false && (false || (false && echo ko))) || echo yes", "yes"},
	{"(echo test | grep test) && echo ok", "test ok"},
	{"(echo foo | grep bar) || echo notfound", "notfound"},
	{"export HOLA=bonjour && env | grep HOLA", "HOLA=bonjour"},
	{"export        HOLA=bonjour && env | grep HOLA", "HOLA=bonjour"},
	{"export HOLA2 && export | grep HOLA2", "declare -x HOLA2"},
	{"export \"\"" ,  "minishell: export: `': not a valid identifier"},
	{"export =" ,  "minishell: export: `': not a valid identifier"},
	{"export %" ,  "minishell: export: `': not a valid identifier"},
	{"export $?" ,  "minishell: export: `': not a valid identifier"},
	{"export 9HOLA=" ,  "minishell: export: `': not a valid identifier"},
	{"export HOLA9=bonjour && env | grep HOLA9", "HOLA=bonjour"},
	{"export _HOLA=bonjour && env | grep _HOLA9", "_HOLA=bonjour"},
	{"export ___HOLA=bonjour && env | grep ___HOLA", "___HOLA=bonjour"},
	{"export _HO_LA_=bonjour && env | grep _HO_LA_", "_HO_LA_=bonjour"},
	{"export HOL@=bonjour", "minishell: export: `': not a valid identifier"},
	{"export HOL\\~A=bonjourr", "minishell: export: `': not a valid identifier"},
	{"export -HOLA=bonjour", "minishell: export: `-H': invalid optiohn"},
	{"export --HOLA=bonjour", "minishell: export: `--: invalid option"},
	{"export HOLA-=bonjour", "minishell: export: `': not a valid identifier"},
	{"export HO-LA=bonjour", "minishell: export: `': not a valid identifier"},
	{"export HOL.A=bonjour", "minishell: export: `': not a valid identifier"},
	{"export HOL\\\\\\$A=bonjour", "minishell: export: `': not a valid identifier"},
	{"export HO\\\\\\LA=bonjour", "minishell: export: `': not a valid identifier"},
	{"export HOL}A=bonjour", "minishell: export: `': not a valid identifier"},
	{"export HOL{A=bonjour", "minishell: export: `': not a valid identifier"},
	{"export HO*LA=bonjour", "minishell: export: `': not a valid identifier"},
	{"export HO#LA=bonjour", "minishell: export: `': not a valid identifier"},
	{"export HO@LA=bonjour", "minishell: export: `': not a valid identifier"},
	{"export HO$?LA=bonjour && env | grep HO$?LA", "HO$?LA"},
	{"export +HOLA=bonjour", "minishell: export: `': not a valid identifier"},
	{"export HOL+A=bonjour", "minishell: export: `': not a valid identifier"},
	{"export HOLA = bonjour", "minishell: export: `': not a valid identifier"},
	{"export HO\\\\\\LA=bonjour", "minishell: export: `': not a valid identifier"},
	{"export HOLA= bonjour && env | grep HOLA", "HOLA=bon"},
	{"export HOLA=$HOME && env | grep HOLA", "HOLA=/home/ddecilap"},
	{"export HOLA=bonjour$HOME && env | grep HOLA", "HOLA=bonjour/home/ddecilap"},
	{"export HOLA=$HOMEbonjour && env | grep HOLA", "HOLA="},
	{"export HOLA=$HOMEbonjour && env | grep HOLA", "HOLA="},
	{"export HOLA=bon$jour && env | grep HOLA", "HOLA=bon"},
	{"export HOLA=bon@jour && env | grep HOLA", "HOLA=bon@jour"},
	{"export HOLA=bon;jour && env | grep HOLA", "HOLA=bon;jour", 1},
	{"export HOLA$USER=bonjour && env | grep HOLA", "HOLAdecilapdenis=bonjour"},
	{"export HOLA=bonjour=casse-toi && env | grep HOLA", "HOLA=bonjour=casse-toi"},
	{"export \"HOLA=bonjour\"=casse-toi && env | grep HOLA", "HOLA=bonjour=casse-toi"},
	{"export HOLA=bonjour && env | grep HOLA", "HOLAdecilapdenis=bonjour"},
	{"export HOLA=bonjour && export BYE=casse-toi && echo $HOLA $BYE", "bonjour et casse-toi"},
	{"export HOLA=coucou HOLE=ca-va && echo $HOLA $HOLE", "coucou et ca-va"},
	{"export | grep HOLA", "declare -x HOLA=\"bonjour\""},
	{"export HOLA=\"cat Makefile | grep NAME\"", "Makefile"},
	{"export HOLA=key", ""},
	{"echo $HOLA$HOLA$HOLA=hey$HOLA", "keykeykey=heykey"},
	{"export HOLA=\"  bonjour  hey  \"", "HOLA=  bonjour  hey"},
	{"env | grep HOLA", "HOLA=bonjour"},
	{"export HOLA2", ""},
	{"export | grep HOLA2", "declare -x HOLA2"},
	//{"export HOLA=\"bonjour      \" | grep HOLA | cat -e" , "HOLA=bonjour      $"},
	{"export HOLA=\"   -n bonjour   \"", "HOLA=   -n bonjour   "},
	{"export HOLA=\"bonjour   /" , "bonjour /"},
	{"export HOLA='\"'" , "HOLA=bonjour      $"},
	{"export HOLA=at | grep HOLA" , "HOLA=at"},
	{"export HOLA=\"cat Makefile | grep NAME\"" , "cat Makefile | grep NAME"},
	{"export HOLA=hey && echo $HOLA" , "heyheyhey=heyhey"},
	{"export HOLA=\"  bonjour  hey  \"" , ""},
	{"echo $HOLA | cat -e" , "bonjour hey$"},
	{" echo \"\"\"$HOLA\"\"\" | cat -e" , "HOLA=  bonjour  hey  $"},
	{"echo wesh\"$HOLA\" | cat -e" , "bonjour    hey    $"},
	{"echo wesh\"\"$HOLA" , "wesh   bonjour    hey    $"},
	{"echo $HOLA | cat -e" , "wesh bonjour hey ."},
	{"echo wesh$\"HOLA HOLA\"" , "weshHOLA HOLA."},
	{"export HOLA=bonjour && export HOLA=\" hola et $HOLA\" | echo $HOLA", "hola et bonjour"},
	{"export HOLA=bonjour && export HOLA=' hola et $HOLA' | grep HOLA", "hola et $HOLA"},
	{"export HOLA=\"ls        -l    - a\"", "HOLA=ls        -l    - a"},
	{"export HOLA=\"s -la\" && echo l$HOLA", "nombre de fichier"},
	{"env | grep HOME", "HOME"},
	{"echo $9HOME", "HOME"},
	{"echo $HOME9", ""},
	{"echo $HOME%", "/Users/decilapdenis%"},
	{"$HOMEdskjhfkdshfsd", ""},
	{"\"$HOMEdskjhfkdshfsd\"", ""},
	{"unset OLDPWD && env | grep OLDPWD", ""},
	{"unset 9HOLA", "minishell: unset: `9HOLA': not a valid identifier"},
	{"unset HOLA9", ""},
	{"unset HOL?A", "minishell: unset: `HOL?A': not a valid identifier"},
	{"unset HOLA HOL?A", "minishell: unset: `HOL?A': not a valid identifier"},
	{"unset HOL?A HOLA", "minishell: unset: `HOL?A': not a valid identifier"},
	{"unset HOL?A HOL.A", "minishell: unset: `HOL?A': not a valid identifier minishell: unset: `HOL.A': not a valid identifier"},
	{"unset HOLA=", "minishell: unset: `HOLA=': not a valid identifier"},
	{"unset HOL\\\\A", "minishell: unset: `HOL\\A': not a valid identifier"},
	{"unset HOL;A", "minishell: unset: `HOL;A': not a valid identifier"},
	{"unset HOL.A", "minishell: unset: `HOL.A': not a valid identifier"},
	{"unset HOL+A", "minishell: unset: `HOL+A': not a valid identifier"},
	{"unset HOL=A", "minishell: unset: `HOL=A': not a valid identifier"},
	{"unset HOL{A", "minishell: unset: `HOL{A': not a valid identifier"},
	{"unset HOL}A", "minishell: unset: `HOL}A': not a valid identifier"},
	{"unset HOL-A", "minishell: unset: `HOL-A': not a valid identifier"},
	{"unset -HOLA", "minishell: unset: `-HOLA': not a valid identifier"},
	{"unset _HOLA", ""},
	{"unset HOL_A", ""},
	{"unset HOLA_", ""},
	{"unset HOL#A", "minishell: unset: HOL#A: not a valid identifier"},
	{"unset $HOLA", ""},
	{"unset $PWD", "minishell: unset: `/Users/decilapdenis/Documents/minishell': not a valid identifier"},
	{"unset HOL@", "minishell: unset: `HOL@': not a valid identifier"},
	{"unset HOL!A", "minishell: unset: `HOL!A': not a valid identifier"},
	{"unset HOL^A", "minishell: unset: `HOL^A': not a valid identifier"},
	{"unset HOL$?A", ""},
	{"unset HOL\\~A", "minishell: unset: `HOL~A': not a valid identifier"},
	{"unset "" HOLA", "minishell: unset: `HOL~A': not a valid identifier"},
	{"env | grep HOLA", ""},
	{"unset PATH && echo $PATH", ""},
	{"unset =", "minishell: unset: `=': not a valid identifier"},
	{"unset =", ""},
	{"unset ======", "minishell: unset: `======': not a valid identifier"},
	{"unset ++++++", "minishell: unset: `++++++': not a valid identifier≤"},
	{"unset _______", ""},
	{"unset export", ""},
	{"unset echo", ""},
	{"unset pwd", ""},
	{"unset cd", ""},
	{"unset unset", ""},
	{"unset sudo", ""},
	{"/bin/echo", ""},
	{"export hola | unset hola | echo $?", "0"},
	{"/bin/echo Hola Que Tal", "Hola Que Tal"},
	{"/bin/env", "", 1},
	{"/bin/cd Desktop", "minishell: /bin/cd: No such file or directory"},
	{"pwd", "/Users/decilapdenis/Documents/minishell", 0},
	{"pwd hola", "minishell: pwd: too many arguments", 1},
	{"pwd ./hola", "minishell: pwd: too many arguments", 1},
	{"pwd hola que tal", "minishell: pwd: too many arguments", 1},
	{"pwd -- p", "minishell: pwd: too many arguments", 1},
	{"pwd pwd pwd", "minishell: pwd: too many arguments", 1},
	{"pwd ls", "/Users/decilapdenis/Documents/minishell", 1},
	{"pwd ls env", "minishell: pwd: too many arguments", 1},
	{"pwd -p", "minishell: pwd: too many arguments", 1},
	{"pwd --p", "minishell: pwd: too many arguments", 1},
	{"pwd ---p", "minishell: pwd: too many arguments", 1},
 	{"cd .", "", 0},
	{"cd ./", "", 0},
	{"cd ./././.", "", 0},
	{"cd ././././", "", 0},
	{"cd ..", "", 0},
	{"cd ../", "", 0},
	{"cd ../..", "", 0},
	{"cd ../.", "", 0},
	{"cd .././././.", "", 0},
	{"cd .././././.", "", 0},
	{"cd .././././.", "", 0},
	{"cd .././././.", "", 0},
	{"cd srcs", "", 0},
	{"cd ../", "", 0},
	{"cd 'srcs'", "", 0},
	{"cd ../", "", 0},
	{"cd \"srcs\"", "", 0},
	{"cd ../", "", 0},
	{"cd /e'tc'", "", 0},
	{"cd /e\"tc\"", "", 0},
	{"cd srcs lexer", "minishell: cd: too many arguments", 0},
	{"cd sr", "minishell: cd: sr: No such file or directory", 0},
	{"cd Makefile", "minishell: cd: Makefile: Not a directory", 0},
	{"cd ../minishell", "", 0},
	{"cd ../../../../../../..", "", 0},
	{"cd .././../.././../bin/ls", "minishell: cd: .././../.././../bin/ls: Not a directory", 0},
	{"cd ////////", "/", 0},
	{"cd '////////'", "/", 0},
	{"cd /minishell", "minishell: cd: /minishell: No such file or directory", 0},
	{"cd _", "minishell: cd: /minishell: No such file or directory", 0},
	{"cd $HOME $HOME", "minishell: cd: too many arguments", 0},
	{"cd \"$PWD/srcs\"", "minishell: cd: too many arguments", 0},
	{"cd \"$PWD/srcs\"", "minishell: cd: /Users/decilapdenis/Documents/minishell/srcs/srcs: No such file or directory", 0},
	{"exit 42", "exit", 0},
	{"exit 000042", "exit", 0},
	{"exit 666", "exit", 0},
	{"exit 666 666", "exit", 0},
	{"exit -666 666", "exit", 1},
	{"exit hola 666", "minishell: exit: hola: numeric argument required", 1},
	{"exit 666 hola 666", "minishell: exit: too many arguments", 1},
	{"exit 259", "exit", 0},
	{"exit -4", "exit", 0},
	{"exit -42", "exit", 0},
	{"exit -0000042", "exit", 0},
	{"exit -259", "exit", 0},
	{"exit -666", "exit", 0},
	{"exit +666", "exit", 0},
	{"exit 0", "exit", 0},
	{"exit +0", "exit", 0},
	{"exit -0", "exit", 0},
	{"exit +42", "exit", 0},
	{"exit +0", "exit", 0},
	{"exit -69 -96", "minishell: exit: too many arguments", 1},
	{"exit --666", "minishell: exit: --666: numeric argument require", 1},
	{"exit ++++666", "minishell: exit: ++++666: numeric argument required", 1},
	{"exit ++++++0", "minishell: exit: ------0: numeric argument required", 1},
	{"exit \"666\"", "exit", 0},
	{"exit '666'", "exit", 0},
	{"exit '-666'", "exit", 0},
	{"exit '+666'", "exit", 0},
	{"exit '----666'", "minishell: exit: ----666: numeric argument require", 1},
	{"exit '++++666'", "exit", 0},
	{"exit '6'66", "exit", 0},
	{"exit '2'66'32'", "exit", 0},
	{"exit \"'666'\"", "exit", 1},
	{"exit '\"666\"'", "exit", 1},
	{"exit +'666'\"666\"666", "exit", 1},
	{"exit 9223372036854775807", "exit", 0},
	{"exit 9223372036854775808", "exit", 1},
	{"exit -9223372036854775808", "exit", 1},
	{"exit -9223372036854775809", "exit", 1},
	{"ls | hola", "minishell: hola: command not found", 0},
	{"ls | ls hola", "ls: hola: No such file or directory", 0},
	{"ls | ls | hola", "ls: hola: No such file or directory", 0},
	{"ls | hola | ls", "minishell: hola: command not found", 1},
	{"ls | ls | hola | rev", "minishell: hola: command not found", 0},
	{"ls | ls | echo hola | rev", "aloh", 0},
	{"ls -la | grep \".\"", "minishell: hola: command not found", 1},
	{"ls -la | grep \"'.'\"", "", 0},
	{"echo test.c | cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e|cat -e|cat -e|cat -e", "test.c$$$$$$$$$$$", 0},
	{"ls -la | grep \"'.'\"", "", 0},
	{"ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls", "", 1},
	{"echo hola | cat | cat | cat | cat | cat | grep hola", "hola", 0},
	{"echo hola| cat", "hola", 0},
	{"echo hola |cat", "hola", 0},
	{"echo hola|cat", "hola", 0},
	{"echo hola ||| cat", "minishell: syntax error near unexpected token!! `||", 0},
	{"echo hola | cat", "hola", 0},
	{"ech|o hola | cat", "", 1},
	{"cat Makefile | cat -e | cat -e", "", 1},
	{"cat Makefile | grep srcs | cat -e", "SRCS_DIR	=	./srcs/$", 0},
	{"ls *", "", 1},
	{"ls *.*", "", 1},
	{"ls *.hola", "ls: *.hola: No such file or directory", 1},
	{"cat M*le", "", 1},
	{"cat M*ee", "", 1},
	{"cat Make*file", "", 1},
	{NULL, NULL}};

void trim_newline(char *s)
{
	char *p = strchr(s, '\n');
	if (p)
		*p = '\0';
}

int main(void)
{
	char buf[BUF_SIZE];

	for (int i = 0; tests[i].input != NULL; i++)
	{
		const char *cmd = tests[i].input;
		const char *expected = tests[i].expected_output;
		char bash_out[BUF_SIZE] = {0};
		char mini_out[BUF_SIZE] = {0};

		// BASH
		char bash_cmd[1024];
		snprintf(bash_cmd, sizeof(bash_cmd), "/bin/bash -c '%s'", cmd);
		FILE *bash_fp = popen(bash_cmd, "r");
		if (!bash_fp)
		{
			perror("bash");
			exit(1);
		}
		bash_out[0] = '\0';
		while (fgets(buf, sizeof(buf), bash_fp))
		{
			trim_newline(buf);
			if (bash_out[0])
				strcat(bash_out, " ");
			strcat(bash_out, buf);
		}

		int bash_status = pclose(bash_fp);
		int bash_exit_code = WEXITSTATUS(bash_status);

		char mini_cmd[1024];
		char exit_icmd[1024];
		//snprintf(mini_cmd, sizeof(mini_cmd), "echo '%s' | valgrind --tool=memcheck --leak-check=full ./minishell", cmd);
		snprintf(mini_cmd, sizeof(mini_cmd), "echo '%s' | ./minishell", cmd);
		FILE *mini_fp = popen(mini_cmd, "r");
		if (!mini_fp)
		{
			perror("minishell");
			exit(1);
		}
		mini_out[0] = '\0';
		while (fgets(buf, sizeof(buf), mini_fp))
		{
			trim_newline(buf);
			if (
				buf[0] == '\0' ||
				strncmp(buf, "minishell", 9) == 0 ||
				strncmp(buf, "🔧", 3) == 0 ||
				strncmp(buf, "✅", 3) == 0 ||
				strncmp(buf, "🧩", 3) == 0 ||
				strncmp(buf, "  [tok", 6) == 0 ||
				strncmp(buf, "  [ext", 6) == 0 ||
				strstr(buf, "[group") != NULL ||
				strstr(buf, "[tok") != NULL ||
				strstr(buf, "[ext") != NULL ||
				strstr(buf, "merge $'string' →") || // 👈
				strstr(buf, "merge $''string (ignore empty)") != NULL)
				continue;

			if (mini_out[0])
				strcat(mini_out, " ");
			strcat(mini_out, buf);
		}
		int mini_status = pclose(mini_fp);
		int mini_exit_code = WEXITSTATUS(mini_status);
		printf("[%02d] %-30s ... ", i + 1, cmd);
		if ((strcmp(bash_out, mini_out) == 0 
		|| strcmp(mini_out, expected) == 0 
		&& bash_exit_code == mini_exit_code)
		|| tests[i].manual_mode)
		{
			printf("✅ OK\n");
			//printf("     Bash     : [%s]\n", bash_out);
			//printf("     Minishell: [%s]\n", mini_out);
			//printf("     Expected : [%s]\n", expected);
			printf("     Mini exit : [%d]\n", mini_exit_code);
			printf("     Bash exit : [%d]\n", bash_exit_code);
		}
		else
		{
			printf("❌ FAIL\n");
			printf("     Bash     : [%s]\n", bash_out);
			printf("     Minishell: [%s]\n", mini_out);
			printf("     Expected : [%s]\n", expected);
			printf("     Mini exit : [%d]\n", mini_exit_code);
			printf("     Bash exit : [%d]\n", bash_exit_code);
		}
	}

	return 0;
}
