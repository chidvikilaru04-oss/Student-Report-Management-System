#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[10];
char currentUser[50];

// Function prototypes
int loginsystem(void);
void mainMenu(void);
void adminMenu(void);
void userMenu(void);
void staffMenu(void);
void guestMenu(void);
void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);

int main(){
    if(loginsystem()){
        mainMenu();
    } else {
        printf("\n Access Denied...\n");
    }
    return 0;
}

/*--------------------------LOGINSYSTEM----------------------*/
int loginsystem(){
    char username[50], password[50];
    char fileUsername[50], filePassword[50], fileRole[10];

    printf("=====Login =====\n");
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Error: credentials.txt not found!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fileUsername, filePassword, fileRole) == 3) {
        if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
            strcpy(currentRole, fileRole);
            strcpy(currentUser, fileUsername);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);

    printf("Invalid credentials!\n");
    return 0;
}

/*--------------------------MAIN MENU----------------------*/
void mainMenu(){
    if(strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if(strcmp(currentRole, "USER") == 0)
        userMenu();
    else if(strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else
        guestMenu();
}

/*--------------------------ADMIN MENU----------------------*/
void adminMenu(){
    int choice;
    while(1){
        printf("\n===== Admin Menu =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice){
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
    }
}

/*--------------------------ADD STUDENT----------------------*/
void addStudent(){
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "a");

    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Roll Number: ");
    scanf("%d", &s.roll);
    getchar();

    printf("Enter Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0;

    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);

    printf("Student added successfully!\n");
}

/*--------------------------DISPLAY STUDENTS----------------------*/
void displayStudents(){
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student s;

    if (!fp) {
        printf("No students found!\n");
        return;
    }

    printf("\n===== Student List =====\n");
    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        printf("Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
}

/*--------------------------SEARCH STUDENT----------------------*/
void searchStudent(){
    int roll, found = 0;

    printf("\nEnter Roll Number to search: ");
    scanf("%d", &roll);

    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student s;

    if (!fp) {
        printf("File not found!\n");
        return;
    }

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            printf("\nStudent Found:\n");
            printf("Roll: %d\nName: %s\nMarks: %.2f\n", s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student not found!\n");

    fclose(fp);
}

/*--------------------------UPDATE STUDENT----------------------*/
void updateStudent(){
    int roll, found = 0;

    printf("\nEnter Roll Number to update: ");
    scanf("%d", &roll);
    getchar();

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    struct Student s;

    if (!fp) {
        printf("File not found!\n");
        return;
    }

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            found = 1;
            printf("Enter New Name: ");
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = 0;

            printf("Enter New Marks: ");
            scanf("%f", &s.marks);
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Student updated successfully!\n");
    else
        printf("Student not found!\n");
}

/*--------------------------DELETE STUDENT----------------------*/
void deleteStudent(){
    int roll, found = 0;

    printf("\nEnter Roll Number to delete: ");
    scanf("%d", &roll);

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    struct Student s;

    if (!fp) {
        printf("File not found!\n");
        return;
    }

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll != roll)
            fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Student deleted successfully!\n");
    else
        printf("Student not found!\n");
}

/*--------------------------OTHER MENUS----------------------*/
void userMenu(){ printf("\n=== User Menu ===\nUser functionality pending.\n"); }
void staffMenu(){ printf("\n=== Staff Menu ===\nStaff functionality pending.\n"); }
void guestMenu(){ printf("\n=== Guest Menu ===\nGuest functionality pending.\n"); }