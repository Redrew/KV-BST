#include "dict2.h"

#define MAX_STR_LENGTH 128
#define MAX_INPUT_LENGTH 256
#define NOT_FOUND "NOTFOUND"
#define HEADER_FILE "2018_headers_only.csv"
#define KEY_INDEX 15
#define PU_INDEX 5

void search_pu_id(dict_t* dict, char *pu_id, FILE *output);
void in_order_traverse_and_collect(void ***array, size_t *size, 
    tree_node_t *node, void *pu_id, int *cmp_used, cmp_t cmp);

int
main(int argc, char *argv[]) {
    assert(argc >= 3);
    // Read header
    size_t header_size;
    char** header = read_header(HEADER_FILE, &header_size, MAX_INPUT_LENGTH);
    
    // Unpack csv data
    dict_t *dict = read_data(argv[1], KEY_INDEX, PU_INDEX, MAX_INPUT_LENGTH,
                             MAX_STR_LENGTH);
    
    // Open output file
    FILE *output = fopen(argv[2], "w");
    assert(output);
    
    // Get inputs
    char input[MAX_STR_LENGTH + 1];
    while (get_line(stdin, input) != -1) {
        // Search PU ID and output the results
        search_pu_id(dict, input, output);
    }

    // Free memory
    free_header(header, header_size);
    free_dict(dict, free_value);
    fclose(output);

    return 0;
}

// Search PU ID and output the results
void
search_pu_id(dict_t* dict, char *pu_id, FILE *output) {
             assert(dict);
    int i, cmp_used = 0;
    size_t keys_size = 0;
    void **key_array = NULL;

    // Store matching keys in dynamic key_array
    in_order_traverse_and_collect(&key_array, &keys_size, dict -> root, pu_id,
                                  &cmp_used, dict -> cmp);
    // Print number of comparisons used    
    printf("%s ---> %i\n", pu_id, cmp_used);

    // Output Not Found if no keys are returned
    if (keys_size == 0) {
        fprintf(output, "%s --> %s\n", pu_id, NOT_FOUND);
    }
    // Otherwise print out all keys
    for (i = 0; i < keys_size; i++) {
        fprintf(output, "%s --> %s\n", pu_id, (char *)key_array[i]);
    }

    // Free key array
    if (key_array != NULL) free(key_array);
}

// Recursive function for traversing dictionary tree, add keys with PU_ID values
// to dynamic key array
void
in_order_traverse_and_collect(void ***key_array, size_t *size, tree_node_t *node, 
                              void *pu_id, int *cmp_used, cmp_t cmp) {
    if (node == NULL) return;

    // Traverse left
    in_order_traverse_and_collect(key_array, size, node -> left, pu_id, 
                                  cmp_used, cmp);

    // Check if PU ID matches any values in node
    list_node_t *linked_node = node -> list_head;
    while (linked_node != NULL) {
        ++*cmp_used;

        if (cmp(((value_t *)linked_node -> value) -> pu_location_id, pu_id) 
            == 0) {
            // Add matching PU ID value's key into key_array
            ++*size;
            *key_array = realloc(*key_array, (*size) * sizeof(void *));
            assert(*key_array);
            (*key_array)[(*size) - 1] = (node -> key);
            break;
        }
        linked_node = linked_node -> next;
    }

    // Traverse right
    in_order_traverse_and_collect(key_array, size, node -> right, pu_id, 
                                  cmp_used, cmp);
}