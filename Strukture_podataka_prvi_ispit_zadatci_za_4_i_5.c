#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _student;
struct _subject;
typedef struct _student *StudentP;
typedef struct _subject *SubjectP;

typedef struct _student {
    char firstName[32];
    char lastName[32];
    int idNumber;
    StudentP Next;
    SubjectP Child;
} Student;

typedef struct _subject {
    char subjectName[32];
    int grade;
    SubjectP Next;
} Subject;

int InitializeHead(StudentP Head);
int AddStudent(StudentP P, char Name[32], char Surname[32], int ID);
int PrintList(StudentP P);
int AddGrades(StudentP P, int ID, int Grade, char SubjectName[32]);
int CreateListPassedAllThree(StudentP P, StudentP Q);

int main()
{
	char Name[32];
	char Surname[32];
	char SubjectName[32];
	strcpy(Name, "");
	strcpy(Surname, "");
	strcpy(SubjectName, "");
	int ID = 0;
	int Grade = 0;
	StudentP Head = NULL;
	StudentP NewHead = NULL;

	Head = (StudentP)malloc(sizeof(Student));
	if (Head == NULL) {
		printf("ERROR! Memory allocation failed.");
		return -1;
	}

	NewHead = (StudentP)malloc(sizeof(Student));
	if (NewHead == NULL) {
		printf("ERROR! Memory allocation failed.");
		return -1;
	}

	FILE* fp1 = NULL;
	fp1 = fopen("studenti.txt", "r");
	if (fp1 == NULL) {
		printf("ERROR! File studenti.txt failed to load.");
		return -1;
	}

	FILE* fp2 = NULL;
	fp2 = fopen("predmet1.txt", "r");
	if (fp2 == NULL) {
		printf("ERROR! File predmet1.txt failed to load.");
		return -1;
	}

	FILE* fp3 = NULL;
	fp3 = fopen("predmet2.txt", "r");
	if (fp3 == NULL) {
		printf("ERROR! File predmet2.txt failed to load.");
		return -1;
	}
	
	FILE* fp4 = NULL;
	fp4 = fopen("predmet3.txt", "r");
	if (fp4 == NULL) {
		printf("ERROR! File predmet3.txt failed to load.");
		return -1;
	}

    InitializeHead(Head);

    while(!feof(fp1)) {
        fscanf(fp1, "%s %s %d", Surname, Name, &ID);
        
        AddStudent(Head, Name, Surname, ID);
    }

    fclose(fp1);

    fscanf(fp2, "%s", SubjectName);

    while(!feof(fp2)) {
        fscanf(fp2, "%d %d", &ID, &Grade);
        
        AddGrades(Head, ID, Grade, SubjectName);
    }

    fclose(fp2);

    fscanf(fp3, "%s", SubjectName);

    while(!feof(fp3)) {
        fscanf(fp3, "%d %d", &ID, &Grade);
        
        AddGrades(Head, ID, Grade, SubjectName);
    }

    fclose(fp3);

    fscanf(fp4, "%s", SubjectName);

    while(!feof(fp4)) {
        fscanf(fp4, "%d %d", &ID, &Grade);
        
        AddGrades(Head, ID, Grade, SubjectName);
    }

    fclose(fp4);

    PrintList(Head->Next);
    
    CreateListPassedAllThree(Head, NewHead);
    
    printf("\nList of those who didnt pass\n");
    PrintList(Head->Next);
    
    printf("\nList of those who did pass\n");
    PrintList(NewHead->Next);

    return 0;
}

int InitializeHead(StudentP Head) {
    strcpy(Head->firstName, "");
    strcpy(Head->lastName, "");
    Head->idNumber = 0;
    Head->Next = NULL;
    Head->Child = NULL;
}

int AddStudent(StudentP P, char Name[32], char Surname[32], int ID) {
	StudentP Q;

	Q = (StudentP)malloc(sizeof(Student));
	if (Q == NULL) {
		printf("ERROR! Memory allocation failed.");
		return -1;
	}

	strcpy(Q->firstName, Name);
	strcpy(Q->lastName, Surname);
	Q->idNumber = ID;

	while (P->Next != NULL && ((strcmp(P->Next->lastName, Q->lastName) < 0) || (strcmp(P->Next->lastName, Q->lastName) == 0 && strcmp(P->Next->firstName, Q->firstName) < 0))) {
		P = P->Next;
	}
	
	Q->Next = P->Next;
	P->Next = Q;

	return 0;
}

int PrintList(StudentP P) {
    SubjectP Q; 
    
    if (P == NULL)
        printf("The list is empty.");
    
    else
    {
        while(P != NULL) {
            printf("%s %s id:%d \n", P->lastName, P->firstName, P->idNumber);
            Q = P->Child;
            while(Q != NULL) {
                printf("%s:%d\n", Q->subjectName, Q->grade);
                
                Q = Q->Next;
                
            }
            printf("\n");         
            P = P->Next;
        }
    }
    
    return 0;
}

int AddGrades(StudentP P, int ID, int Grade, char SubjectName[32]) {
    SubjectP Q = NULL;
    Q = (SubjectP)malloc(sizeof(Subject));
    
    strcpy(Q->subjectName, SubjectName);
    Q->grade = Grade;
    
    if (Q == NULL) {
        printf("ERROR! Memory allocation failed.");
        return -1;
    }
    
    while(P->Next != NULL && P->idNumber != ID)
        P = P->Next;
        
    if (P->idNumber == ID) {
        Q->Next = P->Child;
        P->Child = Q;
    }
    return 0;
}

int CreateListPassedAllThree(StudentP P, StudentP Q) {
    StudentP prev;
    SubjectP R;
    prev = P;
    P = P->Next;
    
    while(P->Next != NULL) {
        R = P->Child;
        if((R != NULL) && (R->grade > 1)){
            R = R->Next;
            if((R != NULL) && (R->grade > 1)){
                R = R->Next;
                if((R != NULL) && (R->grade > 1)) {
                    prev->Next = P->Next;
                    P->Next = Q->Next;
                    Q->Next = P;
                    P = prev->Next;
                }
                else {
                    prev = P;
                    P = P->Next;
                }
            }
            else {
                prev = P;
                P = P->Next;
            }
        }
        else {
            prev = P;
            P = P->Next;
        }
    }

    return 0;   
}