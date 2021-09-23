#include "oni.h"
#include <stdio.h>

int main() {

    FILE* file_in = fopen ("oni_input.txt", "r");

    size_t filesize = get_file_size (file_in);
    size_t buffsize = 0;
    char* buffer = init_buffer (file_in, filesize, &buffsize);

    int numoflines = UNNORMget_num_of_lines (buffer);

    normalize_buffer (buffer, buffsize);

    line_index* indextbl = init_index_tbl (buffer, numoflines);

    qsort (indextbl, numoflines, sizeof (line_index), REVline_compar);

    FILE* Bfile_out = fopen ("oni_outputBUF.txt", "w");
    FILE* Ifile_out = fopen ("oni_outputIND.txt", "w");

    INDEXprint_text (Ifile_out, indextbl, numoflines);
    BUFFERprint_text (Bfile_out, buffer, numoflines);

    clean_memory (indextbl, buffer);
    fclose (Bfile_out);
    fclose (Ifile_out);
    fclose (file_in);
    return 0;
}