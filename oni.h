#ifndef ONI
#define ONI

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

struct line_index {

    const char* ptr;
    int linesize;
};

size_t get_file_size (FILE* const file_in);         //  +

char* init_buffer (FILE* const file_in, const size_t filesize, size_t* buffsize);         //  +

int UNNORMget_num_of_lines (const char* const buffer);          //  +

int get_num_of_lines (const char* const buffer, const size_t buffsize);         //  +

size_t normalize_buffer (char* buffer, const size_t buffsize);          //  +

line_index* init_index_tbl (char* buffer, const int numoflines);            //  +

int sort_text (const line_index* const indextbl, const int numoflines);

int INDEXprint_text (FILE* const file_out, const line_index* const indextbl, const int numoflines);         //  +

int BUFFERprint_text (FILE* const file_out, const char* buffer, const int numoflines);          //  +

void clean_memory (line_index* tbl, char* buffer);          //  +

int strcmp_compar (const void* line1, const void* line2);

int REVline_compar (const void* line1, const void* line2);

int line_compar (const void* line1, const void* line2);

int symb_compar (const char symb1, const char symb2);

void merge_sort (void* array, int n, int (*compar) (const void*, const void*));

char* runline (char* linestart);            //  +

#endif