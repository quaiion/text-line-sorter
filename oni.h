#ifndef ONI
#define ONI

//! @file   oni.h
//! @brief  oni - проект "Онегин", программа для сортировки строк текста по прямому и обратному лексикографическому признаку

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

//! @brief  Константа, возвращаемая функцией печати буфера при ее вызове для ненормализованного буфера (см. BUFFERprint_text (), normalize_buffer ())

#define UNNORM_BUFF -2

//! @brief  Набор констант, используемых для обозначения результата выполнения (успех или ошибка) функции in_out_files

enum FILE_OPEN_SUCCESS {OPENED = 0, FAILED = -1};

//! @brief  Константа, необходимая для наглядности вызова функции fseek (обозначающая нулевой отступ от указанной в последнем параметре позиции)

constexpr long NO_OFFSET = 0;

struct line_index_t {

    const char* ptr;
    int linesize;
};

size_t get_file_size (FILE* const file_in);

char* init_buffer (FILE* const file_in, const size_t filesize);

int get_num_of_lines (const char* const buffer);

size_t normalize_buffer (char* buffer);

line_index_t* init_index_tbl (char* buffer, const int numoflines);

int INDEXprint_text (FILE* const file_out, const line_index_t* const indextbl, const int numoflines);

int BUFFERprint_text (FILE* const file_out, const char* const buffer);

void clean_memory (line_index_t* tbl, char* buffer);

int REVline_compar (const void* line1, const void* line2);

int line_compar (const void* line1, const void* line2);

int symb_compar (char symb1, char symb2);

void bubble_sort (void* arr, size_t nmemb, int size, int (*compar) (const void*, const void*));

void merge_sort (void* arr, size_t nmemb, int size, int (*compar) (const void*, const void*));

int dmin (int val1, int val2);

int dmax (int val1, int val2);

void swap_by_byte (void* data_ptr1, void* data_ptr2, int size);

void copy_by_byte (void* dest_ptr, void* src_ptr, int size);

int open_in_out_files (FILE** file_in, FILE** file_out, int argc, char** argv);

#endif