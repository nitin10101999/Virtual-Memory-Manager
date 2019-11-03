#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#define empty -1
#define disk -2
#define NO_OF_PAGE 256
#define ENTRIES_IN_TLB 16
#define BUFF_SIZE 50

FILE * addresses; 
FILE * disk_file; 
int pageTable_page[NO_OF_PAGE];
int pageTable_frame[NO_OF_PAGE];
int tlb_pageno[ENTRIES_IN_TLB];
int tlb_frameno[ENTRIES_IN_TLB];
int PM[NO_OF_PAGE][NO_OF_PAGE];
int entry_in_TLB=0;
int TLB_hit = 0;
int page_fault=0;
int current_page =0;
int current_frame =0;
int offset;
void initialize_pageTable(){
    for(int i=0;i<NO_OF_PAGE;i++){
        pageTable_page[i]=-1;
    }
    for(int i=0;i<ENTRIES_IN_TLB;i++){
        tlb_pageno[i]=-1;
    }
}
int find_in_TLB(int pageNumber){
    for(int i=0;i<ENTRIES_IN_TLB;i++){
        if(tlb_pageno[i]==pageNumber){
            return tlb_frameno[i];
        }
    }
    return -1;
}
int convertToPA(int LA){
    int pageNumber,frameNumber;
    pageNumber = LA/256;
    offset = LA%256;
    frameNumber = find_in_TLB(pageNumber);
    if(frameNumber!=-1){
        TLB_hit++;
        return frameNumber;
    }
    else{
        for(int i=0;i<NO_OF_PAGE;i++){
            if(pageTable_page[i]==pageNumber){
                frameNumber = pageTable_frame[i];
            }
            if(frameNumber==-1){
                /* I have to write from here */
                /* let's go  to study */
            }
        }
    }
}
int main(int argc, char *argv[]){
    char addressToBeRead[50]; 
    int LA;
    int PA;
    initialize_pageTable();
    addresses = fopen(argv[1], "r");
    disk_file = fopen("BACKING_STORE.bin", "rb");
    while ( fgets(addressToBeRead, BUFF_SIZE, addresses) != NULL) {
        LA = atoi(addressToBeRead);
        PA = convertToPA(LA);   
    }

}