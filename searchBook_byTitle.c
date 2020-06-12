#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct{
    char b_book_title[30];
    char b_book_author[30];
    int b_book_ID; //10 digit ID
    int b_issue_ID; //10 digit ID
    int book_date_of_arrival; //DDMMYYYY format
    int b_user_ID; //10 digit ID
    int b_date_issue; //DDMMYYYY format
    char b_book_status; //'A' for available, 'R': Reserved, 'I': Issued
} BOOK;


int main()
{
    BOOK t={"Sherlock Holmes","Sir Arthur Conan Doyle",1234567890,1876543210,01012020,1112223334,04042020,'A'};
    searchBookbyTitle(t);
}

void searchBookbyTitle(BOOK temp){
        char title_search[30];
        short cs_count=0; //To count number of closest searches
        short flag=0; //To end searching on finding the exact book
        char closest_search[100][30]; //To store 100 closest matching searches
        printf("Search by title: ");
        fgets(title_search,30,stdin);
        while(flag==0){//To be updated to search all books !!!
            //Iterating through all characters of a single book title
            for(int j=0;j<30;j++){
                //Skip the current book if atleast 3 characters don't match
                if((tolower(title_search[j])!=tolower(temp.b_book_title[j]))&&j<3){

                    //printf("\n First 3 not matching"); For ref only, to be removed before submission
                if(strlen(title_search)<=3)
                printf("Enter atleast four characters to search\n");
                else
                printf("No results found!\n");
                flag=1;
                break;
                }
                //Store the closest matching search (book) and skip current iteration
                else if(tolower(title_search[j])!=tolower(temp.b_book_title[j])&&j<strlen(temp.b_book_title)&&j>2){

                //printf("\n Close"); For ref only, to be removed before submission
                if(cs_count<100){
                strcpy(closest_search[cs_count],temp.b_book_title);
                char curr_ID[11];
                sprintf(curr_ID, "%d", temp.b_book_ID); //Appending ID to display
                strcat(closest_search[cs_count],"\n\tID: ");
                strcat(closest_search[cs_count],curr_ID);
                char curr_status[2];
                sprintf(curr_status, "%c", temp.b_book_status);
                strcat(closest_search[cs_count],"\n\tStatus: ");
                strcat(closest_search[cs_count],curr_status);
                cs_count++;
                }
                flag=2;
                break;
                }
                else if(tolower(title_search[j])==tolower(temp.b_book_title[j])&&j<strlen(temp.b_book_title)){

                 //printf("\n Almost"); For ref only, to be removed before submission
                    continue;
                }
                //If the search matches exactly, stop searching
                else{

                    //printf("\n Found");

                printf("Book found: %s ",temp.b_book_title);
                if(temp.b_book_status=='A')
                printf("(Available)\n");
                else if(temp.b_book_status=='R')
                printf("(Reserved)\n");
                else if(temp.b_book_status=='I')
                printf("(Not available (Issued))\n");
                else
                printf("Undefined)\n"); //To account for faulty/incomplete entry of status of book

                printf("\tAuthor: %s\n",temp.b_book_author);
                printf("\tBook ID: %d\n",temp.b_book_ID);
                //if EOF reached, flag=1 (TO BE ADDED)
                flag=3;
                break;
                }
            }

        }

        cs_count=0;
        //To print all closest searches (upto 100) when book is not found
        if(flag==2){
        printf("Closest searches:\n");
        while(strlen(closest_search[cs_count])!=0){
            printf("%hi. %s",cs_count+1,closest_search[cs_count]);
            cs_count++;
            if(cs_count=100)
            break; //Stop on reaching 100 closest search
        }
        }

}
