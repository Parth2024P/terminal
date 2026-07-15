#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 100

void reap_zombies(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        // Keep looping and reaping until no zombies are left
    }
}

int main()
{
    //points to the input we provide
    char *input=NULL;
    //for storing the number of characters
    size_t len =0;
    ssize_t read;
    char *args[MAX_TOKENS];

    // Tell the shell to ignore the Ctrl+C interrupt signal
    signal(SIGINT, SIG_IGN); 
    
    // Tell the shell to run reap_zombies() whenever a child dies
    signal(SIGCHLD, reap_zombies);

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
            break; // Added break to prevent an infinite loop on Ctrl+D
        }
        //replace the newline invisible newline character with null
        if(input[read-1]=='\n'){
            input[read-1]='\0'; // FIXED: Changed == to = for assignment
        }

        int i=0;
        char *token=strtok(input," ");
        while(token!=NULL){
            args[i++]=token;
            token=strtok(NULL," ");
        }
        args[i]=NULL;
        
        if(args[0]==NULL){
            continue;
        }
        if(strcmp(args[0],"exit")==0){
            break ;
        }
        
        //we have to handle cd,mkdir etc. in the parent process.
        
        if(strcmp(args[0],"cd")==0){
            if(args[1]==NULL){ // FIXED: Removed quotes around NULL
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

        // MOVED UP: devloping piping logic
        // This block had to be moved before the if(indx!=-1) check so the variables exist.
        int indx=-1;
        
        // 1. Scan for the pipe symbol
        for (int j = 0; j < i; j++) {
            if (strcmp(args[j], "|") == 0) {
                indx = j; // FIXED: changed pipe_idx to indx to match your declaration
                break;
            }
        }

        //if pipe is found, we will split the command into two parts and execute them separately
        if(indx!=-1){ // FIXED: changed idx to indx
            args[indx]=NULL; // Split the command into two parts
            char **left_cmd=args; // FIXED: matched cmd1 to your left_cmd variable
            char **right_cmd=&args[indx+1]; // FIXED: matched cmd2 to your right_cmd variable

            int fd[2];
            if(pipe(fd)==-1){
                perror("pipe failed");
                continue;
            }

            //fork left child
            pid_t pid1=fork();

            if(pid1==0){
                dup2(fd[1],STDOUT_FILENO); // Redirect stdout to the write end of the pipe
                close(fd[0]); // Close the read end of the pipe
                close(fd[1]); // Close the write end of the pipe
                execvp(left_cmd[0],left_cmd);
                exit(EXIT_FAILURE);
            }
            //4 .FOrk RIgh child
            pid_t pid2=fork();
            if(pid2==0){
                dup2(fd[0],STDIN_FILENO); // Redirect stdin to the read end of the pipe
                close(fd[1]); // Close the write end of the pipe
                close(fd[0]); // Close the read end of the pipe
                execvp(right_cmd[0],right_cmd);
                exit(EXIT_FAILURE);
            }
            //5. Wait for both children to complete
            close(fd[0]);
            close(fd[1]);
            waitpid(pid1,NULL,0); // FIXED: NUll to NULL
            waitpid(pid2,NULL,0);

            continue; // Skip the rest of the loop and prompt for the next command
        }
        
        // checking for the & at the end 
        
        args[i]=NULL;
        
        //flag to track
        int background=0;
        
        if(i>0&&strcmp(args[i-1],"&")==0){
            background=1;
            args[i-1]=NULL;
            i--;
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
                printf("[background process]\n"); // FIXED: Added missing closing bracket and newline
            }
        }

    }
}