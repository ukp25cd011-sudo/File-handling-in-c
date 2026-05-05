# File-handling-in-c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "students.dat"
#define MAX_STUDENTS 100


typedef struct {
    int rollNumber;
    char name[50];
    float marks[3];
} Student;


void addStudent();
void displayStudents();
void generateRankList();

int main() {
    int choice;

    while (1) {
        printf("\n--- Student Record Management ---\n");
        printf("1. Add student record\n");
        printf("2. Display all student records\n");
        printf("3. Generate rank list of students based on total marks\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting...\n");
            break;
        }

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                generateRankList();
                break;
            case 4:
                printf("Exiting program.\n");
                exit(0);
            default:
                printf("Invalid choice! Please select an option from 1 to 4.\n");
        }
    }
    return 0;
}


void addStudent() {
    FILE *fp = fopen(FILE_NAME, "ab");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    Student s;
    
    printf("Enter Roll Number: ");
    scanf("%d", &s.rollNumber);
    
    printf("Enter Name: ");
    scanf(" %[^\n]", s.name); 
    
    printf("Enter Marks for Subject 1: ");
    scanf("%f", &s.marks[0]);
    
    printf("Enter Marks for Subject 2: ");
    scanf("%f", &s.marks[1]);
    
    printf("Enter Marks for Subject 3: ");
    scanf("%f", &s.marks[2]);

   
    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);
    
    printf("Student record added successfully!\n");
}


void displayStudents() {
    FILE *fp = fopen(FILE_NAME, "rb"); // Open in read-binary mode
    if (fp == NULL) {
        printf("No records found. Please add some students first.\n");
        return;
    }

    Student s;
    
    printf("\n--- All Student Records ---\n");
    printf("%-10s %-20s %-10s %-10s %-10s\n", "Roll No", "Name", "Sub 1", "Sub 2", "Sub 3");
    printf("----------------------------------------------------------------\n");
    
    
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        printf("%-10d %-20s %-10.2f %-10.2f %-10.2f\n", 
               s.rollNumber, s.name, s.marks[0], s.marks[1], s.marks[2]);
    }
    
    fclose(fp);
}


void generateRankList() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf("No records found. Please add some students first.\n");
        return;
    }

    Student students[MAX_STUDENTS];
    int count = 0;


    while (fread(&students[count], sizeof(Student), 1, fp) == 1) {
        count++;
        if (count >= MAX_STUDENTS) {
            printf("Maximum student limit reached for sorting.\n");
            break;
        }
    }
    fclose(fp);

    if (count == 0) {
        printf("No records to display.\n");
        return;
    }

    
    float totals[MAX_STUDENTS];
    for (int i = 0; i < count; i++) {
        totals[i] = students[i].marks[0] + students[i].marks[1] + students[i].marks[2];
    }

   
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (totals[j] < totals[j+1]) {
               
                float tempTotal = totals[j];
                totals[j] = totals[j+1];
                totals[j+1] = tempTotal;
                
               
                Student tempStudent = students[j];
                students[j] = students[j+1];
                students[j+1] = tempStudent;
            }
        }
    }

   
    printf("\n--- Rank List (Descending Order by Total Marks) ---\n");
    printf("%-5s %-10s %-20s %-15s\n", "Rank", "Roll No", "Name", "Total Marks");
    printf("----------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-5d %-10d %-20s %-15.2f\n", i + 1, students[i].rollNumber, students[i].name, totals[i]);
    }
}
