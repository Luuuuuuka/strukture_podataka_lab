#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
int AddPerson(PersonP P, char Name[32], char Surname[32], int Day, int Month, int Year);
//int AddDate(PersonP P, int Day, int Month, int Year);
int PrintList(PersonP P);

int main()
{
    char Name[32];
    char Surname[32];
    int Day = 0;
    int Month = 0;
    int Year = 0;
    PersonP Head = NULL;
    Head = (PersonP)malloc(sizeof(Person));
    if (Head == NULL) {
        printf("ERROR! Memory allocation failed");
        return -1;
    }    
    
    FILE* fp = NULL;
    
    fp = fopen("osobe.txt", "r");
    if (fp == NULL) {
        printf("ERROR! File failed to load");
        return -1;
    }
    
    InitializeHead(Head);
    
    while(!feof(fp)) {
        fscanf(fp, "%s %s %d.%d.%d.", Surname, Name, &Day, &Month, &Year);
        
        AddPerson(Head, Name, Surname, Day, Month, Year);
    }
    
    PrintList(Head->Next);

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

int AddPerson(PersonP P, char Name[32], char Surname[32], int Day, int Month, int Year) {
    PersonP Q = NULL;
    Q = (PersonP)malloc(sizeof(Person));

    if (Q == NULL) {
        printf("ERROR! Memory allocation failed");
        return -1;
    }
    
    strcpy(Q->lastName, Surname);
    strcpy(Q->firstName, Name);
    Q->idNumber = 0;
    Q->birthDate.day = Day;
    Q->birthDate.month = Month;
    Q->birthDate.year = Year;
    
    //AddDate(Q, Day, Month, Year);
    
    while((P->Next != NULL) && (P->birthDate.year < Q->birthDate.year)) {
        P = P->Next;
    }
    while((P->birthDate.year == Q->birthDate.year) && (P->birthDate.month < Q->birthDate.month)) {
        P = P->Next;
    }
    while((P->birthDate.year == Q->birthDate.year) && (P->birthDate.month == Q->birthDate.month) && (P->birthDate.day < Q->birthDate.day)) {
        P = P->Next;
    }
    
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
        while(P != NULL) {
            printf("\n%s %s %d.%d.%d.", P->lastName, P->firstName, P->birthDate.day, P->birthDate.month, P->birthDate.year);
            
            P = P->Next;
        }
    }
    return 0;
}
