#include "oni.h"

size_t get_file_size (FILE* const file_in) {

    assert (file_in);

    fseek (file_in, NO_OFFSET, SEEK_END);

    size_t filesize = (size_t) ftell (file_in);

    rewind (file_in);
    return filesize;
}

char* init_buffer (FILE* const file_in, const size_t filesize, size_t* const newbuffsize) {
    
    assert (newbuffsize);
    assert (file_in);

    size_t oldbuffsize = filesize + 1;

    char* buffer = (char*) calloc (oldbuffsize, sizeof (char));

    int items_read = -1;
    items_read = fread (buffer, sizeof (char), filesize, file_in);
    buffer[items_read] = '\0';

    *newbuffsize = items_read + 1;

    rewind (file_in);
    return buffer;
}

int UNNORMget_num_of_lines (const char* buffer) {

    assert (buffer);

    int numoflines = -1;

    buffer--;
    for (numoflines = 0; buffer; numoflines++) {

        buffer = strchr (++buffer, (int) '\n');
    }

    numoflines--;

    return numoflines;
}

int get_num_of_lines (char* buffer, const size_t buffsize) {

    assert (buffer);

    int numoflines = -1;

    if (strchr (buffer, '\n')) {

        buffer--;
        for (numoflines = 0; buffer; numoflines++) {

            buffer = strchr (++buffer, (int) '\n');
        }

        numoflines--;

    } else {

        char* inbuffer_ptr = buffer - 1;
        char* buffer_end = buffer + buffsize - 1;

        for (numoflines = 0; ++inbuffer_ptr < buffer_end; numoflines++) {

            inbuffer_ptr = strchr (inbuffer_ptr, (int) '\0');
        }
    }

    return numoflines;
}

size_t normalize_buffer (char* buffer, const size_t buffsize) {

    assert (buffer);

    size_t wr_pos = 0, r_pos = 0;

    char* inbuffer_ptr = strchr (buffer, (int) '\n');
    while (inbuffer_ptr) {

        *inbuffer_ptr = '\0';
        inbuffer_ptr = strchr (++inbuffer_ptr, (int) '\n');
    }

    if (isblank ((int) buffer[r_pos])) {

        for ( ; isblank ((int) buffer[r_pos]); r_pos++) ;
    }

    buffer[wr_pos] = buffer[r_pos];
    wr_pos++;
    r_pos++;

    for ( ; r_pos < buffsize; r_pos++, wr_pos++) {

        if (isblank ((int) buffer[r_pos])) {

            if (buffer[r_pos + 1] == '\0') {

                r_pos++;

            } else if (buffer[r_pos - 1] == '\0') {

                for ( ; isblank ((int) buffer[r_pos]); r_pos++) ;

            } else if (isblank ((int) buffer[r_pos + 1])) {

                for ( ; isblank ((int) buffer[r_pos + 1]); r_pos++) ;
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

    assert (buffer);
    assert (numoflines >= 0);

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

    assert (numoflines >= 0);
    assert (indextbl);
    assert (file_out);

    int lines_read = 0;

    if (strchr (indextbl[0].ptr, '\n')) {

        for ( ; lines_read < numoflines; lines_read++) {

            fwrite (indextbl[lines_read].ptr, sizeof (char), indextbl[lines_read].linesize + 1, file_out);
        }

    } else {

        for ( ; lines_read < numoflines; lines_read++) {

            fputs (indextbl[lines_read].ptr, file_out);
            fputc ('\n', file_out);
        }
    }

    return lines_read;
}

int BUFFERprint_text (FILE* const file_out, const char* const buffer, const int numoflines) {

    assert (numoflines >= 0);
    assert (buffer);
    assert (file_out);

    int lines_read = -1;

    if (strchr (buffer, '\n')) {

        fputs (buffer, file_out);
        lines_read = UNNORM_BUFF;

    } else {

        const char *inbuffer_ptr1 = buffer, *inbuffer_ptr2 = NULL;
        for (lines_read = 0; lines_read < numoflines; lines_read++) {

            inbuffer_ptr2 = strchr (inbuffer_ptr1, (int) '\0');

            fwrite (inbuffer_ptr1, sizeof (char), (size_t) (inbuffer_ptr2 - inbuffer_ptr1), file_out);
            fputc ((int) '\n', file_out);

            inbuffer_ptr1 = inbuffer_ptr2 + 1;
        }
    }

    return lines_read;
}

void clean_memory (line_index* tbl, char* buffer) {

    assert (tbl);
    assert (buffer);

    free (buffer);
    free (tbl);
}

int strcmp_compar (const void* line1, const void* line2) {

    return strcmp ((*(const line_index*) line1).ptr, (*(const line_index*) line2).ptr);
}

int REVline_compar (const void* line1, const void* line2) {

    const char *line1_ptr = (*(const line_index*) line1).ptr, *line2_ptr = (*(const line_index*) line2).ptr;
    const int line1_size = (*(const line_index*) line1).linesize, line2_size = (*(const line_index*) line2).linesize;

    int minlinesize = dmin (line1_size, line2_size);

    for (int i = 0, j = 0; i < minlinesize && j < minlinesize; i++, j++) {
        
        for ( ; line1_ptr[line1_size - 1 - i] < 'A' && i < minlinesize; i++) ;
        for ( ; line2_ptr[line2_size - 1 - j] < 'A' && j < minlinesize; j++) ;

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

    const char *line1_ptr = (*(const line_index*) line1).ptr, *line2_ptr = (*(const line_index*) line2).ptr;
    const int line1_size = (*(const line_index*) line1).linesize, line2_size = (*(const line_index*) line2).linesize;

    int minlinesize = dmin (line1_size, line2_size);

    for (int i = 0, j = 0; i < minlinesize && j < minlinesize; i++, j++) {

        for ( ; line1_ptr[i] < 'A' && i < minlinesize; i++) ;
        for ( ; line2_ptr[j] < 'A' && j < minlinesize; j++) ;

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

                    COPY (specarray + m * size, array + i * size, size);
                    i++;

                } else {

                    COPY (specarray + m * size, array + j * size, size);
                    j++;
                }
                m++;
            }

            while (i < rght) {

                COPY (specarray + m * size, array + i * size, size);
                i++;
                m++;
            }

            while (j < rend) {

                COPY (specarray + m * size, array + j * size, size);
                j++;
                m++;
            }

            for (m = left; m < rend; m++) {

                COPY (array + m * size, specarray + m * size, size);
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

                SWAP (array + (j - 1) * size, array + j * size, size);
            }
        }
    }
}

char* runline (char* linestart) {

    assert (linestart);

    char* lineend = strchr (linestart, (int) '\n');

    if (lineend == NULL) {

        lineend = strchr (linestart, (int) '\0');
    }

    return lineend;
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
