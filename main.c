// Library management system
// HEADER FILES
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

// DEFINES
#define MAX_TITLE_LENGTH 60

// GLOBAL VARIABLES


// STRUCTURES

//Structure variables starting with single characters will be used in different functions
/*
//struct tm {                                   //struct tm for reference
//   int tm_sec;    /* Seconds (0-60) */
//    int tm_min;    /* Minutes (0-59) */
//    int tm_hour;   /* Hours (0-23) */
//    int tm_mday;   /* Day of the month (1-31) */
//    int tm_mon;    /* Month (0-11) */
//    int tm_year;   /* Year - 1900 */
//    int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
//    int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
//    int tm_isdst;  /* Daylight saving time */
//};


typedef struct BOOK{
    char b_book_title[MAX_TITLE_LENGTH];
    char b_book_author[30];
    unsigned long b_book_ID; //10 digit ID
    unsigned long b_issue_ID; //10 digit ID
    unsigned long b_user_ID; //10 digit ID
    char b_book_status;             // 'A' - Available, 'I' - issued, 'R' - reserved
    struct tm book_date_of_arrival;    //
    struct tm b_date_issue;            //
} BOOK;

typedef struct USER{
    unsigned long u_user_ID; //10 digit ID
    char user_name[20]; //
    unsigned long u_book_ID; //10 digit ID
    unsigned long u_issue_ID; //10 digit ID
    char u_user_pwd[30]; //User password
    bool u_admin;
    struct tm u_date_issue;    //
} USER;

typedef struct BOOKNODE         // this is used for the function titleCount
{
    BOOK book;
    struct BOOKNODE *next;
} BOOKNODE;


// PROTOTYPES
BOOKNODE * loadLibrary(BOOKNODE *);
void menu();
void welcomeScreen();
void login();
void reg();
void addNewUser(USER *user);
void addNewBook(BOOK *book);
USER * loadNextUser(FILE *, USER*);
BOOK * loadNextBook(FILE *, BOOK*);
int titleCount(char *);


int main(void)
{
  welcomeScreen();

  return 0;
}



void welcomeScreen()
{
  int choice;


  printf("\n\n\n\n\n\t\t\t\tLIBRARY MANAGEMENT SYSTEM");

  printf("\n\t\t\t\t*************************");

  printf("\n\n\n\n\t\t\t\tPress Enter to proceed");

  if(getchar() == 13)
  printf("\n\n\n\n\n\n\n\n\n\n\n\n");





  PQ: // just a label to come back to if we want to re-enter choice

  printf("\n\n\n\t\t\t1. LOGIN\n\t\t\t2. REGISTER");

  printf("\n\n\n\t\t\t\tENTER YOUR CHOICE: ");

  scanf("%d",&choice);

  switch(choice)
    {

        case 1:
        printf("Login function\n");
        // login();
        break;

        case 2:
        printf("Register function\n");
        // reg();
        break;

        default:
        printf("\n\n\t\t\t\tNO MATCH FOUND");
        printf("\n\n\t\t\tPress Enter to re-Enter the choice");


        char ch2 = scanf("%c",&ch2);
        if(ch2 == '\n')
        goto PQ;

    }


}

// Give pointer to head BOOKNODE (likely initialized in main)
// Loads the entire library into memory using a dynamic linked list (utilizes higher reading speed of RAM and reduces file I/O process requirement)
BOOKNODE * loadLibrary(BOOKNODE *head)
{
    FILE *fp = fopen("books.txt", "r");
    BOOK book_load;
    loadNextBook(fp, &book_load);
    BOOKNODE *current;
    head->next = (BOOKNODE *)calloc(1, sizeof(BOOKNODE));
    current = head->next;
    current->next = NULL;
    
    while(1)
    {
        strcpy(current->book.b_book_title, book_load.b_book_title);
        strcpy(current->book.b_book_author, book_load.b_book_author);
        current->book.b_book_ID = book_load.b_book_ID;
        current->book.b_issue_ID = book_load.b_issue_ID;
        current->book.b_user_ID = book_load.b_user_ID;
        current->book.b_book_status = book_load.b_book_status;
        //time struct tm part
        current->book.book_date_of_arrival.tm_sec = book_load.book_date_of_arrival.tm_sec;
        current->book.book_date_of_arrival.tm_min = book_load.book_date_of_arrival.tm_min;
        current->book.book_date_of_arrival.tm_hour = book_load.book_date_of_arrival.tm_hour;
        current->book.book_date_of_arrival.tm_mday = book_load.book_date_of_arrival.tm_mday;
        current->book.book_date_of_arrival.tm_mon = book_load.book_date_of_arrival.tm_mon;
        current->book.book_date_of_arrival.tm_year = book_load.book_date_of_arrival.tm_year;
        current->book.book_date_of_arrival.tm_wday = book_load.book_date_of_arrival.tm_wday;
        current->book.book_date_of_arrival.tm_yday = book_load.book_date_of_arrival.tm_yday;
        current->book.book_date_of_arrival.tm_isdst = book_load.book_date_of_arrival.tm_isdst;
        
        current->book.b_date_issue.tm_sec = book_load.b_date_issue.tm_sec;
        current->book.b_date_issue.tm_min = book_load.b_date_issue.tm_min;
        current->book.b_date_issue.tm_hour = book_load.b_date_issue.tm_hour;
        current->book.b_date_issue.tm_mday = book_load.b_date_issue.tm_mday;
        current->book.b_date_issue.tm_mon = book_load.b_date_issue.tm_mon;
        current->book.b_date_issue.tm_year = book_load.b_date_issue.tm_year;
        current->book.b_date_issue.tm_wday = book_load.b_date_issue.tm_wday;
        current->book.b_date_issue.tm_yday = book_load.b_date_issue.tm_yday;
        current->book.b_date_issue.tm_isdst = book_load.b_date_issue.tm_isdst;
        
        
        loadNextBook(fp, &book_load);
        if(&book_load == NULL)
        {
            break;
        }
        else
        {
            current->next = (BOOKNODE *)calloc(1, sizeof(BOOKNODE));
            current = current->next;
        }
    }
    
    return head;
}

void addNewUser(USER *user)
{
    FILE *fp = fopen("userdata.txt", "a");
    fprintf(fp, "%lu,%s,%lu,%lu,%s,%d,", user->u_user_ID, user->user_name, user->u_book_ID, user->u_issue_ID, user->u_user_pwd, user->u_admin);
    fprintf(fp, "%d,%d,%d,%d,%d,%d,%d,%d,%d,\n", user->u_date_issue.tm_sec, user->u_date_issue.tm_min, user->u_date_issue.tm_hour, 
                                                  user->u_date_issue.tm_mday, user->u_date_issue.tm_mon, user->u_date_issue.tm_year, 
                                                  user->u_date_issue.tm_wday, user->u_date_issue.tm_yday, user->u_date_issue.tm_isdst);
    fclose(fp);
    return;
}

void addNewBook(BOOK *book)
{
    FILE *fp = fopen("books.txt", "a");
    
    fprintf(fp, "%s,%s,%lu,%lu,%lu,%c,", book->b_book_title, book->b_book_author, book->b_book_ID, book->b_issue_ID, 
                                         book->b_user_ID, book->b_book_status);
    fprintf(fp, "%d,%d,%d,%d,%d,%d,%d,%d,%d,", book->book_date_of_arrival.tm_sec, book->book_date_of_arrival.tm_min, 
                                               book->book_date_of_arrival.tm_hour, book->book_date_of_arrival.tm_mday,
                                               book->book_date_of_arrival.tm_mon, book->book_date_of_arrival.tm_year,
                                               book->book_date_of_arrival.tm_wday, book->book_date_of_arrival.tm_yday,
                                               book->book_date_of_arrival.tm_isdst);
    fprintf(fp, "%d,%d,%d,%d,%d,%d,%d,%d,%d,\n", book->b_date_issue.tm_sec, book->b_date_issue.tm_min,
                                                 book->b_date_issue.tm_hour, book->b_date_issue.tm_mday,
                                                 book->b_date_issue.tm_mon, book->b_date_issue.tm_year,
                                                 book->b_date_issue.tm_wday, book->b_date_issue.tm_yday,
                                                 book->b_date_issue.tm_isdst);
    fclose(fp);
    return;
}

// This function takes a file pointer opened in reading mode and takes the next line of the file and stores that data using the given USER struct
// WARNING it's upto the calling function to open a file buffer and use the USER structure
USER * loadNextUser(FILE *fp, USER* user)
{
    // if the end of file has been reached, close the file pointer and return NULL
    if(feof(fp))
    {
        //fclose(fp);      // Not sure if I should close the file pointer here
        user = NULL;
        return NULL;
    }
    
    char buffer[4096] = {};
    fgets(buffer, 4096, fp);
    
    char *pointer = strtok(buffer, ",");
    user->u_user_ID = atol(pointer);
    
    pointer = strtok(NULL, ",");
    strcpy(user->user_name, pointer);
    
    pointer = strtok(NULL, ",");
    user->u_book_ID = atol(pointer);
    
    pointer = strtok(NULL, ",");
    user->u_issue_ID = atol(pointer);
    
    pointer = strtok(NULL, ",");
    strcpy(user->u_user_pwd, pointer);
    
    pointer = strtok(NULL, ",");
    user->u_admin = (bool)atoi(pointer);
    
    // TIME tm struct part
    pointer = strtok(NULL, ",");
    user->u_date_issue.tm_sec = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    user->u_date_issue.tm_min = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    user->u_date_issue.tm_hour = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    user->u_date_issue.tm_mday = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    user->u_date_issue.tm_mon = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    user->u_date_issue.tm_year = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    user->u_date_issue.tm_wday = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    user->u_date_issue.tm_yday = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    user->u_date_issue.tm_isdst = atoi(pointer);
    
    return user; 
}

BOOK * loadNextBook(FILE *fp, BOOK *book)
{
    // if the end of file has been reached, close the file pointer and return NULL
    if(feof(fp))
    {
        //fclose(fp);      // Not sure if I should close the file pointer here
        book = NULL;
        return NULL;
    }
    
    char buffer[4096] = {};
    fgets(buffer, 4096, fp);
    
    char *pointer = strtok(buffer, ",");
    strcpy(book->b_book_title, pointer);
    
    pointer = strtok(NULL, ",");
    strcpy(book->b_book_author, pointer);
    
    pointer = strtok(NULL, ",");
    book->b_book_ID = atol(pointer);
    
    pointer = strtok(NULL, ",");
    book->b_issue_ID = atol(pointer);
    
    pointer = strtok(NULL, ",");
    book->b_user_ID = atol(pointer);
    
    pointer = strtok(NULL, ",");
    book->b_book_status = pointer[0];
    
    //time tm struct part for date_of_arrival
    pointer = strtok(NULL, ",");
    book->book_date_of_arrival.tm_sec = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->book_date_of_arrival.tm_min = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->book_date_of_arrival.tm_hour = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->book_date_of_arrival.tm_mday = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->book_date_of_arrival.tm_mon = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->book_date_of_arrival.tm_year = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->book_date_of_arrival.tm_wday = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->book_date_of_arrival.tm_yday = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->book_date_of_arrival.tm_isdst = atoi(pointer);
    
    //time tm struct part for date_of_arrival
    pointer = strtok(NULL, ",");
    book->b_date_issue.tm_sec = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->b_date_issue.tm_min = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->b_date_issue.tm_hour = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->b_date_issue.tm_mday = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->b_date_issue.tm_mon = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->b_date_issue.tm_year = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->b_date_issue.tm_wday = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->b_date_issue.tm_yday = atoi(pointer);
    
    pointer = strtok(NULL, ",");
    book->b_date_issue.tm_isdst = atoi(pointer);
    
    return book;
}

int titleCount(char *title)
{
    FILE *fp = fopen("books.txt", "r");
    BOOK *book;
    book = loadNextBook(fp, book);
    int count = 0;
    
    for(; book != NULL; book = loadNextBook(fp, book))
    {
        if(strcmp(book->b_book_title, title) == 0)
        {
            count++;
        }
    }
    
    return count;
    
    /*
    BOOKNODE *head = NULL, *current = NULL;
    head->next = malloc(sizeof(BOOKNODE));
    current = head->next;
    
    
    // Loads the library into memory
    while(!feof(fp))
    {
        
    }

    if(book = NULL)
    {
        printf("--------------------NO BOOKS TO DISPLAY--------------------\n");
    }
    printf("--------------------------------------------------\n");
    printf("BOOK TITLE\t\t\t\tCOUNT\n");
    while(book != NULL)
    {
        
        book = loadNextBook(fp, book);
    }
    printf("--------------------------------------------------\n");
    
    
    return;
    */
}