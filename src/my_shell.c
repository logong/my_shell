#include "../include/my_shell.h"
#define TRUE 1
#define MAXPIDTABLE 1024

pid_t BPTable[MAXPIDTABLE];

void sig_handler(int sig)
{
    printf("bye!\n");
    exit(0);
}

void proc(void)
{
    int status,i;
    char *command = NULL;
    char **parameters;
    int ParaNum;
    char prompt[MAX_PROMPT];
    struct parse_info info;
    pid_t ChdPid,ChdPid2;


    parameters = malloc(sizeof(char *)*(MAXARG+2));
    buffer = malloc(sizeof(char) * MAXLINE);
    
    if(parameters == NULL || buffer == NULL)
    {
        printf("my_shell error:malloc failed.\n");
        return;
    }


    while(TRUE)
    {
        int pipe_fd[2],in_fd,out_fd;
        
        type_prompt(prompt);
        ParaNum = read_command(&command,parameters,prompt);

        if(-1 == ParaNum)
            continue;
        ParaNum--;//count of units in buffer
        
        parsing(parameters,ParaNum,&info);

        if(builtin_command(command,parameters,&info))
            continue;
        if(info.flag & IS_PIPED) //command2 is not null
        {                
            if(pipe(pipe_fd)<0)
            {
                printf("my_shell error:pipe failed.\n");
                exit(0);
            }
        }  
        if((ChdPid = fork())!=0) //shell
        {
            #if 1
            if(info.flag & IS_PIPED)// 实现 ||
            {
                if((ChdPid2=fork()) == 0) //command2
                {
                    close(pipe_fd[1]);
                    close(fileno(stdin)); 
                    dup2(pipe_fd[0], fileno(stdin));
                    close(pipe_fd[0]); 
                    execvp(info.command2,info.parameters2);
                }
                else
                {
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                    waitpid(ChdPid2,&status,0); //wait command2
                }
            }
            #endif

            if(info.flag & BACKGROUND)
            {
                printf("Child pid:%u\n",ChdPid);
                int i;
                for(i=0;i<MAXPIDTABLE;i++)
                    if(BPTable[i]==0)
                        BPTable[i] = ChdPid; //register a background process
                if(i==MAXPIDTABLE)
                    perror("Too much background processes\nThere will be zombine process");
            }
            else
            {          
                waitpid(ChdPid,&status,0);//wait command1
            } 
        }
        else //command1
        {
			
            if(info.flag & IS_PIPED) //command2 is not null
            {                
                if(!(info.flag & OUT_REDIRECT) && !(info.flag & OUT_REDIRECT_APPEND)) // ONLY PIPED
	           {
                    close(pipe_fd[0]);
                    close(fileno(stdout)); 
                    dup2(pipe_fd[1], fileno(stdout));
                    close(pipe_fd[1]);
                }
                else //OUT_REDIRECT and PIPED
	           {
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);//send a EOF to command2
                    if(info.flag & OUT_REDIRECT)
    	               out_fd = open(info.out_file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
                    else
    	               out_fd = open(info.out_file, O_WRONLY|O_APPEND|O_TRUNC, 0666);
                    close(fileno(stdout)); 
                    dup2(out_fd, fileno(stdout));
                    close(out_fd);	        
                }
            }
            else
            {
                if(info.flag & OUT_REDIRECT) // OUT_REDIRECT WITHOUT PIPE
	           {
                    out_fd = open(info.out_file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
                    close(fileno(stdout)); 
                    dup2(out_fd, fileno(stdout));
                    close(out_fd);
                }
                if(info.flag & OUT_REDIRECT_APPEND) // OUT_REDIRECT_APPEND WITHOUT PIPE
	           {
                    out_fd = open(info.out_file, O_WRONLY|O_CREAT|O_APPEND, 0666);
                    close(fileno(stdout)); 
                    dup2(out_fd, fileno(stdout));
                    close(out_fd);
                }
            }
            
            if(info.flag & IN_REDIRECT)
            {
                in_fd = open(info.in_file, O_CREAT |O_RDONLY, 0666);
                close(fileno(stdin)); 
                dup2(in_fd, fileno(stdin));
                close(in_fd); 
            }

            // exit
            if( execvp(command,parameters) == -1)
            {
                if( info.builtin == yes )
                    break;
                else
                {
                    printf("command not find!\n");
                    exit(0);
                }
            }
        }
    }
    free(parameters);
	free(buffer);
}

void init()
{   

    init_his();
    if(read_conf() == -1){
        printf("\033[31mGet conf error\nWe will use default config\n\033[0m");
    }
    signal(SIGINT,sig_handler);
}


int main() 
{
    int i;
    init();
    //init the BPTable
    for(i=0;i<MAXPIDTABLE;i++)
        BPTable[i] = 0;
    proc();
    return 0;
}
