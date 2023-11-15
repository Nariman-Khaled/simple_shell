#include "shell.h"
#include <stdlib.h>

/**
 * prepend_node - Adds a node to the start of the list.
 * @head: Address of the pointer to the head node.
 * @str: String content of the new node.
 * @num: Numeric identifier for the new node.
 *
 * Return: Pointer to the new head of the list.
 */
list_t *prepend_node(list_t **head, const char *str, int num)
{
    list_t *new_node;

    if (!head)
        return (NULL);

    new_node = malloc(sizeof(list_t));
    if (!new_node)
        return (NULL);

    new_node->num = num;
    new_node->str = str ? _strdup(str) : NULL;
    if (str && !new_node->str)
    {
        free(new_node);
        return (NULL);
    }

    new_node->next = *head;
    *head = new_node;

    return new_node;
}


/**
 * append_node - Adds a node to the end of the list.
 * @head: Address of the pointer to the head node.
 * @str: String content of the new node.
 * @num: Numeric identifier for the new node.
 *
 * Return: Pointer to the new node.
 */
list_t *append_node(list_t **head, const char *str, int num)
{
    list_t *new_node, *current;

    if (!head)
        return (NULL);

    new_node = malloc(sizeof(list_t));
    if (!new_node)
        return (NULL);

    new_node->num = num;
    new_node->str = str ? _strdup(str) : NULL;
    new_node->next = NULL;

    if (!*head)
    {
        *head = new_node;
    }
    else
    {
        current = *head;
        while (current->next)
            current = current->next;
        current->next = new_node;
    }

    return new_node;
}


/**
 * printStringList - Prints the 'str' field of each node in a linked list.
 * @head: Pointer to the first node of the list.
 *
 * Return: The number of nodes in the list.
 */
size_t printStringList(const list_t *head)
{
    const list_t *current = head;
    size_t count = 0;

    while (current)
    {
        _puts(current->str ? current->str : "(nil)");
        _puts("\n");
        current = current->next;
        count++;
    }

    return count;
}


/**
 * remove_node_at - Removes a node from the linked list at the given index.
 * @head: Address of the pointer to the first node.
 * @index: The index of the node to delete.
 *
 * Return: 1 on success, 0 on failure.
 */
int remove_node_at(list_t **head, unsigned int index)
{
    list_t *current, *temp;
    unsigned int i = 0;

    if (!head || !*head)
        return (0);

    current = *head;
    if (index == 0)
    {
        *head = current->next;
        free(current->str);
        free(current);
        return (1);
    }

    while (current && i < index - 1)
    {
        current = current->next;
        i++;
    }

    if (!current || !current->next)
        return (0);

    temp = current->next;
    current->next = temp->next;
    free(temp->str);
    free(temp);

    return (1);
}


/**
 * free_linked_list - Frees all nodes of a linked list.
 * @head_ptr: Address of the pointer to the head node of the list.
 *
 * Return: void.
 */
void free_linked_list(list_t **head_ptr)
{
    list_t *current_node, *next_node;

    if (!head_ptr || !*head_ptr)
        return;

    current_node = *head_ptr;
    while (current_node)
    {
        next_node = current_node->next;
        free(current_node->str);
        free(current_node);
        current_node = next_node;
    }

    *head_ptr = NULL;
}

