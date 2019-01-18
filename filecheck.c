#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <openssl/md5.h>        // require libcrypto

typedef struct {
    char s[40];
} word;

typedef unsigned char        BYTE;
void CalcurateMD5Hash(BYTE *buffer, long size, BYTE* hash);
int main(int argc){
    int fd;
    struct stat st;
    BYTE *buffer;
    BYTE hash[MD5_DIGEST_LENGTH];

	word dict[1000];
    FILE *fp;
	FILE *file;
	FILE *file2;
    char str[100],str2,str3;
	char md5hash[100];
	char temp0;
    int line,result,temp;
	line=0;
    if ((fp=fopen("./pathlist","r")) == NULL) {
        fprintf(stderr, "cannot open dict file\n");
        return 1;
    }
	remove("baselist");
	while((fgets(str,100,fp)) != NULL) {
		char buff[256];
		int count=0;
		file = fopen("baselist","a");
		strtok(str, "\n\0");
		//printf("%s\n",str);
		if ( argc != 1 ) {
	        printf("引数は入力しないでください\n");
	        return 0;
	    }
	    if ( stat(str, &st) < 0 || (fd=open(str, O_RDONLY)) < 0 ){
			printf("%s\n","ファイルが存在しません");
			continue;
		}
	    if ( (buffer = (BYTE*)mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED ){
			printf("%s\n","致命的なエラーが発生しました");
			continue;
		}
	    CalcurateMD5Hash(buffer,st.st_size,hash);
	    for ( int i=0; i<MD5_DIGEST_LENGTH; i++ ) {
			fprintf(file,"%.2x",hash[i]);
			//printf("%.2x",hash[i]);
		}
		//printf("\n");
		fprintf(file,"%s\n","");
		fclose(file);
		temp=0;
		word dict[1000]={};
		file2 = fopen("./baselist","r");
		while((fgets(dict[temp].s,40,file2)) != NULL) {
			temp++;
		}
		fclose(file2);
		file2 = fopen("./baselist","r");
		char *find = dict[temp-1].s;
		count=0;
		while ( fgets(buff,sizeof(buff),file2) != NULL ){
			if ( strstr(buff,find) != NULL ){
				count++;
			}
		}
		if (count>1){
			printf("%sを削除しました\n",str);
			remove(str);
		}
		fclose(file2);
	    if ( st.st_size ) munmap(buffer, st.st_size);
	    close(fd);
		line++;
	}
	fclose(fp);
    return 0;
}

void CalcurateMD5Hash(BYTE *buffer, long size, BYTE* hash){
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, buffer, size);
    MD5_Final(hash, &ctx);
}
