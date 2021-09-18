#include "oni.h"            //      Не забудь поменять типы данных на size_t и прописать консты; спроси про правильное объявление статиков

int get_buffer_size (FILE* file_in) {       //  ++

    fseek (file_in, 0, 2);

    int buffsize = (int) ftell (file_in) + 1;

    rewind (file_in);
    return buffsize;
}

char* init_buffer (FILE* file_in, int buffsize) {       //  ++

    char* buffer = (char*) calloc (buffsize, sizeof (char));

    int items_read = -1;
    items_read = fread (buffer, sizeof (char), buffsize - 1, file_in);
    buffer[items_read] = '\0';

    rewind (file_in);
    return buffer;
}

int get_num_of_lines (char* buffer, int buffsize) {       //  работает и для нормализованного, и для ненормализованного буфера

    int numoflines = 0;

    if (strchr (buffer, '\n')) {

        for (int i = 0; i < buffsize; i++) {

            if (buffer[i] == '\n') {
            
                numoflines++;
            }
        }
    } else {

        for (int i = 0; i < buffsize; i++) {

            if (buffer[i] == '\0') {
            
                numoflines++;
            }
        }
    }

    return numoflines;
}

int normalize_buffer (char** buffer, int buffsize) {

    int wrpos = 0;
    char* normbuffer = (char*) calloc (buffsize, sizeof (char));

    if (buffer[0] == '\n') {

        normbuffer[0] = '\0';
        wrpos++;

    } else if (buffer[0] != ' ') {

        normbuffer[0] = buffer[0];
        wrpos++;
    }

    for (int i = 1; i < buffsize; i++) {

        if ((*buffer)[i] = ' ') {

            if ((*buffer)[i - 1] == '\0' || (*buffer)[i + 1] == '\n' || (*buffer)[i + 1] == ' ' || (*buffer)[i + 1] == '\0') {

                i++;
            }
        } else if ((*buffer)[i] == '\n') {

            (*buffer)[i] = '\0';
        }

        normbuffer[wrpos++] = (*buffer)[i];
    }

    normbuffer[wrpos] = '\0';

    free (*buffer);
    *buffer = normbuffer;

    return wrpos;
}

line_index* init_index_tbl (char* buffer, int numoflines) {       //  ++

    line_index* indextbl = (line_index*) calloc (numoflines, sizeof (line_index));

    char *inbuffer_ptrback = NULL, *inbuffer_ptrfront = buffer;

    for (int i = 0; i < numoflines; i++) {

        inbuffer_ptrback = strchr (inbuffer_ptrfront, (int) '\n');
        indextbl[i].ptr = inbuffer_ptrfront;

        indextbl[i].linesize = (int) (inbuffer_ptrback - inbuffer_ptrfront);

        *inbuffer_ptrback = '\0';

        inbuffer_ptrfront = inbuffer_ptrback + 1;
    }

    return indextbl;
}

int sort_text (line_index* indextbl, int numoflines) {      //  ++

    qsort (indextbl, numoflines, sizeof (line_index), strcmp_compar);
    return 0;
}

int INDEXprint_text (FILE* file_out, line_index* indextbl, int numoflines) {     //  ++

    int lines_read = -1;
    for (lines_read = 0; lines_read < numoflines; lines_read++) {

        fputs (indextbl[lines_read].ptr, file_out);
        fputc ('\n', file_out);
    }
    
    return lines_read;
}

int BUFFERprint_text (FILE* file_out, char* buffer, int numoflines) {       //  +         //      Аче можно ли так писать и какие минусы?

    int lines_read = -1;

    for (lines_read = 0; lines_read < numoflines, lines_read++) {

        int inbuffer_ptr
        for (; *buffer != '\0'; buffer++) {

            fputc ((int) (*buffer), file_out);
        }

        buffer++;
    }

    return lines_read;
}

void clean_memory (line_index* tbl, char* buffer) {       //  +

    free (buffer);
    free (tbl);
}

int strcmp_compar (const void* line1, const void* line2) {       //  +

    return strcmp ((*(const line_index*) line1).ptr, (*(const line_index*) line2).ptr);
}

int REVline_compar (const void* line1, const void* line2) {     //  +

    if ((*(const line_index*) line1).linesize < (*(const line_index*) line2).linesize) {

        int minlinesize = (*(const line_index*) line1).linesize;
        for (int i = 0; i < minlinesize; i++) {

            if (symb_compar ((*(const line_index*) line1).ptr [(*(const line_index*) line1).linesize - 1 - i], (*(const line_index*) line2).ptr [(*(const line_index*) line2).linesize -1 - i]) > 0) {

                return 1;

            } else if (symb_compar ((*(const line_index*) line1).ptr [(*(const line_index*) line1).linesize - 1 - i], (*(const line_index*) line2).ptr [(*(const line_index*) line2).linesize -1 - i]) < 0) {

                return -1;
            }
        }

        return 1;

    } else if ((*(const line_index*) line1).linesize > (*(const line_index*) line2).linesize) {

        int minlinesize = (*(const line_index*) line2).linesize;
        for (int i = 0; i < minlinesize; i++) {

            if (symb_compar ((*(const line_index*) line1).ptr [(*(const line_index*) line1).linesize - 1 - i], (*(const line_index*) line2).ptr [(*(const line_index*) line2).linesize -1 - i]) > 0) {

                return 1;

            } else if (symb_compar ((*(const line_index*) line1).ptr [(*(const line_index*) line1).linesize - 1 - i], (*(const line_index*) line2).ptr [(*(const line_index*) line2).linesize -1 - i]) < 0) {

                return -1;
            }
        }

        return -1;

    } else {

        int minlinesize = (*(const line_index*) line1).linesize;
        for (int i = 0; i < minlinesize; i++) {

            if (symb_compar ((*(const line_index*) line1).ptr [(*(const line_index*) line1).linesize - 1 - i], (*(const line_index*) line2).ptr [(*(const line_index*) line2).linesize -1 - i]) > 0) {

                return 1;

            } else if (symb_compar ((*(const line_index*) line1).ptr [(*(const line_index*) line1).linesize - 1 - i], (*(const line_index*) line2).ptr [(*(const line_index*) line2).linesize -1 - i]) < 0) {

                return -1;
            }
        }

        return 0;
    }
}

int line_compar (const void* line1, const void* line2) {        //  +

    if ((*(const line_index*) line1).linesize < (*(const line_index*) line2).linesize) {

        int minlinesize = (*(const line_index*) line1).linesize;
        for (int i = 0; i < minlinesize; i++) {

            if (symb_compar ((*(const line_index*) line1).ptr[i], (*(const line_index*) line2).ptr[i]) > 0) {

                return 1;

            } else if (symb_compar ((*(const line_index*) line1).ptr[i], (*(const line_index*) line2).ptr[i]) < 0) {

                return -1;
            }
        }

        return 1;

    } else if ((*(const line_index*) line1).linesize > (*(const line_index*) line2).linesize) {

        int minlinesize = (*(const line_index*) line2).linesize;
        for (int i = 0; i < minlinesize; i++) {

            if (symb_compar ((*(const line_index*) line1).ptr[i], (*(const line_index*) line2).ptr[i]) > 0) {

                return 1;

            } else if (symb_compar ((*(const line_index*) line1).ptr[i], (*(const line_index*) line2).ptr[i]) < 0) {

                return -1;
            }
        }

        return -1;

    } else {

        int minlinesize = (*(const line_index*) line1).linesize;
        for (int i = 0; i < minlinesize; i++) {

            if (symb_compar ((*(const line_index*) line1).ptr[i], (*(const line_index*) line2).ptr[i]) > 0) {

                return 1;

            } else if (symb_compar ((*(const line_index*) line1).ptr[i], (*(const line_index*) line2).ptr[i]) < 0) {

                return -1;
            }
        }

        return 0;
    }
}

int symb_compar (const char symb1, const char symb2) {       //  +

    char symb1_reg = symb1, symb2_reg = symb2;

    if (symb1 >= 'A' && symb1 <= 'Z') {
        symb1_reg += ('a' - 'A');
    }

    if (symb2 >= 'A' && symb2 <= 'Z') {
        symb2_reg += ('a' - 'A');
    }

    return (int) (symb2_reg - symb1_reg);
}

void merge_sort (void* array, int n, int (*compar) (const void*, const void*)) {

    if (n == 1) return;

    void *array1, *array2, size1 = n / 2, size2 = n - n / 2;

    array1 = (void*) calloc (size1, sizeof (*array1));
    array2 = (void*) calloc (size2, sizeof (*array2));

    for (int i = 0; i < size1; i++) {

        array1[i] = array[i];
    }

    for (int i = 0; i < size2; i++) {

        array2[i] = array[n / 2 + i];
    }

    merge_sort (array1, size1);
    merge_sort (array2, size2);

    int i = 0, j = 0, k = 0;

    while (i < size1 && j < size2) {

        if (compar (&(array1[i]), &(array2[j])) < 0) {

            array[k] = array1[i];
            k++;
            i++;

        } else {

            array[k] = array2[j];
            k++;
            j++;
        }
    }

    for (; i < size1; i++) {

        array[k] = array1[i];
        k++;
    }

    for (; j < size2; j++) {

        array[k] = array2[j];
        k++;
    }

    free (array1);
    free (array2);
}