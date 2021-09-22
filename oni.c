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

    FILE* Bfile_out = fopen ("oni_outputBUF.txt", "w");
    FILE* Ifile_out = fopen ("oni_outputIND.txt", "w");

    INDEXprint_text (Ifile_out, indextbl, numoflines);
    BUFFERprint_text (Bfile_out, buffer, numoflines);

    clean_memory (indextbl, buffer);
    fclose (Bfile_out);
    fclose (Ifile_out);
    fclose (file_in);
    return 0;

/*
    FILE* file_in = fopen ("oni_input.txt", "r");

    size_t buffsize = get_buffer_size (file_in);
    char* buffer = init_buffer (file_in, buffsize);

    int numoflines = get_num_of_lines (buffer, buffsize);

    normalize_buffer (&buffer, buffsize);

    line_index* indextbl = init_index_tbl (buffer, numoflines));

    for (int i = 0; i < numoflines; i++) {

        printf ("\nstring %d:\t%d\t%s\n\n", i + 1, indextbl[i].linesize, indextbl[i].ptr);
    }

    sort_text (indextbl, numoflines);

    FILE *file_outIND = fopen ("oni_outputIND.txt", "w"), *file_outBUF = fopen ("oni_outputBUF.txt", "w");

    INDEXprint_norm_text (file_outIND, indextbl, numoflines);
    BUFFERprint_text (file_outBUF, buffer, numoflines);

    clean_memory (indextbl, buffer);
    fclose (file_in);
    fclose (file_outIND);
    fclose (file_outBUF);
    return 0;
    */
}