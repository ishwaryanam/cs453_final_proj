#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libDisk.c"

#define BUFFER_LEN 256
#define DISK_SIZE 24576
#define BLOCK_SIZE 256

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

    int num_disks = 0;
    



    for(int i = 0; i<num_commands; i++){
        char disk_name[50];
        char* curr_command = strtok(commands[i], " ");
        printf("%s\n", curr_command);
        int file_number = atoi(strtok(NULL," "));
        printf("%d\n", file_number);

        if(strcmp(curr_command, "CREATE") == 0){
 
            //get info from command 
            int num_blocks = atoi(strtok(NULL, " "));
            printf("%d\n", num_blocks);
            sprintf(disk_name, "step_%d.bin", num_disks);
            num_disks++;
            printf("%s\n", disk_name);

            //open first disk
            openDisk(disk_name, DISK_SIZE);

            //superblock
            char block[BLOCK_SIZE];
            block[0] = 1;
            //keep track of free files

            if (writeBlock(disk_name, 0, block) < 0)
            {
                closeDisk(disk_name);
                return WRITE_FUNC_FAILURE;
            }

            //inode
            memset(block, 0x00, BLOCK_SIZE);
            block[0] = file_number;
            // loop to store where the other blocks will go




            






        }
        else if(strcmp(curr_command, "DELETE") == 0){
            printf("ok\n");
        }

    }

    return 0;
}