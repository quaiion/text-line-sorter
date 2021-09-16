#include "oni.h"
#include <stdio.h>

int main() {

    FILE* file_in = fopen ("oni_input.txt", "r");

    int numoflines = num_of_lines (file_in);
    printf("%d", numoflines);

    char** indextbl = set_index_tbl (file_in, numoflines);

    sort_buffer (indextbl, numoflines);

    FILE* file_out = fopen ("oni_output1.txt", "w");
    print_buffer (file_out, indextbl, numoflines);

    clean_memory (indextbl, numoflines);

    fclose (file_in);
    fclose (file_out);
    return 0;
}