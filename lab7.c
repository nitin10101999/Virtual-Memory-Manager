#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TLB_SIZE 16
#define PAGE_TABLE_SIZE 256
#define NA -1
#define BUFF_SIZE 50
#define SIZE 256
int TLB[TLB_SIZE][2];
int Page_Table[PAGE_TABLE_SIZE];
int Physical_Memory[PAGE_TABLE_SIZE][SIZE];
int current_PM_loc = 0;
int current_TLB_loc = 0;
int TLB_hit = 0, Page_Table_hit = 0;
// input file and backing store
FILE *address_file;
FILE *backing_store;
void initialize()
{
    for (int i = 0; i < TLB_SIZE; i++)
    {
        TLB[i][0] = NA;
    }
    for (int i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        Page_Table[i] = NA;
    }
}
void handle_Page_Table(int PageNo)
{
    if (Page_Table[PageNo] == NA)
    {
        /////page fault
        char buffer[SIZE];
        if (fseek(backing_store, PageNo * SIZE, SEEK_SET) != 0)
        {
            fprintf(stderr, "Error seeking in backing store\n");
        }
        if (fread(buffer, sizeof(signed char), SIZE, backing_store) == 0)
        {
            fprintf(stderr, "Error reading from backing store\n");
        }
        for (int i = 0; i < SIZE; i++)
        {
            Physical_Memory[current_PM_loc][i] = buffer[i];
        }
        Page_Table[PageNo] = current_PM_loc; ///// insert in page Table
        current_PM_loc++;
    }
    else
    {
        Page_Table_hit++;
    }
}
int handle_TLB(int PageNo)
{
    int frame = -1;
    for (int i = 0; i < TLB_SIZE; i++)
    {
        if (TLB[i][0] == PageNo)
        {
            frame = TLB[i][1];
            TLB_hit++;
            Page_Table_hit++;
            return frame;
        }
        
    }
    if (frame == -1)
    {
        handle_Page_Table(PageNo);
        if (current_TLB_loc >= 0 && current_TLB_loc < TLB_SIZE)
        {
            /////inset the page fault value
            TLB[current_TLB_loc][0] = PageNo;
            TLB[current_TLB_loc][1] = Page_Table[PageNo];
            frame = Page_Table[PageNo];
            current_TLB_loc++;
        }
        else
        {
            for (int i = 1; i < TLB_SIZE; i++)
            {
                TLB[i - 1][0] = TLB[i][0];
                TLB[i - 1][1] = TLB[i][1];
            }
            //current_TLB_loc--;
            /////inset the page fault value
            TLB[TLB_SIZE-1][0] = PageNo;
            TLB[TLB_SIZE-1][1] = Page_Table[PageNo];
            frame = Page_Table[PageNo];
        }
        
    }
    return frame;
}
int findPage(int PageNo)
{
    int frame = handle_TLB(PageNo);
    return frame;
}
int main(int argc, char *argv[])
{
    initialize();
    backing_store = fopen("BACKING_STORE.bin", "rb");

    if (backing_store == NULL)
    {
        fprintf(stderr, "Error opening BACKING_STORE.bin %s\n", "BACKING_STORE.bin");
        return -1;
    }

    address_file = fopen(argv[1], "r");

    if (address_file == NULL)
    {
        fprintf(stderr, "Error opening addresses.txt %s\n", argv[1]);
        return -1;
    }
    char address[BUFF_SIZE];
    int LA, frame, d, PageNo;
    float count = 0.0;
    FILE *fptr;
    fptr = fopen("correct.txt", "w");
    while (fgets(address, BUFF_SIZE, address_file) != NULL)
    {
        LA = atoi(address);
        LA = LA % 65536; /////only sixteen bit;
        PageNo = LA / 256;
        d = LA % 256;
        frame = findPage(PageNo);
        count++;

        //printf("%d\n",(frame << 8) | d);

        fprintf(fptr,"Virtual Address: %d and Physical Address:%d\n",LA,(frame << 8) | d);
    }
    printf("TLB hit:%d\n",TLB_hit);
    printf("TLB hit rate:%.4f\n",TLB_hit/count);
    printf("Page fault:%d\n",1000-Page_Table_hit);
    printf("Page  fault rate:%.4f\n",(1000-Page_Table_hit)/count);
    fclose(address_file);
    fclose(backing_store);
    fclose(fptr);
}