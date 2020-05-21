#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h> 
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h> 

#include <ctype.h>
#include <assert.h>

#define MAX_PROMPT 1024
#define MAXLINE 4096 //the length of all args is ARG_MAX
#define MAXARG 20
#define DEBUG 1



struct parse_info;
struct conf shell_conf;
struct passwd *pwd;
char *buffer;

void type_prompt(char*);
int read_command(char **,char **,char*);
int builtin_command(char *,char **,struct parse_info *);
int parsing(char **,int,struct parse_info *);
void proc(void);
void sig_handler(int sig);
void init();
int read_conf();
int change_color(char * color);

#ifndef STRUCT_PARSE_INFO
#define STRUCT_PARSE_INFO
#define BACKGROUND 			1
#define IN_REDIRECT 		2
#define OUT_REDIRECT 		4
#define OUT_REDIRECT_APPEND	8
#define IS_PIPED 			16


#define USERNAME_COLOR      change_color(shell_conf.username_color)
#define HOST_COLOR      change_color(shell_conf.host_color)
#define ROUTE_COLOR      change_color(shell_conf.route_color)






#define init_his() \
{ \
    his_head.his_str = "history:\n"; \
    his_head.next = NULL;            \
    his_head.num = 0  ;           \
    his_now = &his_head;             \
}\


struct parse_info 
{
    int flag;
    int builtin;
    char* in_file;
    char* out_file;
    char* command2;
	char** parameters2;
};

struct conf 
{
    char host_color[16];
	char username_color[16];
	char route_color[16];
};


struct his_info
{
    int num;
    char* his_str;
    struct his_info* next;
    
};

enum 
{
    no = 0,
    yes
};


struct his_info his_head;
struct his_info * his_now;


#endif
