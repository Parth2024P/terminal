#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 100
int main()
{
    //points to the input we provide
    char *input=NULL;
    //for storing the number of characters
    size_t len =0;


    ssize_t read;

    char *args[MAX_TOKENS];
    //infinte loop for keeping the terminal alivel    
    while(1){

        printf("nano-shell>");
            //taking the input
        read = getline(&input,&len,stdin);

        // Handle EOF (Ctrl+D)
        if(read==-1){
            printf("inalid input");
        }
        //replace the newline invisible newline character with null
        if(input[read-1]=='\n'){
            input[read-1]=='\0';
        }
        int i=0;
        char *token=strtok(input,"");
        while(token!=NULL){
            args[i++]=token;
            token=strtok(NULL."");
        }
        args[i];
    }
}
