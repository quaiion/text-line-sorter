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

    merge_sort (indextbl, numoflines, sizeof (line_index), REVline_compar);

    FILE* file_out = fopen ("oni_output.txt", "w");

    INDEXprint_text (file_out, indextbl, numoflines);
    fputc ('\n', file_out);
    fputc ('\n', file_out);
    fputc ('\n', file_out);
    BUFFERprint_text (file_out, buffer, numoflines);

    clean_memory (indextbl, buffer);
    fclose (file_out);
    fclose (file_in);
    return 0;
}