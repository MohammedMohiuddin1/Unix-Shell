#include "shell.h"
/*Program 1: Unix Shell 
Mohammed Mohiuddin and Rayyan Karim */

//global line arguments 
char *args[MAX_LINE / 2 + 1]; 
char commands[MAX_LINE + 1];
char previous_command[MAX_LINE + 1]; 


//initialize input function that goes through and sets the arguments to be parsed
void initializeInput() {

    //code for handling trailing semi colons
    int len = strlen(commands); 
        while(len > 0 && commands[len-1] == ';')
        {
            commands[len-1] = '\0';
            len--;
        }

        //nullify args
        for (int i = 0; i < MAX_LINE / 2 + 1; i++) //
        {
            args[i] = '\0';
        }
}


//print ascii helper function for bonus extra credit
void printAscii()
{
    printf("  |\\_/|        ****************************    (\\_/)\n");
    printf(" / @ @ \\       *  \"Purrrfectly pleasant\"  *   (='.'=)\n");
    printf("( > º < )      *       Poppy Prinz        *   (\")_(\")\n");
    printf(" `>>x<<´       *   (pprinz@example.com)   *\n");
    printf(" /  O  \\      \n");

}

//method that goes and determines the special character case of the command.
//basically each number maps to a specific command in the interactiveshell that needs to be done and an index is mapped
void specialFinder(int* special_command, int* special_command_index, int token_length) {
    
        for (int i = 0; i < token_length; i++) {
            if (strcmp(args[i], "&") == 0) {
                *special_command = 2;
                *special_command_index = i;
                break;
            }
            if (strcmp(args[i], "|") == 0) {
                *special_command = 3;
                *special_command_index = i;
                break;
            }
            if (strcmp(args[i], "<") == 0) {
                *special_command = 4;
                *special_command_index = i;
                break;
            }
            if (strcmp(args[i], ">") == 0) {
                *special_command = 5;
                *special_command_index = i;
                break;
            }
        }
}

//This is the main interactiveShell that runs the majority of the code in the program.
int interactiveShell() {
    //should_run declared for while loop termination and execution
    int should_run = 1;

    strcpy(previous_command, "");

    //main interactive loop
    while (should_run) { 
        printf(PROMPT);
        scanf(" %[^\n]", commands);  // Reads a line of text (ignores leading whitespace) from standard input and stores it in the 'commands' variable.
        fflush(stdout);

        //initializeInput function called in mine to initialize the inputs properly.
        initializeInput();
        

        if (strcasecmp(commands, "exit") == 0) //if the user enters exit, then terminate the loop
        {
            should_run = 0;
            continue;
        }
        if(strcasecmp(commands, "ascii") == 0) //if the user enters ascii, program should output some ascii art using helper function displayed earlier
        {
            printAscii();
            continue;
        }

        if (strcasecmp(commands, "!!") == 0) { //If the user enters the two exclamation marks
            if (strcasecmp(previous_command, "") == 0) { //check if there is a previous command availible
                printf("No commands in history\n"); //if there isn't then output appropriate message.
                continue;
            }

            strcpy(commands, previous_command); //previous_command is then copied into commands
            printf("osh> %s\n",
                   commands);  //print the modified command
        }

        strcpy(previous_command, commands); //Save the current history commmand as a previous command for future use.

        //intialize token
        int token_length = 1;

        //get the token using strtok.
        args[0] = strtok(commands, " ");

        //Loop to extract tokens and count the number of commands
        while (args[token_length] = strtok(NULL, " ")) 
        {
            token_length++;
        }
        

        //initialize special command indexes and values to determine what to do
        int special_command = 0;    
        int special_command_index = 0;

        //pointers to dynamically change and decide values from the specialFinder method
        int* sc = &special_command;
        int* sci = &special_command_index;

        //call specialFinder method to detect what type of command and case we are looking at here. 
        specialFinder(sc, sci, token_length);
    
        //if the special command was an ampersand & then execute this piece of code
        if (special_command == 2) //2 in this case corresponds to ampersand
        {
            //initialize arrays to store arguements for two seperate commands
            char *first_args[MAX_LINE / 2 + 1];
            char *second_args[MAX_LINE / 2 + 1];

            //set the elements of the arrays to null.
            for (int i = 0; i < MAX_LINE / 2 + 1; i++) //
            {
                first_args[i] = '\0';
                second_args[i] = '\0';
            }

            //for loop to parse the first and second arguements based on special characters index.
            for (int i = 0; i < token_length; i++) {
                if (i < special_command_index) {
                    first_args[i] = args[i];
                } else if (i > special_command_index) {
                    second_args[i - (special_command_index + 1)] = args[i];
                }
            }

            //initialize fork and do proper error handling
            int fork_return_value = fork();
            if(fork_return_value == -1)
            {
                printf("error: fork failed \n");
            }
            assert(fork_return_value != -1); //

            if (fork_return_value == 0) //if the pid is 0  it means its a child
            {
                //execute the child processes command
                int execute_command_value = execvp(first_args[0], first_args);  
                
                //error handling incase execution fails due to invalid command
                if(execute_command_value < 0)
                {
                    printf("error: invalid command before & \n");
                }
                return 0;
            } else {
                int fork_return_value_two = fork(); //create a second fork to execute the 2nd argument

                //error handling incase execution fails due to invalid command.
                if(fork_return_value_two == -1)
                {
                    printf("error: fork failed \n");
                }
                assert(fork_return_value_two != -1);
                

                if(fork_return_value_two == 0){  //Second child fork

                    //do the same execution process as first child 
                    int execute_command_value_two = execvp(second_args[0], second_args);
                    if(execute_command_value_two < 0)
                    {
                        printf("error: invalid command after & \n");
                    }
                    return 0;
                } else { //wait for the child processes to finish 
                    int status2; 
                    wait(&status2);
                }
                int status;
                wait(&status);
            }

        }
        //If the special commmand was a | pipe then execute this part of the code
        else if (special_command == 3) //3 corresponds to a | 
        {
            //two arrays created to store arguements on left and right sides.
            char *first_args[MAX_LINE / 2 + 1]; 
            char *second_args[MAX_LINE / 2 + 1];    

            for (int i = 0; i < MAX_LINE / 2 + 1; i++) //nullify both arrays
            {
                first_args[i] = '\0';
                second_args[i] = '\0';
            }

            //parse them to store the appropriate commands on each side based on the location_index of the special command
            for (int i = 0; i < token_length; i++)
            {
                if (i < special_command_index) {
                    first_args[i] = args[i];
                } else if (i > special_command_index) {
                    second_args[i - (special_command_index + 1)] = args[i];
                }
            }

            int the_pipe[2]; //array to hold the file descriptors for pipe
            enum {RD, WR}; //enumerate for better readablity 

            pipe(the_pipe); //create a pipe with two ends for reading and writing.

            //create first fork
            int fork_return_value = fork();
            if(fork_return_value == -1)
            {
                printf("error: fork failed \n");
            }
            assert(fork_return_value != -1);

            if (fork_return_value == 0) // child process
            {
                dup2(the_pipe[WR], STDOUT_FILENO);
                close(the_pipe[RD]);        
                close(the_pipe[WR]);

                int execute_command_value = execvp(first_args[0], first_args); //executing the left side command
                assert(execute_command_value >= 0);
                return 0;

            } else {   //parent process

                //second fork
                int fork_return_value_two = fork();
                if(fork_return_value == -1)
                {
                    printf("error: fork failed \n");
                }
                assert(fork_return_value_two != -1);

                if(fork_return_value_two == 0){ //second child of parent
                    dup2(the_pipe[RD], STDIN_FILENO);
                    close(the_pipe[RD]);
                    close(the_pipe[WR]);
                    int execute_command_value_two = execvp(second_args[0], second_args); //executes the left side command
                    if(execute_command_value_two == -1)
                    {
                        printf("error: invalid command after | \n");
                    }
                    return 0;
                } else {
                    close(the_pipe[RD]);
                    close(the_pipe[WR]);
                    wait(NULL);
                }
                wait(NULL);
            }

        }
        //If the special case is < then execute this code
        else if (special_command == 4) //4 corresponds to < 
        {
            //Arrays to hold shell commands and file sources
            char *shell_commands[MAX_LINE / 2 + 1]; 
            char *file_source[MAX_LINE / 2 + 1];

            for (int i = 0; i < MAX_LINE / 2 + 1; i++) //nullify arrays to be processed
            {
                shell_commands[i] = '\0';
                file_source[i] = '\0';
            }

            //populate arrays with commands and file. e.g cat < file.txt
            for (int i = 0; i < token_length; i++) {
                if (i < special_command_index) {
                    shell_commands[i] = args[i];
                } else if (i > special_command_index) {
                    file_source[i - (special_command_index + 1)] = args[i];
                    shell_commands[i - 1] = args[i];   
                }
            }

            int fork_return_value = fork();
            if(fork_return_value == -1)
            {
                printf("error: fork failed ");
            }
            assert(fork_return_value != -1);

            if(fork_return_value == 0){
                int fd = open(file_source[0], O_RDONLY);
                if(fd == -1)
                {
                    printf("error: failed to open file \n");
                }
                assert(fd != -1);

                dup2(0, fd); //redirect input 
                close(fd);

                int execute_command_value = execvp(shell_commands[0], shell_commands);
                if(execute_command_value < 0)
                {
                    printf("error: invalid command after or before < \n");
                }
                return 0;
            } else {
                int status;
                wait(&status);
                printf("\n");      
            }
        }
        //If the special case is > then execute this part of the code
        else if (special_command == 5) //corresponds to >
        {
            //Arrays to hold the first and seocond arguements.  In this case probably a command and a file
            char *first_args[MAX_LINE / 2 + 1];
            char *second_args[MAX_LINE / 2 + 1];

            //nullify arrays
            for (int i = 0; i < MAX_LINE / 2 + 1; i++) 
            {
                first_args[i] = '\0';
                second_args[i] = '\0';
            }

            //parse the token to populate the commands
            for (int i = 0; i < token_length; i++) {
                if (i < special_command_index) {
                    first_args[i] = args[i];
                } else if (i > special_command_index) {
                    second_args[i - (special_command_index + 1)] = args[i];
                }
            }

            //create a fork for proper redirection
            int fork_return_value = fork();
            if(fork_return_value == -1)
            {
                printf("error: fork failed");
            }
            assert(fork_return_value != -1);

            if(fork_return_value == 0){ //pid child process
                int fd = open(second_args[0], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                if(fd == -1)
                {
                    printf("error: failed to open file \n");
                }
                assert(fd != -1);
                //redirect the output of a command to a file
                dup2(fd, 1);
                dup2(fd, 2);
                close(fd);

                int execute_command_value = execvp(first_args[0], first_args);
                if(execute_command_value < 0)
                {
                    printf("error: invalid command after or before > \n");
                }
                return 0;
            } else {  
                int status;
                wait(&status);
            }
        }

        else { //if the command doesn't have any of the special characters then its a normal command

            int fork_return_value = fork();
            if(fork_return_value == -1)
            {
                printf("error: fork failed");
            }
            assert(fork_return_value != -1);   
            if (fork_return_value == 0) //child process
            {
                int execute_command_value = execvp(args[0], args);  //executes the command
                if(execute_command_value < 0)
                {
                    printf("error: invalid command \n");
                }
                return 0;
            } else {
                int status;
                wait(&status);
            }
        }
    }
    return 0;
}

int main()
{
    //main simply calls interactiveShell.
    return interactiveShell();
}

// return true if C-strings are equal
bool equal(char *a, char *b) { return (strcmp(a, b) == 0); }

// read a line from console
// return length of line read or -1 if failed to read
// removes the \n on the line read
int fetchline(char **line) {
  size_t len = 0;
  size_t n = getline(line, &len, stdin);
  if (n > 0) {
    (*line)[n - 1] = '\0';
  }
  return n;
}

void processLine(char *line) { printf("processing line: %s\n", line); }
