#include "oni.h"

size_t get_file_size (FILE* const file_in) {

    assert (file_in);

    fseek (file_in, NO_OFFSET, SEEK_END);

    size_t filesize = (size_t) ftell (file_in);

    rewind (file_in);
    return filesize;
}

char* init_buffer (FILE* const file_in, const size_t filesize) {
    
    assert (file_in);

    char* buffer = (char*) calloc (filesize + 1, sizeof (char));
    assert (buffer);

    int items_read = -1;
    items_read = fread (buffer, sizeof (char), filesize, file_in);
    buffer[items_read] = '\0';

    if (buffer [items_read - 1] != '\n') {

        printf ("\n\nWorng imput format, please make sure you have placed newline sign at the end of the last line\n\n");
        exit (EXIT_FAILURE);
    }

    rewind (file_in);
    return buffer;
}

int get_num_of_lines (const char* buffer) {

    assert (buffer);

    int numoflines = -1;

    buffer--;
    for (numoflines = 0; buffer; numoflines++) {

        buffer = strchr (++buffer, (int) '\n');
    }

    numoflines--;

    return numoflines;
}

line_index_t* init_index_tbl (char* buffer, const int numoflines) {

    assert (buffer);
    assert (numoflines >= 0);

    line_index_t* indextbl = (line_index_t*) calloc (numoflines, sizeof (line_index_t));
    assert (indextbl);

    char *inbuffer_ptrback = NULL, *inbuffer_ptrfront = buffer;
    size_t wr_pos = 0, r_pos = 0;
    int tbl_cell = 0;

    if (isblank ((int) buffer[r_pos])) {

        for ( ; isblank ((int) buffer[r_pos]); r_pos++) ;
    }

    buffer[wr_pos] = buffer[r_pos];
    wr_pos++;
    r_pos++;

    for ( ; buffer[r_pos] != '\0'; r_pos++, wr_pos++) {

        if (isblank ((int) buffer[r_pos])) {

            if (buffer[r_pos + 1] == '\n') {

                r_pos++;

            } else if (buffer[r_pos - 1] == '\n') {

                for ( ; isblank ((int) buffer[r_pos]); r_pos++) ;

            } else if (isblank ((int) buffer[r_pos + 1])) {

                for ( ; isblank ((int) buffer[r_pos + 1]); r_pos++) ;
                if (buffer[r_pos + 1] == '\n') {

                    r_pos++;
                }
            }
        }

        buffer[wr_pos] = buffer[r_pos];

        if (buffer [wr_pos] == '\n') {

            inbuffer_ptrback = buffer + wr_pos;
            indextbl[tbl_cell].ptr = inbuffer_ptrfront;

            indextbl[tbl_cell].linesize = (int) (inbuffer_ptrback - inbuffer_ptrfront);

            inbuffer_ptrfront = inbuffer_ptrback + 1;
            tbl_cell += 1;
        }
    }
    buffer[wr_pos] = '\0';

    return indextbl;
}

/*
line_index_t* init_index_tbl (char* buffer, const int numoflines) {

    assert (buffer);
    assert (numoflines >= 0);

    line_index_t* indextbl = (line_index_t*) calloc (numoflines, sizeof (line_index_t));
    assert (indextbl);

    char *inbuffer_ptrback = NULL, *inbuffer_ptrfront = buffer;

    for (int i = 0; *inbuffer_ptrfront != '\0'; i++) {

        inbuffer_ptrback = strchr (inbuffer_ptrfront, (int) '\n');
        indextbl[i].ptr = inbuffer_ptrfront;

        indextbl[i].linesize = (int) (inbuffer_ptrback - inbuffer_ptrfront);

        inbuffer_ptrfront = inbuffer_ptrback + 1;
    }

    return indextbl;
}
*/

int INDEXprint_text (FILE* const file_out, const line_index_t* const indextbl, const int numoflines) {

    assert (numoflines >= 0);
    assert (indextbl);
    assert (file_out);

    int lines_written = 0;

    for ( ; lines_written < numoflines; lines_written++) {

        if (indextbl[lines_written].linesize != 0) {                        //  Не пробегаем все пустые строки вначале прямиком, т.к. это сильно понизит универсальность функции

            fwrite (indextbl[lines_written].ptr, sizeof (char), indextbl[lines_written].linesize + 1, file_out);
        }
    }

    return lines_written;
}

int BUFFERprint_text (FILE* const file_out, const char* const buffer) {

    assert (buffer);
    assert (file_out);

    if (fputs (buffer, file_out) >= 0) {

        return TEXT_PRINTED;

    } else {

        return PRINT_FAILED;
    }
}

void clean_memory (line_index_t* tbl, char* buffer) {

    assert (tbl);
    assert (buffer);

    free (buffer);
    free (tbl);
}

int REVline_compar (const void* line1, const void* line2) {

    const char *line1_ptr = (*(const line_index_t*) line1).ptr, *line2_ptr = (*(const line_index_t*) line2).ptr;
    const int line1_size = (*(const line_index_t*) line1).linesize, line2_size = (*(const line_index_t*) line2).linesize;

    int minlinesize = dmin (line1_size, line2_size);

    for (int i = 0, j = 0; i < minlinesize && j < minlinesize; i++, j++) {
        
        for ( ; ! isalpha (line1_ptr[line1_size - 1 - i]) && i < minlinesize; i++) ;
        for ( ; ! isalpha (line2_ptr[line2_size - 1 - j]) && j < minlinesize; j++) ;

        if (i == minlinesize || j == minlinesize) {

            break;
        }

        if (symb_compar (line1_ptr[line1_size - 1 - i], line2_ptr[line2_size - 1 - j]) < 0) {

            return -1;

        } else if (symb_compar (line1_ptr[line1_size - 1 - i], line2_ptr[line2_size - 1 - j]) > 0) {

            return 1;
        }
    }

    if (line1_size < line2_size) {

        return -1;

    } else if (line1_size > line2_size) {

        return 1;

    } else {

        return 0;
    }
}

int line_compar (const void* line1, const void* line2) {

    const char *line1_ptr = (*(const line_index_t*) line1).ptr, *line2_ptr = (*(const line_index_t*) line2).ptr;
    const int line1_size = (*(const line_index_t*) line1).linesize, line2_size = (*(const line_index_t*) line2).linesize;

    int minlinesize = dmin (line1_size, line2_size);

    for (int i = 0, j = 0; i < minlinesize && j < minlinesize; i++, j++) {

        for ( ; ! isalpha (line1_ptr[i]) && i < minlinesize; i++) ;
        for ( ; ! isalpha (line2_ptr[j]) && j < minlinesize; j++) ;

        if (i == minlinesize || j == minlinesize) {

            break;
        }

        if (symb_compar (line1_ptr[i], line2_ptr[j]) < 0) {

            return -1;

        } else if (symb_compar (line1_ptr[i], line2_ptr[j]) > 0) {

            return 1;
        }
    }

    if (line1_size < line2_size) {

        return -1;

    } else if (line1_size > line2_size) {

        return 1;

    } else {

        return 0;
    }
}

int symb_compar (char symb1, char symb2) {

    symb1 = tolower (symb1);
    symb2 = tolower (symb2);

    return (int) (symb1 - symb2);
}

void merge_sort (void* arr, size_t nmemb, const int size, int (*compar) (const void*, const void*)) {

    char* array = (char*) arr;
    char* specarray = (char*) calloc (nmemb, size);
    assert (specarray);

    size_t rght = 0, rend = 0;
    size_t i = 0, j = 0, m = 0;
 
    for (size_t k = 1; k < nmemb; k *= 2) {

        for (size_t left = 0; left + k < nmemb; left += k * 2) {

            rght = left + k;
            rend = rght + k;

            if (rend > nmemb) {
                rend = nmemb;
            }

            m = left;
            i = left;
            j = rght;

            while (i < rght && j < rend) {

                if (compar (array + i * size, array + j * size) <= 0) {

                    copy_by_byte (specarray + m * size, array + i * size, size);
                    i++;

                } else {

                    copy_by_byte (specarray + m * size, array + j * size, size);
                    j++;
                }
                m++;
            }

            while (i < rght) {

                copy_by_byte (specarray + m * size, array + i * size, size);
                i++;
                m++;
            }

            while (j < rend) {

                copy_by_byte (specarray + m * size, array + j * size, size);
                j++;
                m++;
            }

            for (m = left; m < rend; m++) {

                copy_by_byte (array + m * size, specarray + m * size, size);
            }
        }
    }

    free (specarray);
}

void bubble_sort (void *arr, size_t nmemb, const int size, int (*compar) (const void*, const void*)) {

    char* array = (char*) arr;

    for (size_t i = 0; i < nmemb - 1; i++) {

        for (size_t j = (nmemb - 1); j > i; j--) {

            if (compar (array + (j - 1) * size, array + j * size) > 0) {

                swap_by_byte (array + (j - 1) * size, array + j * size, size);
            }
        }
    }
}

int dmin (int val1, int val2) {

    if (val1 < val2) {

        return val1;
    } else {

        return val2;
    }
}

int dmax (int val1, int val2) {

    if (val1 > val2) {

        return val1;
    } else {

        return val2;
    }
}

void swap_by_byte (void* data_ptr1, void* data_ptr2, int size) {

    char *byte_ptr1 = (char*) data_ptr1, *byte_ptr2 = (char*) data_ptr2;
    
    do {

        char tmp_ptr = *byte_ptr1;
        *byte_ptr1++ = *byte_ptr2;
        *byte_ptr2++ = tmp_ptr;

    } while (--size > 0);
}

void copy_by_byte (void* dest_ptr, void* src_ptr, int size) {

    char *dest_byte_ptr = (char*) dest_ptr, *src_byte_ptr = (char*) src_ptr;

    do {

        *dest_byte_ptr++ = *src_byte_ptr++;

    } while (--size > 0); 
}

int open_in_out_files (FILE** file_in, FILE** file_out, int argc, char** argv) {

    if (argc == 1) {

        *file_in = fopen ("oni_input.txt", "r");
        *file_out = fopen ("oni_output.txt", "w");

        return (*file_in && *file_out) ? FILE_OPENED_FULL_AUTO : OPEN_FAILED;

    } else if (argc == 2) {

        *file_in = fopen (argv[1], "r");
        *file_out = fopen ("oni_output.txt", "w");

        return (*file_in && *file_out) ? FILE_OPENED_AUTO_OUTPUT : OPEN_FAILED;

    } else {

        *file_in = fopen (argv[1], "r");
        *file_out = fopen (argv[2], "w");

        return (*file_in && *file_out) ? FILE_OPENED_MANUALLY : OPEN_FAILED;
    }
}