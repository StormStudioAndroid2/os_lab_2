#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
#include <unistd.h>
#include <aio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>


char* delete_tabs(char* new_str,char* text) {
    int j = 0;
    for (int i = 0; i<=strlen(text); ++i) {
        if (text[i]=='\t') {
            for (int k = 0; k<3; ++k) {
                new_str[j]=' ';
                ++j;
            }
        } else {
            new_str[j]=text[i];
            ++j;
        }
    }

    return new_str;
}

int main() 
{ 
  
    int fd[2];  
    char result_str[100];
    char input_str[100]; 
    pid_t p; 
  
    if (pipe(fd)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 

  
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
        int read_count = read(STDIN_FILENO, input_str, 100);
        while (read_count == 100) {
            write(fd[1], input_str, 100);
            read_count = read(STDIN_FILENO, input_str, 100);
        }
        if (read_count != 0) {
            write(fd[1], input_str, read_count);
        }
        wait(0);
  
        close(fd[1]); // Close writing end of second pipe 
  
        // Read string from child, print it and close 
        // reading end. 

    } 
  
    // child process 
    else
    { 
  
        // Read a string using first pipe 
        char get_str[100]; 
       char new_str[100];
        
        // Concatenate a fixed string with it 
        int read_count = read(fd[0], get_str, 100);
        delete_tabs(new_str,get_str);

       for (int i = 0; i<strlen(new_str); ++i) {
           if (new_str[i]==' ') {
               new_str[i]='_';
           }
       }       
       printf("%s",new_str);
            while (read_count == 100) {
            write(STDOUT_FILENO, new_str, strlen(new_str));

            read_count = read(fd[0], get_str, 100);
            delete_tabs(new_str,get_str);

       for (int i = 0; i<strlen(new_str); ++i) {
           if (new_str[i]==' ') {
               new_str[i]='_';
           }
       }
        }
        if (read_count != 0) {
            write(STDOUT_FILENO, new_str, strlen(new_str));
        }
        exit(0);

    } 
    return 0;
}
