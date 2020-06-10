// Library management system
// HEADER FILES
#include <stdio.h>
#include <bool.h>
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
void addUser(USER *);


int main(void)
{

	return 0;
}
