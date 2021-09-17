#ifndef ONI
#define ONI

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

const int line_buffer_size = 100;

int num_of_lines (FILE* file_in);

char** init_index_tbl (FILE* file_in, int numoflines);

int sort_buffer (char** indextbl, int numoflines);

int print_buffer (FILE* file_out, char** indextbl, int numoflines);

void clean_memory (char** charmat, int numoflines);

static int strcmp_comparator (const void* line1, const void* line2);

static int linecompar (const void* line1, const void* line2);

static int symbcompar (const char symb1, const char symb2);

#endif