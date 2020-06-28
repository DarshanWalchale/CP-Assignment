// Library management system
//For BITS Pilani
// HEADER FILES
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

// DEFINES
#define MAX_TITLE_LENGTH 60 //Maximum length of title of book to be stored, can be reduced for optimisation, but cannot be increased (LIMITATION)
#define MAX_CLOSE_TITLE_SEARCH 30
#define MAX_CLOSE_AUTH_SEARCH 200 //Maximum number of closest seraches of authors to display
#define MAX_SAME_AUTHOR 100 //Maximum number of books of same author to display
#define MAX_CLOSE_ID 1000 //Maximum number of closest seraches of ID to display
#define MAX_SAME_TITLE 15 //Maximum number of books of same title

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

// Structure for storing information about a book
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

// Structure for storing information about a user
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

// for Linked List of Books in memory (BookList)
typedef struct BOOKNODE         // this is used to load the library into memory
{
    BOOK book;
    struct BOOKNODE *next;
} BOOKNODE;


// for Linked List of Users in memory (UserList)
typedef struct USERNODE         // this is used to load all userdata into memory
{
    USER user;
    struct USERNODE *next;
} USERNODE;


// GLOBAL VARIABLES
unsigned long Book_ID_Counter;           // 3943000000
unsigned long Issue_ID_Counter;          // 2019500000
unsigned long User_ID_Counter;           // 1791111000
USER Current_User;                       //capitalized cuz global variable
BOOKNODE *BookHead;
USERNODE *UserHead;
char lib[30]="BITS Pilani, Pilani Campus"; //Storing this library name


// PROTOTYPES
BOOKNODE * loadBookList(BOOKNODE *);
USERNODE * loadUsers(USERNODE *);
void loadCounters();
void returnBook();
void adminMenu();
void booksearchMenu();
void addNewUser(USERNODE *head);
void printUserInfo(USER *user);
void addNewBook(BOOKNODE *head);
void saveCounters();
void vendor_inv_Management();
void displayAllBooks(BOOKNODE *head);
void searchBookbyTitle(BOOKNODE *head);
void searchBookbyAuthor(BOOKNODE *head);
void searchBookbyID(BOOKNODE *head);
void newlyAddedBooks(BOOKNODE *head);
void notifications(BOOKNODE *head, USER *user);
void setCurrentUser(USER *);
void saveBookList(BOOKNODE *head);
void freeBookList(BOOKNODE *head);
void saveCurrentUser(USERNODE *);
void saveUserList(USERNODE *head);
void freeUserList(USERNODE *head);
void reviewAdminPrivileges(USERNODE *head);
void displayAdmins(USERNODE *head);
void displayNonAdmins(USERNODE *head);
void displayAdminView(USERNODE *head);
int deleteUser(USERNODE *head, unsigned long id);
int deleteBook(BOOKNODE *head, unsigned long id);
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
    // Allocating memory for Head variables for BookList and UserList
    BookHead = (BOOKNODE *)calloc(1, sizeof(BOOKNODE));
    UserHead = (USERNODE *)calloc(1, sizeof(USERNODE));

    // Loading Counters for generating BookID, UserID, and IssueID
    loadCounters();
    printf("loadcounters done in main\n");

    // These were used for creating the initial entries in books.txt and userdata.txt
    //
    //addNewBook(BookHead); //Don't uncomment
    //addNewUser(UserHead); //Don't uncomment


    // /*
    //printf("Program start\n");
    BookHead = loadBookList(BookHead);
    printf("---Books database loaded into memory successfully---\n");
    UserHead = loadUsers(UserHead);
    printf("---Users successfully loaded---\n");

    // for testing, to make sure books and users loaded correctly
    displayAllBooks(BookHead);
    displayAdminView(UserHead);

    //printf("sizeof(BOOKNODE) = %lu\nsizeof(USER) = %lu\nsizeof(int) = %lu\n", sizeof(BOOKNODE), sizeof(USER), sizeof(int));   // for determining memory required
    //printf("Counters: %lu,%lu,%lu\n", generateBookID(), generateIssueID(), generateUserID()); // for testing counters

    // Prompts user to login, continues into rest of program if successfully logged in, skips to program termination if failed attemots are 5 or more
    if(welcomeScreen(UserHead) == 0)
    {
        notifications(BookHead, &Current_User);
        while(menu() != 0)
        ;
    }
    // */

    // program termination sequence

    // saves counter variables
    saveCounters();

    // saves list of books to books.txt
    saveBookList(BookHead);
    printf("---BookList Saved---\n");

    // frees dynamically allocated linked list of books, BookList, from memory
    freeBookList(BookHead);
    printf("---BookList Freed---\n");

    //saves list of users to userdata.txt
    saveUserList(UserHead);
    printf("---UserList Saved---\n");

    // frees dynamically allocated linked list of users, UserList, from memory
    freeUserList(UserHead);
    printf("---BookList Freed---\n");

    return 0;
}


// Welcome screen for user, displays newly added books and prompts user to login
// Return Value-    0: login successful
//                  1: Login Attempt Limit exceeded
int welcomeScreen(USERNODE *head)
{
    int choice;
    printf("\n\n\n\n\n\t\t\tLIBRARY MANAGEMENT SYSTEM");
    printf("\n\t\t\t*************************\n");

    // displays new additions to the library
    newlyAddedBooks(BookHead);

    // for traversing linked list UserList
    USERNODE *current = head;

    printf("\n\t\t\tPress Enter to login");
    while(getchar() != '\n');//To empty Input Buffer

    static int loginerrorcount = 0; // if user can't register or login successfully in 5 attempts, just exit
    char uName[30];                 // local char array to store username entered
    char pwd[30];                   // local char array to store password entered

    do
    {
        printf("Username: ");
        scanf(" %29s", uName);
        while(getchar() != '\n')//To empty Input Buffer
        ;

        printf("Password: ");
        scanf(" %29s", pwd);
        while(getchar() != '\n')//To empty Input Buffer
        ;

        // traversing UserList and comparing login details
        current = head;
        while (current->next != NULL)
        {
            current = current->next;

            //checks for username match
            if(strcmp(current->user.user_name, uName) == 0)
            {
                printf("Username Matched\n");

                // checks for password match
                if(strcmp(current->user.u_user_pwd, pwd) == 0)
                {
                    printf ("Password Matched\n");

                    // copies details of current user to a global variable
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
        printf("\n\t\t---ERROR:Incorrect Credentials---\n\t\t\tPress Enter to retry");
        while(getchar() != '\n')//To empty Input Buffer
        ;
    }while(loginerrorcount <= 5);

    printf("Too many unsuccessful logins\n");
    return 1;
}

// Main Menu for the program
//Return Value      0: exit program
//                  1: dont exit program
int menu(){
    int choice, option; // variables for taking input
    mainmenuflag: // label to return to Main menu

    printf("\n--------------------MAIN MENU--------------------\n");
    printf("1. Search Books\n");
    printf("2. Book Transaction\n");
    printf("3. Account Info\n");
    if(Current_User.u_admin)
    {
        printf("9. Admin Menu\n"); // Only printed if current user is an admin
    }
    printf("0. Exit from Library Portal\n");

    printf("Enter your choice: ");
    scanf(" %d", &choice);
    while(getchar() != '\n')//To empty Input Buffer
    ;

    switch(choice)
    {
        case 1:
            booksearchMenu(BookHead);
            goto mainmenuflag;
            break;

        case 2:
            transaction:  // label to reach transaction menu
            printf("\n----------Book Transaction Menu----------\n");
            printf("1. Checkout a Book\n");
            printf("2. Return Book\n");
            printf("0. Return Back to Main Menu\n");

            printf("Enter Choice: ");
            scanf("%d", &option);
            while(getchar() != '\n') // To clear input buffer
            ;

            switch(option)
            {
                case 1:                     //checkout book

                printf("First Search for the book with our Search Feature\n");
                // Lists book titles and closest book titles for user to see and make sure of spelling and case before entering
                searchBookbyTitle(BookHead);

                char title[MAX_TITLE_LENGTH]; // to save title entered by user temporarily

                printf("\nEnter Book Title to Checkout (Case Sensitive)\n ->");
                scanf(" %60[^\n]", title);      //MAX_TITLE_LENGTH
                while(getchar() != '\n') // To empty input buffer
                ;

                int result = checkout(title);
                if(result == 0)             //book checked out successfully
                {
                    printf("Your Book has been successfully issued\n");
                    printf("Press Enter to return to Main Menu\n");
                    while(getchar() != '\n');//To empty Input Buffer

                    goto mainmenuflag;

                }
                else if(result == 1)          // book to checkout not found
                {
                    printf("No such Book Title\n");
                    printf("Press Enter to return to Transaction Menu\n");
                    while(getchar() != '\n');//To empty Input Buffer

                    goto transaction;
                }
                else                        // user asked to to notify or book already issued prior
                {
                    printf("Press Enter to return to Transaction Menu\n");
                    while(getchar() != '\n');//To empty Input Buffer
                    goto transaction;
                }
                break;

                case 2:                     //return book

                    returnBook();
                    printf("\nPress Enter to return to Transaction Menu\n");
                    while(getchar() != '\n');//To empty Input Buffer
                    goto transaction;
                    break;

                case 0:
                    goto mainmenuflag;
                    break;

                default:
                printf("\n\n\t\t\t\tINVALID OPTION");
                printf("\n\n\t\t\tPress Enter to return to the User Menu\n");
                while(getchar() != '\n')//To empty Input Buffer
                ;
                goto mainmenuflag;
             }

            break;

        case 3:                                     // provides current user information
            printUserInfo(&Current_User);
            printf("Press Enter to return to the Main Menu");
            while (getchar() != '\n');
            break;

        case 9:                                     // not accessable to non-admins
            if(Current_User.u_admin)
            {
                adminMenu();
            }
            else
            {
                printf("---ERROR: You Do Not Have Admin Privileges---\n");
                printf("Press Enter to return to the Main Menu");
                while (getchar() != '\n');
                goto mainmenuflag;
            }
            break;

        case 0:
            return 0;
            break;

        default:
            printf("\n\n\t\t\t\tINVALID OPTION");
            printf("\n\n\t\t\tPress Enter to return to the User Menu\n");

            while(getchar() != '\n');//To empty Input Buffer
            goto mainmenuflag;

    }
    goto mainmenuflag;
    return 1;
}

// checkout handles checking out given a book title string, it also handles saving book title to notify on next login
// title is the name of the book passed by the user
// Return Value     0: book found, available, and assigned successfully,
//                  1: no book title found,
//                  2: book title found, but not available, option given to set as alert for this book.
//                  3: User already has a book issued
int checkout(char *title)
{

    int status = 1;

    if(Current_User.u_book_ID != 0)
    {
        printf("---ERROR: You have a Book Already Issued, Return It First---\n");
        return 3;
    }

    BOOKNODE *current = BookHead;

    // traversing BookList
    while(current->next != NULL)
    {
        current = current->next;

        // if title in current book matches given title
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
                strcpy(Current_User.u_book_title, current->book.b_book_title);
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
                saveCurrentUser(UserHead);
                saveUserList(UserHead);
                status = 0;
                break;
            }
            else
            {
                status = 2;
            }
        }
    }
    //if book title was found but not available
    if(status == 2)
    {
        char choice;
        do
        {
            printf("All copies of the book you've requested are currently issued by other library members!\n");
            printf("Would you like to be notified if the book becomes available? (Y/N) \n(notified on login when book is available once, only 1 book can be notified at a time)\nChoice: ");
            scanf(" %c", &choice);
            while(getchar() != '\n');//To empty Input Buffer
            choice = toupper(choice);
            switch (choice)
            {
                case 'Y':
                    strcpy(Current_User.u_requested, title);
                    saveCurrentUser(UserHead);
                    saveUserList(UserHead);
                    printf("OK, you'll be notified if the book is available next time you log in\n");
                    break;

                case 'N':
                    printf("Ok, you won't be notified\n");
                    break;

                default:
                    printf("INVALID RESPONSE\t\t");
                    printf("Press Enter\n");
                    while(getchar() != '\n')//To empty Input Buffer
                    ;
                    break;
            }
        }while(!(choice == 'Y' || choice == 'N' || choice == 'y'));
    }
    return status;
}

// for Returning a book
void returnBook()
{
    // case if no book is issued by the Current User
    if(Current_User.u_book_ID == 0)
    {
        printf("No Book Issued currently!\n");
        saveCurrentUser(UserHead);
        saveUserList(UserHead);
        return;
    }

    // prints currently issued book title and asks for confirmation to return
    printf("Currently Issued Book\n -> %s\n", Current_User.u_book_title);
    printf("Press Y to return currently issued book\n");
    char choice;
    BOOKNODE *current = BookHead;


    scanf(" %c", &choice);
    while(getchar() != '\n');// To empty Input Buffer
    choice = toupper(choice);
    switch (choice)
    {
        case 'Y' :                                                          //return
            // traverses through the Booklist
            while(current->next != NULL)
            {
                current = current->next;

                // upon finding a matching title
                if(strcmp(current->book.b_book_title, Current_User.u_book_title) == 0)
                {
                     // checking if user ID matches
                     if(current->book.b_user_ID == Current_User.u_user_ID)
                     {
                         // Reassign parameters to defaults
                         current->book.b_issue_ID = 0;
                         current->book.b_user_ID = 0;
                         current->book.b_book_status = 'A';
                         Current_User.u_book_ID = 0;
                         Current_User.u_issue_ID = 0;
                         strcpy(Current_User.u_book_title, "\0");

                         saveBookList(BookHead);
                         saveCurrentUser(UserHead);
                         saveUserList(UserHead);
                         return;
                     }
                }
            }

            break;

        default:
            printf("Press Enter to return to Menu\n");
            while(getchar() != '\n');//To empty Input Buffer
        return;
    }
    return;
}

// Menu driven, options given to search books based on different parameters, title, author, and bookID,
// also applies a basic autocorrect
void booksearchMenu()
{
    int choice;

    booksearch:
    printf("\n----------Book Search Menu----------\n");

    printf("1. Search Book by Title\n");
    printf("2. Search Book by Author\n");
    printf("3. Search Book by Book ID\n");
    printf("4. Display All Books\n");
    printf("0. Return to Main Menu\n");

    printf("Enter choice: ");
    scanf(" %d", &choice);
    while(getchar() != '\n')//To empty Input Buffer
    ;

    switch(choice)
    {
        case 1:
            searchBookbyTitle(BookHead);
            printf("\nPress Enter to return to the Search menu");
            while(getchar() != '\n');//To empty Input Buffer
            goto booksearch;
            break;

        case 2:
            searchBookbyAuthor(BookHead);
            printf("\nPress Enter to return to the Search menu");
            while(getchar() != '\n');//To empty Input Buffer
            goto booksearch;
            break;

        case 3:
            searchBookbyID(BookHead);
            printf("Press Enter to return to the Search menu");
            while(getchar() != '\n');//To empty Input Buffer
            goto booksearch;
            break;

        case 4:
            displayAllBooks(BookHead);
            printf("Press Enter to return to the Search menu\n");
            while(getchar() != '\n');//To empty Input Buffer
            goto booksearch;
            break;

        case 0:
            return;
            break;

        default:
            printf("\n\n\t\t\t\tINVALID OPTION");
            printf("\n\n\t\t\tPress Enter to re-Enter the choice");
            while(getchar() != '\n');//To empty Input Buffer
            goto booksearch;
            break;
    }
    return;
}

// Menu for Admin Actions
void adminMenu()
{
    int choice;
    unsigned long id; // for deleting user or book
    ADMIN:
    printf("\n\n--------------------ADMIN MENU--------------------\n");
    printf("1. Add New Books to Library\n");
    printf("2. Add Library members\n");
    printf("3. Delete Library members\n");
    printf("4. Count books of a particular title\n");
    printf("5. Vendor / Library requests\n");
    printf("6. Review Admin Privileges\n");
    printf("7. Delete a book from Library\n");
    printf("0. Return to Main Menu\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);
    while(getchar() != '\n');//To empty Input Buffer

    switch(choice)
    {
        case 1:
            addNewBook(BookHead);
            break;

        case 2:
            addNewUser(UserHead);

            printf("\nPress enter to return to the Admin menu");
            while(getchar() != '\n');//To empty Input Buffer
            goto ADMIN;
            break;

        case 3:
            displayAdminView(UserHead);
            printf("Enter uUsr ID of the user you want to remove: ");
            scanf(" %lu", &id);
            while(getchar() != '\n')//To empty Input Buffer
            ;
            deleteUser(UserHead, id);

            printf("\nPress enter to return to the Admin menu");
            while(getchar() != '\n');//To empty Input Buffer
            goto ADMIN;
            break;

        case 4:
            // takes title string from user and searches in BookList
            printf("Enter title: ");
            char title[MAX_TITLE_LENGTH];
            scanf(" %60[^\n]", title); //MAX_TITLE_LENGTH
            while(getchar() != '\n');//To empty Input Buffer

            printf("Number of books by the title \"%s\" = %d", title, titleCount(BookHead, title));

            printf("\nPress enter to return to the Admin menu");
            while(getchar() != '\n');//To empty Input Buffer
            goto ADMIN;
            break;

        case 5:
            vendor_inv_Management();
            printf("\nPress enter to return to the Admin menu");
            while(getchar() != '\n');//To empty Input Buffer
            goto ADMIN;
            break;

        case 6:
            reviewAdminPrivileges(UserHead);
            goto ADMIN;
            break;

        case 7:
            // takes input for the ID of the book to remove
            printf("Enter Book ID of the book you want to remove: ");
            scanf(" %lu", &id);
            while(getchar() != '\n')//To empty Input Buffer
            ;
            deleteBook(BookHead, id);

            printf("\nPress enter to return to the Admin menu");
            while(getchar() != '\n');//To empty Input Buffer
            goto ADMIN;
            break;

        case 0:
            return;
            break;

        default:
        printf("\n\n\t\t\t\tINVALID OPTION");
        printf("\n\n\t\t\tPress Enter");
        while(getchar() != '\n');//To empty Input Buffer
        goto ADMIN;

    }
    goto ADMIN;
    return;
}

// To Change admin status of members
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
    printf("Enter your choice: ");
    scanf("%d", &choice);
    while(getchar() != '\n');//To empty Input Buffer

    switch(choice)
    {
        case 1:
            displayAdmins(UserHead);
            printf("Press Enter\n");
            while(getchar() != '\n')//To empty Input Buffer
            ;
            break;

        case 2:
            displayNonAdmins(UserHead);
            printf("Press Enter\n");
            while(getchar() != '\n')//To empty Input Buffer
            ;
            break;

        case 3:
            displayAdminView(UserHead);
            printf("Press Enter\n");
            while(getchar() != '\n')//To empty Input Buffer
            ;
            break;

        case 4:
            makeAdmin(UserHead);
            printf("Press Enter\n");
            while(getchar() != '\n')//To empty Input Buffer
            ;
            break;

        case 5:
            removeAdmin(UserHead);
            printf("Press Enter\n");
            while(getchar() != '\n')//To empty Input Buffer
            ;
            break;

        case 0:
            return;

            break;

        default:
            printf("INVALID OPTION\t\t");
            printf("Press Enter\n");
            while(getchar() != '\n');//To empty Input Buffer
            break;
    }
    goto reviewAdminPrivilegesFlag;
    return;
}

// Displays all users, User ID, Username, and Admin Status
// who have admin privileges
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
            printf("%lu\t%s\t\t\t%s\n", current->user.u_user_ID, current->user.user_name, (current->user.u_admin)?"YES":"NO");
        }
    }
    printf("----------------------------------------------\n");
    return;
}

// Displays all users, User ID, Username, and Admin Status
// who do not have admin privileges
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

// Displays all users, User ID, Username, and Admin Status
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

//Returns   0: No errors, works as expected
//          1: User Not Found
//          2: User Already Admin
int makeAdmin(USERNODE *head)
{
    int status = 1;
    USERNODE *current = head;
    displayAdmins(head);
    unsigned long id = 0;
    printf("Enter UserID of account to grant admin priveleges to\n->");
    scanf(" %lu", &id);
    while(getchar() != '\n');//To empty Input Buffer

    // traverses UserList
    while(current->next != NULL)
    {
        current = current->next;
        if(current->user.u_user_ID == id)
        {
            // if the user is already an admin
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
    while(getchar() != '\n');//To empty Input Buffer
    return status;
}

// removes admin privileges from a given user ID
// Returns   0: No errors, works as expected
//          1: User Not Found
//          2: User Not Admin
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
    while(getchar() != '\n');//To empty Input Buffer

    // travserses UserList
    while(current->next != NULL)
    {
        current = current->next;
        if(current->user.u_user_ID == id)
        {
            // In case the user is already not an admin
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
    while(getchar() != '\n');//To empty Input Buffer
    return status;
}

// Gets necessary details from user, assigns rest of the details automatically
// Adds new node to UserList and then saves the list to userdata.txt
void addNewUser(USERNODE *head)
{
    char c, adminrights = 'N';
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
            while(getchar() != '\n')//To empty Input Buffer
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

        // creates a new node
        current->next = (USERNODE *)calloc(1, sizeof(USERNODE));
        current = current->next;

        // assigning username adnd other details
        strcpy(current->user.user_name, uname);
        printf("Choose A Password: ");
        scanf(" %29s", current->user.u_user_pwd);
        while(getchar() != '\n');//To empty Input Buffer

        printf("Enter Y/y to make user admin: ");
        adminrights = 'N';
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

        // generating and assigning a User ID
        current->user.u_user_ID = generateUserID();

        //Assigning values 0 because no book issued yet
        current->user.u_user_ID = generateUserID();
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


        saveUserList(UserHead);

        printf("Press Y/y to add another account: ");
        scanf(" %c", &c);
        while(getchar() != '\n')//To empty Input Buffer
        ;
    }
    while(c == 'Y'|| c == 'y');
    return;
}

// Gets necessary details from user, assigns rest of the details automatically
// Adds new node to BookList and then saves the list to books.txt
void addNewBook(BOOKNODE *head)
{
    BOOKNODE *current = head;

    // moves to the end of the linked list
    while(current->next != NULL)
    {
        current = current->next;
    }

    time_t sec = time(NULL);
    printf("sec = %lu \n", sec);
    printf("current time: %s\n", ctime(&sec));
    struct tm time_of_event = *(localtime(&sec)); // For assigning date of arrival to the library
    char choice;
    do
    {
        // creates a new node
        current->next = (BOOKNODE *)calloc(1, sizeof(BOOKNODE));
        current = current->next;

        // getting and assigning details
        printf("Enter Book Title:\n->");
        scanf(" %60[^\n]", current->book.b_book_title); //MAX_TITLE_LENGTH
        while(getchar() != '\n');//To empty Input Buffer

        printf("Enter Book Author\n->");
        scanf(" %30[^\n]", current->book.b_book_author);
        while(getchar() != '\n');//To empty Input Buffer

        // generates and assigns a unique Book ID
        current->book.b_book_ID = generateBookID();
        current->book.b_issue_ID = 0;
        current->book.b_user_ID = 0;
        current->book.b_book_status = 'A';

        sec = time(NULL);
        printf("sec = %lu \n", sec);
        printf("current time: %s\n", ctime(&sec));
        time_of_event = *(localtime(&sec));

        // date of arrival struct tm
        current->book.book_date_of_arrival.tm_sec = time_of_event.tm_sec;
        current->book.book_date_of_arrival.tm_min = time_of_event.tm_min;
        current->book.book_date_of_arrival.tm_hour = time_of_event.tm_hour;
        current->book.book_date_of_arrival.tm_mday = time_of_event.tm_mday;
        current->book.book_date_of_arrival.tm_mon = time_of_event.tm_mon;
        current->book.book_date_of_arrival.tm_year = time_of_event.tm_year;
        current->book.book_date_of_arrival.tm_wday = time_of_event.tm_wday;
        current->book.book_date_of_arrival.tm_yday = time_of_event.tm_yday;
        current->book.book_date_of_arrival.tm_isdst = time_of_event.tm_isdst;
        // date_issue struct tm
        current->book.b_date_issue.tm_sec = 0;
        current->book.b_date_issue.tm_min = 0;
        current->book.b_date_issue.tm_hour = 0;
        current->book.b_date_issue.tm_mday = 0;
        current->book.b_date_issue.tm_mon = 0;
        current->book.b_date_issue.tm_year = 0;
        current->book.b_date_issue.tm_wday = 0;
        current->book.b_date_issue.tm_yday = 0;
        current->book.b_date_issue.tm_isdst = 0;


        saveBookList(head);

        printf("Enter Y/y to enter another book: ");
        scanf(" %c", &choice);
        while(getchar() != '\n');//To empty Input Buffer

    }while((choice == 'Y') || (choice == 'y'));

    saveBookList(head);
    return;
}

// increments Book ID counter and saves couners, then returns the ID to the calling function
unsigned long generateBookID()
{
    Book_ID_Counter++;
    saveCounters();
    return (Book_ID_Counter);
}

// increments Issue ID counter and saves couners, then returns the ID to the calling function
unsigned long generateIssueID()
{
    Issue_ID_Counter++;
    saveCounters();
    return (Issue_ID_Counter);
}

// increments User ID counter and saves couners, then returns the ID to the calling function
unsigned long generateUserID()
{
    User_ID_Counter++;

    saveCounters();
    return (User_ID_Counter);
}

// Saves all 3 counter variables to a file calles counterdata.txt
void saveCounters()
{
    FILE *fp = fopen("counterdata.txt", "w");
    fprintf(fp, "%lu,%lu,%lu", Book_ID_Counter, Issue_ID_Counter, User_ID_Counter);
    fclose(fp);
    return;
}

// Give pointer to head BOOKNODE (initialized in main)
// Loads the entire library into memory using a dynamic linked list (utilizes higher reading speed of RAM and reduces file I/O process requirement)
// Returns head pointer to the Linked List BookList created
BOOKNODE * loadBookList(BOOKNODE *head)
{
    FILE *fp = fopen("books.txt", "r");
    BOOK *book_load = calloc(1, sizeof(BOOK)); // tmeporary object pointer for storing each book read
    BOOKNODE *current = head;

    // loop ends when no more books can be read
    while(fread(book_load, sizeof(BOOK), 1, fp))
    {
        // creates a new node in BookList
        current->next = (BOOKNODE *)calloc(1, sizeof(BOOKNODE));
        current = current->next;

        // copies info into the new node
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
    }

    fclose(fp);
    free(book_load);

    return head;
}

// Saves book in each node to the file from head to last node
void saveBookList(BOOKNODE *head)
{
    saveCounters();
    FILE *fp = fopen("books.txt", "w");
    BOOKNODE *current = head;

    while(current->next != NULL)
    {
        // saves each node's book struct
        current = current->next;
        fwrite(&(current->book), sizeof(BOOK), 1, fp);
    }
    fclose(fp);
    return;
}

// Recursive function to free the entire linked list BookList
void freeBookList(BOOKNODE *head)
{
    if(head->next == NULL)
    {
        free(head);
        return;
    }
    else
    {
        freeBookList(head->next);
    }
    return;
}

// reads the counter variables from counterdata.txt
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

// Give pointer to head USERNODE (initialized in main)
// Loads the entire list of users into memory using a dynamic linked list (utilizes higher reading speed of RAM and reduces file I/O process requirement)
// Returns head pointer to the Linked List UserList created
USERNODE * loadUsers(USERNODE *head)
{
    FILE *fp = fopen("userdata.txt", "r");
    USER *user_load = (USER *)calloc(1, sizeof(USER)); // tmeporary object pointer for storing each user read
    USERNODE *current = head;

    // loop ends when no more users can be read
    while(fread(user_load, sizeof(USER), 1, fp))
    {
        // creates a new node in UserList
        current->next = (USERNODE *)calloc(1, sizeof(USERNODE));
        current = current->next;

        // copies details
        current->user.u_user_ID = user_load->u_user_ID;
        strcpy(current->user.user_name, user_load->user_name);
        current->user.u_book_ID = user_load->u_book_ID;
        current->user.u_issue_ID = user_load->u_issue_ID;
        strcpy(current->user.u_book_title, user_load->u_book_title);
        strcpy(current->user.u_user_pwd, user_load->u_user_pwd);
        current->user.u_admin = user_load->u_admin;
        strcpy(current->user.u_requested, user_load->u_requested);
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
    }
    fclose(fp);
    free(user_load);

    return head;
}

// links the node previous to the node to delete to the node after the node to delete, and then frees that node memory
// Return Value     0: normal, no errors
//                  1: ID not found
int deleteBook(BOOKNODE *head, unsigned long id)
{
    BOOKNODE *current = head, *temp;

    //In case first node matches
    if(current->next->book.b_book_ID == id)
    {
        temp = current->next;
        current->next = current->next->next;
        free(temp);
        saveBookList(head);
        return 0;
    }

    // in case After first node matches
    while(current->next->next != NULL)
    {
        // removes matching node from list and frees its memory
        if(current->next->book.b_book_ID == id)
        {
            temp = current->next;
            current->next = current->next->next;
            free(temp);
            saveBookList(head);
            return 0;
        }
        current = current->next;
    }
    // saves booklist in memory to file
    saveBookList(head);
    // execution only reaches here if ID not found
    printf("\t---ERROR: ID not found\n");
    return 1;
}

// links the node previous to the node to delete to the node after the node to delete, and then frees that node memory
// Return Value     0: normal, no errors
//                  1: ID not found
int deleteUser(USERNODE *head, unsigned long id)
{
    USERNODE *current = head, *temp;

    //In case first node matches
    if(current->next->user.u_user_ID == id)
    {
        // removes matching node from list and frees its memory
        temp = current->next;
        current->next = current->next->next;
        free(temp);
        saveUserList(head);
        return 0;
    }

    // in case After first node matches
    while(current->next->next != NULL)
    {
        if(current->next->user.u_user_ID == id)
        {
            // removes matching node from list and frees its memory
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
    // execution only reaches here if ID not found
    printf("\t---ERROR: ID not found\n");
    return 1;
}

// Copies data from global struct cariable Current_User to the appropriate place in the UserList
void saveCurrentUser(USERNODE *head)
{
    USERNODE *current = head;

    // traverses UserList
    while(current->next != NULL)
    {
        current = current->next;

        // if ID matches Current_User ID
        if(current->user.u_user_ID == Current_User.u_user_ID)
        {
            current->user.u_user_ID = Current_User.u_user_ID;
            strcpy(current->user.user_name, Current_User.user_name);
            current->user.u_book_ID = Current_User.u_book_ID;
            current->user.u_issue_ID = Current_User.u_issue_ID;
            strcpy(current->user.u_book_title, Current_User.u_book_title);
            strcpy(current->user.u_user_pwd, Current_User.u_user_pwd);
            current->user.u_admin = Current_User.u_admin;
            strcpy(current->user.u_requested, Current_User.u_requested);

            //date of issue struct tm part
            current->user.u_date_issue.tm_sec = Current_User.u_date_issue.tm_sec;
            current->user.u_date_issue.tm_min = Current_User.u_date_issue.tm_min;
            current->user.u_date_issue.tm_hour = Current_User.u_date_issue.tm_hour;
            current->user.u_date_issue.tm_mday = Current_User.u_date_issue.tm_mday;
            current->user.u_date_issue.tm_mon = Current_User.u_date_issue.tm_mon;
            current->user.u_date_issue.tm_year = Current_User.u_date_issue.tm_year;
            current->user.u_date_issue.tm_wday = Current_User.u_date_issue.tm_wday;
            current->user.u_date_issue.tm_yday = Current_User.u_date_issue.tm_yday;
            current->user.u_date_issue.tm_isdst = Current_User.u_date_issue.tm_isdst;
        }
    }
    saveUserList(head);
    return;
}

// Saves book in each node to the file from head to last node
void saveUserList(USERNODE *head)
{
    FILE *fp = fopen("userdata.txt", "w");

    USERNODE *current = head;

    while(current->next != NULL)
    {
        // saves each node's user struct
        current = current->next;
        fwrite(&current->user, sizeof(USER), 1, fp);
    }
    fclose(fp);

    return;
}

// frees dynamically allocated linked list of users' data from memory
// Recursive function to free the entire linked list UserList
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

// returns number of nodes in BookList having the title as passed
int titleCount(BOOKNODE *head, char *title)
{
    BOOKNODE *current = head;
    int count = 0;

    // traverses BookList
    while(current->next != NULL)
    {
        current = current->next;
        // if title matches
        if(strcmp(current->book.b_book_title, title) == 0)
        {
            count++;
        }
    }
    return count;
}

// Displays All Information about the Current_User
void printUserInfo(USER *user)
{
    printf("\n--------------------ACCOUNT INFO--------------------\n");
    printf("User ID:\t\t%lu\n", user->u_user_ID);
    printf("Username:\t\t%s\n", user->user_name);

    printf("Issued Book ID:\t\t");

    // If no book issued
    if(user->u_book_ID == 0)
    {
        printf("No Book Issued\n");
        printf("Issued Book Title:\tNo Book Issued\n");
        printf("Issue ID:\t\tNo Book Issued\n");
        char issueDate[128] = {};
        strftime(issueDate, 128, "%d-%b-%Y %H:%M:%S", &(user->u_date_issue));
        if(strcmp(issueDate, "00-Jan-1900 00:00:00") == 0)
        {
            printf("Date Last Issued:\tNever Issued a Book\n");
        }
        else
        {
            printf("Date Last Issued:\t%s\n", issueDate);
        }
    }
    // if book is issued
    else
    {
        printf("%lu\n", user->u_book_ID);
        printf("Issued Book Title:\t%s\n", user->u_book_title/*getBookName(user->u_book_ID)*/);
        printf("Issue ID:\t\t%lu\n", user->u_issue_ID);
        char issueDate[128] = {};
        strftime(issueDate, 128, "%d-%b-%Y %H:%M:%S", &(user->u_date_issue));
        printf("Date Issued:\t\t%s\n", issueDate);
    }
    // if no book requested
    if(strcmp(user->u_requested, "\0") == 0)
    {
        printf("Book Notify:\t\tNo Book to Notify\n");
    }
    // if book is requested
    else
    {
        printf("Book Notify:\t\t%s\n\t\t\t(You'll be notified once this book becomes available in the library opon login)\n", user->u_requested);
    }
    printf("Admin Privileges:\t%s\n", (user->u_admin)?"Yes":"No");
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

//Function takes in pointer to head node and travesrses through all books to search by title.
void searchBookbyTitle(BOOKNODE *head)
{
    BOOKNODE *current = head; //Initialising head pointer of linked list of all books to BOOKNODE type pointer current
    char title_search[MAX_TITLE_LENGTH];
    char title_matched[MAX_TITLE_LENGTH]; //To store exact case (as in books database) search matched book
    short cs_count=0; //To count number of closest searches
    char closest_search[MAX_CLOSE_TITLE_SEARCH][MAX_TITLE_LENGTH+50]; //To store MAX_CLOSE_TITLE_SEARCH closest matching title searches
    char same_title_found[MAX_SAME_TITLE][MAX_TITLE_LENGTH+50];
    short title_found=0;
    LAB1: //a label to return if <=3 chars entered
    printf("\nSearch by title: ");
    scanf(" %60[^\n]", title_search);
    while(getchar() != '\n');//To empty Input Buffer

    //Restart search if less than three characters entered
    if(strlen(title_search)<4)
    {
        printf("Enter at least four characters to search\n");
        goto LAB1;
    }

    while(current->next != NULL) //while loop to access all books in linked list of books
    {
        current=current->next;

        for(int j=0;j<MAX_TITLE_LENGTH;j++) //Iterating through all characters of a single book title
        {

            //Skip the current book if atleast 3 characters don't match
        if((tolower(title_search[j])!=tolower(current->book.b_book_title[j]))&&j<3){
            break;
            }

            //If atleast one character does not match, and atleast 3 characters match
            //store the closest matching search (book) and skip current iteration
        else if(tolower(title_search[j])!=tolower(current->book.b_book_title[j])&&j<strlen(current->book.b_book_title)&&j>2){

            if(cs_count<MAX_CLOSE_TITLE_SEARCH){ //To ensure at most MAX_CLOSE_TITLE_SEARCH closest searches are stored and displayed
                //Appending ID, author, status to display for the current book
                strcpy(closest_search[cs_count],current->book.b_book_title);
                char curr_ID[11];
                sprintf(curr_ID, "%ld", current->book.b_book_ID);
                strcat(closest_search[cs_count],"\n\tID: ");
                strcat(closest_search[cs_count],curr_ID);
                strcat(closest_search[cs_count],"\n\tAuthor: ");
                 strcat(closest_search[cs_count],current->book.b_book_author);
                strcat(closest_search[cs_count],"\n\tStatus: ");
                    if(current->book.b_book_status=='A')
                        strcat(closest_search[cs_count], "Available ");
                    else if(current->book.b_book_status=='R')
                        strcat(closest_search[cs_count], "Reserved ");
                    else if(current->book.b_book_status=='I')
                        strcat(closest_search[cs_count], "Not available (Issued)");
                    else
                        strcat(closest_search[cs_count], "Undefined ");

                 strcat(closest_search[cs_count],"\n");
                cs_count++;
            }
            break;
            }

            //Continue if some (but not all) characters match
        else if(tolower(title_search[j])==tolower(current->book.b_book_title[j])&&j<strlen(current->book.b_book_title)){
                continue;
        }
                //If the search matches exactly, store the current book's details
        else{
                strcpy(title_matched,current->book.b_book_title);
                strcpy(same_title_found[title_found],current->book.b_book_title);
                char curr_ID[11];
                sprintf(curr_ID, "%ld", current->book.b_book_ID); //Appending ID to display
                strcat(same_title_found[title_found],"\n\tID: ");
                strcat(same_title_found[title_found],curr_ID);
                strcat(same_title_found[title_found],"\n\tAuthor: ");
                strcat(same_title_found[title_found],current->book.b_book_author);
                strcat(same_title_found[title_found],"\n\tStatus: ");
                    if(current->book.b_book_status=='A')
                        strcat(same_title_found[title_found], "Available ");
                    else if(current->book.b_book_status=='R')
                        strcat(same_title_found[title_found], "Reserved ");
                    else if(current->book.b_book_status=='I')
                        strcat(same_title_found[title_found], "Not available (Issued)");
                    else
                        strcat(same_title_found[title_found], "Undefined ");
                strcat(same_title_found[title_found],"\n");
                title_found++;
                }
                break;
            }

        }

        //If no results after search
        if(cs_count==0&&title_found==0){
            printf("No results found\n");
        }

        //To print all closest searches (atmost MAX_CLOSE_TITLE_SEARCH) when book title is not exactly matched
        if(cs_count>0&&title_found==0)
        {
        printf("Closest searches:\n");
        for(int i=0;i<cs_count;i++)
            printf("%d. %s",i+1,closest_search[i]);
        }

        //To print all books found of same title (if any)
        if(title_found>0){
         for(int i=0;i<title_found;i++)
            printf("%d. %s",i+1,same_title_found[i]);
        if(title_found>1)
            printf("\nThere are %hi copies of %s available\n",title_found,title_matched);
        else
            printf("\nThere is one copy of %s available\n",title_matched);
        }

        //To ask if the user wants to search again
            char ch;
            printf("Would you like to search again by title? (Y/N): ");
            ch=getchar();
            while(getchar() != '\n');
            if(ch=='Y'||ch=='y')
            searchBookbyTitle(head);
}


//Function to display all books
void displayAllBooks(BOOKNODE *head)
{
    BOOKNODE *current = head;
    if(head->next == NULL)
    {
        printf("--------------------NO BOOKS TO DISPLAY--------------------\n");
    }

    printf("------------------------------------------------------------\n");
    printf("AVAILABILITY\tBOOK ID\t\tAUTHOR\t\tTITLE\n");
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


//Function takes in pointer to head node and travesrses through all books to search by author.
void searchBookbyAuthor(BOOKNODE *head){
    BOOKNODE *current = head; //Initialising head pointer of linked list of all books to BOOKNODE type pointer current
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
    while(getchar() != '\n')//To empty Input Buffer
    ;

        //Restart search if less than three characters entered
     if(strlen(author_search)<4){
            printf("Enter atleast four characters to search\n");
            goto LAB2;
        }

    while(flag==0&&current->next != NULL){  //while loop to access all books in linked list of books
        current=current->next;
        //Iterating through all characters of a single book author
        for(int j=0;j<30;j++){

            //Skip the current book if atleast 3 characters don't match
            if((tolower(author_search[j])!=tolower(current->book.b_book_author[j]))&&j<3){
                break;
            }

                //If atleast one character does not match, and three characters have matched for this book
                //store the closest matching search (author) and skip current iteration
            else if((tolower(author_search[j])!=tolower(current->book.b_book_author[j]))&&j<strlen(current->book.b_book_author)&&j>2){
                short check_repeat=0;
                //To prevent repetition of closest search 'authors' (Same author is not suggested twice)
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
                break;
            }

                //Continue if some (not all) characters match
            else if(tolower(author_search[j])==tolower(current->book.b_book_author[j])&&j<strlen(current->book.b_book_author)){
                continue;
            }

                    //If the search matches exactly, store the details
            else{
                //printf("\n Found");
                strcpy(auth_found,current->book.b_book_author);
                strcpy(all_same_auth[same_auth_count],"Title: ");
                strcat(all_same_auth[same_auth_count],current->book.b_book_title);
                char curr_ID[11];
                sprintf(curr_ID, "%ld", current->book.b_book_ID); //Appending ID to display
                strcat(all_same_auth[same_auth_count],"\n\tID: ");
                strcat(all_same_auth[same_auth_count],curr_ID);
                strcat(all_same_auth[same_auth_count],"\n\tAuthor: ");
                 strcat(all_same_auth[same_auth_count],current->book.b_book_author);
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

        //If no exact\close matches for search
        if(flag==0&&cs_count==0)
        printf("No results found\n");

        //To print all closest searches of authors when exact match is not found
        if(cs_count>0&&same_auth_count==0){
        printf("Did you mean?:\n");
        for(int i=0;i<cs_count;i++)
            printf("%d. %s\n",(i+1),closest_search[i]);
        }

        //To print all books authored by the searched author (when no exact match)
        if(same_auth_count>0){
            for(int m=0;m<same_auth_count;m++)
                printf("%d. %s",(m+1),all_same_auth[m]);
            if(same_auth_count>1)
                printf("Total of %hi books authored by %s\n",(short)same_auth_count,auth_found);
            else
                printf("One book authored by %s\n",auth_found);
        }

        //If the user wants to search by author again
            char ch;
            printf("Do you want to search books by Author again (Y\\N): ");
            ch=getchar();
            if(ch=='Y'||ch=='y')
            searchBookbyAuthor(head);

}

//Function takes in pointer to head node and travesrses through all books to search by ID
void searchBookbyID(BOOKNODE *head){

    LAB3: ;//Label to return if invalid (not 10 digits)
    BOOKNODE *current = head; //Initialising head pointer of linked list of all books to BOOKNODE type pointer current
    char ID_search[11];
    short cs_count=0; //To count number of closest searches
    short flag=0; //To end searching on finding the exact search or when no results
    char closest_search[MAX_CLOSE_ID][80]; //To store MAX_CLOSE_ID closest matching searches of ID
    printf("Search by ID: ");
    scanf("%s",ID_search);
    while(getchar() != '\n');//To empty Input Buffer

    if((strlen(ID_search))>10){
    printf("Invalid ID entered! (ID is 10 digits)\n");
    goto LAB3;
    }

    else if(strlen(ID_search)<5){
    printf("Enter atleast 6 digits to search\n");
    goto LAB3;
    }
    else{

    };
    while(flag==0&&current->next != NULL){  //while loop to access all books in linked list of books
        current=current->next;
        char str_ID[11]; //Storing current bookID in a separate variable for convenience
        sprintf(str_ID,"%ld",current->book.b_book_ID);

            //Iterating through all characters of a single book ID
        for(int j=0;j<10;j++){

             //Skip the current book if atleast 6 characters of ID don't match
            if((ID_search[j]!=str_ID[j])&&j<6){
               break;
            }

            //If atleast one character does not match, and atleast 6 match
            //store the closest matching search (ID) and skip current iteration
            else if(((ID_search[j])!=(str_ID[j]))&&j<strlen(str_ID)&&j>5){
                //To ensure at most MAX_CLOSE_ID closest searches are stored and displayed
                if(cs_count<MAX_CLOSE_ID){
                    //Adding title, ID, author status associated with the book
                    strcpy(closest_search[cs_count],"Title: ");
                    strcat(closest_search[cs_count],current->book.b_book_title);
                    strcat(closest_search[cs_count],"\n\tID:");
                    strcat(closest_search[cs_count],str_ID);
                    strcat(closest_search[cs_count],"\n\tAuthor: ");
                    strcat(closest_search[cs_count],current->book.b_book_author);
                    strcat(closest_search[cs_count],"\n\tStatus: ");
                    if(current->book.b_book_status=='A')
                    strcat(closest_search[cs_count],"Available");
                    else if(current->book.b_book_status=='R')
                    strcat(closest_search[cs_count],"Reserved");
                    else if(current->book.b_book_status=='I')
                    strcat(closest_search[cs_count],"Not Available (Issued)");
                    else
                    strcat(closest_search[cs_count],"Undefined");
                    //strcat(closest_search[cs_count], "\n");
                    cs_count++;
                }
            break;
            }

                //Continue if some (not all) characters match
            else if((ID_search[j])==tolower(str_ID[j])&&j<(strlen(str_ID))-1){
                continue;
            }
                //If the search matches exactly, stop searching and display details of the book
            else{
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

                flag=3;
                break;
                }
            }

        }

        //To print all closest searches of authors when exact match is not found
        if(cs_count>0&&flag!=3){
        printf("Closest searches:\n");
        for(int i=0;i<cs_count;i++)
            printf("%d. %s\n",(i+1),closest_search[i]);
        }

         //If no exact\close matches for search
        if(cs_count==0&&flag!=3)
        printf("No results found\n");

        //If the user wants to search by ID again
            char ch;
            printf("Do you want to search books by ID again (Y\\N): ");
            ch=getchar();
            if(ch=='Y'||ch=='y')
            searchBookbyID(head);
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

    //printf("------------------------------------------------------------\n");
    printf("AVAILABILITY\tAUTHOR\t\tTITLE\n");
    //printf("------------------------------------------------------------\n");
    char availability[15] = {};

    while(current->next != NULL)
    {
        current = current->next;
        if(current->book.book_date_of_arrival.tm_year == now.tm_year)
        {
            if((now.tm_mon - current->book.book_date_of_arrival.tm_mon) > 4)
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
        else if(current->book.book_date_of_arrival.tm_year < now.tm_year)
        {
            continue;
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
        saveBookList(head);
    }
    return;
}

void setCurrentUser(USER *user)
{
    Current_User.u_user_ID = user->u_user_ID;
    strcpy(Current_User.user_name, user->user_name);
    Current_User.u_book_ID = user->u_book_ID;
    Current_User.u_issue_ID = user->u_issue_ID;
    strcpy(Current_User.u_book_title, user->u_book_title);
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

//Function for vendor\inventory management
//To make requests for books to other libraries and vendors, and store them in txt files
//and take requests from other libraries, and store them
void vendor_inv_Management(){
    //Vendor/Inventory management
    FILE *fp1, *fp2, *fp3, *fp1_1, *fp3_3;
    char choice, req;
    char name[30]; //To store name of vendor/other library
    char title[MAX_TITLE_LENGTH]; //To store title of the book
    char auth[30]; //To store author of the book
    char buf[1024];
    fp1=fopen("sentRequests.txt","a");
    fp3=fopen("recvdRequests.txt","a");
    //fp2 is dynamic, to store 'recieved requests' for vendors/libraries
    printf("\n----------VENDOR MANAGEMENT----------\n");
    LAB: //Label to return if
    printf("Enter your choice: \n");
    printf("1. Make new request to other library\\vendor\n");
    printf("2. Make new request to this library\n");
    printf("3. View sent or received requests\n");
    choice=getchar();
    while(getchar()!= '\n');
    switch(choice){
        case '1': //New requests to other library\vendor from this library
            printf("\nSELECT\n");
            printf("1.Request Vendor\n");
            printf("2.Request Library\n");
            scanf(" %c",&req);
            printf("Enter the title of the book: ");
            scanf(" %30[^\n]",title);
            while(getchar() != '\n');
            printf("Enter author of the book: ");
            scanf(" %30[^\n]",auth);
            while(getchar() != '\n');
            if(req=='1'){
                //Requesting vendor
                printf("Enter name of vendor to be requested: ");
                scanf(" %30[^\n]",name);
                while(getchar()!= '\n');
                if(strlen(name)==0||strlen(title)==0||strlen(auth)==0){
                    printf("Error\n");
                    goto LAB;
                }
                //Updating sentRequests.txt
                fputs("\nVendor: ",fp1);
                fputs(name,fp1);
                fputs("\n\tBook requested: ",fp1);
                fputs(title,fp1);
                fputs("\n\tAuthor: ",fp1);
                fputs(auth,fp1);
                //Updating vendor's received requests in file created for that vendor
                fp2=fopen(name,"w+");
                fputs("\nLibrary: ",fp2); //Requesting library name updated
                fputs(lib,fp2);
                fputs("\n\tBook requested: ",fp2);
                fputs(title,fp2);
                fputs("\n\tAuthor: ",fp2);
                fputs(auth,fp2);
            }
            else if(req=='2'){
                //Requesting a library
                printf("\nEnter the name of the library to be requested: ");
                scanf(" %30[^\n]",name);
                while(getchar()!= '\n');
                if(fp1==NULL||strlen(name)==0||strlen(title)==0||strlen(auth)==0){
                    printf("Error");
                    goto LAB;
                }
                //Updating sentRequests.txt
                fputs("\nLibrary: ",fp1);
                fputs(name,fp1);
                fputs("\n\tBook requested: ",fp1);
                fputs(title,fp1);
                fputs("\n\tAuthor: ",fp1);
                fputs(auth,fp1);
                //Updating "requested library's" received requests
                fp2=fopen(name,"w+");
                fputs("\nRequesting Library: ",fp2);
                fputs(lib,fp2);
                fputs("\n\tBook requested: ",fp2);
                fputs(title,fp2);
                fputs("\n\tAuthor: ",fp2);
                fputs(auth,fp2);
            }
            else{
                printf("Invalid inputs!");
                goto LAB;
            }
        break;

        //Other library requesting this library
        case '2':
            OTHER: ;//label to return if invalid inputs given
            char c;
            printf("\nEnter your library name: ");
            scanf(" %30[^\n]",name);
            while(getchar()!= '\n');
            printf("Would you like to search books in this library (Y\\N): ");
            c=getchar();
            if(c=='Y'||c=='y')
            searchBookbyTitle(BookHead);
            printf("Enter the title of the book you would like to request: \n\t(Please match case if requesting from this library)\n");
            scanf("%s",title);
            while(getchar()!= '\n');
            short if_lib=0; //If library is present in library

                //Check if book is available in library
                BOOKNODE *current = BookHead;
                while(current->next != NULL)
                {
                    current = current->next;
                    if(strcmp(current->book.b_book_title, title) == 0){
                        strcpy(auth,current->book.b_book_author);
                        if_lib=1;
                        break;
                    }
                }

            if(if_lib==0){
            printf("Enter author of the book: ");
            scanf("%s",auth);
            while(getchar()!= '\n');
            }
            if(strlen(name)==0||strlen(title)==0||strlen(auth)==0){
                    printf("Error, please try again");
                    goto OTHER;
                }
            //Updating recvdRequests for this library
            fputs("\nLibrary: ",fp3);
            fputs(name,fp3);
            fputs("\n\tBook requested: ",fp3);
            fputs(title,fp3);
            fputs("\n\tAuthor: ",fp3);
            fputs(auth,fp3);
            if(if_lib){
                fputs("\n\t(Book present in library)",fp3);
            }
            else{
                fputs("\n\t(Book not present in library)",fp3);
            }
            if(if_lib)
            printf("\nRequested book was present in library");
            else
            printf("\nRequested book was not present in library, we will try our best to get it for you!");
            printf("\nYour request has been successfully sent\n");
        break;

        case '3': //To view sent requests and received requests for this library
            REQ: ;//label to return if invalid inputs given
            char ch1;
            fp1_1=fopen("sentRequests.txt","r");
            fp3_3=fopen("recvdRequests.txt","r");
            printf("\nSELECT:");
            printf("\n1.Sent requests\n");
            printf("2.Received requests\n");
            ch1=getchar();
            while(getchar()!= '\n');
            if(ch1=='1'){
                while(fgets(buf, 1024, fp1_1)!= NULL)
                fputs(buf, stdout);
                printf("\n");
            }
            else if(ch1=='2'){
                while(fgets(buf, 1024, fp3_3)!= NULL)
                fputs(buf, stdout);
                printf("\n");
            }
            else{
                printf("Invalid choice!");
                goto REQ;
            }
            fclose(fp1_1);
            fclose(fp3_3);
        break;
        default:
        printf("Invalid entry!\n");
        goto LAB;
    }
}