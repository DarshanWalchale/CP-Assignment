// Library management system
// HEADER FILES
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>


// DEFINES

// GLOBAL VARIABLES

// STRUCTURES

//Structure variables starting with single characters will be used in different functions
typedef struct{
    char b_book_title[40];
    char b_book_author[30];
    int b_book_ID; //10 digit ID
    int b_issue_ID; //10 digit ID
    time_t book_date_of_arrival;    //
    int b_user_ID; //10 digit ID
    time_t b_date_issue;            //
    bool b_available;               //true means available
} BOOK;

typedef struct{
    int u_user_ID; //10 digit ID
    char user_name; //
    int u_book_ID; //10 digit ID
    int u_issue_ID; //10 digit ID
    time_t u_date_issue;    //
    char u_user_pwd[30]; //User password
    bool admin;
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