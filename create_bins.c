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
    int free_blocks[TOTAL_BLOCKS];
    int file_to_inode[10][TOTAL_BLOCKS - 1];         // will just keep 0 and 1 as unused
    int no_inode[TOTAL_BLOCKS-1];
    memset(no_inode, 0, sizeof(no_inode));
    memset(file_to_inode, 0, sizeof(file_to_inode)); // okay bc will never have 0 as a data block
    uint8_t hdd[TOTAL_BLOCKS][BLOCK_SIZE];
    memset(hdd, 0, sizeof(hdd));
    int num_disks = 0;
    int curr_free_vert = 1;
    char inode_block[BLOCK_SIZE];
    char data_block[BLOCK_SIZE];
    char empty_block[BLOCK_SIZE];
    memset(empty_block, 0, BLOCK_SIZE);

    hdd[0][0] = 1; // superblock
    free_blocks[0] = 1;

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

        sprintf(disk_name, "step_%d.bin", num_disks);
        num_disks++;

        int disk = openDisk(disk_name, DISK_SIZE);
        if (disk < 0)
        {
            return -1; // return error code from openDisk
        }

        if (strcmp(curr_command, "CREATE") == 0)
        {
            int num_blocks = atoi(strtok(NULL, " "));

            memset(inode_block, 0, BLOCK_SIZE);
            inode_block[0] = file_number;
            for (int i = 1; i < num_blocks; i++)
            {
                int temp = curr_free_vert + (24 * i);
                if (temp > 95)
                {
                    curr_free_vert++;
                    temp = curr_free_vert;
                }
                inode_block[i] = temp;
                file_to_inode[file_number][i] = temp;
                free_blocks[temp] = 1;
            }
            memcpy(hdd[curr_free_vert], inode_block, BLOCK_SIZE);
            file_to_inode[file_number][0] = curr_free_vert;
            free_blocks[curr_free_vert] = 1;
            printf("%d\n", curr_free_vert);
            curr_free_vert++;
     

            // UPDATE FREE LIST FOR SUPER BLOCK

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
            int index = 0;

            while (file_to_inode[file_number][index] != 0)
            {
                memcpy(hdd[file_to_inode[file_number][index]], empty_block, BLOCK_SIZE);
                free_blocks[file_to_inode[file_number][index]] = 0;
                index++;
            }

            for (int i = 0; i < TOTAL_BLOCKS; i++)
            {
                writeBlock(disk, i, hdd[i]);
            }
            memcpy(file_to_inode[file_number], no_inode, TOTAL_BLOCKS-1);
            printf("ok\n");
        }
    }

    return 0;
}