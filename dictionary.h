#include <stdlib.h>
#include <assert.h>

typedef int (*cmp_t)(const void *v1, const void *v2);

typedef struct list_node list_node_t;
typedef struct tree_node tree_node_t;
typedef struct dict dict_t;

#ifndef DICT_H
#define DICT_H
struct tree_node {
    void *key;
    list_node_t *list_head;
    tree_node_t *right;
    tree_node_t *left;
};

struct dict {
    tree_node_t *root;
    cmp_t cmp;
};
struct list_node {
    list_node_t *next;
    void *value;
};
#endif

dict_t *makedict(cmp_t _cmp);
void insert(dict_t *dict, void *key, void *value);
list_node_t *search(dict_t* dict, void* key, int *cmp_used);
void free_dict(dict_t *dict, void (*_free)(void *value));
list_node_t *_new_list_node(void *value);
void _traverse_and_insert(tree_node_t *node, void* key, void *value, 
                     tree_node_t **prev_pointer, cmp_t cmp);
list_node_t *_traverse_to_key(tree_node_t *node, void *key, int *cmp_used,
                              cmp_t cmp);
void _traverse_and_free(tree_node_t *node, void (*_action)(void *value));