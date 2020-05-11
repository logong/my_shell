#include "my_shell.h"

int parse_info_init(struct parse_info *info)
{
    info->flag = 0;
    info->in_file = NULL;
    info->out_file = NULL;
    info->command2 = NULL;
    info->parameters2 = NULL;
    return 0;
}

int parsing(char **parameters,int ParaNum,struct parse_info *info)
{
    parse_info_init(info);
    if(strcmp(parameters[ParaNum-1],"&") ==0)
    {
        info->flag |= BACKGROUND;
        parameters[ParaNum-1] = NULL;
        ParaNum--;
    }
    return 1;
}
