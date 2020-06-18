Library Management System created by
Darshan Walchale                 (2019B3PS0569P)
Shreyas Ravishankar Sheeranali   (2019B3PS0387P)
Aditya Krishna Rao               (2019B3PS0576P)
of Lab Section 12 (Sakshi)



How To Use It
on running the program, you'll be greeted with a list of newly added books to the Library, and on pressing Enter to continue,
A login will appear, here you can login with either of the following two accounts
username: admin, password: admin
username: user,  password: user
(one of them has admin authority)
upon failing to successfully login 5 times, the program will print an error and terminate.
After successful login, notifications will appear and the menu will be presented. 
1)
2)
3)
4)
5)

6)Admin Menu
    This will only appear if the logged in user has admin authority. and selecting this option will call forth an admin menu with
    the following options
    A)
    B)
    C)
    D)
    

How It Works:
when the program is run, first the library and counter variables are loaded into memory,
counter variables are used for generating unique BookIDs, UserIDs, and IssueIDs
The contents of books.txt are loaded into memory using a dynamically allocated linked list. This is for fast search functions
Then using this linked list, newly added books are displayed
A menu is called to allow the user to login.

The search functions take the pointer to head node of the linked list as input, and searches through all the books (elements of the Linked List).

void searchBookbyTitle(BOOKNODE *head): In case the user exactly matches the title of the book, the ID, status, author of all books with same title is displayed. Else, 'MAX_CLOSE_TITLE_SEARCH' titles matching atleast three characters are suggested with status and ID. 

void searchBookbyAuthor(BOOKNODE *head):  We assume that each book has only one author. In case the user enters atleast three characters but does not match the name of the author, atmost 'MAX_CLOSE_AUTH_SEARCH' names of authors are suggested. Otherwise, a maximum of 'MAX_SAME_AUTHOR' books by that author are displayed, along with Title, ID and status of each book.


void searchBookbyID(BOOKNODE *head): The entered ID is checked if it matches the format (10 digits). Atmost 'MAX_CLOSE_ID' number of searches for IDs not matching any book, but matches a minimum of 6 digits are suggested with title and author. For exact matches, the book's title, author and status is displayed.

void vendorManagement(): Creates requests to vendors and other libraries for new books. All requests made by the library are stored in sentRequests.txt. Text files named the respective vendor/library records all request made to it along with the name of the requesting library. Therefore, any requests made to the current library is stored in a .txt file with the current Library's name.
