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

void type_prompt(char *);
int read_command(char **, char **, char *);
int builtin_command(char *, char **, struct parse_info *);
int parsing(char **, int, struct parse_info *);
void proc(void);
void sig_handler(int sig);
void init();
int read_conf();
int change_color(char *color);

#ifndef STRUCT_PARSE_INFO
#define STRUCT_PARSE_INFO
#define BACKGROUND 1
#define IN_REDIRECT 2
#define OUT_REDIRECT 4
#define OUT_REDIRECT_APPEND 8
#define IS_PIPED 16

#define USERNAME_COLOR change_color(shell_conf.username_color)
#define HOST_COLOR change_color(shell_conf.host_color)
#define ROUTE_COLOR change_color(shell_conf.route_color)

struct parse_info
{
    int flag;
    int builtin;
    char *in_file;
    char *out_file;
    char *command2;
    char **parameters2;
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
    char *his_str;
    struct his_info *next;
};

enum
{
    no = 0,
    yes
};

struct his_info his_head;
struct his_info *his_now;

/////////////////////////////////////////////////////////////////////
//
//
//          about his , use and store
//
/////////////////////////////////////////////////////////////////////
#define init_his()                       \
    {                                    \
        his_head.his_str = "history:\n"; \
        his_head.next = NULL;            \
        his_head.num = 0;                \
        his_now = &his_head;             \
    }

// to-do error handler
#define add_his()                                                                           \
    do                                                                                      \
    {                                                                                       \
        struct his_info *p_new_info = (struct his_info *)(malloc(sizeof(struct his_info))); \
        p_new_info->his_str = malloc(sizeof(strlen(*command)) + 1);                         \
        strcpy(p_new_info->his_str, *command);                                              \
        p_new_info->num = his_now->num + 1;                                                 \
        p_new_info->next = NULL;                                                            \
        his_now->next = p_new_info;                                                         \
        his_now = his_now->next;                                                            \
    } while (0)

//////////////////////////////////////////////////////////////////////
//
//
//          free res , expend free_res(res_name)
//
//
//////////////////////////////////////////////////////////////////////

#define free_res(res_name)           \
    do                               \
    {                                \
        assert(res_name == NULL);    \
        free(res_name);              \
        if (res_name != NULL)         \
        {                            \
            printf("free error!\n"); \
        }                            \
} while (0)

#endif
