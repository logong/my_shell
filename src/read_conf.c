
#include "my_shell.h"


#define KEYVALLEN 100
 
/* 删除左边的空格   */
 
static char * l_trim(char * szOutput, const char *szInput)
{
	assert(szInput != NULL);
	assert(szOutput != NULL);
	assert(szOutput != szInput);
 
	for(; *szInput != '\0' && isspace(*szInput); ++szInput)
	{
		;
	}
 
	return strcpy(szOutput, szInput);
}
 
/*  删除右边的空格  */
 
static char *r_trim(char *szOutput, const char *szInput)
{
	char *p = NULL;
 
	assert(szInput != NULL);
	assert(szOutput != NULL);
	assert(szOutput != szInput);
	strcpy(szOutput, szInput);
 
	for(p = szOutput + strlen(szOutput) - 1; p >= szOutput && isspace(*p); --p)
	{
		;
	}
 
	*(++p) = '\0';
 
	return szOutput;
}
 
/*   删除两边的空格   */
 
static char * a_trim(char * szOutput, const char * szInput)
{
	char *p = NULL;
 
	assert(szInput != NULL);
	assert(szOutput != NULL);
 
	l_trim(szOutput, szInput);
 
	for(p = szOutput + strlen(szOutput) - 1;p >= szOutput && isspace(*p); --p)
	{
		;
	}
 
	*(++p) = '\0';
 
	return szOutput;
}
 
 //get config from file
 //--example.txt
 //[COLOR]
 //test_COlOR=RED
 //--
 // you can use GetProfileString("example.txt","COLOR","test_COLOR",varYouUseToStoreValue)
int GetProfileString(char *profile, char *AppName, char *KeyName, char *KeyVal)
{
	char appname[32],keyname[32];
	char *buf,*c;
	char buf_i[KEYVALLEN], buf_o[KEYVALLEN]; //input output
 
	FILE *fp;
	int found=0; /* 1 AppName 2 KeyName */
 
	if( (fp=fopen( profile,"r" ))==NULL )
	{
		printf( "openfile [%s] error [%s]\n",profile,strerror(errno) );
		return(-1);
	}
 
	//Set a pointer to the beginning of the file
	fseek( fp, 0, SEEK_SET );
	memset( appname, 0, sizeof(appname) );

	//Surround the name of the configuration block with []
	sprintf( appname,"[%s]", AppName );
 
	while( !feof(fp) && fgets( buf_i, KEYVALLEN, fp )!=NULL )
	{
		l_trim(buf_o, buf_i);

		if( strlen(buf_o) <= 0 )
			continue;
 
		buf = NULL;
		buf = buf_o;
 
		//find App block
		if( found == 0 )
		{
			if( buf[0] != '[' ) 
			{
				continue;
			} 
			else if ( strncmp(buf,appname,strlen(appname))==0 )
			{
				found = 1;
				continue;
			}
		} 
		else if( found == 1 )
		{
			if( buf[0] == '#' )
			{
				continue;
			} 
			else if ( buf[0] == '[' ) 
			{
				break;
			} 
			else 
			{
				if( (c = (char*)strchr(buf, '=')) == NULL )
					continue;
 

				memset( keyname, 0, sizeof(keyname) );
				
				//Cut the left half
				sscanf( buf, "%[^=|^ |^\t]", keyname );
 
				if( strcmp(keyname, KeyName) == 0 )
				{
					//Cut the right half
					sscanf( ++c, "%[^\n]", KeyVal );

					char *KeyVal_o = (char *)malloc(strlen(KeyVal) + 1);
 
					if(KeyVal_o != NULL)
					{
						memset(KeyVal_o, 0, sizeof(KeyVal_o));
						a_trim(KeyVal_o, KeyVal);
						if(KeyVal_o && strlen(KeyVal_o) > 0)
							strcpy(KeyVal, KeyVal_o);
						free(KeyVal_o);
						KeyVal_o = NULL;
					}
 
					found = 2;
					break;
				} 
				else 
				{
					continue;
				}
			}
		}
	}
 
	fclose( fp );
 
	if( found == 2 )
		return(0);
 
	return(-1);
}

// used to change color, every time we need to print word to screen 
// we use this function to get right color number.
// for example, if we got RED we choose 'D',and return 31

int change_color(char * color){
	//printf("%s",color);
	switch(color[2]){
		case 'A': return 30;
		case 'D': return 31;
		case 'E': return 32;
		case 'L': return 33;
		case 'U': return 34;
		case 'R': return 35;
		case 'B': return 36;
		case 'I': return 37;
		default:return 37;
	}
}


//got three config string from config file.
//if it's not get right config file or configs. It will be -1
int read_conf()
{
	extern struct conf shell_conf;
	if (GetProfileString("../.my_shellrc", "color", "host_color", shell_conf.host_color) != -1)
		if (GetProfileString("../.my_shellrc", "color", "username_color", shell_conf.username_color) != -1)
			if (GetProfileString("../.my_shellrc", "color", "route_color", shell_conf.route_color)!= -1){
				return 0;
			}
	if (GetProfileString(".my_shellrc", "color", "host_color", shell_conf.host_color) != -1)
		if (GetProfileString(".my_shellrc", "color", "username_color", shell_conf.username_color) != -1)
			if (GetProfileString(".my_shellrc", "color", "route_color", shell_conf.route_color)!= -1){
				return 0;
			}
	return -1;
}
