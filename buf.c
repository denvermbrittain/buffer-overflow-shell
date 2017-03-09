#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length of a command */
#define BUFFER_SIZE 50 //max buffer size
void INThandler(int); 

//Fuction to get the command from shell, tokenize it and set the args parameter
int formatCommand(char inputBuffer[], char *args[],int *flag);

int main(void)
{
    char inputBuffer[MAX_LINE]; /* buffer to hold the input command */
    int flag; // equals 1 if a command is followed by "&"
    char *args[MAX_LINE/2 + 1];/* max arguments */
    int should_run =1;
    
    pid_t pid,tpid;
    int i;
   
    
    while (should_run) //infinite loop for shell prompt
    {            
        signal(SIGINT, INThandler);
        flag = 0; //flag =0 by default
        printf("buffer-overflow>");
        fflush(stdout);
        if(-1!=formatCommand(inputBuffer,args,&flag)) // get next command  
        
	{
		pid = fork();
        
        	if (pid < 0)//if pid is less than 0, forking fails
        	{
            
            		printf("Fork failed.\n");
            		exit (1);
        	}
        
       		 else if (pid == 0)//if pid ==0
        	{
            
           	 	//command not executed
            		if (execvp(args[0], args) == -1)
           	 	{	
		
                		printf("Error executing command\n");
            		}
       		 }
        
       		 // if flag == 0, the parent will wait,
        	// otherwise returns to the formatCommand() function.
        	else
        	{
                wait(NULL);
            		i++;
           	 	if (flag == 0)      
           		 {
                		i++;
                		wait(NULL);
           		 }
        	}
   	    }
     }
    
}


void  INThandler(int sig)
{
     char entry[10];
     signal(sig, SIG_IGN);
     printf("\nCan't leave till you figure out how.\nIn two lines, tell me how you feel about this. Then the shell will continue.\n");
     scanf("%s", entry);

     int overflow[4];
     overflow[0] = 0;
     if (overflow[0] != 0) {
        printf("I guess you can leave...\n");
        exit(0);
     } else {
        signal(SIGINT, INThandler);
        return;
     }
}

int formatCommand(char inputBuffer[], char *args[],int *flag)
{
   	int length; // # of chars in command line
    	int i;     // loop index for inputBuffer
    	int start;  // index of beginning of next command
    	int ct = 0; // index of where to place the next parameter into args[]
    	int hist;
    	//read user input on command line and checking whether the command is !! or !n

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
    for (i=0;i<length;i++)
    {
        switch (inputBuffer[i])
        {
            case ' ':
            case '\t' :               // to seperate arguments
                if(start != -1)
                {
                    args[ct] = &inputBuffer[start];    
                    ct++;
                }
                inputBuffer[i] = '\0'; // add a null char at the end
                start = -1;
                break;
                
            case '\n':                 //final char 
                if (start != -1)
                {
                    args[ct] = &inputBuffer[start];
                    ct++;
                }
                inputBuffer[i] = '\0';
                args[ct] = NULL; // no more args
                break;
                
            default :           
                if (start == -1)
                    start = i;
                if (inputBuffer[i] == '&')
                {
                    *flag  = 1; //this flag is the differentiate whether the child process is invoked in background
                    inputBuffer[i] = '\0';
                }
        }
    }
    
    args[ct] = NULL; //if the input line was > 80
}
