#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

void ls(char dirname[]) {
	DIR *dir_ptr;
	struct dirent *direntp;

	if((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr, "ls:cannot open %s\n", dirname);
	else {
		while((direntp = readdir(dir_ptr)) != NULL) {
			if(strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0) {
				continue;
			}
			printf("%s\n", direntp->d_name);
		}
		closedir(dir_ptr);
	}
}

/*
char *uid_to_name(uid_t uid) {
	struct passwd *getpwuid(), *pw_ptr;
	static char numstr[10];

	if((pw_ptr = getpwuid(uid)) == NULL) {
		sprintf(numstr, "%d", uid);
		return numstr;
	} else {
		return pw_ptr->pw_name;
	}
}

char *gid_to_name(gid_t gid) {
	struct group *getgrgid(), *grp_ptr;
	static char numstr[10];

	if((grp_ptr = getgrgid(gid)) == NULL) {
		sprintf(numstr, "%d", gid);
		return numstr;
	} else {
		return grp_ptr->gr_name;
	}
}
*/

//void display_info(struct stat);

int main(int argc, char *argv[]) {
	if(argc == 1)
		ls(".");
}

