#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length of a command */
#define BUFFER_SIZE 50 //max buffer size
void INThandler(int); //signal handler

//get commands
int getCommand(char inputBuffer[], char *args[]);

int main(void) {
    srand((unsigned int)time(NULL));    //seed random function
    char inputBuffer[MAX_LINE]; /* buffer to hold the input command */
    char *args[MAX_LINE/2 + 1];/* max arguments */
    int should_run =1;
    
    pid_t pid,tpid;
    int i;
   
    //shell prompt
    while (should_run) {            
        signal(SIGINT, INThandler);     //handle signals
        printf("buffer-overflow>");     //print shell prompt
        fflush(stdout);

        //get next command 
        if(-1 != getCommand(inputBuffer, args))  {
		    pid = fork();
        
            //failed fork 
        	if (pid < 0) {
            	printf("Fork failed.\n");
            	exit (1);
        
            } else if (pid == 0) {

           	 	//command not executed
            	if (execvp(args[0], args) == -1) {	
                	printf("Error executing command\n");
            	}
       		} else {
                wait(NULL);
            	i++;
        	}
   	    }
     }
}

//signal handler
void  INThandler(int sig) {

    char entry[10];
    signal(sig, SIG_IGN);
    printf("\nCan't leave till you figure out how.\nIn two lines, tell me how you feel about this. Then the shell will continue.\n");
    scanf("%s", entry);

    //pointers and things, intentionally ugly but not too difficult
    
    int overflow;
    if (overflow == 49) {
        printf("I guess you can leave...\n");
        exit(0);
    } else {
        signal(SIGINT, INThandler);
        return;
    }
}

int getCommand(char inputBuffer[], char *args[]) {
   	int length;
    int i;     
    int start;  
    int ct = 0; 

 	length = read(STDIN_FILENO, inputBuffer, MAX_LINE);	
 
   
    start = -1;
    if (length == 0)
        exit(0);   //end of command
    if (length < 0)
    {
        printf("Command not read\n");
        exit(-1);  //terminate
    }
    
   //examine each character
    for (i = 0; i < length; i++) {
        switch (inputBuffer[i]) {
            case ' ':
            case '\t' :               //separate args
                if(start != -1)
                {
                    args[ct] = &inputBuffer[start];    
                    ct++;
                }
                inputBuffer[i] = '\0'; //end with a null character
                start = -1;
                break;
                
            case '\n':                 //last character in a line
                if (start != -1)
                {
                    args[ct] = &inputBuffer[start];
                    ct++;
                }
                inputBuffer[i] = '\0';
                args[ct] = NULL; 
                break;
                
            default :           
                if (start == -1)
                    start = i;
        }
    }
    args[ct] = NULL; //if the input line was > 80 return NULL
}
