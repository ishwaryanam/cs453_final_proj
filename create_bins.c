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

    // CHANGE FILE TO INPUT AGAIN
    fptr = fopen("input_test.txt", "r");

    char buffer[BUFFER_LEN];
    int num_commands = 0;

    while (fgets(buffer, BUFFER_LEN, fptr))
    {
        strcpy(commands[num_commands], buffer);
        num_commands++;
    }

    int num_disks = 0;
    int curr_free_vert = 1;

    for (int i = 0; i < num_commands; i++)
    {
        char disk_name[50];
        char *curr_command = strtok(commands[i], " ");
        // printf("%s\n", curr_command);
        int file_number = atoi(strtok(NULL, " "));
        // printf("%d\n", file_number);

        if (strcmp(curr_command, "CREATE") == 0)
        {

            // get info from command
            int num_blocks = atoi(strtok(NULL, " "));

            sprintf(disk_name, "step_%d.bin", num_disks);
            num_disks++;

            // open first disk
            int disk = openDisk(disk_name, DISK_SIZE);
            // error check disk

            // superblock
            char super_block[BLOCK_SIZE];
            super_block[0] = 1;
            // keep track of free blocks

            if (writeBlock(disk, 0, super_block) < 0)
            {

                return WRITE_FUNC_FAILURE;
            }

            /*
            if (writeBlock(disk, 1, super_block) < 0)
            {

                return WRITE_FUNC_FAILURE;
            }
            */

            // inode
            char inode_block[BLOCK_SIZE];
            inode_block[0] = file_number;
            //int block_index = 1;
            for (int i = 1; i < num_blocks; i++)
            {
                inode_block[i] = curr_free_vert + (24 * i);
                printf("%d\n", curr_free_vert + (24 * i));
            }

            /*
            printf("here");
            printf("%d\n", inode_block[1]);
            printf("%d\n", inode_block[2]);
            printf("%d\n", inode_block[3]);
            printf("here");
            */

            if (writeBlock(disk, 1, inode_block) < 0)
            {
                return WRITE_FUNC_FAILURE;
            }

            char data_block[BLOCK_SIZE];
            data_block[0] = file_number;

            
            for(int i = 1; i<num_blocks-1;i++){
                writeBlock(disk, inode_block[i], data_block);
            }
            
        }
        else if (strcmp(curr_command, "DELETE") == 0)
        {
            printf("ok\n");
        }
    }

    return 0;
}