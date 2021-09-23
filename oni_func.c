#include "oni.h"

size_t get_file_size (FILE* const file_in) {

    fseek (file_in, 0, 2);

    size_t filesize = (size_t) ftell (file_in);

    rewind (file_in);
    return filesize;
}

char* init_buffer (FILE* const file_in, const size_t filesize, size_t* const newbuffsize) {

    size_t oldbuffsize = filesize + 1;

    char* buffer = (char*) calloc (oldbuffsize, sizeof (char));

    int items_read = -1;
    items_read = fread (buffer, sizeof (char), filesize, file_in);
    buffer[items_read] = '\0';

    *newbuffsize = items_read + 1;

    rewind (file_in);
    return buffer;
}

int UNNORMget_num_of_lines (const char* const buffer) {

    int numoflines = 0;

    for (size_t i = 0; buffer[i] != '\0'; i++) {

        if (buffer[i] == '\n') {
            
                numoflines++;
            }
    }

    return numoflines;
}

int get_num_of_lines (const char* const buffer, const size_t buffsize) {

    int numoflines = 0;

    if (strchr (buffer, '\n')) {

        for (size_t i = 0; i < buffsize - 1; i++) {

            if (buffer[i] == '\n') {
            
                numoflines++;
            }
        }
    } else {

        for (size_t i = 0; i < buffsize - 1; i++) {

            if (buffer[i] == '\0') {
            
                numoflines++;
            }
        }
    }

    return numoflines;
}

size_t normalize_buffer (char* buffer, const size_t buffsize) {

    size_t wr_pos = 0, r_pos = 0;

    for (size_t i = 0; i < buffsize; i++) {

        if (buffer[i] == '\n') {

            buffer[i] = '\0';
        }
    }

    if (buffer[r_pos] == ' ') {

        for ( ; buffer[r_pos] == ' '; r_pos++) ;
    }

    buffer[wr_pos] = buffer[r_pos];
    wr_pos++;
    r_pos++;

    for ( ; r_pos < buffsize; r_pos++, wr_pos++) {

        if (buffer[r_pos] == ' ') {

            if (buffer[r_pos + 1] == '\0') {

                r_pos++;

            } else if (buffer[r_pos - 1] == '\0') {

                for ( ; buffer[r_pos] == ' '; r_pos++) ;

            } else if (buffer[r_pos + 1] == ' ') {

                for ( ; buffer[r_pos + 1] == ' '; r_pos++) ;
                if (buffer[r_pos + 1] == '\0') {

                    r_pos++;
                }
            }
        }

        buffer[wr_pos] = buffer[r_pos];
    }

    return wr_pos;
}

line_index* init_index_tbl (char* buffer, const int numoflines) {

    line_index* indextbl = (line_index*) calloc (numoflines, sizeof (line_index));

    char *inbuffer_ptrback = NULL, *inbuffer_ptrfront = buffer;

    for (int i = 0; i < numoflines; i++) {

        inbuffer_ptrback = runline (inbuffer_ptrfront);
        indextbl[i].ptr = inbuffer_ptrfront;

        indextbl[i].linesize = (int) (inbuffer_ptrback - inbuffer_ptrfront);

        inbuffer_ptrfront = ++inbuffer_ptrback;
    }

    return indextbl;
}

int INDEXprint_text (FILE* const file_out, const line_index* const indextbl, const int numoflines) {

    int lines_read = 0;

    if (strchr (indextbl[0].ptr, '\n')) {

        for ( ; lines_read < numoflines; lines_read++) {

            for (int i = 0; i <= indextbl[lines_read].linesize; i++) {

                fputc (indextbl[lines_read].ptr[i], file_out);
            }
        }

    } else {

        for ( ; lines_read < numoflines; lines_read++) {

            fputs (indextbl[lines_read].ptr, file_out);
            fputc ('\n', file_out);
        }
    }

    return lines_read;
}

int BUFFERprint_text (FILE* const file_out, const char* buffer, const int numoflines) {

    int lines_read = -1;
    const char* inbuffer_ptr = buffer;            //      Нужно ли делать такие штуки для ясности кода? Или параметр переименовать?

    for (lines_read = 0; lines_read < numoflines; lines_read++) {

        for (; *inbuffer_ptr != '\0' && *inbuffer_ptr != '\n'; inbuffer_ptr++) {

            fputc ((int) (*inbuffer_ptr), file_out);
        }
        fputc ((int) '\n', file_out);

        inbuffer_ptr++;
    }

    return lines_read;
}

void clean_memory (line_index* tbl, char* buffer) {

    free (buffer);
    free (tbl);
}

int strcmp_compar (const void* line1, const void* line2) {

    return strcmp ((*(const line_index*) line1).ptr, (*(const line_index*) line2).ptr);
}

int REVline_compar (const void* line1, const void* line2) {

    int minlinesize = dmin ((*(const line_index*) line1).linesize, (*(const line_index*) line2).linesize);

    for (int i = 0, j = 0; i < minlinesize && j < minlinesize; i++, j++) {
        
        for ( ; (*(const line_index*) line1).ptr[(*(const line_index*) line1).linesize - 1 - i] < 'A' && i < minlinesize; i++) ;
        for ( ; (*(const line_index*) line2).ptr[(*(const line_index*) line2).linesize - 1 - j] < 'A' && j < minlinesize; j++) ;

        if (i == minlinesize || j == minlinesize) {

            break;
        }
        
        if (symb_compar ((*(const line_index*) line1).ptr [(*(const line_index*) line1).linesize - 1 - i], (*(const line_index*) line2).ptr [(*(const line_index*) line2).linesize - 1 - j]) < 0) {

            return -1;

        } else if (symb_compar ((*(const line_index*) line1).ptr [(*(const line_index*) line1).linesize - 1 - i], (*(const line_index*) line2).ptr [(*(const line_index*) line2).linesize - 1 - j]) > 0) {

            return 1;
        }
    }

    if ((*(const line_index*) line1).linesize < (*(const line_index*) line2).linesize) {

        return -1;

    } else if ((*(const line_index*) line1).linesize > (*(const line_index*) line2).linesize) {

        return 1;

    } else {

        return 0;
    }
}

int line_compar (const void* line1, const void* line2) {

    int minlinesize = dmin ((*(const line_index*) line1).linesize, (*(const line_index*) line2).linesize);

    for (int i = 0, j = 0; i < minlinesize && j < minlinesize; i++, j++) {

        for ( ; (*(const line_index*) line1).ptr[i] < 'A' && i < minlinesize; i++) ;
        for ( ; (*(const line_index*) line2).ptr[j] < 'A' && j < minlinesize; j++) ;

        if (i == minlinesize || j == minlinesize) {

            break;
        }

        if (symb_compar ((*(const line_index*) line1).ptr[i], (*(const line_index*) line2).ptr[j]) < 0) {

            return -1;

        } else if (symb_compar ((*(const line_index*) line1).ptr[i], (*(const line_index*) line2).ptr[j]) > 0) {

            return 1;
        }
    }

    if ((*(const line_index*) line1).linesize < (*(const line_index*) line2).linesize) {

        return -1;

    } else if ((*(const line_index*) line1).linesize > (*(const line_index*) line2).linesize) {

        return 1;

    } else {

        return 0;
    }
}

int symb_compar (const char symb1, const char symb2) {

    char symb1_reg = symb1, symb2_reg = symb2;

    if (symb1 >= 'A' && symb1 <= 'Z') {
        symb1_reg += ('a' - 'A');
    }

    if (symb2 >= 'A' && symb2 <= 'Z') {
        symb2_reg += ('a' - 'A');
    }

    return (int) (symb1_reg - symb2_reg);
}

void merge_sort (void** array, size_t numofel, int (*compar) (const void*, const void*)) {

    if (numofel == 1) return;

    void **array1, **array2;
    size_t size1 = numofel / 2, size2 = numofel - numofel / 2;

    array1 = (void**) calloc (size1, sizeof (*array1));
    array2 = (void**) calloc (size2, sizeof (*array2));

    for (size_t i = 0; i < size1; i++) {

        *(array1 + i * sizeof (*array1)) = *(array + i * sizeof (*array));
    }

    for (size_t i = 0; i < size2; i++) {

        *(array2 + i * sizeof (*array2)) = *(array + (numofel / 2 + i) * sizeof (*array));
    }

    merge_sort (array1, size1, compar);
    merge_sort (array2, size2, compar);

    size_t i = 0, j = 0, k = 0;

    while (i < size1 && j < size2) {

        if (compar (&(*(array1 + i * sizeof (*array1))), &(*(array2 + j * sizeof (*array2)))) < 0) {

            *(array + k * sizeof (*array)) = *(array1 + i * sizeof (*array1));
            k++;
            i++;

        } else {

            *(array + k * sizeof (*array)) = *(array2 + j * sizeof (*array2));
            k++;
            j++;
        }
    }

    for ( ; i < size1; i++) {

        *(array + k * sizeof (*array)) = *(array1 + i * sizeof (*array1));
        k++;
    }

    for ( ; j < size2; j++) {

        *(array + k * sizeof (*array)) = *(array2 + j * sizeof (*array2));
        k++;
    }

    free (array1);
    free (array2);
}

char* runline (char* linestart) {

    char* inline_ptr = linestart;

    for ( ; *inline_ptr != '\0' && *inline_ptr != '\n'; inline_ptr++) ;

    return inline_ptr;
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


// Версия нормализатора, про которую хорошо бы понять, почему она не работает

/*

size_t normalize_buffer (char* buffer, const size_t buffsize) {

    size_t wr_pos = 0, r_pos = 0;
    int N = 0;

    if (buffer[r_pos] == '\n') {

        buffer[r_pos] = '\0';
        N++;

    } else if (buffer[r_pos] == ' ') {

        for ( ; buffer[r_pos] == ' '; r_pos++) ;
    }

    buffer[wr_pos] = buffer[r_pos];
    wr_pos++;
    r_pos++;

    for ( ; r_pos < buffsize; r_pos++, wr_pos++) {

        if (buffer[r_pos] == '\n') {

            buffer[r_pos] = '\0';
            N++;

        } else if (buffer[r_pos] == ' ') {

            if (buffer[r_pos + 1] == '\n' || buffer[r_pos + 1] == '\0') {

                r_pos++;

            } else if (buffer[r_pos - 1] == '\0' || buffer[r_pos - 1] == '\n') {

                for ( ; buffer[r_pos] == ' '; r_pos++) ;

            } else if (buffer[r_pos + 1] == ' ') {

                for ( ; buffer[r_pos + 1] == ' '; r_pos++) ;
                if (buffer[r_pos + 1] == '\n' || buffer[r_pos + 1] == '\0') {

                    r_pos++;
                }
            }
        }

        buffer[wr_pos] = buffer[r_pos];
    }

    printf ("\n\nATTENTION: N is %d\n\n", N);
    return wr_pos;
}

*/