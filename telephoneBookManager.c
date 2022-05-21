#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHOWLINE printf("Num. %-15s %-15s %s\n", "Name", "Tel", "Email"); puts("------------------------------------------");
#define EXIT 6

typedef struct _MEMBER {
	struct _MEMBER* next; //다음 리스트 저장
	struct _MEMBER* prev; //이전 리스트 저장
	char name[256];
	char telephoneNumber[256];
	char emailAddress[256];
	int order;
} MEMBER;

void printMenu();
void selectMenu(MEMBER*, int);
int getMenuSelection();
void initializeHead(MEMBER* head);
MEMBER* getNode(MEMBER* ptr);
void printAllData(MEMBER*);
void printMember(MEMBER*);
void insert(MEMBER*);
void appendMember(MEMBER* list, const char* name, const char* telephoneNumber, const char* emailAddress, int order);
MEMBER* findMember(MEMBER* list, const char* name);
void search(MEMBER* head);
void deleteAllData(MEMBER* head);
void deleteTargetMember(MEMBER*);
void deleteMember(MEMBER* head);
void save(MEMBER* head);


int main(void)
{
	int menuSelection = 0;
	MEMBER* head = malloc(sizeof(MEMBER));
	initializeHead(head);

	while (menuSelection != EXIT) {
		printMenu();
		menuSelection = getMenuSelection();
		selectMenu(head, menuSelection);
	}

	deleteAllData(head);
	return 0;
}

void printMenu()
{
	puts("-- Telephone Book Menu --");
	puts("1. Insert");
	puts("2. Delete");
	puts("3. Search");
	puts("4. Print All");
	puts("5. Save");
	printf("%d. Exit\n", EXIT);
}

void selectMenu(MEMBER* head, int selection)
{
	switch (selection) {
	case 1:
		insert(head);
		break;
	case 2:
		deleteMember(head);
		break;
	case 3:
		search(head);
		break;
	case 4:
		printAllData(head);
		break;
	case 5:
		save(head);
		break;
	case EXIT:
		puts("Exit, thanks you!");
		return;
	default:
		puts("Thou selected wrong menu");
	}
}

int getMenuSelection()
{
	int menuSelection = 0;
	printf("Choose your item: ");
	scanf_s("%d", &menuSelection);

	while (getchar() != '\n');
	// clear the stdin buffer

	return menuSelection;
}

void initializeHead(MEMBER* head)
{
	head->prev = NULL;
	head->next = NULL;
	head->order = -1;
	strcpy_s(head->name, sizeof(head->name), "HEAD NAME");
	strcpy_s(head->emailAddress, sizeof(head->emailAddress), "HEAD ADDRESS");
	strcpy_s(head->telephoneNumber, sizeof(head->telephoneNumber), "000-000-0000");
}

MEMBER* getNode(MEMBER* ptr)
{
	if (ptr == NULL) {
		ptr = malloc(sizeof(MEMBER));
		ptr->next = NULL;
		ptr->prev = NULL;

		return ptr;
	}
	printf("ERROR : node creation failed\n");
	return NULL;
}

void appendMember(MEMBER* head, const char* name, const char* telephoneNumber, const char* emailAddress, int order)
{
	MEMBER* newMember = malloc(sizeof(MEMBER));
	newMember->prev = NULL;
	newMember->next = NULL;

	strcpy_s(newMember->name, sizeof(newMember->name), name);
	strcpy_s(newMember->telephoneNumber, sizeof(newMember->telephoneNumber), telephoneNumber);
	strcpy_s(newMember->emailAddress, sizeof(newMember->emailAddress), emailAddress);
	newMember->order = order;

	if (head->next == NULL) {
		newMember->prev = head;
		newMember->next = NULL;

		head->next = newMember;
		head->prev = NULL;
	}
	else {
		MEMBER* cur = head;
		while (cur->next != NULL) {
			cur = cur->next;
		}
		cur->next = newMember;
		newMember->prev = cur;
	}
}

MEMBER* findMember(MEMBER* head, const char* name)
{
	MEMBER* cur = head;
	while (cur->next != NULL) {
		cur = cur->next;
		if (strcmp(cur->name, name) == 0) {
			return cur;
		}
	}

	return NULL;
}

void search(MEMBER* head)
{
	char targetName[256];
	printf("Input Name to Search: ");
	gets_s(targetName, sizeof(targetName));

	MEMBER* targetMember = findMember(head, targetName);
	if (targetMember == NULL) {
		printf("Error: No data found for Name “%s”\n", targetName);
		return;
	}
	else {
		SHOWLINE;
		printMember(targetMember);
	}
}

void deleteAllData(MEMBER* head)
{
	MEMBER* cur = head;
	MEMBER* next = NULL;
	while (cur != NULL) {
		next = cur->next;
		free(cur);
		cur = next;
	}
}

void deleteMember(MEMBER* head)
{
	puts("[DELETE]");

	char targetName[256];
	printf("Input Name to Delete: ");
	gets_s(targetName, sizeof(targetName));

	MEMBER* targetMember = malloc(sizeof(MEMBER));
	targetMember = findMember(head, targetName);
	if (targetMember == NULL) {
		printf("Data for Name “%s” is not exist\n", targetName);
	}
	else {
		deleteTargetMember(head, targetMember);
	}
}

void save(MEMBER* head)
{
	puts("[Save to File]");
	char fileName[256] = "";
	printf("Input file name to save: ");
	gets_s(fileName, sizeof(fileName));
	FILE* bookFile = 0;
	fopen_s(&bookFile, fileName, "wt");
	if (bookFile == NULL) {
		printf("ERROR: %s open failed\n", fileName);
		return;
	}

	fprintf(bookFile, "Num. % -20s % -20s %s\n", "Name", "Tel", "Email");
	MEMBER* cur = head;
	while (cur->next != NULL) {
		cur = cur->next;
		fprintf(bookFile, "[%d]    %-20s %-20s %s\n", \
			cur->order, \
			cur->name, \
			cur->telephoneNumber, \
			cur->emailAddress);
	}

	fclose(bookFile);
	printf("Telephone Book “%s” is saved\n", fileName);
}

void deleteTargetMember(MEMBER* target)
{
	// if target is the last node
	if (target->next == NULL) {
		target->prev->next = NULL;
	}
	else {
		target->prev->next = target->next;
		target->next->prev = target->prev;
	}

	free(target);
}

void printAllData(MEMBER* head)
{
	puts("[Print All Data]");
	
	SHOWLINE;
	MEMBER* cur = head;
	while (cur->next != NULL) {
		cur = cur->next;
		printMember(cur);
	}

	printf("\n");
}

void printMember(MEMBER* member)
{
	printf("[%d]  %-15s %-15s %s\n", \
		member->order,\
		member->name, \
		member->telephoneNumber, \
		member->emailAddress);
}

void insert(MEMBER* head)
{
	char name[256];
	char telephoneNumber[256];
	char emailAddress[256];
	static int memberOrder = 1;

	printf("[INSERT]\n");
	printf("Input Name: ");
	gets_s(name, sizeof(name));
	printf("Input Tel. Number: ");
	gets_s(telephoneNumber, sizeof(telephoneNumber));
	printf("Input Email Address: ");
	gets_s(emailAddress, sizeof(emailAddress));

	appendMember(head, name, telephoneNumber, emailAddress, memberOrder);
	memberOrder++;
}
