#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libDisk.c"

#define BUFFER_LEN 256
#define DISK_SIZE 24576
#define BLOCK_SIZE 256
#define TOTAL_BLOCKS 96

int main()
{
    char commands[10][BLOCK_SIZE];
    _Bool free_block[TOTAL_BLOCKS];
    int file_to_inode[10][TOTAL_BLOCKS-1]; //will just keep 0 and 1 as unused
    memset(file_to_inode, 0, sizeof(file_to_inode)); //okay bc will never have 0 as a data block
    uint8_t hdd[TOTAL_BLOCKS][BLOCK_SIZE];
    memset(hdd, 0, sizeof(hdd));
    int num_disks = 0;
    int curr_free_vert = 1;
    char inode_block[BLOCK_SIZE];
    char data_block[BLOCK_SIZE];
    
    hdd[0][0] = 1; //superblock

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
    
    for (int i = 0; i < num_commands; i++)
    {
        char disk_name[50];
        char *curr_command = strtok(commands[i], " ");
        int file_number = atoi(strtok(NULL, " "));

        if (strcmp(curr_command, "CREATE") == 0)
        {
            int num_blocks = atoi(strtok(NULL, " "));

            sprintf(disk_name, "step_%d.bin", num_disks);
            num_disks++;
    
            int disk = openDisk(disk_name, DISK_SIZE);
            if (disk < 0)
            {
                return -1; // return error code from openDisk
            }

            
            memset(inode_block, 0, BLOCK_SIZE);
            inode_block[0] = file_number;
            for (int i = 1; i < num_blocks; i++)
            {
                inode_block[i] = curr_free_vert + (24 * i);
            }
            memcpy(hdd[curr_free_vert], inode_block, BLOCK_SIZE);
            curr_free_vert++;

            //UPDATE FREE LIST FOR SUPER BLOCK
            //UPDATE FILE TO POINTER TO USE FOR DELETE

            memset(data_block, 0, BLOCK_SIZE);
            data_block[0] = file_number;

            for (int i = 1; i < num_blocks; i++)
            {
                memcpy(hdd[inode_block[i]], data_block, BLOCK_SIZE);
            }

            for (int i = 0; i < TOTAL_BLOCKS; i++)
            {
                writeBlock(disk, i, hdd[i]);
            }
        }
        else if (strcmp(curr_command, "DELETE") == 0)
        {
            printf("ok\n");
        }
    }

    return 0;
}