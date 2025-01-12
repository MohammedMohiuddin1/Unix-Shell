#ifndef SHELL_H_
#define SHELL_H_

#include <assert.h>  //  assert
#include <fcntl.h>   //  O_RDWR, O_CREAT
#include <stdbool.h> //  bool
#include <stdio.h>   //  printf, getline
#include <stdlib.h>  //  calloc
#include <string.h>  // strcmp
#include <unistd.h>  // execvp
#include <sys/types.h>
#include <wait.h>


#define MAX_LINE 80 
#define PROMPT "osh> "


int fetchline(char **line);
bool equal(char *a, char *b);
void processLine(char *line);
void initalizeInput();
void specialFinder(int* special_command, int* special_command_index, int token_length);
void printAscii(); 
int interactiveShell();
int main();




#endif