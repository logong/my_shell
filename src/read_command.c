#include "../include/my_shell.h"
#ifdef READLINE_ON
#include <readline/readline.h>
#include <readline/history.h>
#endif 


//return value: number of parameters
//0 represents only command without any parameters
//-1 represents wrong input

int read_command(char **command,char **parameters,char *prompt)
{
#ifdef READLINE_ON
    free(buffer);
    buffer = readline(prompt);
    if(feof(stdin)) {
        printf("\n");
        exit(0);
    }

#else
    printf("%s",prompt);
    char* Res_fgets = fgets(buffer,MAXLINE,stdin);
    if(Res_fgets == NULL)
    {
        printf("\n");
        exit(0);
    }		
#endif

    if(buffer[0] == '\0')
        return -1;
    char *pStart,*pEnd;
    int count = 0;
    int isFinished = 0;
    pStart = pEnd = buffer;
    while(isFinished == 0)
    {
        while((*pEnd == ' ' && *pStart == ' ') || (*pEnd == '\t' && *pStart == '\t'))
        {
            pStart++;
            pEnd++;
        }

        if(*pEnd == '\0' || *pEnd == '\n')
        {
            if(count == 0)
                return -1;
            break;
        }

        while(*pEnd != ' ' && *pEnd != '\0' && *pEnd != '\n')
            pEnd++;


        if(count == 0)
        {
            char *p = pEnd;
            *command = pStart;
            while(p!=pStart && *p !='/')
                p--;
            if(*p == '/')
                p++;
            //else //p==pStart
            parameters[0] = p;
            count += 2;

            add_his();

#ifdef DEBUG
            printf("\ncommand:%s\n",*command);
            printf("\nnow his num:%d\n",his_now->num);
            printf("\nnow string :%s\n",his_now->his_str);
#endif

        }
        else if(count <= MAXARG)
        {
            parameters[count-1] = pStart;
            count++;
        }
        else
        {
            break;
        }

        if(*pEnd == '\0' || *pEnd == '\n')
        {
            *pEnd = '\0';
            isFinished = 1;
        }
        else
        {
            *pEnd = '\0';
            pEnd++;
			pStart = pEnd;
        }
    }

    parameters[count-1] = NULL;

#ifdef DEBUG
    /*input analysis*/
    printf("input analysis:\n");
    printf("pathname:[%s]\ncommand:[%s]\nparameters:\n",*command,parameters[0]);
    int i;
    for(i=0;i<count-1;i++)
        printf("[%s]\n",parameters[i]);
    pid_t pid = getpid();
    printf("pid = %d \n",pid);
#endif

    return count;
}

