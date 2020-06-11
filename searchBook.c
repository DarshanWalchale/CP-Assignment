void searchBook(){
    char search; //char to account for non-numeric invalid inputs
    printf("Search books by \n1. Title\n2. Author\n3. Book ID\n");
    scanf(" %c",search);
    switch(search){
        case '1': {
        char title_search[30];
        short cs_count=0; //To count number of closest searches
        short flag=0; //To end searching on finding the exact book
        char closest_search[100][30]; //To store 100 closest matching searches
        printf("Search by title: ");
        scanf("%s",title_search);
        while(flag==0){//To be updated to search all books !!!

            //Iterating through all characters of a single book title
            for(int j=0;j<30;j++){
                //Skip the current book if atleast 3 characters don't match
                if(tolower(title_search[j])!=BOOK.b_book_title[j]&&j<3){
                break;
                }
                //Store the closest matching search (book) and skip current iteration
                else if(tolower(title_search[j])!=BOOK.b_book_title[j]&&j<strlen(title_search)){
                if(cs_count<100){
                strcpy(closest_search[cs_count],BOOK.b_book_title);
                cs_count++;
                }
                break;
                }
                //If the search matches exactly, stop searching
                else{
                printf("Book found: %s ",BOOK.b_book_title);
                if(BOOK.b_book_status=='A')
                printf("(Available)\n");
                else if(BOOK.b_book_status=='R')
                printf("(Reserved)\n");
                else if(BOOK.b_book_status=='R')
                printf("(Not available (Issued))\n");
                else
                printf("Undefined)\n"); //To account for faulty/incomplete entry of status of book
                }
                printf("\tAuthor: %s\n",BOOK.b_book_author);
                printf("\tBook ID: %s\n",BOOK.b_book_ID);
                //if EOF reached, flag=1 (TO BE ADDED)
            }

        }

        cs_count=0;
        //To print all closest searches (upto 100) when book is not found
        if(flag==0){
        printf("Closest searches:\n");
        while(strlen(closest_search[cs_count])!=0){
            printf("%hi. %s",cs_count,closest_search[cs_count]);
            cs_count++;
            if(cs_count=100)
            break; //Stop on reaching 100 closest search
        }
        }
        } //end of case 1 'block'
        break; //for switch case


        //Search book by author
        case '2': {
        short cs_count=0; //To count number of closest searches
        char closest_search[100][30]; //To store 100 closest matching searches
        short flag=0; //To end searching on finding the exact book
        printf("Search by author: ");
        scanf("%s",author_search);
        while(flag==0){//To be updated to search all books !!!

            //Iterating through all characters of a single book author
            for(int j=0;j<30;j++){
                //Skip the current book if atleast 3 characters don't match
                if(tolower(author_search[j])!=BOOK.b_book_author[j]&&j<3){
                break;
                }
                //Store the closest matching search (book) and skip current iteration
                else if(tolower(author_search[j])!=BOOK.b_book_author[j]&&j<strlen(author_search)){
                if(cs_count<100){
                strcpy(closest_search[cs_count],BOOK.b_book_title); //Storing title of the closest search book
                strcat(closest_search[cs_count],"\n\tAuthor:");
                strcat(closest_search[cs_count],BOOK.b_book_author);//Storing author of the closest search book
                strcat(closest_search[cs_count],"\n\tBook ID:");
                strcat(closest_search[cs_count],itoa(BOOK.b_book_ID)); //Storing ID of the closest search book
                strcat(closest_search[cs_count],"\n");
                count++;
                }
                break;
                }
                //If the search matches exactly, print and continue searching
                else{
                printf("Book found: %s",BOOK.b_book_title);
                if(BOOK.b_book_status=='A')
                printf("(Available)\n");
                else if(BOOK.b_book_status=='R')
                printf("(Reserved)\n");
                else if(BOOK.b_book_status=='R')
                printf("(Not available (Issued))\n");
                else
                printf("Undefined)\n"); //To account for faulty/incomplete entry of status of book
                printf("\n\tAuthor: %s \n\tBook ID: %d",BOOK.b_book_author,BOOK.b_book_ID);
                //if EOF reached, flag=1 (TO BE ADDED)
                }
            }

        }
        cs_count=0;
        printf("Closest searches:\n");
        //To print all closest searches (upto 100) when book is not found
        if(flag==0){
        while(strlen(closest_search[cs_count])!=0){
            printf("%hi. %s\n",cs_count,closest_search[cs_count]);
            cs_count++;
            if(cs_count==100)
            break; //Stop on reaching 100 closest search
        }
        }
        break; //for switch case '2'
    }//end of case '2'

    case '3':
    {
        int ID_search;
    printf("Search by ID: ");
    scanf("%d",ID_search);
     while(flag==0){//To be updated to search all books !!!
     //Iterating through all characters of a single book title
        if(BOOK.b_book_ID==ID_search){
            printf("Book found: %s",BOOK.b_book_title);
            if(BOOK.b_book_status=='A')
                printf("(Available)\n");
            else if(BOOK.b_book_status=='R')
                printf("(Reserved)\n");
            else if(BOOK.b_book_status=='R')
                printf("(Not available (Issued))\n");
            else
                printf("Undefined)\n"); //To account for faulty/incomplete entry of status of book
            printf("\n\tAuthor: %s \n\tBook ID: %d",BOOK.b_book_author,BOOK.b_book_ID);
            break; //Stop searching
        }

     }

    break;  //for switch case '3'
    } //end of case '3'
    }//end of switch
}
