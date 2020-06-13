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
#define MAX_CLOSE_TITLE_SEARCH 30
#define MAX_CLOSE_AUTH_SEARCH 20 //Maximum number of closest seraches of authors to display
#define MAX_SAME_AUTHOR 100 //Maximum number of books of same author to display

// GLOBAL VARIABLES
unsigned long Book_ID_Counter = 394;

// STRUCTURES

//Structure variables starting with single characters will be used in different functions
/*
//struct tm {                                   //struct tm FOR REFFERENCE
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
void makeFile();
unsigned long generateBookID();
void searchBookbyTitle();
void displayAllBooks(BOOKNODE *head);
void searchBookbyAuthor(BOOKNODE *head);

//--------------------------------------------------------------------------------------------------------------

int main(void)
{
    //printf("sizeof(BOOKNODE) = %lu\nsizeof(USER) = %lu\nsizeof(int) = %lu\n", sizeof(BOOKNODE), sizeof(USER), sizeof(int));
    //makeFile();
    welcomeScreen();

    return 0;
}


void makeFile()
{
    BOOK book;
    time_t sec = 0.9 * time(NULL);
    printf("sec = %lu \n", sec);
    printf("current time: %s\n", ctime(&sec));
    struct tm time_of_event = *(localtime(&sec));
    char choice;
    do
    {
        //printf("DO\n");
        printf("Enter Book Title:\n->");
        scanf(" %60[^\n]", book.b_book_title); //MAX_TITLE_LENGTH
        while(getchar() != '\n');

        printf("Enter Book Author\n->");
        scanf(" %30[^\n]", book.b_book_author);
        while(getchar() != '\n');

        book.b_book_ID = generateBookID();
        book.b_issue_ID = 0;
        book.b_user_ID = 0;
        book.b_book_status = 'A';

        sec = 0.9 * time(NULL);
        printf("sec = %lu \n", sec);
        printf("current time: %s\n", ctime(&sec));
        time_of_event = *(localtime(&sec));

        //date of arrival struct tm
        book.book_date_of_arrival.tm_sec = time_of_event.tm_sec;
        book.book_date_of_arrival.tm_min = time_of_event.tm_min;
        book.book_date_of_arrival.tm_hour = time_of_event.tm_hour;
        book.book_date_of_arrival.tm_mday = time_of_event.tm_mday;
        book.book_date_of_arrival.tm_mon = time_of_event.tm_mon;
        book.book_date_of_arrival.tm_year = time_of_event.tm_year;
        book.book_date_of_arrival.tm_wday = time_of_event.tm_wday;
        book.book_date_of_arrival.tm_yday = time_of_event.tm_yday;
        book.book_date_of_arrival.tm_isdst = time_of_event.tm_isdst;
        //date_issue struct tm
        book.b_date_issue.tm_sec = 0;
        book.b_date_issue.tm_sec = 0;
        book.b_date_issue.tm_sec = 0;
        book.b_date_issue.tm_sec = 0;
        book.b_date_issue.tm_sec = 0;
        book.b_date_issue.tm_sec = 0;
        book.b_date_issue.tm_sec = 0;
        book.b_date_issue.tm_sec = 0;
        book.b_date_issue.tm_sec = 0;


        addNewBook(&book);

        printf("again?(y/n): ");
        scanf(" %c", &choice);
        while(getchar() != '\n');
        if(choice == 'n')
        {
            break;
        }
        if(choice == 'N')
        {
            break;
        }

    }while(1);

    return;
}

unsigned long generateBookID()
{
    Book_ID_Counter++;
    return (Book_ID_Counter - 1);
}


void welcomeScreen()
{
  int choice;


  printf("\n\n\n\n\n\t\t\t\tLIBRARY MANAGEMENT SYSTEM");

  printf("\n\t\t\t\t*************************");

  printf("\n\n\n\n\t\t\t\tPress Enter to proceed");

  while(getchar() != '\n'); // Won't proceed till \n entered, wont leave remanents in input buffer
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
    BOOK *book_load = calloc(1, sizeof(BOOK));
    loadNextBook(fp, book_load);
    BOOKNODE *current;
    head->next = (BOOKNODE *)calloc(1, sizeof(BOOKNODE));
    current = head->next;
    current->next = NULL;

    while(1)
    {
        strcpy(current->book.b_book_title, book_load->b_book_title);
        strcpy(current->book.b_book_author, book_load->b_book_author);
        current->book.b_book_ID = book_load->b_book_ID;
        current->book.b_issue_ID = book_load->b_issue_ID;
        current->book.b_user_ID = book_load->b_user_ID;
        current->book.b_book_status = book_load->b_book_status;
        //time struct tm part
        current->book.book_date_of_arrival.tm_sec = book_load->book_date_of_arrival.tm_sec;
        current->book.book_date_of_arrival.tm_min = book_load->book_date_of_arrival.tm_min;
        current->book.book_date_of_arrival.tm_hour = book_load->book_date_of_arrival.tm_hour;
        current->book.book_date_of_arrival.tm_mday = book_load->book_date_of_arrival.tm_mday;
        current->book.book_date_of_arrival.tm_mon = book_load->book_date_of_arrival.tm_mon;
        current->book.book_date_of_arrival.tm_year = book_load->book_date_of_arrival.tm_year;
        current->book.book_date_of_arrival.tm_wday = book_load->book_date_of_arrival.tm_wday;
        current->book.book_date_of_arrival.tm_yday = book_load->book_date_of_arrival.tm_yday;
        current->book.book_date_of_arrival.tm_isdst = book_load->book_date_of_arrival.tm_isdst;

        current->book.b_date_issue.tm_sec = book_load->b_date_issue.tm_sec;
        current->book.b_date_issue.tm_min = book_load->b_date_issue.tm_min;
        current->book.b_date_issue.tm_hour = book_load->b_date_issue.tm_hour;
        current->book.b_date_issue.tm_mday = book_load->b_date_issue.tm_mday;
        current->book.b_date_issue.tm_mon = book_load->b_date_issue.tm_mon;
        current->book.b_date_issue.tm_year = book_load->b_date_issue.tm_year;
        current->book.b_date_issue.tm_wday = book_load->b_date_issue.tm_wday;
        current->book.b_date_issue.tm_yday = book_load->b_date_issue.tm_yday;
        current->book.b_date_issue.tm_isdst = book_load->b_date_issue.tm_isdst;


        loadNextBook(fp, book_load);
        if(book_load == NULL)
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

/*
void addNewBook(BOOK *book)
{
    FILE *fp = fopen("books.txt", "a");

    //fwrite(book, sizeof(BOOK), 1, fp);

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
*/

int titleCount(char *title)
{
    FILE *fp = fopen("books.txt", "r");
    BOOK *book = calloc(1, sizeof(BOOK));
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


void searchBookbyTitle(BOOKNODE *head){
    BOOKNODE *current = head;
    char title_search[MAX_TITLE_LENGTH];
    short cs_count=0; //To count number of closest searches
    short flag=0; //To end searching on finding the exact book or when no results
    char closest_search[MAX_CLOSE_TITLE_SEARCH][MAX_TITLE_LENGTH+30]; //To store MAX_CLOSE_TITLE_SEARCH closest matching title searches
    LAB1: //a label to return if <=3 chars entered
    printf("Search by title: ");
    fgets(title_search,MAX_TITLE_LENGTH,stdin);

    //Restart search if less than three characters entered
    if(strlen(title_search)<4){
        printf("Enter at least three characters to search");
        goto LAB1;
    }

    while(flag==0&&current->next != NULL){
        current=current->next;
        //Iterating through all characters of a single book title
        for(int j=0;j<MAX_TITLE_LENGTH;j++){

            //Skip the current book if atleast 3 characters don't match
        if((tolower(title_search[j])!=tolower(current->book.b_book_title[j]))&&j<3){
            //printf("\n First 3 not matching"); For ref only, to be removed before submission
            break;
            }

            //If atleast one character does not match,Store the closest matching search (book) and skip current iteration
        else if(tolower(title_search[j])!=tolower(current->book.b_book_title[j])&&j<strlen(current->book.b_book_title)&&j>2){
                //printf("\n Close"); For ref only, to be removed before submission
            if(cs_count<MAX_CLOSE_TITLE_SEARCH){
                strcpy(closest_search[cs_count],current->book.b_book_title);
                char curr_ID[11];
                sprintf(curr_ID, "%ld", current->book.b_book_ID); //Appending ID to display
                strcat(closest_search[cs_count],"\n\tID: ");
               strcat(closest_search[cs_count],curr_ID);
                strcat(closest_search[cs_count],"\n\tStatus: ");
                    if(current->book.b_book_status=='A')
                        strcat(closest_search[cs_count], "Available");
                    else if(current->book.b_book_status=='R')
                        strcat(closest_search[cs_count], "Reserved");
                    else if(current->book.b_book_status=='I')
                        strcat(closest_search[cs_count], "Not available (Issued");
                    else
                        strcat(closest_search[cs_count], "Undefined");

                cs_count++;
            }
            break;
            }

            //Continue if some (not all) characters match
        else if(tolower(title_search[j])==tolower(current->book.b_book_title[j])&&j<strlen(current->book.b_book_title)){
             //printf("\n Almost"); For ref only, to be removed before submission
                continue;
        }
                //If the search matches exactly, stop searching
        else{
                //printf("\n Found");
                printf("Book found: %s ",current->book.b_book_title);
                if(current->book.b_book_status=='A')
                printf("(Available)\n");
                else if(current->book.b_book_status=='R')
                printf("(Reserved)\n");
                else if(current->book.b_book_status=='I')
                printf("(Not available (Issued))\n");
                else
                printf("Undefined)\n"); //To account for faulty/incomplete entry of status of book

                printf("\tAuthor: %s\n",current->book.b_book_author);
                printf("\tBook ID: %ld\n",current->book.b_book_ID);
                //if EOF reached, flag=1 (TO BE ADDED)
                flag=3;
                break;
                }
            }

        }

        if(flag==0&&cs_count==0)
        printf("No results found");

        //To print all closest searches (upto MAX_CLOSE_TITLE_SEARCH) when book is not found
        if(cs_count>0&&flag!=3){
        cs_count=0;
        printf("Closest searches:\n");
        while(strlen(closest_search[cs_count])!=0){
            printf("%hi. %s",cs_count+1,closest_search[cs_count]);
            cs_count++;
            if(cs_count=MAX_CLOSE_TITLE_SEARCH)
            break; //Stop on reaching MAX_CLOSE_TITLE_SEARCH closest search
        }
        }

}

void displayAllBooks(BOOKNODE *head)
{
    BOOKNODE *current = head;
    if(head->next == NULL)
    {
        printf("--------------------NO BOOKS TO DISPLAY--------------------");
    }

    printf("------------------------------------------------------------\n");
    printf("TITLE\t\t\t\tAUTHOR\t\t\t\tAVAILABILITY\n");
    char availability[15] = {};

    while(current->next != NULL)
    {
        current = current->next;
        if(current->book.b_book_status == 'A')
        {
            strcpy(availability, "Available");
        }
        else if(current->book.b_book_status == 'I')
        {
            strcpy(availability, "Already Issued");
        }
        else if(current->book.b_book_status == 'R')
        {
            strcpy(availability, "Reserved");
        }
        else
        {
            strcpy(availability, "Unknown");
        }
        printf("%s\t\t\t\t%s\t\t\t\t%s", current->book.b_book_title, current->book.b_book_author, availability);
    }
    printf("------------------------------------------------------------\n");

    return;
}



void searchBookbyAuthor(BOOKNODE *head){
    BOOKNODE *current = head;
    char author_search[30];
    short cs_count=0; //To count number of closest searches
    int same_auth_count=0; //To count number of books of same author
    short flag=0; //To end searching on finding the exact search or when no results
    char closest_search[MAX_CLOSE_AUTH_SEARCH][30]; //To store MAX_CLOSE_AUTH_SEARCH closest matching searches of authors' names
    char all_same_auth[MAX_SAME_AUTHOR][80]; //To store list of all books of same author
    char auth_found[30]; //Author (matched with search)

    LAB2: //a label to return if <=3 chars entered

    printf("Search by author: ");
    fgets(author_search,30,stdin);

        //Restart search if less than three characters entered
     if(strlen(author_search)<4){
            printf("Enter atleast three characters to search\n");
            goto LAB2;
        }

    while(flag==0&&current->next != NULL){
        current=current->next;
        //Iterating through all characters of a single book author
        for(int j=0;j<30;j++){


        //Skip the current book if atleast 3 characters don't match
        if((tolower(author_search[j])!=tolower(current->book.b_book_author[j]))&&j<3){
            //printf("\n First 3 not matching"); For ref only, to be removed before submission
            break;
            }

            //If atleast one character does not match, store the closest matching search (author) and skip current iteration
        else if(tolower(author_search[j])!=tolower(current->book.b_book_author[j])&&j<strlen(current->book.b_book_author)&&j>2){
                //printf("\n Close"); For ref only, to be removed before submission

            short check_repeat=0;
            //To prevent repetition of closest search 'authors'
            for(int m=0;m<cs_count;m++)
            {
                if(strcmp(closest_search[m],current->book.b_book_author)==0)
                check_repeat=1;
            }
            if(check_repeat!=0)
            break;

            if(cs_count<MAX_CLOSE_AUTH_SEARCH){
                strcpy(closest_search[cs_count],current->book.b_book_author);
                cs_count++;
            }
            flag=2;
            break;
            }

            //Continue if some (not all) characters match
        else if(tolower(author_search[j])==tolower(current->book.b_book_author[j])&&j<strlen(current->book.b_book_author)){
             //printf("\n Almost"); For ref only, to be removed before submission
                continue;
        }
                //If the search matches exactly, stop searching
        else{
                //printf("\n Found");
                strcpy(auth_found,current->book.b_book_title);
                strcat(all_same_auth[same_auth_count],"Title: ");
                strcat(all_same_auth[same_auth_count],current->book.b_book_title);
                char curr_ID[11];
                sprintf(curr_ID, "%ld", current->book.b_book_ID); //Appending ID to display
                strcat(all_same_auth[same_auth_count],"\n\tID: ");
                strcat(all_same_auth[same_auth_count],curr_ID);

                 strcat(all_same_auth[same_auth_count],"\n\tStatus: ");
                if(current->book.b_book_status=='A')
                    strcat(all_same_auth[same_auth_count], "Available\n\n");
                else if(current->book.b_book_status=='R')
                    strcat(all_same_auth[same_auth_count], "Reserved\n\n");
                else if(current->book.b_book_status=='I')
                    strcat(all_same_auth[same_auth_count], "Not available (Issued)\n\n");
                else
                    strcat(all_same_auth[same_auth_count], "Undefined\n\n");
                same_auth_count++;
                break;
                }
            }

        }

        if(flag==0&&cs_count==0)
        printf("No results found");

        //To print all closest searches of authors when exact match is not found
        if(cs_count>0&&same_auth_count==0){
        cs_count=0;
        printf("Did you mean?:\n");
        while(strlen(closest_search[cs_count])!=0){
            printf("%hi. %s",cs_count+1,closest_search[cs_count]);
            cs_count++;
            if(cs_count=MAX_CLOSE_AUTH_SEARCH)
            break; //Stop on reaching MAX_CLOSE_AUTH_SEARCH closest search
        }
        }

        if(same_auth_count>0){
            for(int m=0;m<same_auth_count;m++)
                printf("%hi. %s",m+1,all_same_auth[m]);
            printf("Total of %hi books authored by %s",same_auth_count,auth_found);
        }

}
