#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "BTree.h"

void play() {
	printf("Please press any key to continue ........\n");
	getchar();
	getchar();
}

/****************≤‚ ‘ µœ÷¿‡****************/
int main() {
	BTree T = NULL;
	int choice, element;
	result result;
	printf("------------------------^_^-------------------------\n");
	printf("|      Welcome to the Btree operation interface     |\n");
	printf("| Author: GP(Class 1,software engineering,grade 18) |\n");
	printf("------------------------^_^-------------------------\n\n");
	printf("Please press any key to continue ........\n");
	getchar();
	while (1) {
		system("cls");
		printf("|------------BTree function menu-------------|\n");
		printf("|------------0.End the task------------------|\n");
		printf("|------------1.Insert the element------------|\n");
		printf("|------------2.Search the element------------|\n");
		printf("|------------3.Delete the element------------|\n");
		printf("|------------4.Display BTree-----------------|\n");
		printf("|------------5.Destroy BTree-----------------|\n");
		printf("Please enter the option£∫");
		scanf_s("%d", &choice);
		if (0 == choice) {
			system("cls");
			DestroyBTree(T);
			printf("------------------^_^----------------------\n");
			printf("|Thanks,looking forward to your next visit|\n");
			printf("------------------^_^----------------------\n");
			break;
		}
		switch (choice) {
		case 1:
			system("cls");
			printf("Please enter the element you want to insert: ");
			scanf_s("%d", &element);
			SearchBTree(T, element, result);
			if (0 == result.tag) {
				InsertBTree(T, element, result.pt, result.i);
				printf("Insert successfully!\n");
			}
			else {
				printf("The element %d has been exited!\n", element);
			}
			play();
			break;
		case 2:
			system("cls");
			printf("Please enter the element you want to search: ");
			scanf_s("%d", &element);
			SearchBTree(T, element, result);
			if (1 == result.tag) {
				printf("The element %d exits\n", element);
			}
			else {
				printf("The element %d does not exit\n", element);
			}
			play();
			break;
		case 3:
			system("cls");
			printf("Please enter the element you want to delete: ");
			scanf_s("%d", &element);
			SearchBTree(T, element, result);
			if (0 == result.tag) {
				printf("The element %d does not exit\n", element);
			}
			else {
				DeleteBTree(result.pt, result.i, T);
				printf("Delete successfully!\n");
			}
			play();
			break;
		case 4:
			system("cls");
			if (NULL == T) {
				printf("BTree is empty!\n");
			}
			else {
				printf("Now the BTree is : \n");
				Display(T, 1);
			}
			play();
			break;
		case 5:
			system("cls");
			DestroyBTree(T);
			printf("Destroy successfully!\n");
			play();
			break;
		default:
			system("cls");
			printf("Wrong option entered! Please enter the right option!\n");
			play();
			break;
		}
	}
	return 0;
}