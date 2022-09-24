// Implements a dictionary's functionality

#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//The choosen number of buckets in hash table is 1325
const unsigned int N = 1000000;

// Hash table
node *table[N];

int num_of_words = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Checks if the word is in the linked list
    bool check = false;
    int hash_index = hash(word);
    node *tmp = table[hash_index];

    while (tmp != NULL)
    {
        if (strcasecmp(word, tmp->word) == 0)
        {
            check = true;
        }
        tmp = tmp->next;
    }

    return check;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // The hash function checks the first three letters of each word and returns a number from 0 to 1325
    unsigned long hash = 5381;
    int c;
    while ((c = toupper(*word++)))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //Open the dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    //Create a buffer to store the current word in
    char dict_word[LENGTH + 1];

    //Read the individual string from the file into the buffer dict_word
    while (fscanf(file, "%s", dict_word) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, dict_word);
        n->next = NULL;
        //Hashing the word
        unsigned int hash_index = hash(dict_word);

        n->next = table[hash_index];
        // Set head to new pointer
        table[hash_index] = n;

        // Increment word count
        num_of_words++;
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    //Returns the number of words in the dictionary loaded, counted using the load function
    return num_of_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //Iterating over the hash table
    for (int i = 0; i < N; i++)
    {
        // Set cursor to this each bucket location
        node *cursor = table[i];
        // If cursor is not NULL, free
        while (cursor)
        {
            // Create temp
            node *tmp = cursor;
            // Move cursor to next node
            cursor = cursor->next;
            // Free up temp
            free(tmp);
        }
        // If cursor is NULL
        if (i == N - 1 && cursor == NULL)
        {
            return true;
        }
    }
    return false;
}