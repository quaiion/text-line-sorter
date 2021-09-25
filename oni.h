#ifndef ONI
#define ONI

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#define UNNORM_BUFF -2
#define NO_OFFSET 0

#define SWAP(a, b, size)                                \
    do { 												\
        size_t __size = (size);                         \
        char *__a = (a), *__b = (b);                    \
        do {                                            \
            char __tmp = *__a;                          \
            *__a++ = *__b;                              \
            *__b++ = __tmp;                             \
        } while (--__size > 0);                         \
    } while (0)

#define COPY(dst, src, size)							\
	do {												\
		size_t __size = (size);							\
		char *__dst = (dst), *__src = (src);			\
		do {											\
			*__dst++ = *__src++;						\
		} while (--__size > 0);							\
	} while (0)

struct line_index {

    const char* ptr;
    int linesize;
};

size_t get_file_size (FILE* const file_in);

char* init_buffer (FILE* const file_in, const size_t filesize, size_t* buffsize);

int UNNORMget_num_of_lines (const char* const buffer);
int get_num_of_lines (char* const buffer, const size_t buffsize);

size_t normalize_buffer (char* buffer, const size_t buffsize);

line_index* init_index_tbl (char* buffer, const int numoflines);

int INDEXprint_text (FILE* const file_out, const line_index* const indextbl, const int numoflines);
int BUFFERprint_text (FILE* const file_out, const char* const buffer, const int numoflines);          //  ?

void clean_memory (line_index* tbl, char* buffer);

int strcmp_compar (const void* line1, const void* line2);

int REVline_compar (const void* line1, const void* line2);
int line_compar (const void* line1, const void* line2);

int symb_compar (char symb1, char symb2);

void bubble_sort (void* arr, size_t nmemb, int size, int (*compar) (const void*, const void*));
void merge_sort (void* arr, size_t nmemb, int size, int (*compar) (const void*, const void*));

char* runline (char* linestart);

int dmin (int val1, int val2);
int dmax (int val1, int val2);

#endif