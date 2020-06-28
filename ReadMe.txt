#############################################################
ReadMe for assignment submitted by 2019B3PS0569P
Practical section number: P12
Asignment question attempted: 2
#############################################################
Team:
1. 2019B3PS0569P	Darshan Walchale
2. 2019B3PS0387P	Shreyas Ravishankar Sheeranali
3. 2019B3PS0576P	Aditya Krishna Rao
#############################################################
Description of how the code operates, and what to expect when running

Note: This program was developed in CS50's IDE using Git for version control between members.

1. The linux executable is called 2019B3PS0569P.exe, but in case this does not run,
	please try compiling using the command "gcc 2019B3PS0569P.c -o 2019B3PS0569.exe"

2. On running the program, first of all, the data stored in the files are brought into main memory
	counter variables for assigning unique IDs are assigned as global variables and
	the list of books stored are brought into memory using a dynamically allocated linked list.
	the list of users are also brought into memory using a dynamically allocated linked list.
	this is done by creating a global head for both linked lists
	a list of new additions to the library are displayed and a login prompt is displayed

3. The user will be asked to login, 5 unsuccessful logins will terminate the program

4. Once logged in, the Main Menu is displayed. Below is a map of all menu and their options, descriptions of the options are given
Main Menu
	1. Search Books (all search functions are case-insensitive)
		1. Search Book by Title:
			*	Atleast four characters to be entered to search books by Title
			*	If entire title of book(s) is matched, all book(s), and number of copies (defined to be at most MAX_SAME_TITLE)
				of the book is displayed with their details (ID, Author and availability).
			*	If search is not exactly matched with any book, atmost MAX_CLOSE_TITLE_SEARCH books matching atleast first four
				characters are suggested with their details.
		2. Search Book by Author
			*	Atleast four characters to be entered to search books by Author
			*	If entire author's name of book(s) is matched, all book(s), and number of books (defined to be at most MAX_SAME_AUTHOR)
				by that author is displayed with their Title, ID, Author's name (correct case) and availability.
			*	If search is not exactly matched with any author name in all books, atmost MAX_CLOSE_AUTH_SEARCH authors' are suggested,
				where atleast four characters of the author's name matches
		3. Search Book by Book ID (ID is a 10 digit number)
			*	Atleast 6 digits to be entered to search books by ID
			*	If ID is exactly matched, the book's title, author and availability is displayed
			*	If search is not exactly matched, all books (maximum possible is 10^4), having their ID matching atleast 6 digits
				are suggested with their title, author, ID and availability
		4. Display All Books
			Displays Availability, Title, Author, and ID
		0. Return to Main Menu

	2. Book Transaction
		1. Checkout a Book
		2. Return Book
		0. Return to Main Menu

	3. Account Info
			displays all details about the users account

	9. Admin Menu
			This menu will be displayed to and accessible by the user if they are an admin (have admin privileges)

		1. Add New Books to the Library
			User is asked to enter book title and author, other details are assigned automatically
			using the generateBookID() function
		2. Add Library Members
			User is asked to enter username, this username is cross checked with the existing list of users and if
			an existing username is found, the user is prompted again.
			Then the user enters a password and asked if the new user to be created should have admin privileges,
			other details assigned automatically using generateUserID() function
		3. Delete Library Members
			A list of users is shown to the user and the user must enter the ID of the user to delete
			displays error if ID now found
		4. Count Books of a Particular Title
			Takes input from user for title to search for and returns number of books with the same title
		5. Vendor / Library Requests
			*	Can request vendors and other libraries for new books (title, author, vendor\library name required).
				All requests made by the library are stored in sentRequests.txt. Text files named the respective
				vendor/library records all requests made to it along with the name of the requesting library (LIMITATION %)
			*	Other libraries can make requests for books in this library (title, library name required).
				Search option is given here to find books in the current library.
				If book with same title (case sensitive) is present, author is not asked; else request is still taken, and user
				is informed about it. All requests are stored in recvdRequests.txt.
			*	Option to display all sent requests and received requests for current library

		6. Review Admin Privileges
			1. View Admins
				Displays all admin users along with their ID and Admin Status
			2. View Non-Admins
				Displays all non-admin users along with their ID and Admin Status
			3. View All
				Displays all users along with their ID and Admin Status
			4. Make a user an Admin
				Displays all users along with their ID and Admin Status
				asks for userID if user to grant admin privileges to.
		`		returns prompt if user is already an admin
				returns an error if userID not found
			5. Remove an Admin
				Displays all users along with their ID and Admin Status
				asks for userID if user to remove admin privileges from.
		`		returns prompt if user is not an admin
				returns an error if userID not found
			0. Return to Admin Menu

		0. Return to Main Menu

	0. Exit from Library Portal

5. Upon exiting the program, the Linked List of Books is saved to file and the linked list is freed from memory
	the Linked List of Users is saved to file and the linked list is freed from memory
	counters are also saved at the same time.

############################################################
Known limitations of the code:
% Text files for vendors\libraries storing requests sent to them cannot be accessed by the program, has to be opened manually.
% Code will check if a book is reserved or not, and if it is, won't be possible to issue the book. Limitations are, not being able to set or remove Reservations.
#############################################################
Contributions of the team members:
Shreyas did searchBookbyTitle, searchBookbyAuthor, searchBookbyID, vendorManagement and helped in bookTransaction
Aditya did the user login, addNewUser, menus, book transaction.
#############################################################
