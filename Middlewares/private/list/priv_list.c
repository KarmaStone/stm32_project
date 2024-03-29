/* list.c - a generic doubly linked list implementation
 * Original Author: Salvatore Sanfilippo <antirez at gmail dot com>
 * Author: hardy liu (hardy.liu at foxmail dot com)
 * License:  This is licensed under the same as redis 
 */
#include <stdlib.h>
#include <string.h>
#include "priv_list.h"

/* Create a new list. the created list can be freed with
 * list_release(), but private value of every node need to be freed
 * by the user before to call list_release().
 *
 * On error, NULL is returned. otherwise the pointer to the new list. */
list *list_create(void)
{
    struct list *list;

    if ((list = malloc(sizeof(*list))) == NULL)
        return NULL;
    list->head = list->tail = NULL;
    list->len = 0;
    list->dup = NULL;
    list->free = NULL;
    list->match = NULL;
    return list;
}

/* Free the whole list.
 *
 * This function can't fail. */
void list_release(list *list)
{
    unsigned long len;
    list_node *current, *next;

    current = list->head;
    len = list->len;
    while(len--) {
        next = current->next;
        if (list->free) list->free(current->value);
        free(current);
        current = next;
    }
    free(list);
}

/* Add a new node to the list, to head, contaning the specified 'value'
 * pointer as value.
 *
 * On error, NULL is returned and no operation is performed (i.e. the
 * list remains unaltered).
 * On success the 'list' pointer you pass to the function is returned. */
list *list_add_node_head(list *list, void *value)
{
    list_node *node;

    if ((node = malloc(sizeof(*node))) == NULL)
        return NULL;
    node->value = value;
    if (list->len == 0) {
        list->head = list->tail = node;
        node->prev = node->next = NULL;
    } else {
        node->prev = NULL;
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }
    list->len++;
    return list;
}

/* Add a new node to the list, to tail, containing the specified 'value'
 * pointer as value.
 *
 * On error, NULL is returned and no operation is performed (i.e. the
 * list remains unaltered).
 * On success the 'list' pointer you pass to the function is returned. */
list *list_add_node_tail(list *list, void *value)
{
    list_node *node;

    if ((node = malloc(sizeof(*node))) == NULL)
        return NULL;
    node->value = value;
    if (list->len == 0) {
        list->head = list->tail = node;
        node->prev = node->next = NULL;
    } else {
        node->prev = list->tail;
        node->next = NULL;
        list->tail->next = node;
        list->tail = node;
    }
    list->len++;
    return list;
}

list *list_insert_node(list *list, list_node *old_node, void *value, int after) 
{
    list_node *node;

    if ((node = malloc(sizeof(*node))) == NULL)
        return NULL;
    node->value = value;
    if (after) {
        node->prev = old_node;
        node->next = old_node->next;
        if (list->tail == old_node) {
            list->tail = node;
        }
    } else {
        node->next = old_node;
        node->prev = old_node->prev;
        if (list->head == old_node) {
            list->head = node;
        }
    }
    if (node->prev != NULL) {
        node->prev->next = node;
    }
    if (node->next != NULL) {
        node->next->prev = node;
    }
    list->len++;
    return list;
}

/* Remove the specified node from the specified list.
 * It's up to the caller to free the private value of the node.
 *
 * This function can't fail. */
void list_del_node(list *list, list_node *node)
{
    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;
    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;
    if (list->free) list->free(node->value);
    free(node);
    list->len--;
}

/* Returns a list iterator 'iter'. After the initialization every
 * call to list_next() will return the next element of the list.
 *
 * This function can't fail. */
list_iter *list_get_iterator(list *list, int direction)
{
    list_iter *iter;

    if ((iter = malloc(sizeof(*iter))) == NULL) return NULL;
    if (direction == START_FROM_HEAD)
        iter->next = list->head;
    else
        iter->next = list->tail;
    iter->direction = direction;
    return iter;
}

/* Release the iterator memory */
void list_release_iterator(list_iter *iter) {
    free(iter);
}

/* Create an iterator in the list private iterator structure */
void list_rewind(list *list, list_iter *li) {
    li->next = list->head;
    li->direction = START_FROM_HEAD;
}

void list_rewind_tail(list *list, list_iter *li) {
    li->next = list->tail;
    li->direction = START_FROM_TAIL;
}

/* Return the next element of an iterator.
 * It's valid to remove the currently returned element using
 * list_del_node(), but not to remove other elements.
 *
 * The function returns a pointer to the next element of the list,
 * or NULL if there are no more elements, so the classical usage patter
 * is:
 *
 * iter = list_get_iterator(list,<direction>);
 * while ((node = list_next(iter)) != NULL) {
 *     do_something_with(list_node_value(node));
 * }
 *
 * */
list_node *list_next(list_iter *iter)
{
    list_node *current = iter->next;

    if (current != NULL) {
        if (iter->direction == START_FROM_HEAD)
            iter->next = current->next;
        else
            iter->next = current->prev;
    }
    return current;
}

/* Duplicate the whole list. _on out of memory NULL is returned.
 * On success a copy of the original list is returned.
 *
 * The 'Dup' method set with list_set_dup_method() function is used
 * to copy the node value. Otherwise the same pointer value of
 * the original node is used as value of the copied node.
 *
 * The original list both on success or error is never modified. */
list *list_dup(list *orig)
{
    list *copy;
    list_iter *iter;
    list_node *node;

    if ((copy = list_create()) == NULL)
        return NULL;
    copy->dup = orig->dup;
    copy->free = orig->free;
    copy->match = orig->match;
    iter = list_get_iterator(orig, START_FROM_HEAD);
    while((node = list_next(iter)) != NULL) {
        void *value;

        if (copy->dup) {
            value = copy->dup(node->value);
            if (value == NULL) {
                list_release(copy);
                list_release_iterator(iter);
                return NULL;
            }
        } else
            value = node->value;
        if (list_add_node_tail(copy, value) == NULL) {
            list_release(copy);
            list_release_iterator(iter);
            return NULL;
        }
    }
    list_release_iterator(iter);
    return copy;
}

/* Search the list for a node matching a given key.
 * The match is performed using the 'match' method
 * set with list_set_match_method(). If no 'match' method
 * is set, the 'value' pointer of every node is directly
 * compared with the 'key' pointer.
 *
 * On success the first matching node pointer is returned
 * (search starts from head). If no matching node exists
 * NULL is returned. */
list_node *list_search_key(list *list, void *key)
{
    list_iter *iter;
    list_node *node;

    iter = list_get_iterator(list, START_FROM_HEAD);
    while((node = list_next(iter)) != NULL) {
        if (list->match) {
            if (list->match(node->value, key)) {
                list_release_iterator(iter);
                return node;
            }
        } else {
            if (key == node->value) {
                list_release_iterator(iter);
                return node;
            }
        }
    }
    list_release_iterator(iter);
    return NULL;
}

/* Return the element at the specified zero-based index
 * where 0 is the head, 1 is the element next to head
 * and so on. Negative integers are used in order to count
 * from the tail, -1 is the last element, -2 the penultimate
 * and so on. If the index is out of range NULL is returned. */
list_node *list_index(list *list, long index) 
{
    list_node *n;

    if (index < 0) {
        index = (-index)-1;
        n = list->tail;
        while(index-- && n) n = n->prev;
    } else {
        n = list->head;
        while(index-- && n) n = n->next;
    }
    return n;
}

/* Rotate the list removing the tail node and inserting it to the head. */
void list_rotate(list *list) {
    list_node *tail = list->tail;

    if (list_length(list) <= 1) return;

    /* Detach current tail */
    list->tail = tail->prev;
    list->tail->next = NULL;
    /* Move it as head */
    list->head->prev = tail;
    tail->prev = NULL;
    tail->next = list->head;
    list->head = tail;
}


