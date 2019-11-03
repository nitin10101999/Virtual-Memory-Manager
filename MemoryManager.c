#include<stdio.h>
#define empty -1
#define disk -2
#define NO_OF_PAGE 256
#define ENTRIES_IN_TLB 16
int pageTable[NO_OF_PAGE];
int tlb_pageno[ENTRIES_IN_TLB];
int tlb_frameno[ENTRIES_IN_TLB];
void initialize_pageTable(){
    for(int i=0;i<256;i++)
        pageTable[i]=-1;
}
int main(){
    initialize_pageTable();
    

}