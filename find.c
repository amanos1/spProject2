#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int contains(char *one, char *two) {
    int j = 0;
    for(int i = 0; i <= strlen(one); i++) {
        if(two[j] == '\0') return 1;
        if(one[i] == two[j]) j++;
        else j = 0;
    }
    return 0;
}

void search(char *matchTo, DIR *shiit, char *stuff) {
	struct dirent *dir;
	int errno = 0;
	while((dir = readdir(shiit)) != NULL) {
		//process dir
		if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0 || dir->d_name[0] == '.') {
			continue;
		}

		int oldLen = strlen(stuff);
		int newLen = oldLen + strlen(dir->d_name) + 1;
		char *fullName = malloc(sizeof(char) * (newLen + 1));
		strcpy(fullName, stuff);
		fullName[oldLen] = '/'; fullName[oldLen+1] = '\0';
		strcat(fullName, dir->d_name);

		DIR *nextVictim = opendir(fullName);
		if(nextVictim != NULL){
			search(matchTo, nextVictim, fullName);
		}

		if(contains(fullName, matchTo)) printf("%s\n", fullName);

		closedir(nextVictim);
		free(fullName);
	}
	if(errno){
		printf("error!");
	}
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("error: wrong number of arguments\n");
		return 1;
	}

	char *toughtActToFollow = argv[1];

	DIR *dirp = opendir(".");
	if(dirp == NULL){
		fprintf(stderr, "ls:cannot open current directory\n");
	}

	search(toughtActToFollow, dirp, ".");

	closedir(dirp);

	return EXIT_SUCCESS;
}
