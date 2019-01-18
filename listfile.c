#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>


void listfiles(char *path);
void joinpath(char *path, const char *path1, const char *path2);

int main(int argc, char **argv){
    char path[256];
    if (argc == 1) {
        strcpy(path, ".");
    } else {
		strcpy(path, "./");
        strcat(path, argv[1]);
    }
    listfiles(path);
    return 0;
}

void listfiles(char *path){
    DIR *dir;
    struct dirent *dp;
    struct stat fi;
    char path2[256];

    dir = opendir(path);
	FILE *file;
	file = fopen("pathlist","w");
    for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
        if (dp->d_name[0] != '.') {
			printf("%s\n",path2);
            joinpath(path2, path, dp->d_name);
            stat(path2, &fi);
            if (!S_ISDIR(fi.st_mode)) {
				fprintf(file,"%s\n",path2);
            }
        }
    }
	fclose(file);
    closedir(dir);
    return;
}

void joinpath(char *path, const char *path1, const char *path2){
	strcpy(path, path1);
    strcat(path, "/");
    strcat(path, path2);
	//strcat("./",path);
    return;
}
