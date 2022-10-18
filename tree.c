#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

typedef struct fileNames{
	char *word;
	int layers;
	struct fileNames *next;
} fileNames;

fileNames* createNode(char *wordPntr, int howDeep){
	fileNames *temp = malloc(sizeof(fileNames));
	temp->word = wordPntr;
	temp->next = NULL;
	temp->layers = howDeep;
	return temp;
}

void addNode(fileNames **head,fileNames *toAdd) {
	if(*head == NULL) {
		*head = toAdd;
		return;
	} else if(strcasecmp((*head)->word, toAdd->word) > 0) {
		toAdd->next = *head;
		*head = toAdd;
		return;
	} else {
		fileNames *current = *head;
		//printf("%i\n", current->next == NULL);
		while(current->next != NULL /*&& strcasecmp(current->next->word, toAdd->word) <= 0*/) {
			current = current->next;
		}
		toAdd->next = current->next;
		current->next = toAdd;
		return;
	}
}

void printNRelease(fileNames *head){
	while(head != NULL){
		for(int i = 0; i < head->layers; i++) printf("  ");
		printf("- %s\n", head->word);
		fileNames *temp = head;
		head = head->next;
		free(temp);
	}
}

void printIt(DIR *shiit, int layer, char *stuff, fileNames *head) {
	struct dirent *dir;
	int errno = 0;
	while((dir = readdir(shiit)) != NULL) {
		//process dir
		if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0 || dir->d_name[0] == '.') {
			continue;
		}
		fileNames *newNode = createNode(dir->d_name, layer);
		addNode(&head, newNode);

		int oldLen = strlen(stuff);
		int newLen = oldLen + strlen(dir->d_name) + 1;
		char *folder = malloc(sizeof(char) * (newLen + 1));
		strcpy(folder, stuff);
		if(layer > 0) { folder[oldLen] = '/'; folder[oldLen+1] = '\0'; }
		strcat(folder, dir->d_name);

		DIR *nextVictim = opendir(folder);
		if(nextVictim != NULL){
			printIt(nextVictim, layer + 1, folder, head);
		}
		closedir(nextVictim);
		free(folder);
	}
	if(errno){
		printf("error!");
	}
}

int main(int argc, char *argv[]) {
	fileNames *head = NULL;

	DIR *dirp = opendir(".");
	if(dirp == NULL){
		fprintf(stderr, "ls:cannot open current directory\n");
	}

	printf(".\n");
	printIt(dirp, 0, "", head);
	printNRelease(head);

	closedir(dirp);

	return EXIT_SUCCESS;
}
