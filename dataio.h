#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "dictionary.h"

typedef struct value_s value_t;
typedef struct value_s {
    char *data;
    char *pu_location_id;
} value_t;

int segment_at_index (char *line, int index, int *start, int *end);
char ** read_header(char *filename, size_t *size, size_t max_buffer);
void free_header(char **header, size_t header_size);
int get_comma_pos(char * string, int index);
dict_t* read_data(char *filename, int key_index, int pickup_index,
          size_t max_buffer, size_t max_data);
void get_string_at_index(char *field, char *data, int index);
int get_line (FILE *datafile, char *buffer); 
void free_value(void *_value);