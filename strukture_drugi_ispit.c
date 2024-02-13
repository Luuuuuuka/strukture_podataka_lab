#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_ID (100)
#define MAX_ID (180)
	
typedef struct _date {
	int day;
	int month;
	int year;
} Date;

struct _person;
typedef struct _person *PersonP;
typedef struct _person {
	char firstName[32];
	char lastName[32];
	int idNumber;
	Date birthDate;
	PersonP Next;
} Person;

int InitializeHead(PersonP Head);
int AddPerson(PersonP P, char Name[32], char Surname[32], int Day, int Month, int Year, int Id);
//int AddDate(PersonP P, int Day, int Month, int Year);
int PrintList(PersonP P);
int RandomNum(int min, int max);
int GenerateId(int id_used[81]);
int CreateListMonthWithMostPeople(PersonP P, PersonP Q);

int main()
{
	srand(time(NULL));			/* inicijalizacija rand generatora */
	int id_used[81] = { 0 };    /* niz u kojem spremamo iskorištene ID-ove, služi za provjeru jedinstvenosti */

	char Name[32];
	char Surname[32];
	int Day = 0;
	int Month = 0;
	int Year = 0;
	int Id = 0;
	PersonP Head = NULL;
	PersonP NewHead = NULL;
	Head = (PersonP)malloc(sizeof(Person));
	if (Head == NULL) {
		printf("ERROR! Memory allocation failed");
		return -1;
	}
	
	NewHead = (PersonP)malloc(sizeof(Person));
	if (NewHead == NULL) {
		printf("ERROR! Memory allocation failed");
		return -1;
	}

	FILE* fp = NULL;

	fp = fopen("osobe.txt", "r");
	if (fp == NULL) {
		printf("ERROR! File osobe.txt failed to load");
		return -1;
	}

	InitializeHead(Head);

	//OK ali samo ako ima \n, inače čita "šporki" zadnji red
	int nOsoba = 0;
	while (!feof(fp)) {
		fscanf(fp, "%s %s %d.%d.%d.\n", Surname, Name, &Day, &Month, &Year);
		Id = GenerateId(id_used);
		AddPerson(Head, Name, Surname, Day, Month, Year, Id);
		nOsoba++;
	}
	
	//provjera br. osoba s obzirom na borj jedistvenih ID-ova koje generiramo
	if (nOsoba > MAX_ID - MIN_ID + 1)
	{
		printf("ERROR! Number of persons exceeds possible number of unique IDs!");
		return -1;
	}

	//ako nema \n onda treba dodat if
	/*while (!feof(fp)) {
		fscanf(fp, "%s %s %d.%d.%d.", Surname, Name, &Day, &Month, &Year);
		if (!feof(fp))
			AddPerson(Head, Name, Surname, Day, Month, Year);
	}*/

	//može i ovako - kraće
	/*while (fscanf(fp, "%s %s %d.%d.%d.", Surname, Name, &Day, &Month, &Year)!=EOF) {
		AddPerson(Head, Name, Surname, Day, Month, Year);
	}*/

	
	//test
	/*for (int i = 0; i <= 100; i++){
		int n = RandomNum(MIN_ID, MAX_ID);
		while (id_used[n-100])
			n = RandomNum(MIN_ID, MAX_ID);
		id_used[n-100] = 1;
	}*/


	PrintList(Head->Next);

    CreateListMonthWithMostPeople(Head, NewHead);
    
    printf("\n");
    printf("\nPeople not born in the month with most births\n");
    PrintList(Head->Next);

    printf("\n");    
    printf("\nPeople born in the month with most births\n");
    PrintList(NewHead->Next);

	return 0;
}

int InitializeHead(PersonP Head) {
	strcpy(Head->firstName, "");
	strcpy(Head->lastName, "");
	Head->idNumber = 0;
	Head->Next = NULL;
	//Head->birthDate = NULL;

	return 0;
}

int GenerateId(int id_used[81]){
    int n = (rand() % (MAX_ID - MIN_ID + 1)) + MIN_ID;
    while (id_used[n-100])
        n = (rand() % (MAX_ID - MIN_ID + 1)) + MIN_ID;
    id_used[n-100] = 1;
    
    return n;
}

int AddPerson(PersonP P, char Name[32], char Surname[32], int Day, int Month, int Year, int Id) {
	PersonP Q = NULL;
	Q = (PersonP)malloc(sizeof(Person));

	if (Q == NULL) {
		printf("ERROR! Memory allocation failed");
		return -1;
	}

	PersonP Head = P;

	strcpy(Q->lastName, Surname);
	strcpy(Q->firstName, Name);
	Q->idNumber = Id;
	Q->birthDate.day = Day;
	Q->birthDate.month = Month;
	Q->birthDate.year = Year;

	//loop dok ne dođe do iste godine
	while ((P->Next != NULL) && (P->Next->birthDate.year < Q->birthDate.year)) {
		P = P->Next;
	}

	//ista godina, loop dok ne dođe do istog mjeseca
	while ((P->Next != NULL) && (P->Next->birthDate.year == Q->birthDate.year) && (P->Next->birthDate.month < Q->birthDate.month)) {
		P = P->Next;
	}

	//ista god i mjesec, loop dok ne dođe do istog datuma
	while ((P->Next != NULL) && (P->Next->birthDate.year == Q->birthDate.year) && (P->Next->birthDate.month == Q->birthDate.month) && (P->Next->birthDate.day < Q->birthDate.day)) {
		P = P->Next;
	}

	//ili je našao istog ili je datum manji od P
	Q->Next = P->Next;
	P->Next = Q;

	return 0;
}

/*int AddDate(PersonP P, int Day, int Month, int Year) {
Date BirthDate = NULL;
BirthDate = (Date)malloc(sizeof(Date));
if (BirthDate == NULL) {
printf("ERROR! Memory allocation failed");
return -1;
}

BirthDate.day = Day;
BirthDate.month = Month;
BirthDate.year = Year;

P->birthDate = BirthDate;

return 0;
}*/

int PrintList(PersonP P) {
	if (P == NULL)
		printf("List is empty");
	else {
		while (P != NULL) {
			printf("\n%s %s %d.%d.%d. Id:%d", P->lastName, P->firstName, P->birthDate.day, P->birthDate.month, P->birthDate.year, P->idNumber);

			P = P->Next;
		}
	}
	return 0;
}

int RandomNum(int min, int max)
{
	int n = 0;
	n = max - min + 1;
	int m = RAND_MAX - (RAND_MAX % n);
	int rand_num = rand();

	if (rand_num < 99 && rand_num > 100) {
		while (rand_num > m)
			rand_num = rand();
	}

	return rand_num % n + min;
}

int CreateListMonthWithMostPeople(PersonP P, PersonP Q) {
    int MonthWithMax = 0;
    int MaxNumber = 0;
    int StudentsPerMonth[12] = { 0 };
    int i = 0;
    PersonP R = P;
    PersonP prev = P;
    
    P = P->Next;
    
    while(R->Next != NULL) {
        StudentsPerMonth[R->Next->birthDate.month - 1] += 1;
    
        R = R->Next;
    }
    
    for(i = 0;i < 12; i++) {
        if (MaxNumber < StudentsPerMonth[i]) {
            MaxNumber = StudentsPerMonth[i];
            MonthWithMax = i + 1;
        }
    }
    
    while(P != NULL) {
        if (P->birthDate.month == MonthWithMax) {
            prev->Next = P->Next;
            P->Next = Q->Next;
            Q->Next = P;
            P = prev ->Next;
        }
        else {
            prev = P;
            P = P->Next;
        }
        
    }

    return 0;
}
