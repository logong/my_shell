#include <sys/utsname.h>
#include "my_shell.h"
const int max_name_len = 256;
const int max_path_len = 1024;



void type_prompt(char *prompt)
{
    extern struct passwd *pwd;
    extern struct conf shell_conf;
    char hostname[max_name_len];
    char pathname[max_path_len];
    int length;
    pwd = getpwuid(getuid());
    getcwd(pathname,max_path_len);
    if(gethostname(hostname,max_name_len)==0)
        sprintf(prompt,"[myshell][\033[%dm%s\033[0m@\033[%dm%s\033[32m:",USERNAME_COLOR,pwd->pw_name,HOST_COLOR,hostname);
    else
        sprintf(prompt,"[myshell]%s@unknown:",pwd->pw_name);
    length = strlen(prompt);
    if(strlen(pathname) < strlen(pwd->pw_dir) || 
            strncmp(pathname,pwd->pw_dir,strlen(pwd->pw_dir))!=0)
        sprintf(prompt+length,"\033[1;%dm%s\033[0m",ROUTE_COLOR,pathname);
    else
        sprintf(prompt+length,"\033[1;%dm~%s\033[0m",ROUTE_COLOR,pathname+strlen(pwd->pw_dir));
    length = strlen(prompt);
    if(geteuid()==0)
        sprintf(prompt+length," \033[33m# \033[0m");
    else
        sprintf(prompt+length," \033[33m$ \033[0m");
    return;
}

