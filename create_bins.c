#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libDisk.c"

#define BUFFER_LEN 256
#define DISK_SIZE 24576

int main()
{
    char commands[10][256];
    FILE *fptr;

    //CHANGE FILE TO INPUT AGAIN
    fptr = fopen("input_test.txt", "r");

    char buffer[BUFFER_LEN];
    int num_commands = 0;

    while (fgets(buffer, BUFFER_LEN, fptr))
    {
        strcpy(commands[num_commands], buffer);
        num_commands++;
    }

    /*
    for(int k = 0; k<num_commands; k++){
         printf("%s\n", commands[k]);
    }
    */

    //char* command = "CREATE 2 4";
    int num_disks = 0;



    for(int i = 0; i<num_commands; i++){
        char bin_name[50];
        char* curr_command = strtok(commands[i], " ");
        printf("%s\n", curr_command);
        int file_number = atoi(strtok(NULL," "));
        printf("%d\n", file_number);

        if(strcmp(curr_command, "CREATE") == 0){
            printf("hey\n");
            int num_blocks = atoi(strtok(NULL, " "));
            printf("%d\n", num_blocks);
            sprintf(bin_name, "step_%d.bin", num_disks);
            num_disks++;
            printf("%s\n", bin_name);
            openDisk(bin_name, DISK_SIZE);

        }
        else if(strcmp(curr_command, "DELETE") == 0){
            printf("ok\n");
        }

    }

    return 0;
}