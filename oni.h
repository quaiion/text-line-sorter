#ifndef ONI
#define ONI

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

struct line_index {

    char* ptr;
    int linesize;
};

int get_buffer_size (FILE* file_in);

char* init_buffer (FILE* file_in, int buffsize);

int get_num_of_lines (char* buffer, int buffsize);

int normalize_buffer (char** buffer, int buffsize);

line_index* init_index_tbl (char* buffer, int* numoflines);

int sort_text (line_index* indextbl, int numoflines);

int INDEXprint_text (FILE* file_out, line_index* indextbl, int numoflines);

int BUFFERprint_text (FILE* file_out, char* buffer, int numoflines);

void clean_memory (line_index* tbl, char* buffer);

int strcmp_compar (const void* line1, const void* line2);

int REVline_compar (const line_index line1, const line_index line2);

int line_compar (const void* line1, const void* line2);

int symb_compar (const char symb1, const char symb2);

void merge_sort (void* array, int n, int (*compar) (const void*, const void*));

#endif