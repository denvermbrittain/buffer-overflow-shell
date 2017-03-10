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

struct Thing {
    int* number;
    char* character;
    char** characters;
};

struct Thing2 {
    struct Thing* thing;
};

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

    //structs
    struct Thing thing1; 
    struct Thing* thingpointer;
    struct Thing** things;
    thingpointer = &thing1;

    char entry[10];
    signal(sig, SIG_IGN);
    printf("\nCan't leave till you figure out how.\nIn two lines, tell me how you feel about this. Then the shell will continue.\n");
    scanf("%s", entry);

    //pointers and things, intentionally ugly but not too difficult
    struct Thing* pointerthing;
    int* numberpointer;
    int thingnumber[4];
    *thingnumber = 17;
    numberpointer = &thingnumber[0];
    char num[4];
    *num = *thingnumber + '0';
    pointerthing = &thing1;
    int thingnumber_[4];
    *thingnumber_ = 13;
    int *numptr = &thingnumber_[0];
    char num2[4];
    *num2 = *thingnumber_ + '0';
    pointerthing->number = numberpointer;
    thingpointer->character = &num[0];
    pointerthing->character = thingpointer->character;
    thingpointer->character = &num2[0];
    struct Thing2 thisThing;
    struct Thing2* thatThingptr;
    struct Thing2* thisThingptr;
    thisThingptr = &thisThing;
    thatThingptr = &thisThing;
    thisThingptr->thing = thingpointer;
    thingpointer->character = &num[0];
    thisThingptr->thing->character = pointerthing->character;
    thatThingptr = thisThingptr;
    

    if (*(thatThingptr->thing->character) - '0' == 49) {
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
