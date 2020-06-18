Library Management System created by
Darshan Walchale        (2019B3PS0569P)
Shreyas Sheeranali      ()
Aditya Krishna Rao      ()
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
