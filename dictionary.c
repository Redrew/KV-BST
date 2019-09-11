#include "dictionary.h"

/** ----------------------------------------------------------------------------
 * Dictionary functions */

// Create dictionary, with a in built function to compare values
dict_t 
*makedict(cmp_t _cmp) {
    dict_t *dict = malloc(sizeof(dict_t));
    assert(dict != NULL);
    
    dict -> root = NULL;
    dict -> cmp = _cmp;

    return dict;
};

// Insert key-value pair into dictionary
void 
insert(dict_t *dict, void *key, void *value) {
    _traverse_and_insert(dict -> root, key, value, &(dict -> root), 
                         dict -> cmp);
}

// Return all values with matching key in a linked list
list_node_t
*search(dict_t* dict, void* key, int *cmp_used) {
    assert(dict);
    *cmp_used = 0;
    return _traverse_to_key(dict -> root, key, cmp_used, dict -> cmp);
}

// Free dictionary, takes a function that frees the data structure pointed by
// * void *value */
void
free_dict(dict_t *dict, void (*_free)(void *value)) {
    _traverse_and_free(dict -> root, _free);
    free(dict);
}

/** ----------------------------------------------------------------------------
 * Helper function implementations */

/* Make new linked list node */
list_node_t
*_new_list_node(void *value) {
    list_node_t *list_node = malloc(sizeof(list_node_t));
    assert(list_node != NULL);
    list_node -> value = value;
    list_node -> next = NULL;
    return list_node;
}

/* Recursive implementation to insert value into tree */
void
_traverse_and_insert(tree_node_t *node, void* key, void *value, 
                     tree_node_t **prev_pointer, cmp_t cmp) {
    int dif;
    
    /* Create new tree node if needed, and add node to parent */
    if (node == NULL) {
        node = malloc(sizeof(tree_node_t));
        assert(node != NULL);

        node -> left = node -> right = NULL;
        node -> key = key;
        list_node_t *list_head = _new_list_node(value);
        node -> list_head = list_head;

        *prev_pointer = node;
    } 
    /* Add value to linked list if key matches */
    else if ((dif = cmp(key, node -> key)) == 0) {
        list_node_t *cur = node -> list_head;
        list_node_t *new_node = _new_list_node(value);

        while (cur -> next != NULL) cur = cur -> next;
        cur -> next = new_node;
    } 
    /* Else traverse to child */
    else if (dif < 0){
        _traverse_and_insert(node -> left, key, value, &(node -> left), cmp);
    }
    else {
        _traverse_and_insert(node -> right, key, value, &(node -> right), cmp);
    }
}

/* Recursive implementation to search for a key in the tree */
list_node_t
*_traverse_to_key(tree_node_t *node, void *key, int *cmp_used, cmp_t cmp) {
    if (node == NULL) return NULL;

    int dif;
    ++*cmp_used;
    
    if ( (dif = cmp(key, node -> key)) < 0 ) {
        return _traverse_to_key(node -> left, key, cmp_used, cmp);
    }
    else if ( dif > 0 ) {
        return _traverse_to_key(node -> right, key, cmp_used, cmp);
    }
    else {
        return node -> list_head;
    }
}

/* Perform action for each node in the tree with post-order traversal */
void
_traverse_and_free(tree_node_t *node, void (*_free)(void *value)) {
    if (node == NULL) return;

    _traverse_and_free(node -> left, _free);
    _traverse_and_free(node -> right, _free);
    
    list_node_t *cur_elem = node -> list_head;
    list_node_t *next_elem;
    while (cur_elem != NULL) {
        _free(cur_elem -> value);
        next_elem = cur_elem -> next;
        free(cur_elem);
        cur_elem = next_elem;
    }

    free(node -> key);
    free(node);
}
