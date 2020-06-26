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
#define MAX_SAME_TITLE 15

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
    char b_book_title[MAX_TITLE_LENGTH];    // Title of Book
    char b_book_author[30];                 // Author of Book
    unsigned long b_book_ID;                // Unique User ID
    unsigned long b_issue_ID;               // Unique Book ID
    unsigned long b_user_ID;                // Unique Issue ID
    char b_book_status;                     // 'A' - Available, 'I' - issued, 'R' - reserved
    struct tm book_date_of_arrival;         // Time of book being added to database
    struct tm b_date_issue;                 // Time of issuing book
} BOOK;

typedef struct USER{
    unsigned long u_user_ID;                // Unique User ID
    char user_name[30];                     // Username
    unsigned long u_book_ID;                // Unique Book ID
    unsigned long u_issue_ID;               // Unique Issue ID
    char u_book_title[MAX_TITLE_LENGTH];    //title of book already issued
    char u_user_pwd[30];                    //User password
    bool u_admin;                           // true if admin
    struct tm u_date_issue;                 // Time of book being added to database
    char u_requested[MAX_TITLE_LENGTH];     // Title of book requested ot notify upon availability on login

} USER;

typedef struct BOOKNODE         // this is used to load the library into memory
{
    BOOK book;
    struct BOOKNODE *next;
} BOOKNODE;

typedef struct USERNODE         // this is used to load all userdata into memory
{
    USER user;
    struct USERNODE *next;
} USERNODE;

// GLOBAL VARIABLES
unsigned long Book_ID_Counter;           // 3943
unsigned long Issue_ID_Counter;          // 20195
unsigned long User_ID_Counter;           // 1791
USER Current_User;                       //capitalized cuz global variable
BOOKNODE *BookHead;
USERNODE *UserHead;

// PROTOTYPES
BOOKNODE * loadLibrary(BOOKNODE *);
USERNODE * loadUsers(USERNODE *);
void loadCounters();
void returnBook();
void adminMenu();
void booksearchMenu();
void addNewUser(USERNODE *head);
void printUserInfo(USER *user);
void addNewBook(BOOKNODE *head);
void saveCounters();
void vendorManagement();
void displayAllBooks(BOOKNODE *head);
void searchBookbyTitle(BOOKNODE *head);
void searchBookbyAuthor(BOOKNODE *head);
void searchBookbyID(BOOKNODE *head);
void newlyAddedBooks(BOOKNODE *head);
void notifications(BOOKNODE *head, USER *user);
void setCurrentUser(USER *);
void saveBookList(BOOKNODE *head);
void freeLibrary(BOOKNODE *head);
void saveCurrentUser(USERNODE *, USER *);
void saveUserList(USERNODE *head);
void freeUserList(USERNODE *head);
void reviewAdminPrivileges(USERNODE *head);
void displayAdmins(USERNODE *head);
void displayNonAdmins(USERNODE *head);
void displayAdminView(USERNODE *head);
int deleteUser(USERNODE *head, unsigned long id);
int makeAdmin(USERNODE *head);
int removeAdmin(USERNODE *head);
int welcomeScreen(USERNODE *head);
int menu();
int checkout(char *title);
int titleCount(BOOKNODE *head, char *);
unsigned long generateBookID();
unsigned long generateIssueID();
unsigned long generateUserID();
//void addNewBook(BOOK *book);
//USER * loadNextUser(FILE *, USER*);
//BOOK * loadNextBook(FILE *, BOOK*);


//--------------------------------------------------------------------------------------------------------------

int main(void)
{
    BookHead = (BOOKNODE *)calloc(1, sizeof(BOOKNODE));
    //printf("BookHead->next = %p\n", BookHead->next);
    UserHead = (USERNODE *)calloc(1, sizeof(USERNODE));
    //printf("UserHead->next = %p\n", UserHead->next);
    loadCounters();
    //addNewBook(BookHead);
    //addNewUser(UserHead);
    
    
    printf("Program start\n");
    BookHead = loadLibrary(BookHead);
    printf("Books database loaded into memory successfully\n");
    UserHead = loadUsers(UserHead);
    printf("Users successfully loaded\n");
    displayAllBooks(BookHead);
    //printf("sizeof(BOOKNODE) = %lu\nsizeof(USER) = %lu\nsizeof(int) = %lu\n", sizeof(BOOKNODE), sizeof(USER), sizeof(int));



    //printf("Press Enter to Login"); Bruh
    //while(getchar() != '\n');
    //printf("Counters: %lu,%lu,%lu\n", generateBookID(), generateIssueID(), generateUserID());

    if(welcomeScreen(UserHead) == 0)
    {
        notifications(BookHead, &Current_User);
        while(menu() != 0);
    }
    
    

    saveBookList(BookHead);
    freeLibrary(BookHead);
    saveUserList(UserHead);
    freeUserList(UserHead);
    return 0;
}

int welcomeScreen(USERNODE *head)
{
    int choice;
    printf("\n\n\n\n\n\t\t\tLIBRARY MANAGEMENT SYSTEM");
    printf("\n\t\t\t*************************\n");

    USERNODE *current = head;

    printf("\n\n\t\t\tPress Enter to login");
    while(getchar() != '\n');

    static int loginerrorcount = 0; // if user can't register or login successfully in 5 attempts, just exit
    char c;
    char uName[30];
    char pwd[30]; // local char arrays to store details just as entered

    do
    {
        printf("Username: ");
        scanf(" %29s", uName);
        while(getchar() != '\n')
        ;

        printf("Password: ");
        scanf(" %29s", pwd);
        while(getchar() != '\n')
        ;
        current = head;
        while (current->next != NULL)
        {
            current = current->next;
            if(strcmp(current->user.user_name, uName) == 0)
            {
                printf("Username Matched\n");
                if(strcmp(current->user.u_user_pwd, pwd) == 0)
                {
                    printf ("Password Matched\n");
                    setCurrentUser(&current->user);
                    return 0;
                }
                else
                {
                    loginerrorcount++;
                }
            }
            else
            {
                loginerrorcount++;
            }
        }
        current = head;
        printf("\n\t\t\tIncorrect Credentials\n\t\t\tPress Enter to retry");
        while(getchar() != '\n')
        ;
    }while(loginerrorcount <= 5);

    printf("Too many unsuccessful logins\n");
    return 1;
}

int menu(){
    int choice;
    userchoice: // label to return to user menu
    printf("\n--------------------MENU--------------------\n");
    printf("1. Search Books\n");
    printf("2. Book Transaction\n");
    printf("3. Account Info\n");
    if(Current_User.u_admin)
    {
        printf("9. Admin Menu\n");
    }
    printf("0. Exit from Library Portal\n");

    printf("Enter your choice: ");
    scanf(" %d", &choice);
    while(getchar() != '\n');
    int option;

    switch(choice)
    {
        case 1:
        booksearchMenu(BookHead);

        printf("Press Enter to return to the User menu\n");
        while (getchar() != '\n');
        goto userchoice;
        break;

        case 2:

        transaction:  // label to reach transaction menu
        printf("\n----------Book Transaction Menu----------\n");
        printf("1. Checkout a Book\n");
        printf("2. Return Book\n");
        printf("0. Return Back to Main Menu\n");

        printf("Enter Choice: ");
        scanf("%d", &option);

        switch(option)
        {
            case 1:
            //checkout book
            searchBookbyTitle(BookHead);

            char title[MAX_TITLE_LENGTH]; // to save title entered by user temporarily

            printf("\nEnter Book Title to Checkout (Case Sensitive)\n ->");
            fgets(title, MAX_TITLE_LENGTH, stdin);

            int result = checkout(title);
            if(result == 0)
            {
                printf("Your Book has been successfully issued\n");
                printf("Press Enter to return to User Menu\n");
                while(getchar() != '\n');

                goto userchoice;

            }
            else if(result == 1)       //(checkout(title) == 1) Bruhh
            {
                printf("No such Book Title\n");
                printf("Press Enter to return to Transaction Menu\n");
                while(getchar() != '\n');

                goto transaction;

            }
            else                        // user asked to to notify if him or not
            {
                printf("Press Enter to return to Transaction Menu\n");
                while(getchar() != '\n');

                goto transaction;

            }

            break;


            case 2:
                //return book

                returnBook();
                printf("\nPress Enter to return to Transaction Menu\n");
                while(getchar() != '\n');
                goto transaction;
                break;

        }
            break;

        case 3:
            printUserInfo(&Current_User);
            printf("Press Enter to return to the Main Menu");
            while (getchar() != '\n');
            break;

        case 9:
                                                   // not accessable to non-admins
            if(Current_User.u_admin == 1)
            {
                adminMenu();
            }
            else
            {
                printf("NOT AN ADMIN!\n");
                printf("Press Enter to return to the Main Menu");
                while (getchar() != '\n');
                goto userchoice;
            }
            break;

        case 0:
            return 0;
            break;

        default:
            printf("\n\n\t\t\t\tINVALID OPTION");
            printf("\n\n\t\t\tPress Enter\n");

            while(getchar() != '\n');
            goto userchoice;

    }
    goto userchoice;
    return 1;
}

// checkout handles checking out given a book title string, it also handles saving book title to notify on next login
// title is the name of the book passed by the user
int checkout(char *title)
{

    int status = 1; // 0: book found, available, and assigned successfully,
                    // 1: no book title found,
                    // 2: book title found, but not available, option given to set as alert for this book.


    BOOKNODE *current = BookHead;

    while(current->next != NULL)
    {
        current = current->next;

        if(strcmp(current->book.b_book_title, title) == 0)
        {
            if(current->book.b_book_status == 'A')
            {
                // generate and assign issueID
                unsigned long issue_id =  generateIssueID();
                current->book.b_issue_ID = issue_id;
                Current_User.u_issue_ID = issue_id;

                //change, u_book_ID,     b_user_ID, updating issued title to user  and  b_book_status;
                Current_User.u_book_ID = current->book.b_book_ID;
                current->book.b_user_ID = Current_User.u_user_ID;
                strcpy(Current_User.u_book_title, title);
                current->book.b_book_status = 'I';

                // Assign Dates
                time_t sec = time(NULL);
                struct tm time_of_event = *(localtime(&sec));
                //u_date_issue
                Current_User.u_date_issue.tm_sec = time_of_event.tm_sec;
                Current_User.u_date_issue.tm_min = time_of_event.tm_min;
                Current_User.u_date_issue.tm_hour = time_of_event.tm_hour;
                Current_User.u_date_issue.tm_mday = time_of_event.tm_mday;
                Current_User.u_date_issue.tm_mon = time_of_event.tm_mon;
                Current_User.u_date_issue.tm_year = time_of_event.tm_year;
                Current_User.u_date_issue.tm_wday = time_of_event.tm_wday;
                Current_User.u_date_issue.tm_yday = time_of_event.tm_yday;
                Current_User.u_date_issue.tm_isdst = time_of_event.tm_isdst;
                //b_date_issue
                current->book.b_date_issue.tm_sec = time_of_event.tm_sec;
                current->book.b_date_issue.tm_min = time_of_event.tm_min;
                current->book.b_date_issue.tm_hour = time_of_event.tm_hour;
                current->book.b_date_issue.tm_mday = time_of_event.tm_mday;
                current->book.b_date_issue.tm_mon = time_of_event.tm_mon;
                current->book.b_date_issue.tm_year = time_of_event.tm_year;
                current->book.b_date_issue.tm_wday = time_of_event.tm_wday;
                current->book.b_date_issue.tm_yday = time_of_event.tm_yday;
                current->book.b_date_issue.tm_isdst = time_of_event.tm_isdst;

                saveBookList(BookHead);
                saveCurrentUser(UserHead, &Current_User);
                saveUserList(UserHead);
                status = 0;
            }
        }
    }
    if(status == 2)
    {
        status = 2;
        char choice;
        do
        {
            printf("All copies of the book you've requested are currently issued by other library members!\n");
            printf("Would you like to be notified if the book becomes available? (Y/N) (notified on login when book is available once, only 1 book can be notified at a time)\n");
            scanf(" %c", &choice);
            while(getchar() != '\n');
            choice = toupper(choice);
            switch (choice)
            {
                case 'Y':
                    strcpy(Current_User.u_requested, title);
                    saveCurrentUser(UserHead, &Current_User);
                    saveUserList(UserHead);
                    printf("OK, you'll be notified if the book is available next time you log in");
                    break;

                case 'N':
                    printf("Ok, you won't be notified\n");
                    break;

                default:
                    printf("INVALID RESPONSE\t\t");
                    printf("Press Enter\n");
                    while(getchar() != '\n')
                    ;
                    break;
            }
        }while(!(choice == 'Y' || choice == 'N' || choice == 'y'));
    }
    return status;
}

void returnBook()
{
    printf("Currently Issued Book\n -> %s\n", Current_User.u_book_title);
    printf("Press Y to return currently issued book\n");
    char choice;

    scanf(" %c", &choice);
    while(getchar() != '\n');
    choice = toupper(choice);
    switch (choice)
    {
        case 'Y' :                                                          //return

        Current_User.u_book_ID = 0;
        Current_User.u_issue_ID = 0;

        BOOKNODE *current = BookHead;

        while(current->next != NULL)
        {
            current = current->next;

            if(strcmp(current->book.b_book_title, Current_User.u_book_title ) == 0)
            {
                 if(current->book.b_book_status == 'I')
                 {
                     current->book.b_issue_ID = 0;
                     current->book.b_user_ID = 0;
                     current->book.b_book_status = 'A';
                     saveBookList(BookHead);
                     saveCurrentUser(UserHead, &Current_User);
                     saveUserList(UserHead);
                     return;
                 }
            }
        }
        break;

        default:
        printf("Press Enter to return to Menu\n");
        while(getchar() != '\n');
        return;


    }
    return;
}

void booksearchMenu()
{
    int choice;

    booksearch:
    printf("\n----------Book Search Menu----------\n");

    printf("1. Search Book by Title\n");
    printf("2. Search Book by Author\n");
    printf("3. Search Book by Book ID\n");
    printf("4. Display All Books\n");
    printf("0. Return to User Menu\n");

   printf("Enter choice: ");
   scanf("%d", &choice);

   switch(choice)
   {
       case 1:
            searchBookbyTitle(BookHead);
            printf("\nPress Enter to return to the Search menu");
            while(getchar() != '\n');
            goto booksearch;
            break;

        case 2:
            searchBookbyAuthor(BookHead);
            printf("\nPress Enter to return to the Search menu");
            while(getchar() != '\n');
            goto booksearch;
            break;

        case 3:
            searchBookbyID(BookHead);
            printf("Press Enter to return to the Search menu");
            while(getchar() != '\n');
            goto booksearch;
            break;

        case 4:
            displayAllBooks(BookHead);
            printf("Press Enter to return to the Search menu");
            while(getchar() != '\n');
            goto booksearch;
            break;

        case 0:
            return;
            break;


        default:
            printf("\n\n\t\t\t\tINVALID OPTION");
            printf("\n\n\t\t\tPress Enter to re-Enter the choice");
            while(getchar() != '\n');
            goto booksearch;
            break;
   }
   return;
}

void adminMenu()
{
    int choice;
    ADMIN:
    printf("\n\n--------------------ADMIN MENU--------------------\n");
    printf("1. Add New Books to Library\n");
    printf("2. Add Library members\n");
    printf("3. Delete Library members\n");
    printf("4. Count books of a particular title\n");
    printf("5. Vendor / Library requests\n");
    printf("6. Review Admin Privileges\n");
    printf("0. Return to Main Menu\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);
    while(getchar() != '\n');

    switch(choice)
    {
        case 1:
            addNewBook(BookHead);
            break;

        case 2:
            addNewUser(UserHead);

            printf("\nPress enter to return to the Admin menu");
            while(getchar() != '\n');
            goto ADMIN;
            break;

        case 3:
            displayAdminView(UserHead);
            printf("Enter user_id of the user you want to remove: ");
            unsigned long id;
            scanf(" %lu", &id);
            while(getchar() != '\n')
            ;
            deleteUser(UserHead, id);
            
            printf("\nPress enter to return to the Admin menu");
            while(getchar() != '\n');
            goto ADMIN;
            break;

        case 4:
            printf("Enter title: ");
            char title[MAX_TITLE_LENGTH];
            scanf(" %60[^\n]", title); //MAX_TITLE_LENGTH
            while(getchar() != '\n');

            printf("Number of books by the title \"%s\" = %d", title, titleCount(BookHead, title));

            printf("\nPress enter to return to the Admin menu");
            while(getchar() != '\n');
            goto ADMIN;
            break;

        case 5:
            vendorManagement();
            printf("\nPress enter to return to the Admin menu");
            while(getchar() != '\n');
            goto ADMIN;
            break;

        case 6:
            reviewAdminPrivileges(UserHead);
            goto ADMIN;
            break;

        case 0:
            return;
            break;

        default:
        printf("\n\n\t\t\t\tINVALID OPTION");
        printf("\n\n\t\t\tPress Enter");
        while(getchar() != '\n');
        goto ADMIN;

    }
    goto ADMIN;
    return;
}

void reviewAdminPrivileges(USERNODE *head)
{
    reviewAdminPrivilegesFlag:
    printf("\n----------SubMenu: Review Admin Privileges----------\n");
    printf("1. View Admins\n");
    printf("2. View non-Admins\n");
    printf("3. View All\n");
    printf("4. Make a user an Admin\n");
    printf("5. Remove an Admin\n");
    printf("0. Return to Admin Menu\n");
    int choice;
    printf("\n\nEnter your choice: ");
    scanf("%d", &choice);
    while(getchar() != '\n');

    switch(choice)
    {
        case 1:
            displayAdmins(UserHead);
            printf("Press Enter\n");
            while(getchar() != '\n')
            ;
            break;

        case 2:
            displayNonAdmins(UserHead);
            printf("Press Enter\n");
            while(getchar() != '\n')
            ;
            break;

        case 3:
            displayAdminView(UserHead);
            printf("Press Enter\n");
            while(getchar() != '\n')
            ;
            break;

        case 4:
            makeAdmin(UserHead);
            printf("Press Enter\n");
            while(getchar() != '\n')
            ;
            break;

        case 5:
            removeAdmin(UserHead);
            printf("Press Enter\n");
            while(getchar() != '\n')
            ;
            break;

        case 0:
            return;
            
            break;

        default:
            printf("INVALID OPTION\t\t");
            printf("Press Enter\n");
            while(getchar() != '\n');
            break;
    }
    goto reviewAdminPrivilegesFlag;
    return;
}

void displayAdmins(USERNODE *head)
{
    printf("--------------------ADMINS--------------------\n");

    printf("USER ID\t\tUSERNAME\t\tADMIN STATUS\n");
    USERNODE *current = head;
    while(current->next != NULL)
    {
        current = current->next;
        if(current->user.u_admin)
        {
            printf("%lu\t%s\t\t\t\t%s\n", current->user.u_user_ID, current->user.user_name, (current->user.u_admin)?"YES":"NO");
        }
    }
    printf("----------------------------------------------\n");
    return;
}

void displayNonAdmins(USERNODE *head)
{
    printf("--------------------NON-ADMINS--------------------\n");

    printf("USER ID\tUSERNAME\t\tADMIN STATUS\n");
    USERNODE *current = head;
    while(current->next != NULL)
    {
        current = current->next;
        if(!(current->user.u_admin))
        {
            printf("%lu\t%s\t\t%s\n", current->user.u_user_ID, current->user.user_name, (current->user.u_admin)?"YES":"NO");
        }
    }
    printf("--------------------------------------------------\n");
    return;
}

void displayAdminView(USERNODE *head)
{
    printf("--------------------ADMIN VIEW--------------------\n");

    printf("USER ID\tUSERNAME\t\tADMIN STATUS\n");
    USERNODE *current = head;
    while(current->next != NULL)
    {
        current = current->next;
        printf("%lu\t%s\t\t%s\n", current->user.u_user_ID, current->user.user_name, (current->user.u_admin)?"YES":"NO");
    }
    printf("--------------------------------------------------\n");
    return;
}

int makeAdmin(USERNODE *head)
{
    int status = 1;     //Returns   0: No errors, works as expected
                        //          1: User Not Found
                        //          2: User Already Admin



    USERNODE *current = head;
    displayAdmins(head);
    unsigned long id = 0;
    printf("Enter UserID of account to grant admin priveleges to\n->");
    scanf(" %lu", &id);
    while(getchar() != '\n');

    while(current->next != NULL)
    {
        current = current->next;
        if(current->user.u_user_ID == id)
        {
            if(current->user.u_admin)
            {
                printf("This user is already an admin");
                saveUserList(UserHead);
                status = 2;
                return status;
            }
            else
            {
                current->user.u_admin = true;
                saveUserList(UserHead);
                status = 0;
                return status;
            }
        }
    }

    // Execution reaches here only if given ID does not match in the database
    printf("ERROR: User Not Found\t\t");
    printf("Press Enter");
    while(getchar() != '\n');
    return status;
}

//
int removeAdmin(USERNODE *head)
{
    int status = 1;     //Returns   0: No errors, works as expected
                        //          1: User Not Found
                        //          2: User Not Admin



    USERNODE *current = head;
    displayAdmins(head);
    unsigned long id = 0;
    printf("Enter UserID of account to remove admin priveleges from\n->");
    scanf(" %lu", &id);
    while(getchar() != '\n');

    while(current->next != NULL)
    {
        current = current->next;
        if(current->user.u_user_ID == id)
        {
            if(current->user.u_admin == false)
            {
                printf("This user is already not an admin");
                saveUserList(UserHead);
                status = 2;
                return status;
            }
            else
            {
                current->user.u_admin = false;
                saveUserList(UserHead);
                status = 0;
                return status;
            }
        }
    }

    // Execution reaches here only if given ID does not match in the database
    printf("ERROR: User Not Found\t\t");
    printf("Press Enter");
    while(getchar() != '\n');
    return status;
}

void addNewUser(USERNODE *head)
{
    char c;
    bool exists = false;
    char uname[60] = {};
    USERNODE *current = head;
    do
    {
        // choosing a username
        do{
            exists = false;
            printf("Choose A Username: ");
            scanf(" %30[^\n]", uname);
            while(getchar() != '\n')
            ;
            //checking if username exists or not
            current = head;
            while(current->next != NULL)
            {
                current = current->next;
                if(strcmp(current->user.user_name, uname) == 0)
                {
                    exists = true;
                    printf("\t*Username Already Exists!*\n");
                    break;
                }
            }
            current = head;
        }while(exists);

        // reaching end of linked list
        current = head;
        while(current->next != NULL)
        {
            current = current->next;
        }
        current->next = (USERNODE *)calloc(1, sizeof(USERNODE));
        current = current->next;

        strcpy(current->user.user_name, uname);

        printf("Choose A Password: ");
        scanf(" %29s", current->user.u_user_pwd);
        while(getchar() != '\n');

        printf("Enter Y/y to make user admin: ");
        char adminrights = 'N';
        scanf(" %c", &adminrights);
        adminrights = toupper(adminrights);
        if(adminrights == 'Y')
        {
            current->user.u_admin = true;
        }
        else
        {
            current->user.u_admin = false;
        }
        printf("---admin parameter assigned---\n");
        
        current->user.u_user_ID = generateUserID();
        /*
        //Assigning values 0 because no book issued yet
        current->user.u_user_ID = 0;
        current->user.u_book_ID = 0;
        current->user.u_issue_ID = 0;
        current->user.u_date_issue.tm_sec = 0;
        current->user.u_date_issue.tm_min = 0;
        current->user.u_date_issue.tm_hour = 0;
        current->user.u_date_issue.tm_mday = 0;
        current->user.u_date_issue.tm_mon = 0;
        current->user.u_date_issue.tm_year = 0;
        current->user.u_date_issue.tm_wday = 0;
        current->user.u_date_issue.tm_yday = 0;
        current->user.u_date_issue.tm_isdst = 0;
        */
        
        //fwrite(&user, sizeof(USER), 1, fp);
        saveUserList(UserHead);

        printf("Press Y/y to add another account: ");
        scanf(" %c", &c);
        while(getchar() != '\n')
        ;
    }
    while(c == 'Y'|| c == 'y');
    return;
}

void addNewBook(BOOKNODE *head)
{
    printf("addnewbook called\n");
    BOOKNODE *current = head;

    // moves to the end of the linked list
    while(current->next != NULL)
    {
        current = current->next;
    }

    time_t sec = 0.9 * time(NULL); // leave as 0.9 * ... for now, this allows us to add books so that date of issue is much earlier
    printf("sec = %lu \n", sec);
    printf("current time: %s\n", ctime(&sec));
    struct tm time_of_event = *(localtime(&sec));
    char choice;
    do
    {
        current->next = (BOOKNODE *)calloc(1, sizeof(BOOKNODE));
        current = current->next;
        printf("DO\n");
        printf("Enter Book Title:\n->");
        scanf(" %60[^\n]", current->book.b_book_title); //MAX_TITLE_LENGTH
        while(getchar() != '\n');

        printf("Enter Book Author\n->");
        scanf(" %30[^\n]", current->book.b_book_author);
        while(getchar() != '\n');

        current->book.b_book_ID = generateBookID();
        current->book.b_issue_ID = 0;
        current->book.b_user_ID = 0;
        current->book.b_book_status = 'A';

        sec = 0.9 * time(NULL);
        printf("sec = %lu \n", sec);
        printf("current time: %s\n", ctime(&sec));
        time_of_event = *(localtime(&sec));

        //date of arrival struct tm
        current->book.book_date_of_arrival.tm_sec = time_of_event.tm_sec;
        current->book.book_date_of_arrival.tm_min = time_of_event.tm_min;
        current->book.book_date_of_arrival.tm_hour = time_of_event.tm_hour;
        current->book.book_date_of_arrival.tm_mday = time_of_event.tm_mday;
        current->book.book_date_of_arrival.tm_mon = time_of_event.tm_mon;
        current->book.book_date_of_arrival.tm_year = time_of_event.tm_year;
        current->book.book_date_of_arrival.tm_wday = time_of_event.tm_wday;
        current->book.book_date_of_arrival.tm_yday = time_of_event.tm_yday;
        current->book.book_date_of_arrival.tm_isdst = time_of_event.tm_isdst;
        //date_issue struct tm
        current->book.b_date_issue.tm_sec = 0;
        current->book.b_date_issue.tm_min = 0;
        current->book.b_date_issue.tm_hour = 0;
        current->book.b_date_issue.tm_mday = 0;
        current->book.b_date_issue.tm_mon = 0;
        current->book.b_date_issue.tm_year = 0;
        current->book.b_date_issue.tm_wday = 0;
        current->book.b_date_issue.tm_yday = 0;
        current->book.b_date_issue.tm_isdst = 0;


        //fwrite(&book, sizeof(BOOK), 1, fp);
        saveBookList(BookHead);

        printf("Enter Y/y to enter another book: ");
        scanf(" %c", &choice);
        while(getchar() != '\n');

    }while((choice == 'Y') || (choice == 'y'));

    return;
}

unsigned long generateBookID()
{
    Book_ID_Counter++;
    saveCounters();
    return (Book_ID_Counter);
}

unsigned long generateIssueID()
{
    Issue_ID_Counter++;
    saveCounters();
    return (Issue_ID_Counter);
}

unsigned long generateUserID()
{
    User_ID_Counter++;

    saveCounters();
    return (User_ID_Counter);
}

void saveCounters()
{
    FILE *fp = fopen("counterdata.txt", "w");
    fprintf(fp, "%lu,%lu,%lu", Book_ID_Counter, Issue_ID_Counter, User_ID_Counter);
    fclose(fp);
    return;
}

// Give pointer to head BOOKNODE (likely initialized in main)
// Loads the entire library into memory using a dynamic linked list (utilizes higher reading speed of RAM and reduces file I/O process requirement)
BOOKNODE * loadLibrary(BOOKNODE *head)
{
    printf("loadlibrary called\n");
    loadCounters();
    printf("loadcounters successful\n");
    FILE *fp = fopen("books.txt", "r");
    printf("1");
    BOOK *book_load = calloc(1, sizeof(BOOK));
    //loadNextBook(fp, book_load);
    fread(book_load, sizeof(BOOK), 1, fp);
    printf("1");

    BOOKNODE *current;
    head->next = (BOOKNODE *)calloc(1, sizeof(BOOKNODE));
    current = head->next;
    current->next = NULL;
    printf("1");
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
            printf("1");
            current->next = (BOOKNODE *)calloc(1, sizeof(BOOKNODE));
            current = current->next;
            //loadNextBook(fp, book_load);
            fread(book_load, sizeof(BOOK), 1, fp);
        }
    }

    fclose(fp);

    return head;
}

void saveBookList(BOOKNODE *head)
{
    saveCounters();
    FILE *fp = fopen("books.txt", "w");
    BOOKNODE *current = head;

    while(current->next != NULL)
    {
        current = current->next;
        fwrite(&(current->book), sizeof(BOOK), 1, fp);
    }
    fclose(fp);
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
    //printf("%lu\t", atol(pointer));
    Issue_ID_Counter = atol(pointer);

    pointer = strtok(NULL, ",");
    User_ID_Counter = atol(pointer);

    printf("%lu\t%lu\t%lu\n", Book_ID_Counter, Issue_ID_Counter, User_ID_Counter);
    fclose(fp);

    return;
}

USERNODE * loadUsers(USERNODE *head)
{
    printf("loadusers called");
    FILE *fp = fopen("userdata.txt", "r");
    USER *user_load = (USER *)calloc(1, sizeof(USER));
    fread(user_load, sizeof(USER), 1, fp);

    USERNODE *current;
    //head = (USERNODE *)calloc(1, sizeof(USERNODE)); Already allocated in main
    head->next = (USERNODE *)calloc(1, sizeof(USERNODE));
    current = head->next;
    current->next = NULL;
    printf("\t1\n");
    while(1)
    {
        current->user.u_user_ID = user_load->u_user_ID;
        strcpy(current->user.user_name, user_load->user_name);
        current->user.u_book_ID = user_load->u_book_ID;
        current->user.u_issue_ID = user_load->u_issue_ID;
        strcpy(current->user.u_book_title, user_load->u_book_title);
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

    return head;
}

// RETURN 0: normal, 1: ID not found
int deleteUser(USERNODE *head, unsigned long id)
{
    USERNODE *current = head, *temp;
    
    //In case first node matches
    if(current->next->user.u_user_ID == id)
    {
        temp = current->next;
        current->next = current->next->next;
        free(temp);
        saveUserList(head);
        return 0;
    }
    
    // in case after first node matches
    while(current->next->next != NULL)
    {
        if(current->next->user.u_user_ID == id)
        {
            temp = current->next;
            current->next = current->next->next;
            free(temp);
            saveUserList(head);
            return 0;
        }
        current = current->next;
    }
    // saves userlist in memory to file
    saveUserList(head);
    return 1;
}

void saveCurrentUser(USERNODE *head, USER *CurrentUser)
{
    FILE *fp = fopen("userdata.txt", "w");
    USERNODE *current = head;
    while(current->next == NULL)
    {
        current = current->next;
        if(current->user.u_user_ID == CurrentUser->u_user_ID)
        {
            current->user.u_user_ID = CurrentUser->u_user_ID;
            strcpy(current->user.user_name, CurrentUser->user_name);
            current->user.u_book_ID = CurrentUser->u_book_ID;
            current->user.u_issue_ID = CurrentUser->u_issue_ID;
            strcpy(current->user.u_user_pwd, CurrentUser->u_user_pwd);
            current->user.u_admin = CurrentUser->u_admin;

            //date of issue struct tm part
            current->user.u_date_issue.tm_sec = CurrentUser->u_date_issue.tm_sec;
            current->user.u_date_issue.tm_min = CurrentUser->u_date_issue.tm_min;
            current->user.u_date_issue.tm_hour = CurrentUser->u_date_issue.tm_hour;
            current->user.u_date_issue.tm_mday = CurrentUser->u_date_issue.tm_mday;
            current->user.u_date_issue.tm_mon = CurrentUser->u_date_issue.tm_mon;
            current->user.u_date_issue.tm_year = CurrentUser->u_date_issue.tm_year;
            current->user.u_date_issue.tm_wday = CurrentUser->u_date_issue.tm_wday;
            current->user.u_date_issue.tm_yday = CurrentUser->u_date_issue.tm_yday;
            current->user.u_date_issue.tm_isdst = CurrentUser->u_date_issue.tm_isdst;
        }
    }

    saveUserList(head);

    return;
}

// saves userlist in memory to file
void saveUserList(USERNODE *head)
{
    //printf("entered saveUserList()\n");
    saveCounters();
    //printf("savecounters() done\n");
    FILE *fp = fopen("userdata.txt", "w");
    if(fp == NULL)
    {
        printf("file wasn't opened\n");
    }

    USERNODE *current = head;

    while(current->next != NULL)
    {
        current = current->next;
        fwrite(&current->user, sizeof(USER), 1, fp);
    }
    fclose(fp);
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

int titleCount(BOOKNODE *head, char *title)
{
    BOOKNODE *current = head;
    int count = 0;

    while(current->next != NULL)
    {
        current = current->next;
        if(strcmp(current->book.b_book_title, title) == 0)
        {
            count++;
        }
    }
    return count;
}

void printUserInfo(USER *user)
{
    printf("--------------------ACCOUNT INFO--------------------\n");
    printf("User ID: %lu\n", user->u_user_ID);
    printf("Username: %s\n", user->user_name);
    printf("Issued Book ID: %lu\n", user->u_book_ID);
    printf("Issued Book Title: %s\n", user->u_book_title/*getBookName(user->u_book_ID)*/);
    printf("Issue ID = %lu\n", user->u_issue_ID);
    char issueDate[128] = {};
    strftime(issueDate, 128, "%d-%b-%Y %H:%M:%S", &(user->u_date_issue));
    printf("Date Issued: %s\n", issueDate);
    printf("Book Notify %s\n(You'll be notified once this book becomes available in the library opon login)\n", user->u_requested);
    printf("Admin Privileges: %s\n", (user->u_admin)?"Yes":"No");
    printf("----------------------------------------------------\n");
    return;
}

/* // Was causing an error as it returns a local stack variable
char * getBookName(BOOKNODE *head, unsigned long bookID)
{
    char title[MAX_TITLE_LENGTH] = "BOOK NOT FOUND";
    BOOKNODE *current = head;
    while(current->next != NULL)
    {
        current = current->next;
        if(current->book.b_book_ID == bookID)
        {
            strcpy(title, current->book.b_book_title);
            return title;
        }
    }
    return title;
}
*/


void searchBookbyTitle(BOOKNODE *head)
{
    BOOKNODE *current = head;
    char title_search[MAX_TITLE_LENGTH];
    short cs_count=0; //To count number of closest searches
    short flag=0; //To end searching on finding the exact book or when no results
    char closest_search[MAX_CLOSE_TITLE_SEARCH][MAX_TITLE_LENGTH+50]; //To store MAX_CLOSE_TITLE_SEARCH closest matching title searches
    char same_title_found[MAX_SAME_TITLE][MAX_TITLE_LENGTH+50];
    short title_found=0;
    LAB1: //a label to return if <=3 chars entered
    printf("Search by title: ");
    scanf(" %60[^\n]", title_search);
    while(getchar() != '\n');

    //Restart search if less than three characters entered
    if(strlen(title_search)<4)
    {
        printf("Enter at least three characters to search\n");
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
                        strcat(closest_search[cs_count], "Available ");
                    else if(current->book.b_book_status=='R')
                        strcat(closest_search[cs_count], "Reserved ");
                    else if(current->book.b_book_status=='I')
                        strcat(closest_search[cs_count], "Not available (Issued ");
                    else
                        strcat(closest_search[cs_count], "Undefined ");

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


                strcpy(same_title_found[title_found],current->book.b_book_title);
                char curr_ID[11];
                sprintf(curr_ID, "%ld", current->book.b_book_ID); //Appending ID to display
                strcat(same_title_found[title_found],"\n\tID: ");
                strcat(same_title_found[title_found],curr_ID);
                strcat(same_title_found[title_found],"\n\tStatus: ");
                    if(current->book.b_book_status=='A')
                        strcat(same_title_found[title_found], "Available ");
                    else if(current->book.b_book_status=='R')
                        strcat(same_title_found[title_found], "Reserved ");
                    else if(current->book.b_book_status=='I')
                        strcat(same_title_found[title_found], "Not available (Issued ");
                    else
                        strcat(same_title_found[title_found], "Undefined ");

                title_found++;

                }
            }

        }

        if(flag==0&&cs_count==0)
        printf("No results found");

        //To print all closest searches (upto MAX_CLOSE_TITLE_SEARCH) when book is not found
        if(cs_count>0&&title_found==0)
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
        //To print all books found of same title (if any)
        if(title_found>0){
            title_found=0;
            while(strlen(same_title_found[cs_count])!=0)
        {
            printf("%hi. %s",(short)(title_found+1),same_title_found[title_found]);
            title_found++;
            if(title_found==MAX_SAME_TITLE)
            break; //Stop on reaching MAX_SAME_TITLE books of same title
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
    printf("AVAILABILITY\tBOOK ID\tAUTHOR\t\tTITLE\n");
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
        printf("%s\t%lu\t%s\t\t%s\n", availability, current->book.b_book_ID, current->book.b_book_author,current->book.b_book_title);
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
    scanf(" %30[^\n]", author_search);
    while(getchar() != '\n')
    ;

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
    scanf(" %11[^\n]", ID_search);
    while(getchar() != '\n');
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
