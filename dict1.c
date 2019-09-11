#include "dict1.h"

#define MAX_STR_LENGTH 128
#define MAX_INPUT_LENGTH 256
#define NOT_FOUND "NOTFOUND"
#define HEADER_FILE "2018_headers_only.csv"
#define KEY_INDEX 15
#define PU_INDEX 5

void search_key(dict_t* dict, char *key, FILE *output, char **header, 
                size_t header_size);

int
main(int argc, char *argv[]) {
    assert(argc >= 3);
    // Read header
    size_t header_size;
    char** header = read_header(HEADER_FILE, &header_size, MAX_INPUT_LENGTH);
    
    // Unpack csv data
    dict_t *dict = read_data(argv[1], KEY_INDEX, PU_INDEX, MAX_INPUT_LENGTH, MAX_STR_LENGTH);
    
    // Open output file
    FILE *output = fopen(argv[2], "w");
    assert(output);
    
    // Get inputs
    char input[MAX_STR_LENGTH + 1];
    while (get_line(stdin, input) != -1) {
        // Search key and output the results
        search_key(dict, input, output, header, header_size);
    }

    // Free memory
    free_header(header, header_size);
    free_dict(dict, free_value);
    fclose(output);
    return 0;
}

// Search key and output the results
void
search_key(dict_t* dict, char *key, FILE *output, char **header,
           size_t header_size) {
    char *data;
    char field[MAX_STR_LENGTH];
    int i, cmp_used = 0;

    list_node_t *node = search(dict, key, &cmp_used);
    // Print number of comparisons used
    printf("%s ---> %i\n", key, cmp_used);

    if (node == NULL) {
        fprintf(output, "%s ---> %s\n", key, NOT_FOUND);
    }
    else {
        // Output all data in linked list
        while (node != NULL) {
            fprintf(output, "%s ---> ", key);
            data = ((value_t *)node -> value) -> data;

            // Match data with header. Skip the key's header in header array
            for (i = 0; i < header_size - 1; i++) {
                get_string_at_index(field, data, i);
                if (i < KEY_INDEX) {
                    fprintf(output, "%s: %s || ", header[i], field);
                }
                else {
                    fprintf(output, "%s: %s || ", header[i + 1], field);
                }
            }
            fprintf(output, "\n");
            
            node = node -> next;
        }
    }
}