#include "libcdll.h"
#include <stdlib.h>

/*
 * Initializes a NULL CDLL (empty list) and adds the first node.
 *
 * If list is not NULL it does nothing.
 *
 * Called by: cdll_push,
 *            cdll_add
 */

void
cdll_init (CDLL *list,
           void *data)
{
        // Checks if list has been initialized
        if (*list != NULL)
                return;

        *list = malloc(sizeof(struct cdll_node));

        (*list)->data     = data;
        (*list)->next     = *list;
        (*list)->previous = *list;
}


/*
 * Returns a pointer to first node in the list.
 */

CDLL
cdll_first (const CDLL* list)
{
        // Checks if list is empty
        if ((*list) == NULL)
                return NULL;

        return (*list)->next;
}


/*
 * Returns a pointer to last node in the list.
 */

CDLL
cdll_last (const CDLL* list)
{
        // Checks if list is empty
        if ((*list) == NULL)
                return NULL;

        return *list;
}

/*
 * A handler for counting nodes in a list.
 *
 * Used by cdll_length function.
 */

static void
cdll_length_foreach_handler (void *data,
                             void *length)
{
    ++(*((size_t *) length));
}


/*
 * Returns number of nodes in the list.
 */

size_t
cdll_length (const CDLL* list)
{
        // Checks if list is empty
        if (*list == NULL)
                return 0;

        // Checks if list only has one element.
        if (*list == (*list)->next)
                return 1;

        size_t *length;
        *length = 0;
        cdll_foreach(list,
                     cdll_length_foreach_handler,
                      length);

        return *length;
}


/*
 * Adds a node with given data to end of list.
 *
 * If list is not initialized yet, this function initializes it.
 */

void
cdll_push (CDLL *list,
           void *data)
{
        // Checks if list is empty
        if (*list == NULL) {
                // Inits the list with data
                cdll_init(list, data);
                return;
        }

        // Creates new node
        CDLL new_node      = malloc(sizeof(struct cdll_node));
        new_node->data     = data;
        new_node->next     = (*list)->next;
        new_node->previous = *list;

        (*list)->next->previous = new_node;
        // Attaches new node to list
        (*list)->next = new_node;
        // Moves list pointer at the end
        *list = new_node;
}


/*
 * Iterates over list's nodes starting from the first node
 * to the last one and calls the given handler function
 * with these two parameters :
 *     1- The node's data (is probably different in each handler's invocation)
 *     2- The given user_data (is same in every handler's invocation)
 */

void
cdll_foreach (const CDLL *list,
              void      (*function) (void*, void*),
              void       *user_data)
{
        // Checks if list is empty
        if (*list == NULL)
                return;

        CDLL iterator = (*list)->next;
        do {
                function(iterator->data, user_data);

                iterator = iterator->next;
        } while (iterator != (*list)->next);
}


/*
 * Iterates over list's nodes starting from the last node
 * to the first one and calls the given handler function
 * with these two parameters :
 *     1- The node's data (is probably different in each handler's invocation)
 *     2- The given user_data (is same in every handler's invocation)
 */

void
cdll_reverse_foreach (const CDLL *list,
                      void      (*function) (void*, void*),
                      void       *user_data)
{
        // Checks if list is empty
        if (*list == NULL)
            return;


        CDLL iterator = (*list);
        do {
                function(iterator->data, user_data);

                iterator = iterator->previous;
        } while (iterator != (*list));
}


/*
 * Removes and frees the last node in the list and returns it's data.
 *
 * This function does not free the last node's data, so it is on you to
 * do whatever you want with it such as freeing it's dynamic memory.
 */

void*
cdll_pop (CDLL* list)
{
        // Checks if list is empty
        if (*list == NULL)
                return NULL;

        // If list has only one node
        if ((*list)->next == *list) {
                void* last_node_data = (*list)->data;

                if (*list)
                        free(*list);
                *list = NULL;

                return last_node_data;
        }

        CDLL last_node = (*list);
        void *last_node_data = last_node->data;

        *list = last_node->previous;

        (*list)->next = last_node->next;

        last_node->next->previous = *list;

        if (last_node)
                free(last_node);

        return last_node_data;
}


/*
 * Adds a node to first of list with the given data.
 *
 * If list is not initialized yet, this function initializes it.
 */

void
cdll_add (CDLL *list,
          void *data)
{
        // Checks if list is empty
        if (*list == NULL) {
                // Inits the list with data
                cdll_init(list, data);
                return;
        }

        // Creates new node
        CDLL new_node = malloc(sizeof(struct cdll_node));
        new_node->data = data;
        new_node->next = (*list)->next;
        new_node->previous = (*list);

        new_node->next->previous = new_node;
        (*list)->next = new_node;
}

/*
 * Removes and frees the first node in the list and returns it's data.
 *
 * This function does not free the first node's data, so it is on you to
 * do whatever you want with it such as freeing it's dynamic memory.
 */

void*
cdll_remove_first (CDLL *list)
{
        // Checks if list is empty
        if (*list == NULL)
                return NULL;

        // If list only has one node
        if ((*list)->next == *list) {
                void* last_node_data = (*list)->data;

                if (*list)
                        free(*list);
                *list = NULL;

                return last_node_data;
        }

        CDLL first_node = (*list)->next;
        void *first_node_data = first_node->data;

        (*list)->next = first_node->next;

        first_node->next->previous = *list;
        if (first_node)
                free(first_node);

        return first_node_data;
}


/*
 * Shallow frees the list.
 *
 * This function removes all the nodes and frees them but doesn't do
 * anything with node's data. So note that calling this function may
 * cause memory leak.
 */

void
cdll_free (CDLL* list)
{
        cdll_deep_free(list, NULL);
}


/*
 * Deep frees the list.
 *
 * This function removes all the nodes and frees them and also
 * calls the given function pointer on every node's data.
 *
 * Example: cdll_deep_free(&my_list, free)
 * Above example calls free (<stdlib.h> function on every node's data.
 */

void
cdll_deep_free (CDLL  *list,
                void (*function) (void*))
{
        // Checks if list is empty
        if (*list == NULL)
                return;

        CDLL iterator = *list;
        do {
                CDLL node = iterator;
                iterator = iterator->next;

                // Checks if function is not NULL and node_data is freeable
                if (function && node->data)
                        function(node->data);

                if (node)
                        free(node);
        } while (iterator != *list);

        *list = NULL;
}


/*
 * Compares two void pointers and returns 0 if they are equal, i.e. they point
 * to same memory address.
 */

static int
compare_shallow_void_pointer(const void *first_data,
                             const void *second_data)
{
        if (first_data == second_data)
                return 0;

        return 1;
}


/*
 * Returns the node in the list that it's data is the same as the the one
 * that you're looking for. In other word, the node that it's data is pointing
 * to the same place that give data argument points to.
 */

CDLL
cdll_find (const CDLL *list,
           const void *data)
{
        return cdll_deep_find(list,
                              data,
                              compare_shallow_void_pointer);
}


/*
 * Returns the node in the list that it's data is the same as the the one
 * that you're looking for.
 * But you have to provide a function that tells
 * when two data valus are equal.
 */

CDLL
cdll_deep_find (const CDLL *list,
                const void *data,
                int       (*compare)(const void *, const void *))
{
        // Checks if list is empty or give compare functions is not invocable.
        if ((*list) == NULL || !compare)
                return NULL;

        CDLL iterator = (*list)->next;
        do {
                // Checks if data has been found
                if (compare(iterator->data, data) == 0)
                        return iterator;

                iterator = iterator->next;
        } while (iterator != (*list)->next);

        // No node found
        return NULL;
}


/*
 * Returns index of node storing the given data starting from 0.
 * Returns -1 if data not found.
 */

int
cdll_data_position (const CDLL *list,
                    const void *data)
{
        const CDLL node = cdll_find(list, data);

        if (node == NULL)
                return -1;

        return cdll_position(list, node);
}


/*
 * Returns index of node storing the give data starting from 0
 * Returns -1 if data not found.
 */
int
cdll_deep_data_position (const CDLL *list,
                         const void *data,
                         int       (*compare)(const void *, const void *))
{
        const CDLL node = cdll_deep_find(list, data, compare);

        if (node == NULL)
                return -1;

        return cdll_position(list, node);
}


/*
 * Removes a node with given pointer in the list and returns it's data.
 *
 * This function does not free the node's data, so it is on you to
 * do whatever you want with it such as freeing it's dynamic memory.
 */

void*
cdll_delete_node (CDLL *list,
                  CDLL  node)
{
        // Checks if list is not empty and node has valid value
        if ((*list) == NULL || node == NULL || !node)
                return NULL;

        // Checks if node is the last node in the list
        if (node == *list)
            // pops the list
                return cdll_pop(list);

        // Checks if node is the first node in the list
        if (node == (*list)->next)
            // Calls the cdll_remove_first function to remove the first element
                return cdll_remove_first(list);

        // Checks if node exists in the list
        if (cdll_position(list, node) == -1)
                return NULL;

        if ((*list)->next == *list) {
                void* node_data = (*list)->data;

                if (*list)
                        free(*list);
                *list = NULL;

                return node_data;
        }

        void *node_data = node->data;

        node->next->previous = node->previous;
        node->previous->next = node->next;

        if (node)
                free(node);

        return node_data;
}


/*
 * Returns index of node starting from 0.
 * Returns -1 if node not found.
 */
int
cdll_position (const CDLL *list,
               const CDLL  node)
{
        if (*list == NULL || node == NULL || !node)
                return -1;

        int position = 0;
        CDLL iterator = (*list)->next;
        do {
                if (iterator == node)
                        return position;

                iterator = iterator->next;
                ++position;
        } while (iterator != (*list)->next);

        return -1;
}


/*
 * A handler for copying a list.
 *
 * Used by cdll_copy function.
 */

static void
cdll_copy_handler (void *data,
                   void *new_list)
{
        cdll_push(new_list, data);
}


/*
 * Returns a shallow copy of given list.
 *
 * Shallow copy means this function does not duplicate any node's data,
 * it just copy it's address (pointer).
 */

CDLL
cdll_copy (const CDLL *list)
{
        // Checks if list is empty
        if (*list == NULL)
                return NULL;

        CDLL new_list = NULL;
        cdll_foreach(list, cdll_copy_handler, &new_list);

        return new_list;
}


/*
 * A struct for storing multiple arguments passed to cdll_deep_copy_handler.
 */

struct cdll_deep_copy_handler_args
{
        void* (*function)(const void*, void*);

        CDLL   *new_list;

        void   *user_data;
};

/*
 * A handler for deep copying a list.
 *
 * Used by cdll_length function.
 */

static void
cdll_deep_copy_handler (void *data,
                        void *user_data)
{
        struct cdll_deep_copy_handler_args *args = user_data;

        // Push a new node to the list.
        cdll_push(args->new_list,
                  args->function(data, args->user_data));
}


/*
 * Returns a deep copy of given list.
 *
 * This functions gets a function pointer for copying every node's data.
 */

CDLL
cdll_deep_copy (const CDLL *list,
                void*     (*function)(const void*, void*),
                void       *user_data)
{
        // Checks if list is empty
        if (*list == NULL)
                return NULL;

        // Creates a new empty list
        CDLL new_list = NULL;

        // Arguments for passing to cdll_deep_copy_handler
        struct cdll_deep_copy_handler_args *func_args;
        func_args = malloc(sizeof(struct cdll_deep_copy_handler_args));
        func_args->new_list  = &new_list;
        func_args->function  = function;
        func_args->user_data = user_data;

        cdll_foreach(list,
                     cdll_deep_copy_handler,
                     func_args);

        if (func_args)
                free(func_args);

        return new_list;
}


/*
 * Returns the nth node in the list.
 */

CDLL
cdll_nth (const CDLL *list,
          size_t      nth_node)
{
        // Checks if list is empty
        if (*list == NULL)
                return NULL;

        CDLL iterator = (*list)->next;
        do {
                // If enough nodes have been iterated
                if (nth_node == 0)
                        return iterator;

                // Goes to the next node
                iterator = iterator->next;
                --nth_node;
        } while (iterator != (*list)->next);

        // Index was out of range.
        return NULL;
}


/*
 * Returns the nth data in the list.
 */

void*
cdll_nth_data (const CDLL *list,
               size_t      nth_data)
{
        // Finds the nth node
        CDLL node = cdll_nth(list, nth_data);

        // Checks if index is out of range.
        if (node == NULL)
                return NULL;

        return node->data;
}


/*
 * Removes the first node that has the given data.
 */

void*
cdll_remove_data (CDLL       *list,
                  const void *data)
{
        CDLL node = cdll_find(list, data);

        if (node == NULL)
                return NULL;

        return cdll_delete_node(list, node);
}


/*
 * Deep removes the first node that has the given data.
 */

void*
cdll_deep_remove_data (CDLL       *list,
                       const void *data,
                       int       (*compare)(const void*, const void*))
{
        CDLL node = cdll_deep_find(list, data, compare);

        if (node == NULL)
                return NULL;

        return cdll_delete_node(list, node);
}
