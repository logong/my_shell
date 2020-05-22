#include"../include/my_shell.h"

int parse_info_init(struct parse_info *info)
{
    info->flag = 0;
    info->in_file = NULL;
    info->out_file = NULL;
    info->command2 = NULL;
    info->parameters2 = NULL;
    info->builtin = no;
    return 0;
}

int parsing(char **parameters, int ParaNum, struct parse_info *info)
{
    parse_info_init(info);
    // /*
    // 一般是使用getch获取
    // 方向键百(←)： 0xe04b
    // 方向键(↑)： 0xe048
    // 方向键(→)： 0xe04d
    // 方向键(↓)： 0xe050
    // */
    // if ( (int)(*parameters[ParaNum - 1]) == 0xe0)
    // {
    //     switch ( (int)(*(parameters[ParaNum - 1])) )
    //     {
    //         case 0x4b:
    //             printf("left");break;
    //         case 0x48:
    //             printf("up");break;
    //         case 0x4d:
    //             printf("right");break;
    //         case 0x50:
    //             printf("down");break;
    //     }

    if (strcmp(parameters[ParaNum - 1], "&") == 0)
    {
        info->flag |= BACKGROUND;
        parameters[ParaNum - 1] = NULL;
        ParaNum--;
    }
    
    int i;
    parse_info_init(info);
    if (strcmp(parameters[ParaNum - 1], "&") == 0)
    {
        info->flag |= BACKGROUND;
        parameters[ParaNum - 1] = NULL;
        ParaNum--;
    }
    for (i = 0; i < ParaNum;)
    {
        if (strcmp(parameters[i], "<<") == 0 || strcmp(parameters[i], "<") == 0)
        {
            info->flag |= IN_REDIRECT;
            info->in_file = parameters[i + 1];
            parameters[i] = NULL;
            i += 2;
        }
        else if (strcmp(parameters[i], ">") == 0)
        {
            info->flag |= OUT_REDIRECT;
            info->out_file = parameters[i + 1];
            parameters[i] = NULL;
            i += 2;
        }
        else if (strcmp(parameters[i], ">>") == 0)
        {
            info->flag |= OUT_REDIRECT_APPEND;
            info->out_file = parameters[i + 1];
            parameters[i] = NULL;
            i += 2;
        }
        else if (strcmp(parameters[i], "|") == 0)
        {
            char *pCh;
            info->flag |= IS_PIPED;
            parameters[i] = NULL;
            info->command2 = parameters[i + 1];
            info->parameters2 = &parameters[i + 1];
            for (pCh = info->parameters2[0] + strlen(info->parameters2[0]);
                 pCh != &(info->parameters2[0][0]) && *pCh != '/'; pCh--)
                ;
            if (*pCh == '/')
                pCh++;
            info->parameters2[0] = pCh;
            break;
        }
        else
            i++;
    }
    return 1;
}
