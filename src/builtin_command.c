#include "my_shell.h"

int builtin_command(char *command, char **parameters )
{
    extern struct passwd *pwd;
    if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0)
    {
        
#ifdef DEBUG
        pid_t pid = getpid();
        printf("pid = %d",pid);
#endif
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
        }
        return 0;
    }
