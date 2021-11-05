#include "oni.h"
#include <stdio.h>

int main (int argc, char** argv) {

    FILE *file_in = NULL, *file_out = NULL;

    open_in_out_files (&file_in, &file_out, argc, argv);

    size_t filesize = get_file_size (file_in);

    char* buffer = init_buffer (file_in, filesize);

    int numoflines = get_num_of_lines (buffer);

    line_index_t* indextbl = init_index_tbl (buffer, numoflines);

    merge_sort (indextbl, numoflines, sizeof (line_index_t), line_compar);

    INDEXprint_text (file_out, indextbl, numoflines);
    fputs ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n", file_out);

    qsort (indextbl, numoflines, sizeof (line_index_t), REVline_compar);

    INDEXprint_text (file_out, indextbl, numoflines);
    fputs ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n", file_out);

    BUFFERprint_text (file_out, buffer);

    clean_memory (indextbl, buffer);
    fclose (file_out);
    fclose (file_in);
    return 0;
}