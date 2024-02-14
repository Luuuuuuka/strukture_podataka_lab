#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _year;
typedef struct _year *YearP;
struct _date;
typedef struct _date *DateP;

typedef struct _year {
	int number;
	YearP Next;
	DateP Child;
} Year;

typedef struct _date {
	char personName[32];
	char personLastname[32];
	int day;
	int month;
	DateP Next;
} Date;

int InitializeHead(YearP Head);
int AddDate(YearP P, char Surname[32], char Name[32], int Day, int Month, int Year);
int PrintList(YearP P);
int DeleteYear(YearP P);
int DeleteDate(DateP P);

int main()
{
	char Name[32];
	char Surname[32];
	int Day = 0;
	int Month = 0;
	int Year = 0;
	YearP Head = NULL;

	Head = (YearP)malloc(sizeof(Year));
	if (Head == NULL) {
		printf("ERROR! Memory allocation failed");
		return -1;
	}

	InitializeHead(Head);

	strcpy(Name, "");
	strcpy(Surname, "");

	FILE* fp = NULL;
	fp = fopen("osobe.txt", "r");
	if (fp == NULL) {
		printf("ERROR! File failed to load");
		return -1;
	}

	while (!feof(fp)) {
		fscanf(fp, "%s %s %d.%d.%d.\n", Surname, Name, &Day, &Month, &Year);

		AddDate(Head, Surname, Name, Day, Month, Year);

	}

	fclose(fp);

	PrintList(Head->Next);

	DeleteYear(Head);

	return 0;
}

int InitializeHead(YearP Head) {
	Head->number = 0;
	Head->Next = NULL;
	Head->Child = NULL;

	return 0;
}

int AddDate(YearP P, char Surname[32], char Name[32], int Day, int Month, int Year) {
	DateP Q;
	DateP Q_prev;
	DateP NewDate = NULL;
	YearP NewYear = NULL;
	//samo za debug
	YearP Head = P;

	NewDate = (DateP)malloc(sizeof(Date));
	if (NewDate == NULL) {
		printf("ERROR! Memory allocation failed");
		return -1;
	}

	NewYear = (YearP)malloc(sizeof(Year));
	if (NewYear == NULL) {
		printf("ERROR! Memory allocation failed");
		return -1;
	}

	while ((P->Next != NULL) && (P->Next->number < Year))
		P = P->Next;

	if ((P->Next != NULL) && (P->Next->number == Year)) {
		P = P->Next;
		free(NewYear);
	}
	else {
		NewYear->number = Year;
		NewYear->Next = P->Next;
		P->Next = NewYear;
		P = P->Next;

		P->Child = NULL;
	}

	strcpy(NewDate->personLastname, Surname);
	strcpy(NewDate->personName, Name);
	NewDate->day = Day;
	NewDate->month = Month;

	if (P->Child == NULL){
		P->Child = NewDate;
		P->Child->Next = NULL;
	}
	else
	{
		Q = P->Child;
		Q_prev = NULL;

		while ((Q != NULL) && (Q->month < NewDate->month)){
			Q_prev = Q;
			Q = Q->Next;
		}
		while ((Q != NULL) && (Q->month == NewDate->month) && (Q->day < NewDate->day)){
			Q_prev = Q;
			Q = Q->Next;
		}

		if (Q == P->Child) {
			NewDate->Next = Q;
			P->Child = NewDate;
		}
		else {
			//ako je dosao dovde, Q je sljedbenik, ide u Next
			NewDate->Next = Q;
			//moramo sad pridruziti novi clan kao Next prethodniku
			Q_prev->Next = NewDate;
		}

		//OLD
		/*if (P->Child->month < NewDate->month) {
			NewDate->Next = P->Child->Next;
			P->Child = NewDate;
			}
		else if ((P->Child->month == NewDate->month) && (P->Child->day < NewDate->day)) {
			NewDate->Next = P->Child->Next;
			P->Child = NewDate;	
		}
		else {

			Q = P->Child;

			while ((Q->Next != NULL) && (Q->Next->month < NewDate->month))
				Q = Q->Next;
			while ((Q->Next != NULL) && (Q->Next->month == NewDate->month) && (Q->Next->day < NewDate->day))
				Q = Q->Next;

			if (Q == P->Child) {
				NewDate->Next = Q;
				P->Child = NewDate;
			}
			else {
				NewDate->Next = Q->Next;
				Q->Next = NewDate;
			}
		}*/
	}

	return 0;
}

int PrintList(YearP P) {
	while (P != NULL) {
		printf("\n%d:     ", P->number);
		DateP Q = P->Child;
		while (Q != NULL) {
			printf(" %s %s %d.%d.,", Q->personLastname, Q->personName, Q->day, Q->month);
			Q = Q->Next;
		}
		P = P->Next;
	}
	return 0;
}

int DeleteYear(YearP P) {
	if (P->Child != NULL)
		DeleteDate(P->Child);

	if (P->Next != NULL)
		DeleteYear(P->Next);

	free(P); //puca na brisanju godine

	return 0;
}

int DeleteDate(DateP P) {
	if (P->Next != NULL)
		DeleteDate(P->Next);

	free(P);

	return 0;
}
