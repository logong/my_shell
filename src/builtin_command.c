#include "../include/my_shell.h"

int builtin_command(char *command, char **parameters ,struct parse_info* info)
{
    extern struct passwd *pwd;
    extern struct conf shell_conf;
    info->builtin = yes;

    if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0)
    {
        
#ifdef DEBUG
        pid_t pid = getpid();
        printf("pid = %d",pid);
#endif
        destructor(parameters,NULL);
        exit(0);

    }
        else if (strcmp(command, "about") == 0)
        {
            printf("This is a simulation of shell (bash) in Linux.\n");
            return 1;
        }
        else if (strcmp(command, "cd") == 0)
        {

            char *cd_path = NULL;

            if (parameters[1] == NULL)
            //make "cd" to "cd .." as in bash
            {
                parameters[1] = malloc(3 * sizeof(char));
                parameters[1][0] = '.';
                parameters[1][1] = '.';
                parameters[1][2] = '\0';
            }
            if (parameters[1][0] == '~')
            {
                cd_path = malloc(strlen(pwd->pw_dir) + strlen(parameters[1]));
                //'~' makes length 1 more,but instead of '\0'
                if (cd_path == NULL)
                {
                    printf("cd:malloc failed.\n");
                }
                strcpy(cd_path, pwd->pw_dir);
                strncpy(cd_path + strlen(pwd->pw_dir), parameters[1] + 1, strlen(parameters[1]));
                //printf("path with ~:\n%s\n",cd_path);
                return 0;
            }
            else
            {
                cd_path = malloc(strlen(parameters[1] + 1));
                if (cd_path == NULL)
                {
                    printf("cd:malloc failed.\n");
                }
                strcpy(cd_path, parameters[1]);
            }
            if (chdir(cd_path) != 0)
                printf("shell: cd: %s:%s\n", cd_path, strerror(errno));
            free(cd_path);
            return 0;
        }
        else if(strcmp(command, "history") == 0)
        {
            //no free
            his_now = &his_head;
            for(;his_now->next != NULL ; his_now = his_now->next)
            {
                printf("num: %d , str: %s\n",his_now->num,his_now->his_str);
            }
            return 0;
        }else if(strcmp(command, "setcolor") == 0)
        {
            if (parameters[1] == NULL){
                printf("\n       _                                      _            \n      | |                                    | |           \n   ___| |__   __ _ _ __   __ _  ___  ___ ___ | | ___  _ __ \n  / __| '_ \\ / _` | '_ \\ / _` |/ _ \\/ __/ _ \\| |/ _ \\| '__|\n | (__| | | | (_| | | | | (_| |  __/ (_| (_) | | (_) | |   \n  \\___|_| |_|\\__,_|_| |_|\\__, |\\___|\\___\\___/|_|\\___/|_|   \n                         __/ |                            \n                         |___/                             \n");
                printf("set command color as you want!\n");
                printf("use 'setcolor [Option] <Color>' to defind your color\n");
                printf("Option:\n");
                printf("\thostColor\n");
                printf("\tusernameColor\n");
                printf("\trouteColor\n");
                printf("Color:\n");
                printf("\tRED\tBLACK\tGREEN\n");
                printf("\tYELLO\tBLUE\tPURPLE\n");
                printf("\tRED\tBLACK\tGREEN\n");
                printf("\tL_BLUE\tWHITE\n");
            }else if (strcmp(parameters[1],"hostColor") == 0)
            {
                strcpy(shell_conf.host_color,parameters[2]);
            } else if (strcmp(parameters[1],"usernameColor") == 0){
                strcpy(shell_conf.username_color,parameters[2]);
            } else if (strcmp(parameters[1],"routeColor") == 0){
                strcpy(shell_conf.route_color,parameters[2]);
            } else {
                printf("\n       _                                      _            \n      | |                                    | |           \n   ___| |__   __ _ _ __   __ _  ___  ___ ___ | | ___  _ __ \n  / __| '_ \\ / _` | '_ \\ / _` |/ _ \\/ __/ _ \\| |/ _ \\| '__|\n | (__| | | | (_| | | | | (_| |  __/ (_| (_) | | (_) | |   \n  \\___|_| |_|\\__,_|_| |_|\\__, |\\___|\\___\\___/|_|\\___/|_|   \n                         __/ |                            \n                         |___/                             \n");
                printf("set command color as you want!\n");
                printf("use 'setcolor [Option] <Color>' to defind your color\n");
                printf("Option:\n");
                printf("\thostColor\n");
                printf("\tusernameColor\n");
                printf("\trouteColor\n");
                printf("Color:\n");
                printf("\tRED\tBLACK\tGREEN\n");
                printf("\tYELLO\tBLUE\tPURPLE\n");
                printf("\tRED\tBLACK\tGREEN\n");
                printf("\tL_BLUE\tWHITE\n");
            }
            return 0;
        }
        info->builtin = no;
        return 0;
    }
