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
        
        char cwd[1024];
        
        if(getcwd(cwd,sizeof(cwd))!=NULL){
            
            // \033[1;32m is Bold Green, \033[1;34m is Bold Blue, \033[0m resets the color
            printf("\033[1;32mnano-shell\033[0m:\033[1;34m%s\033[0m$ ", cwd);
        }
        else{
            // Fallback if getcwd fails
            perror("getcwd() error");
            printf("nano-shell> ");
        }


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
            token=strtok(NULL,"");
        }
        args[i];
        
        if(args[0]==NULL){
            continue;
        }
        if(strcmp(args[0],"exit")==0){
            break ;
        }
        
        //we have to handle cd,mkdir etc. in the parent process.
        
        if(strcmp(args[0],"cd")==0){
            if(args[1]=="NULL"){
                // If the user just types "cd" with no folder, you can print an error
                // or ideally, redirect them to their HOME directory.
                fprintf(stderr, "nano-shell: expected argument to \"cd\"\n");
            }
            else{
                
                if(chdir(args[1])!=0){
                    //the if returns true
                    perror("nano-shell");
                }
            }
            continue ;
        }
        // checking for the & at the end 
        
        args[i]=NULL;
        
        //flag to track
        int background=0;
        
        if(i>0&&strcmp(args[i-1],"&")==0){
            background=1;
            args[i-1]==NULL;
        }
        
        
        pid_t pid=fork();
        if(pid<0){
            perror("fork failed");
        }
        if(pid==0){
            
            //replace the child program with what we want to execute
            if(execvp(args[0],args)==-1){
                perror("execution failed");
            }
            exit(EXIT_FAILURE); 
        } 
        else{
            if(background==0){
                wait(NULL);
            }
            else{
                printf("[background process");
            }
        }
    }
}
