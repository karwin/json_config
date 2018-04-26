#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include "cf_json.h"

typedef enum
{
	A01,
	A02,
	A03,
	A04,
	A05
}M_TYPE;

typedef enum
{
	OEM_T,
	OEM_N,
	OEM_S,
	OEM_B
}M_OEM;

typedef enum
{
	CONCTRY_CHINA
}M_CONCTRY;

typedef struct{
	M_TYPE type;
	int sn;
	int pn;
	M_OEM oem;
	M_CONCTRY country;
}MachineInfo;

static int sava_json_data(int data);
static int create_json_file(const char *json_path, MachineInfo data);

int write_file(char *filename,char *out)  
{  
	FILE *fp = NULL;  
	
	if(access(filename, F_OK) == 0){
		remove(filename);
	}
	fp = fopen(filename,"ab+");  
	if(fp == NULL)
	{  
		fprintf(stderr,"open file failed\n");  
		exit(-1);  
	}  
	fprintf(fp,"%s",out);  

	if(fp != NULL){  
		fflush(fp);
		fsync(fileno(fp));	
		fclose(fp);  
	}

	return 0;
}

static int sava_json_data(int data)
{
	CFJson *root,*basicpara;
	char *out;
	CFString* gConfigString;
	
	gConfigString = cf_string_new("");
	if(!gConfigString){
		printf("gConfigString failed\n");
		return -1;
	}

	if(cf_file_to_string(gConfigString, "./data.conf")){
		printf("Read global json config file: %s failed\n", "./data.conf");
		return -1;
	}

	root = CFJsonParse(cf_string_get_str(gConfigString));
	if (!root) {
		printf("Parse json config file: %s failed\n", "./data.conf");
		return -1;
	}

	basicpara = cJSON_GetObjectItem(root,"basicparameter"); 

	cJSON_GetObjectItem(basicpara,"num")->valueint = data;
	cJSON_GetObjectItem(basicpara,"num")->valuedouble = data;

	out = CFJsonPrint(root);
	
	write_file("./data.conf.01", out);
	free(out);

	CFJsonPut(root);
    cf_string_free(gConfigString);
	
	if( execl("/bin/mv", "mv", "./data.conf.01", "./data.conf", NULL) < 0 )
	{
		printf("execl faild\n");
		return -1;
	}
	
	return 0;	
}

static int create_json_file(const char *json_path, MachineInfo data)
{
	CFJson *root;
	char *out;

	root = CFJsonObjectNew();

	CFJsonObjectAddInt(root, "SN", data.sn);
	CFJsonObjectAddInt(root, "PN", data.pn);
	CFJsonObjectAddInt(root, "TYPE", data.type);
	CFJsonObjectAddInt(root, "OEM", data.oem);
	CFJsonObjectAddInt(root, "COUNTRY", data.country);

	out = CFJsonPrint(root);
	write_file(json_path, out);

	free(out);
	CFJsonPut(root);

	return 0;
}

int main(void)
{	
	MachineInfo data;
	printf("[usage]:this is json parse program v0.0.1\n");

	//sava_json_data(10);
	data.sn = 10000;
	data.pn = 20000;
	data.oem = OEM_T;
	data.type = A01;
	data.country = CONCTRY_CHINA;

	create_json_file("./machine.conf", data);
	return 0;
}
