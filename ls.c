#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>

typedef struct fileNames{
	char *word;
	
	struct fileNames *next;
} fileNames;


fileNames* createNode(char *wordPntr){
	fileNames *temp = malloc(sizeof(fileNames));
	temp->word = wordPntr;
	temp->next = NULL;
	return temp;
}


void addNode(fileNames **head,fileNames *toAdd){

	if(*head == NULL){
		*head = toAdd;
		return;
	}
	else if(strcasecmp((*head)->word, toAdd->word) > 0){
		toAdd->next = *head;
		*head = toAdd;
		return;
	}
	else{
		fileNames *current = *head;
		while(current->next != NULL && strcasecmp(current->next->word, toAdd->word) <= 0){
			current = current->next;
		}
		toAdd->next = current->next;
		current->next = toAdd;
		return;
	}	
						
}

void printNRelease(fileNames *head, int longFlag){
	if(longFlag == 0){
		while(head != NULL){
			printf("%s\n", head->word);
			fileNames *temp = head;
			head = head->next;
			free(temp);
		}
	}
	else{
		while(head != NULL){
			struct stat stats;
			stat(head->word, &stats);
			
			//collect permission string
			char buffer[11];
			buffer[0] = (S_ISREG(stats.st_mode) ? '-' : 'd');
			buffer[1] = (S_IRUSR & stats.st_mode ? 'r' : '-');
			buffer[2] = (S_IWUSR & stats.st_mode ? 'w' : '-');
			buffer[3] = (S_IXUSR & stats.st_mode ? 'x' : '-');
			buffer[4] = (S_IRGRP & stats.st_mode ? 'r' : '-');
			buffer[5] = (S_IWGRP & stats.st_mode ? 'w' : '-');
			buffer[6] = (S_IXGRP & stats.st_mode ? 'x' : '-');
			buffer[7] = (S_IROTH & stats.st_mode ? 'r' : '-');
			buffer[8] = (S_IWOTH & stats.st_mode ? 'w' : '-');
			buffer[9] = (S_IXOTH & stats.st_mode ? 'x' : '-');
			buffer[10] = '\0';
			
			//collect group and user information
			int uidFlag = 0;
			int gidFlag = 0;
			struct passwd *p = getpwuid(stats.st_uid);
			struct group *grp = getgrgid(stats.st_gid);
			if(p->pw_name == NULL){
				uidFlag = 1;
			}
			if(grp->gr_name == NULL){
				gidFlag = 1;
			}

			
			//collect date and time information
			char date[100];
			struct tm *dm;
			dm = localtime(&stats.st_mtime);
			strftime(date, sizeof(date), "%a %d %H:%M", dm);	
			
			//print all information
			if(uidFlag == 0 && gidFlag == 0){
				printf("%s %s %s %ld %s %s\n", buffer, p->pw_name, grp->gr_name, (long) stats.st_size, date, head->word);	
			}
			else if(uidFlag == 0 && gidFlag == 1){
				printf("%s, %s %d %ld %s %s\n", buffer, p->pw_name, (int) grp->gr_gid, (long) stats.st_size, date, head->word);			
			}
			else if(uidFlag == 1 && gidFlag == 0){
				printf("%s, %d %s %ld %s %s\n", buffer, (int) p->pw_uid, grp->gr_name, (long) stats.st_size, date, head->word);			
			}
			else{
				printf("%s, %d %d %ld %s %s\n", buffer, (int) p->pw_uid, (int) grp->gr_gid, (long) stats.st_size, date, head->word);		
			}

			
			//iterate and free
			fileNames *temp = head;
			head = head->next;
			free(temp);
		}
	}
}
			

int main(int argc, char **argv){

	int flag = 0;
        if(argc == 2){
                char* temp = argv[1];
             	if(strcmp(temp,"-l") == 0){
             		flag = 1;
             	}
        }       
        
	fileNames *head = NULL;
        struct dirent *dir;
	
	if((dir_ptr = opendir(dirname)) == NULL){
		fprintf(stderr, "ls:cannot open %s\n", dirname);
	}
	
        DIR* dirp = opendir(".");
        int errno = 0;
        while((dir = readdir(dirp)) != NULL){
                //process dir
                if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
			continue;
		}
                fileNames *newNode = createNode(dir->d_name);
                addNode(&head, newNode);
	}
        if(errno){      
                printf("error!");        
        }
        printNRelease(head, flag);   
        closedir(dirp);                
}
