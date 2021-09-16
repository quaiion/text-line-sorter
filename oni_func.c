#include "oni.h"

int num_of_lines (FILE* file_in) {

    int i = -1;
    char temp [100] = "";
    for (i = 0; ! feof (file_in); i++) {

        fgets (temp, 100, file_in);
    }

    rewind (file_in);

    return i - 1;
}

char** set_index_tbl (FILE* file_in, int numoflines) {

    char** indextbl = (char**) calloc (numoflines, sizeof (char*));

    char line [100] = "";
    for (int i = 0; i < numoflines; i++) {

        fgets (line, 100, file_in);
        indextbl[i] = strdup (line);
    }

    rewind (file_in);
    return indextbl;
}

int sort_buffer (char** indextbl, int numoflines) {

    qsort (indextbl, numoflines, sizeof (char*), (int (*) (const void*, const void*)) strcmp);
    return 0;
}

int print_buffer (FILE* file_out, char** indextbl, int numoflines) {

    int i = -1;
    for (i = 0; i < numoflines; i++) {

        fputs (indextbl[i], file_out);
    }
    
    return i;
}

void clean_memory (char** charmat, int numoflines) {

    for (int i = 0; i < numoflines; i++) {

        free (charmat[i]);
    }
    free (charmat);
}

static int linecompar (const char* line1, const char* line2) {

    for (int i = 0; line1[i - 1] && line2[i - 1]; i++) {

        if (symbcompar (line1[i], line2[i]) > 0) {

            return -1;
        } else {

            if (symbcompar (line1[i], line2[i]) < 0) {

                return 1;
            }
        }
    }

    return 0;
}

static int symbcompar (const char symb1, const char symb2) {

    if (symb1 <= 'Z') {

        if (symb2 <= 'Z') {

            return (int) (symb1 - symb2);
        } else {

            return (int) (symb1 + ('a' - 'A') - symb2);
        }
    } else {
        if (symb2 <= 'Z') {

            return (int) (symb1 - symb2 - ('a' - 'A'));
        }
    }

    return (int) (symb1 - symb2);
}