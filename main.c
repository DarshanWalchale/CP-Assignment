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
#define MAX_CLOSE_ID 20 //Maximum number of closest seraches of ID to display

// STRUCTURES
/*
//struct tm {                                   //struct tm FOR REFFERENCE
//   int tm_sec;     Seconds (0-60)
//    int tm_min;     Minutes (0-59)
//    int tm_hour;    Hours (0-23)
//    int tm_mday;    Day of the month (1-31)
//    int tm_mon;     Month (0-11)
//    int tm_year;    Year - 1900
//    int tm_wday;    Day of the week (0-6, Sunday = 0)
//    int tm_yday;    Day in the year (0-365, 1 Jan = 0)
//    int tm_isdst;   Daylight saving time
//};
*/

//Structure variables starting with single characters will be used in different functions
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
    char user_name[30]; //
    unsigned long u_book_ID; //10 digit ID
    unsigned long u_issue_ID; //10 digit ID
    char u_user_pwd[30]; //User password
    bool u_admin; // 1 if admin
    struct tm u_date_issue;    //
    char u_requested[MAX_TITLE_LENGTH];
} USER;

typedef struct BOOKNODE         // this is used to load the library into memory
{
    BOOK book;
    struct BOOKNODE *next;
} BOOKNODE;

typedef struct USERNODE         // this is used to load all userdata into memory (for deleteUser)
{
    USER user;
    struct USERNODE *next;
} USERNODE;

// GLOBAL VARIABLES
unsigned long Book_ID_Counter;           // 3943
unsigned long Issue_ID_Counter;          // 20195
unsigned long User_ID_Counter;           // 1791
USER Current_User; //capitalized cuz global variable

// PROTOTYPES
BOOKNODE * loadLibrary(BOOKNODE *);
void loadCounters();
void welcomeScreen();
void menu(USER *user);
void adminMenu(USER *user);
void booksearchMenu(BOOKNODE *head);
void addUser(void);
void deleteUser(USER *user);
//void addNewBook(BOOK *book);
//USER * loadNextUser(FILE *, USER*);
//BOOK * loadNextBook(FILE *, BOOK*);
int titleCount(char *);
void makeFile();
unsigned long generateBookID();
unsigned long generateIssueID();
unsigned long generateUserID();
void vendorManagement();
void displayAllBooks(BOOKNODE *head);
void searchBookbyTitle(BOOKNODE *head);
void searchBookbyAuthor(BOOKNODE *head);
void searchBookbyID(BOOKNODE *head);
void newlyAddedBooks(BOOKNODE *head);
void notifications(BOOKNODE *head, USER *user);
void setCurrentUser(USER *);
void saveLibrary(BOOKNODE *head);
void freeLibrary(BOOKNODE *head);
void saveUserList(USERNODE *head);
void freeUserList(USERNODE *head);


//--------------------------------------------------------------------------------------------------------------

int main(void)
{
    //printf("sizeof(BOOKNODE) = %lu\nsizeof(USER) = %lu\nsizeof(int) = %lu\n", sizeof(BOOKNODE), sizeof(USER), sizeof(int));
    //makeFile();
    BOOKNODE *head = calloc(1, sizeof(BOOKNODE));
    head = loadLibrary(head);



    printf("Books database loaded into memory successfully\n");
    displayAllBooks(head);
    printf("Counters: %lu,%lu,%lu\n", generateBookID(), generateIssueID(), generateUserID());

     welcomeScreen();



    saveLibrary(head);
    freeLibrary(head);
    return 0;
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
  while(getchar() != '\n');

  static int count = 0; // if user can't register or login successfully in 5 attempts, just exit
  char c;
  USER *pUser = (USER *)malloc(sizeof(USER));
        FILE *fp;
        char uName[30];
        char pwd[30]; // local char arrays to store details just as entered

  switch(choice)
    {

        case 1:

        // login();

        if (( fp = fopen("userdata.txt", "r+")) == NULL)
        {
            printf ("Could not open file\n");
            count++;
        }


        printf("Username: ");
        scanf("%29s",uName);


        printf("Password: ");
        scanf("%29s",pwd);


            while (fread (pUser, sizeof(struct USER), 1, fp) == 1)
            {
                if(strcmp (pUser -> user_name, uName) == 0)
                {
                    printf ("Username Matched\n");
                    if(strcmp ( pUser -> u_user_pwd, pwd) == 0)
                    {
                        printf ("Password Matched\n");
                        // particular user's account opened
                        setCurrentUser(pUser);

                        menu(pUser);
                    }
                }
            }

        break;

        case 2:

        // reg();
        // REGISTER will be an admin aciton bhut let's keep it for now to add users conveniently :)


        do
            {
                if ( ( fp = fopen("userdata.txt", "a+")) == NULL)
                {

                printf ("Could not open file\n");
                count++;

                }
                printf("Choose A Username: ");
                scanf("%29s",pUser -> user_name);
                while(getchar() != '\n');

                printf("Choose A Password: ");
                scanf("%29s",pUser -> u_user_pwd);
                while(getchar() != '\n');

                fwrite (pUser, sizeof(struct USER), 1, fp);

                printf("Add another account? (Y/N): ");
                scanf(" %c", &c);
                while(getchar() != '\n');
            }
            while(c == 'Y'|| c == 'y');
        break;

        default:
        printf("\n\n\t\t\t\tINVALID OPTION");
        count++;
        if(count >= 5)
        {
            printf("Sorry, too many unsuccessful attempts!\n");
            return;
        }

        printf("\n\n\t\t\tPress Enter to re-Enter the choice");

        char ch2 = scanf("%c",&ch2);
        if(ch2 == '\n')
        goto PQ;

    }
}

void menu(USER *user)
{
    printf("Welcome User\n");

    int choice;

    userchoice: // label to return to user menu
    printf("1. Search Books\n");
    printf("2. Book Transaction\n");

    if(user -> u_admin == 1)
    {
        printf("3. Admin Menu\n");
    }

    printf("\n\nEnter your choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
        booksearchMenu(head);

        printf("Press 0 to return to the User menu");
        char ch2 = scanf("%c",&ch2);
        if(ch2 == '0')
        goto userchoice;
        break;

        case 2:
        int option;

        transaction:  // label to reach transaction menu
        printf("Book Transaction Menu\n");

        printf("1. Checkout a Book\n");
        printf("2. Return Book\n");

        printf("Enter Choice: ");
        scanf("%d",&option);

        switch(option)
        {
            case 1:
            //checkout book
            break;

            case 2:
            //return book
            break;

            default:
            printf("\n\n\t\t\t\tINVALID OPTION");
            printf("\n\n\t\t\tPress Enter to re-Enter the choice");

            char ch3 = scanf("%c",&ch3);
            if(ch3 == '\n')
            goto transaction;
        }

        case 3:
        adminMenu(user);
        break;

        default:

        printf("\n\n\t\t\t\tINVALID OPTION");
        printf("\n\n\t\t\tPress Enter to re-Enter the choice");

        char ch4 = scanf("%c",&ch4);
        if(ch4 == '\n')
        goto userchoice;

    }
}

void booksearchMenu()
{
    int choice;

    booksearch:
    printf("Book Search Menu\n");

    printf("1. Search Book by Title\n");
    printf("2. Search Book by Author\n");
    printf("2. Search Book by Book ID\n");

   printf("Enter choice: ");
   scanf("%d", &choice);

   switch(choice)
   {
       case 1:
       searchBookbyTitle(head);
        printf("Press 0 to return to the Search menu");
        char ch13 = scanf("%c",&ch13);
        if(ch13 == '0')
        goto booksearch;
        break;

        case 2:
        searchBookbyAuthor(head);
        printf("Press 0 to return to the Search menu");
        char ch12 = scanf("%c",&ch12);
        if(ch12 == '0')
        goto booksearch;
        break;

        case 3:
        searchBookbyID(head);
        printf("Press 0 to return to the Search menu");
        char ch11 = scanf("%c",&ch11);
        if(ch11 == '0')
        goto booksearch;
        break;

        default:

        printf("\n\n\t\t\t\tINVALID OPTION");
        printf("\n\n\t\t\tPress Enter to re-Enter the choice");

        char ch10 = scanf("%c",&ch10);
        if(ch10 == '\n')
        goto booksearch;

   }
}

void adminMenu(USER *user)
{
    printf("Welcome Admin\n");

    int choice;

    ADMIN: // in case we need to retun here again
    printf("1. Add Library members\n");
    printf("2. Delete Library members\n");
    printf("3. Count books of a particular title\n");
    printf("4. Vendor / Library requests\n");


    printf("\n\nEnter your choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
        addUser();

        printf("Press 0 to return to the Admin menu");
        char ch2 = scanf("%c",&ch2);
        if(ch2 == '0')
        goto ADMIN;
        break;

        case 2:
        deleteUser(user);
        printf("Press 0 to return to the Admin menu");
        char ch9 = scanf("%c",&ch9);
        if(ch9 == '0')
        goto ADMIN;
        break;

        case 3:
        searchBookbyTitle(head);
        printf("Press 0 to return to the Admin menu");
        char ch7 = scanf("%c",&ch7);
        if(ch7 == '0')
        goto ADMIN;
        break;

        case 4:
        vendorManagement();
        printf("Press 0 to return to the Admin menu");
        char ch6 = scanf("%c",&ch6);
        if(ch6 == '0')
        goto ADMIN;
        break;

        default:
        printf("\n\n\t\t\t\tINVALID OPTION");
        printf("\n\n\t\t\tPress Enter to re-Enter the choice");

        char ch5 = scanf("%c",&ch5);
        if(ch5 == '\n')
        goto ADMIN;

    }
}


void addUser(void)
{
    FILE *fp;
    char c;
    USER *pUser = (USER *)malloc(sizeof(USER));
    do
    {
        if (( fp = fopen("userdata.txt", "a+")) == NULL)
        {
            printf ("Could not open file\n");
        }

    printf("Choose A Username: ");
    scanf("%29s",pUser -> user_name);
    while(getchar() != '\n');

    printf("Choose A Password: ");
    scanf("%29s",pUser -> u_user_pwd);
    while(getchar() != '\n');

    fwrite (pUser, sizeof(struct USER), 1, fp);

    printf("Add another account? (Y/N): ");
    scanf(" %c", &c);
    while(getchar() != '\n');

    }
    while(c == 'Y'|| c == 'y');
    free (pUser);//free allocated memory
    fclose(fp);

}




void makeFile()
{
    FILE *fp = fopen("books.txt", "a");
    BOOK book;
    time_t sec = time(NULL);
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


        fwrite(&book, sizeof(BOOK), 1, fp);

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

    fclose(fp);

    return;
}

unsigned long generateBookID()
{
    Book_ID_Counter++;
    FILE *fp = fopen("counterdata.txt", "w");
    fprintf(fp, "%lu,%lu,%lu", Book_ID_Counter, Issue_ID_Counter, User_ID_Counter);
    fclose(fp);
    return (Book_ID_Counter);
}

unsigned long generateIssueID()
{
    Issue_ID_Counter++;
    FILE *fp = fopen("counterdata.txt", "w");
    fprintf(fp, "%lu,%lu,%lu", Book_ID_Counter, Issue_ID_Counter, User_ID_Counter);
    fclose(fp);
    return (Issue_ID_Counter);
}

unsigned long generateUserID()
{
    User_ID_Counter++;
    FILE *fp = fopen("counterdata.txt", "w");
    fprintf(fp, "%lu,%lu,%lu", Book_ID_Counter, Issue_ID_Counter, User_ID_Counter);
    fclose(fp);
    return (User_ID_Counter);
}






// Give pointer to head BOOKNODE (likely initialized in main)
// Loads the entire library into memory using a dynamic linked list (utilizes higher reading speed of RAM and reduces file I/O process requirement)
BOOKNODE * loadLibrary(BOOKNODE *head)
{
    loadCounters();
    //printf("loadLibrary");
    FILE *fp = fopen("books.txt", "r");
    BOOK *book_load = calloc(1, sizeof(BOOK));
    //loadNextBook(fp, book_load);
    fread(book_load, sizeof(BOOK), 1, fp);

    BOOKNODE *current;
    head->next = (BOOKNODE *)calloc(1, sizeof(BOOKNODE));
    current = head->next;
    current->next = NULL;
    //printf("\t1\n");
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
        //date of issue struct tm part
        current->book.b_date_issue.tm_sec = book_load->b_date_issue.tm_sec;
        current->book.b_date_issue.tm_min = book_load->b_date_issue.tm_min;
        current->book.b_date_issue.tm_hour = book_load->b_date_issue.tm_hour;
        current->book.b_date_issue.tm_mday = book_load->b_date_issue.tm_mday;
        current->book.b_date_issue.tm_mon = book_load->b_date_issue.tm_mon;
        current->book.b_date_issue.tm_year = book_load->b_date_issue.tm_year;
        current->book.b_date_issue.tm_wday = book_load->b_date_issue.tm_wday;
        current->book.b_date_issue.tm_yday = book_load->b_date_issue.tm_yday;
        current->book.b_date_issue.tm_isdst = book_load->b_date_issue.tm_isdst;
        //printf("A");


        //printf("B");
        if(feof(fp))
        {
            break;
        }
        else
        {
            current->next = (BOOKNODE *)calloc(1, sizeof(BOOKNODE));
            current = current->next;
            //loadNextBook(fp, book_load);
            fread(book_load, sizeof(BOOK), 1, fp);
        }
    }

    fclose(fp);

    return head;
}

void saveLibrary(BOOKNODE *head)
{
    FILE *fp = fopen("books.txt", "w");
    BOOKNODE *current, *temp;
    current = head -> next;

    while(current != NULL)
    {

        fwrite(&(current->book), sizeof(BOOK), 1, fp);
        current = current->next;
    }
    return;
}

void freeLibrary(BOOKNODE *head)
{
    if(head->next == NULL)
    {
        free(head);
        return;
    }
    else
    {
        freeLibrary(head->next);
    }
    return;
}

void loadCounters()
{
    FILE *fp = fopen("counterdata.txt", "r");
    char buffer[256] = {};
    fgets(buffer, 256, fp);

    char * pointer = strtok(buffer, ",");
    //printf("loadcounters:\t%lu\t", atol(pointer));
    Book_ID_Counter = atol(pointer);

    pointer = strtok(NULL, ",");
    //printf("%lu\n", atol(pointer));
    Issue_ID_Counter = atol(pointer);

    pointer = strtok(NULL, ",");
    User_ID_Counter = atol(pointer);

    printf("%lu\t%lu\t%lu\n", Book_ID_Counter, Issue_ID_Counter, User_ID_Counter);
    fclose(fp);

    return;
}

int titleCount(char *title)
{
    FILE *fp = fopen("books.txt", "r");
    BOOK *book = calloc(1, sizeof(BOOK));
    fread(book, sizeof(BOOK), 1, fp);
    int count = 0;

    while(book != NULL)
    {
        if(strcmp(book->b_book_title, title) == 0)
        {
            count++;
        }
        fread(book, sizeof(BOOK), 1, fp);
    }

    return count;

}




void searchBookbyTitle(BOOKNODE *head)
{
    BOOKNODE *current = head;
    char title_search[MAX_TITLE_LENGTH];
    short cs_count=0; //To count number of closest searches
    short flag=0; //To end searching on finding the exact book or when no results
    char closest_search[MAX_CLOSE_TITLE_SEARCH][MAX_TITLE_LENGTH+30]; //To store MAX_CLOSE_TITLE_SEARCH closest matching title searches
    LAB1: //a label to return if <=3 chars entered
    printf("Search by title: ");
    fgets(title_search,MAX_TITLE_LENGTH,stdin);

    //Restart search if less than three characters entered
    if(strlen(title_search)<4)
    {
        printf("Enter at least three characters to search");
        goto LAB1;
    }

    while(flag==0&&current->next != NULL)
    {
        current=current->next;
        //Iterating through all characters of a single book title
        for(int j=0;j<MAX_TITLE_LENGTH;j++)
        {

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
                }
            }

        }

        if(flag==0&&cs_count==0)
        printf("No results found");

        //To print all closest searches (upto MAX_CLOSE_TITLE_SEARCH) when book is not found
        if(cs_count>0&&flag!=3)
        {
        cs_count=0;
        printf("Closest searches:\n");
        while(strlen(closest_search[cs_count])!=0)
        {
            printf("%hi. %s",(short)(cs_count+1),closest_search[cs_count]);
            cs_count++;
            if(cs_count==MAX_CLOSE_TITLE_SEARCH)
            break; //Stop on reaching MAX_CLOSE_TITLE_SEARCH closest search
        }
        }


}


void displayAllBooks(BOOKNODE *head)
{
    BOOKNODE *current = head;
    if(head->next == NULL)
    {
        printf("--------------------NO BOOKS TO DISPLAY--------------------\n");
    }

    printf("------------------------------------------------------------\n");
    printf("AVAILABILITY\tAUTHOR\t\tTITLE\n");
    printf("------------------------------------------------------------\n");
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
        printf("%s\t%s\t\t%s\n", availability, current->book.b_book_author,current->book.b_book_title);
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
            printf("%hi. %s",(short)(cs_count+1),closest_search[cs_count]);
            cs_count++;
            if(cs_count==MAX_CLOSE_AUTH_SEARCH)
            break; //Stop on reaching MAX_CLOSE_AUTH_SEARCH closest search
        }
        }

        if(same_auth_count>0){
            for(int m=0;m<same_auth_count;m++)
                printf("%hi. %s",(short)(m+1),all_same_auth[m]);
            printf("Total of %hi books authored by %s",(short)same_auth_count,auth_found);
        }

}

void searchBookbyID(BOOKNODE *head){
    BOOKNODE *current = head;
    char ID_search[11];
    short cs_count=0; //To count number of closest searches
    short flag=0; //To end searching on finding the exact search or when no results
    char closest_search[MAX_CLOSE_ID][11]; //To store MAX_CLOSE_ID closest matching searches of ID
    LAB3: //Label to return if invalid (not 10 digits)
    printf("Search by ID: ");
    fgets(ID_search,11,stdin);
    printf("%ld",strlen(ID_search));
    if((strlen(ID_search))!=10){
    printf("Invalid ID entered! (ID is 10 digits)\n");
    goto LAB3;
    }
    while(flag==0&&current->next != NULL){
        current=current->next;
        //Iterating through all characters of a single book author
        for(int j=0;j<11;j++){
        char str_ID[11];
        sprintf(str_ID,"%ld",current->book.b_book_ID);

            //Skip the current book if atleast 6 characters don't match
        if((tolower(ID_search[j])!=tolower(str_ID[j]))&&j<6){
            //printf("\n First 6 not matching"); For ref only, to be removed before submission
            break;
            }

            //If atleast one character does not match, store the closest matching search (author) and skip current iteration
        else if(tolower(ID_search[j])!=tolower(str_ID[j])&&j<strlen(str_ID)&&j>5){
               // printf("\n Close"); //For ref only, to be removed before submission

            if(cs_count<MAX_CLOSE_ID){
                strcat(closest_search[cs_count],"Title: ");
                strcat(closest_search[cs_count],current->book.b_book_title);
                strcat(closest_search[cs_count],"\n\tID:");
                strcat(closest_search[cs_count],str_ID);
                strcat(closest_search[cs_count],"\n\tAuthor: ");
                strcat(closest_search[cs_count],current->book.b_book_title);
                cs_count++;
            }
            break;
            }

            //Continue if some (not all) characters match
        else if(tolower(ID_search[j])==tolower(str_ID[j])&&j<strlen(str_ID)){
             //printf("\n Almost"); //For ref only, to be removed before submission
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

                printf("\tID: %s\n",str_ID);
                printf("\tAuthor: %s\n",current->book.b_book_author);

                flag=3;
                break;
                }
            }

        }

        //To print all closest searches of authors when exact match is not found
        if(cs_count>0&&flag!=3){
        cs_count=0;
        printf("Closest searches:\n");
        while(strlen(closest_search[cs_count])!=0){
            printf("%hi. %s",(short)(cs_count+1),closest_search[cs_count]);
            cs_count++;
            if(cs_count==MAX_CLOSE_ID)
            break; //Stop on reaching MAX_CLOSE_ID closest search
        }
        }

        if(cs_count==0&&flag!=3)
        printf("No results found");
}

void newlyAddedBooks(BOOKNODE *head)
{
    time_t sec = time(NULL);
    struct tm now = *(localtime(&sec));
    printf("----------------------NEWLY ADDED BOOKS----------------------\n");
    BOOKNODE *current = head;
    if(head->next == NULL)
    {
        printf("--------------------NO BOOKS TO DISPLAY--------------------\n");
    }

    printf("------------------------------------------------------------\n");
    printf("AVAILABILITY\tAUTHOR\t\tTITLE\n");
    printf("------------------------------------------------------------\n");
    char availability[15] = {};

    while(current->next != NULL)
    {
        current = current->next;
        if(current->book.book_date_of_arrival.tm_year == now.tm_year)
        {
            if(now.tm_mon - current->book.book_date_of_arrival.tm_mon > 4)
            {
                continue;
            }
        }
        else if(current->book.book_date_of_arrival.tm_year == (now.tm_year - 1))
        {
            if((now.tm_mon + 12) - current->book.book_date_of_arrival.tm_mon > 4)
            {
                continue;
            }
        }



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
            printf("%s\t%s\t\t%s\n", availability, current->book.b_book_author,current->book.b_book_title);

    }
    printf("------------------------------------------------------------\n");

    return;
}

void notifications(BOOKNODE *head, USER *user)
{
    time_t sec = time(NULL);
    struct tm now = *(localtime(&sec));
    printf("----------------------PLEASE RETURN----------------------\n");
    BOOKNODE *current = head;
    bool no_books_due = true, book_notify = false;

    printf("AUTHOR\t\tTITLE\n");
    char availability[15] = {};

    while(current->next != NULL)
    {
        current = current->next;

        if(!(book_notify) && strcmp(current->book.b_book_title, user->u_requested) == 0)
        {
            if(current->book.b_book_status == 'A')
            {
                book_notify = true;
            }
        }

        if(no_books_due)
        {
            if(current->book.b_user_ID == user->u_user_ID)
            {
                if(((current->book.b_date_issue.tm_year == now.tm_year) && (now.tm_yday - current->book.b_date_issue.tm_yday < 15))
                    ||((now.tm_year - current->book.b_date_issue.tm_year == 1) && (365 + now.tm_wday - current->book.b_date_issue.tm_wday < 15)))
                {
                    no_books_due = false;
                    continue;
                }
                else
                {
                    no_books_due = false;
                    printf("%s\t\t%s\n", current->book.b_book_title, current->book.b_book_author);
                    break;
                }
            }
        }
    }
    if(no_books_due)
    {
        printf("No Book Returns Due\n");
    }
    printf("------------------------------------------------------------\n");

    if(book_notify)
    {
        printf("%s IS NOW AVAILABLE TO CHECKOUT!\n", user->u_requested);
        strcpy(user->u_requested, "\0");
    }

    return;
}

void setCurrentUser(USER *user)
{
    Current_User.u_user_ID = user->u_user_ID;
    strcpy(Current_User.user_name, user->user_name);
    Current_User.u_book_ID = user->u_book_ID;
    Current_User.u_issue_ID = user->u_issue_ID;
    strcpy(Current_User.u_user_pwd, user->u_user_pwd);
    Current_User.u_admin = user->u_admin;
    strcpy(Current_User.u_requested, user->u_requested);
    // struct tm date of issue
    Current_User.u_date_issue.tm_sec = user->u_date_issue.tm_sec;
    Current_User.u_date_issue.tm_min = user->u_date_issue.tm_min;
    Current_User.u_date_issue.tm_hour = user->u_date_issue.tm_hour;
    Current_User.u_date_issue.tm_mday = user->u_date_issue.tm_mday;
    Current_User.u_date_issue.tm_mon = user->u_date_issue.tm_mon;
    Current_User.u_date_issue.tm_year = user->u_date_issue.tm_year;
    Current_User.u_date_issue.tm_wday = user->u_date_issue.tm_wday;
    Current_User.u_date_issue.tm_yday = user->u_date_issue.tm_yday;
    Current_User.u_date_issue.tm_isdst = user->u_date_issue.tm_isdst;
    return;
}

void deleteUser(USER *user)
{
    //loads all users into a linked list in memory except the user matching that passed into this function
    FILE *fp = fopen("userdata.txt", "r");
    USER *user_load = calloc(1, sizeof(USER));
    fread(user_load, sizeof(USER), 1, fp);

    USERNODE *current, *head = (USERNODE *)calloc(1, sizeof(USERNODE));
    head->next = (USERNODE *)calloc(1, sizeof(USERNODE));
    current = head->next;
    current->next = NULL;
    //printf("\t1\n");
    while(1)
    {
        if(user_load->u_user_ID == user->u_user_ID)
        {
            continue;
        }
        current->user.u_user_ID = user_load->u_user_ID;
        strcpy(current->user.user_name, user_load->user_name);
        current->user.u_book_ID = user_load->u_book_ID;
        current->user.u_issue_ID = user_load->u_issue_ID;
        strcpy(current->user.u_user_pwd, user_load->u_user_pwd);
        current->user.u_admin = user_load->u_admin;

        //date of issue struct tm part
        current->user.u_date_issue.tm_sec = user_load->u_date_issue.tm_sec;
        current->user.u_date_issue.tm_min = user_load->u_date_issue.tm_min;
        current->user.u_date_issue.tm_hour = user_load->u_date_issue.tm_hour;
        current->user.u_date_issue.tm_mday = user_load->u_date_issue.tm_mday;
        current->user.u_date_issue.tm_mon = user_load->u_date_issue.tm_mon;
        current->user.u_date_issue.tm_year = user_load->u_date_issue.tm_year;
        current->user.u_date_issue.tm_wday = user_load->u_date_issue.tm_wday;
        current->user.u_date_issue.tm_yday = user_load->u_date_issue.tm_yday;
        current->user.u_date_issue.tm_isdst = user_load->u_date_issue.tm_isdst;
        //printf("A");

        if(feof(fp))
        {
            break;
        }
        else
        {
            current->next = (USERNODE *)calloc(1, sizeof(USERNODE));
            current = current->next;
            fread(user_load, sizeof(USER), 1, fp);
        }
    }
    fclose(fp);

    // saves userlist in memory to file
    saveUserList(head);

    // frees dynamically allocated linked list of users' data from memory
    freeUserList(head);

    return;
}

// saves userlist in memory to file
void saveUserList(USERNODE *head)
{
    FILE *fp = fopen("userdata.txt", "w");
    USERNODE *current;
    current = head -> next;

    while(current != NULL)
    {

        fwrite(&current->user, sizeof(USER), 1, fp);
        current = current->next;
    }
    return;
}

// frees dynamically allocated linked list of users' data from memory
void freeUserList(USERNODE *head)
{
    if(head->next == NULL)
    {
        free(head);
        return;
    }
    else
    {
        freeUserList(head->next);
    }
    return;
}

void vendorManagement(){
    FILE *fp1, *fp2;
    char choice, req;
    char lib[30]; //Store name of current library
    char name[30];
    char title[30];
    char buf[80];
    fp1=fopen("sentRequests.txt","a");
    //fp2 is dynamic, to store 'recieved requests' for vendors/libraries
    LAB: //Label to return
    printf("Enter your choice: \n");
    printf("1. Make new request\n");
    printf("2. View sent or received requests\n");
    scanf("%c",&choice);
    switch(choice){
        case '1':
            printf("\n1.Request Vendor\n");
            printf("2.Request Library\n");
            scanf(" %c",&req);
            if(req=='1'){
                printf("\nEnter your library name: ");
                scanf("%s",lib);
                printf("Enter vendor name: ");
                scanf("%s",name);
                printf("Enter the title of the book: ");
                scanf("%s",title);
                if(strlen(name)==0||strlen(title)==0){
                    printf("Error\n");
                    goto LAB;
                }
                //Updating sent requests
                fputs("\nVendor: ",fp1);
                fputs(name,fp1);
                fputs("\tBook requested: ",fp1);
                fputs(title,fp1);
                //Updating vendor's received requests
                fp2=fopen(name,"w+");
                fputs("\nLibrary: ",fp2);
                fputs(lib,fp2);
                fputs("\tBook requested: ",fp2);
                fputs(title,fp2);
            }
            else if(req=='2'){
                printf("Enter name of the library to be requested: ");
                scanf("%s",name);
                printf("Enter the title of the book: ");
                scanf("%s",title);
                if(fp1==NULL||strlen(name)==0||strlen(title)==0){
                    printf("Error");
                    goto LAB;
                }
                fputs("\nLibrary: ",fp1);
                fputs(name,fp1);
                fputs("\tBook requested: ",fp1);
                fputs(title,fp1);
                //Updating requested library's received requests
                fp2=fopen(name,"w+");
                fputs("\nRequesting Library: ",fp2);
                fputs(lib,fp2);
                fputs("\tBook requested: ",fp2);
                fputs(title,fp2);
            }
            else{
                printf("Error!");
                goto LAB;
            }
        break;

        case '2':
            fp1=fopen("sentRequests.txt","r");
            printf("\n1.Sent requests\n");
            printf("2.Received requests\n");
            scanf(" %c",&req);
            if(req=='1'){
                while(fgets(buf, 80, fp1)!= NULL)
                fputs(buf, stdout);
            }
        break;

        default:
        printf("Invalid entry!\n");
        goto LAB;
    }
}














//JUNK
/*
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
