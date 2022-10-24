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
	temp->word = malloc(strlen(wordPntr) + 1);
	strcpy(temp->word, wordPntr);
	temp->next = NULL;
	temp->layers = howDeep;
	return temp;
}

void addNode(fileNames **head, fileNames *toAdd) {
	//printf("%s\n", toAdd->word);
	if(*head == NULL) {
		*head = toAdd;
		return;
	} else if(strcasecmp((*head)->word, toAdd->word) > 0) {
		toAdd->next = *head;
		*head = toAdd;
		return;
	} else {
		fileNames *current = *head;
		while(current->next != NULL && strcasecmp(current->next->word, toAdd->word) <= 0 && current->layers != toAdd->layers) {
			current = current->next;
		}
		toAdd->next = current->next;
		current->next = toAdd;
		return;
	}
}

void addChild(fileNames *parent, fileNames *toAdd) {
	if(parent->next == NULL) {
		parent->next = toAdd;
		return;
	}
	fileNames *current = parent;
	while(current->next != NULL && current->next->layers == toAdd->layers && strcasecmp(current->next->word, toAdd->word) <= 0) {
		current = current->next;
	}
	toAdd->next = current->next;
	current->next = toAdd;
	return;
}

void printNRelease(fileNames *head){
	if(head == NULL) return;
	for(int i = 0; i < head->layers; i++) printf("  ");
	printf("- %s\n", head->word);
	printNRelease(head->next);
	free(head->word);
	free(head);
}

fileNames *printIt(DIR *shiit, int layer, char *stuff, fileNames *prevHead, fileNames *parent) {
	struct dirent *dir = readdir(shiit);
	fileNames *head = prevHead;
	int errno = 0;
	while((dir = readdir(shiit)) != NULL) {
		//process dir
		if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0 || dir->d_name[0] == '.') {
			continue;
		}
		fileNames *newNode = createNode(dir->d_name, layer);
		if(parent != NULL) addChild(parent, newNode);
		else addNode(&head, newNode);

		int oldLen = strlen(stuff);
		int newLen = oldLen + strlen(dir->d_name) + 1;
		char *folder = malloc(sizeof(char) * (newLen + 1));
		strcpy(folder, stuff);
		if(layer > 0) { folder[oldLen] = '/'; folder[oldLen+1] = '\0'; }
		strcat(folder, dir->d_name);

		DIR *nextVictim = opendir(folder);
		if(nextVictim != NULL){
			printIt(nextVictim, layer + 1, folder, head, newNode);
		}
		closedir(nextVictim);
		free(folder);
	}
	if(errno){
		printf("error!");
	}
	return head;
}

int main(int argc, char *argv[]) {
	DIR *dirp = opendir(".");
	if(dirp == NULL){
		fprintf(stderr, "ls:cannot open current directory\n");
	}

	printf(".\n");
	fileNames *head = printIt(dirp, 0, "", NULL, NULL);
	printNRelease(head);

	closedir(dirp);

	return EXIT_SUCCESS;
}
