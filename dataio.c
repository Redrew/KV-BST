#include "dataio.h"

/** dataio.h is a library to reuse sections of code common to both task 1 and 2.
 * Includes functions to read header files, read data files, insert data into
 * dictionary, format csv lines and free data structures
/** ----------------------------------------------------------------------------
 * Functions */

// Read header, return header array
char **
read_header(char *filename, size_t *size, size_t max_buffer) {
    char buffer[max_buffer + 1];
    int start, end, i;
    char *field;
    char **fields;

    FILE *header = fopen(filename, "r");
    assert(header != NULL);

    fscanf(header, "%[^\n]", buffer);

    // Find the number of fields and malloc fields array
    for (i = 0; segment_at_index(buffer, i, &start, &end) != -1; i++);
    fields = malloc(i * sizeof(char *));
    assert(fields != NULL);
    
    for (i = 0; segment_at_index(buffer, i, &start, &end) != -1; i++) {
        field = malloc((end - start + 1) * sizeof(char));
        assert(field != NULL);
        strncpy(field, buffer + start, end - start);
        field [end - start] = '\0';
        fields[i] = field;
    }
    
    *size = i;

    fclose (header);
    return fields;
}

void 
free_header(char **header, size_t header_size) {
    /* Need to be coded */
    for (int i = 0; i < header_size; i++) {
        free(header[i]);
    }
    free(header);
}

// Read data from datafile and inserts all data into dictionary
dict_t* 
read_data(char *filename, int key_index, int pickup_index,
          size_t max_buffer, size_t max_data) {
    char buffer[max_buffer + 1];
    char temp_data[max_buffer + 1];
    int start, end;

    FILE *datafile = fopen(filename, "r");
    assert(datafile);
    dict_t *dict = makedict(strcmp);

    while (get_line(datafile, buffer) != -1) {
        value_t *value = malloc(sizeof(value_t));
        assert(value);

        assert(segment_at_index(buffer, key_index, &start, &end) != -1);
        char *key = malloc((end - start + 1) * sizeof(char));
        char *data = malloc((strlen(buffer) - end + start + 1) * sizeof(char));
        assert(key && data);

        strncpy(key, buffer + start, end - start);
        key[end - start] = '\0';
        strncpy(data, buffer, start - 1);
        data[start - 1] = '\0';
        strncpy(temp_data, buffer + end, strlen(buffer) - end);
        temp_data[strlen(buffer) - end] = '\0';
        strcat(data, temp_data);
        
        assert(segment_at_index(buffer, pickup_index, &start, &end) != -1);
        char *pu_id = malloc((end - start + 1) * sizeof(char));
        assert(pu_id);
        strncpy(pu_id, buffer + start, end - start);
        pu_id[end - start] = '\0';


        value -> data = data;
        value -> pu_location_id = pu_id;

        insert(dict, key, value);
    }


    return dict;
}

// Get field at index
void
get_string_at_index(char *field, char *data, int index) {
    int start, end;
    assert(segment_at_index(data, index, &start, &end) != -1);
    strncpy(field, data + start, end - start);
    field[end - start] = '\0';
}
// Updates start and end character indexes of index'th data in a csv line
int
segment_at_index(char *line, int index, int *start, int *end) {
    *start = get_comma_pos(line, index) + 1;
    if (*start < 0) return -1;
    *end = get_comma_pos(line, index + 1);
    if (*end < 0)  *end = strlen(line);
    return 0;
}

// Get the pos of the index'th comma
int 
get_comma_pos(char *string, int index) {
    int i;
    for (i = 0; (index > 0) && (string[i] != '\0'); i++) {
        if (string[i] == ',') index--;
    }
    // Return -1 if index is 0, -2 if index is invalid
    if (string[i] == '\0') return -2;
    else return i - 1;
}

// Read a line from datafile to buffer
int
get_line(FILE *datafile, char *buffer) {
    char c;
    int i;
    for (i = 0; (c = getc(datafile)) != '\n' && c != EOF; i++) {
        buffer[i] = c;
    }
    buffer[i] = '\0';
    if (i == 0) return -1;
    else return 0;
}

void
free_value(void *_value) {
    value_t *value = (value_t *)_value;
    free(value -> data);
    free(value -> pu_location_id);
    free(value);
}