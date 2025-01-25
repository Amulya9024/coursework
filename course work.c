#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store student data
typedef struct {
    int roll_number;
    char name[50];
    float marks[5];
} Student;

// Function prototypes
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void menu();

// File name for storing student records
const char *FILENAME = "students.dat";

int main() {
    menu();
    return 0;
}

// Function to display the menu
void menu() {
    int choice;
    do {
        printf("\n--- Student Data Management System ---\n");
        printf("1. Add Student Record\n");
        printf("2. Display All Records\n");
        printf("3. Search Student Record\n");
        printf("4. Update Student Record\n");
        printf("5. Delete Student Record\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
}

// Function to add a new student record
void addStudent() {
    FILE *file = fopen(FILENAME, "ab");
    if (!file) {
        printf("Error: Unable to open file.\n");
        return;
    }

    Student student;
    printf("\nEnter Roll Number: ");
    scanf("%d", &student.roll_number);
    printf("Enter Name: ");
    getchar(); // Clear newline character
    fgets(student.name, sizeof(student.name), stdin);
    student.name[strcspn(student.name, "\n")] = '\0'; // Remove newline
    printf("Enter Marks for 5 Subjects: ");
    for (int i = 0; i < 5; i++) {
        scanf("%f", &student.marks[i]);
    }

    fwrite(&student, sizeof(Student), 1, file);
    fclose(file);
    printf("Student record added successfully.\n");
}

// Function to display all student records
void displayStudents() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("No records found.\n");
        return;
    }

    Student student;
    printf("\n--- All Student Records ---\n");
    printf("Roll Number\tName\t\tMarks\n");
    printf("-------------------------------------------------\n");
    while (fread(&student, sizeof(Student), 1, file)) {
        printf("%d\t\t%s\t", student.roll_number, student.name);
        for (int i = 0; i < 5; i++) {
            printf("%.2f ", student.marks[i]);
        }
        printf("\n");
    }
    fclose(file);
}

// Function to search for a student record
void searchStudent() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("No records found.\n");
        return;
    }

    int roll_number;
    printf("\nEnter Roll Number to Search: ");
    scanf("%d", &roll_number);

    Student student;
    int found = 0;
    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.roll_number == roll_number) {
            printf("\nRecord Found:\n");
            printf("Roll Number: %d\n", student.roll_number);
            printf("Name: %s\n", student.name);
            printf("Marks: ");
            for (int i = 0; i < 5; i++) {
                printf("%.2f ", student.marks[i]);
            }
            printf("\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("No record found for Roll Number %d.\n", roll_number);
    }
    fclose(file);
}

// Function to update a student record
void updateStudent() {
    FILE *file = fopen(FILENAME, "rb+");
    if (!file) {
        printf("No records found.\n");
        return;
    }

    int roll_number;
    printf("\nEnter Roll Number to Update: ");
    scanf("%d", &roll_number);

    Student student;
    int found = 0;
    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.roll_number == roll_number) {
            printf("Enter New Name: ");
            getchar(); // Clear newline character
            fgets(student.name, sizeof(student.name), stdin);
            student.name[strcspn(student.name, "\n")] = '\0'; // Remove newline
            printf("Enter New Marks for 5 Subjects: ");
            for (int i = 0; i < 5; i++) {
                scanf("%f", &student.marks[i]);
            }

            fseek(file, -sizeof(Student), SEEK_CUR);
            fwrite(&student, sizeof(Student), 1, file);
            printf("Record updated successfully.\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("No record found for Roll Number %d.\n", roll_number);
    }
    fclose(file);
}

// Function to delete a student record
void deleteStudent() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("No records found.\n");
        return;
    }

    int roll_number;
    printf("\nEnter Roll Number to Delete: ");
    scanf("%d", &roll_number);

    FILE *tempFile = fopen("temp.dat", "wb");
    if (!tempFile) {
        printf("Error: Unable to open temporary file.\n");
        fclose(file);
        return;
    }

    Student student;
    int found = 0;
    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.roll_number == roll_number) {
            found = 1;
        } else {
            fwrite(&student, sizeof(Student), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(FILENAME);
    rename("temp.dat", FILENAME);

    if (found) {
        printf("Record deleted successfully.\n");
    } else {
        printf("No record found for Roll Number %d.\n", roll_number);
    }
}
