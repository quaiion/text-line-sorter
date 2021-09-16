#ifndef ONI
#define ONI

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

int num_of_lines (FILE* file_in);

char** set_index_tbl (FILE* file_in, int numoflines);

int sort_buffer (char** indextbl, int numoflines);

int print_buffer (FILE* file_out, char** indextbl, int numoflines);

void clean_memory (char** charmat, int numoflines);

static int linecompar (const char* line1, const char* line2);

static int symbcompar (const char symb1, const char symb2);

#endif