// Library management system
// HEADER FILES
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

// DEFINES

// GLOBAL VARIABLES

// STRUCTURES

//Structure variables starting with single characters will be used in different functions
/*
//struct tm {
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


typedef struct{
    char b_book_title[30];
    char b_book_author[30];
    int b_book_ID; //10 digit ID
    int b_issue_ID; //10 digit ID
    int b_user_ID; //10 digit ID
    char b_book_status;             // 'A' - Available, 'I' - issued, 'R' - reserved
    struct tm book_date_of_arrival;    //
    struct tm b_date_issue;            //
} BOOK;

typedef struct{
    int u_user_ID; //10 digit ID
    char user_name; //
    int u_book_ID; //10 digit ID
    int u_issue_ID; //10 digit ID
    char u_user_pwd[30]; //User password
    bool admin;
    struct tm u_date_issue;    //
} USER;


// PROTOTYPES
void menu();
void welcomeScreen();
void addUser(USER *);
void login();
void reg();


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

void addUser(USER *user)
{
    FILE *fp = fopen("userdata.txt", "a");
    fprintf(fp, "%d,%c,%d,%d,%s,%d,", user->u_user_ID, user->user_name, user->u_book_ID, user->u_issue_ID, user->u_user_pwd, user->admin);
    fprintf(fp, "%d,%d,%d,%d,%d,%d,%d,%d,%d,\n", user->u_date_issue.tm_sec, user->u_date_issue.tm_min, user->u_date_issue.tm_hour, user->u_date_issue.tm_mday, user->u_date_issue.tm_mon, user->u_date_issue.tm_year, user->u_date_issue.tm_wday, user->u_date_issue.tm_yday, user->u_date_issue.tm_isdst);
    return;
}