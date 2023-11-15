#include "shell.h"
#include <stdlib.h>

/**
 * listLen - determines the length of a linked list
 * @h: pointer to the first node of the list
 *
 * Return: the number of elements in the list
 */

size_t listLen(const list_t *h)
{
    size_t count = 0;

    while (h)
    {
        h = h->next;
        count++;
    }
    return (count);
}

/**
 * listToStrings - Returns an array of strings from a linked list
 * @head: Pointer to the first node of the list
 *
 * Return: Pointer to an array of strings
 */

char **listToStrings(list_t *head)
{
    list_t *current_node;
    size_t num_nodes, node_index;
    char **string_array, *current_string;

    num_nodes = listLen(head); /* Renamed variable to avoid name clash */
    if (!head || !num_nodes)
        return (NULL);

    string_array = malloc(sizeof(char *) * (num_nodes + 1));
    if (!string_array)
        return (NULL);

    for (current_node = head, node_index = 0; current_node;
         current_node = current_node->next, node_index++)
    {
        current_string = malloc(_strlen(current_node->str) + 1);
        if (!current_string)
        {
            while (node_index--)
                free(string_array[node_index]);
            free(string_array);
            return (NULL);
        }
        string_array[node_index] = _strcpy(current_string,
                                           current_node->str);
    }
    string_array[num_nodes] = NULL;

    return (string_array);
}

/**
 * printList - Prints all elements of a list_t linked list
 * @h: Pointer to the first node
 *
 * Return: Number of nodes in the list
 */

size_t printList(const list_t *h)
{
    size_t node_count = 0;

    while (h)
    {
        _puts(convert_number(h->num, 10, 0));
        _putchar(':');
        _putchar(' ');
        _puts(h->str ? h->str : "(nil)");
        _puts("\n");
        h = h->next;
        node_count++;
    }

    return node_count;
}

/**
 * nodeStartsWith - Returns node whose string starts with prefix
 * @node: Pointer to list head
 * @prefix: String to match
 * @c: The next character after prefix to match, or -1 for any character
 *
 * Return: Matching node or NULL if no match
 */

list_t *nodeStartsWith(list_t *node, char *prefix, char c)
{
    char *match_point = NULL;

    while (node)
    {
        match_point = starts_with(node->str, prefix);
        if (match_point && ((c == -1) || (*match_point == c)))
            return (node);
        node = node->next;
    }
    return (NULL);
}

/**
 * get_node_index - Gets the index of a node in a list
 * @head: Pointer to the head of the list
 * @node: Pointer to the node to find the index of
 *
 * Return: Index of the node or -1 if not found
 */

ssize_t get_node_index(list_t *head, list_t *node)
{
    size_t index = 0;

    while (head)
    {
        if (head == node)
            return index;
        head = head->next;
        index++;
    }
    return -1;
}
