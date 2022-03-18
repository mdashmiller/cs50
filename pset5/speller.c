// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1430;

// Hash table
node *table[N];

// Track size of dictionary as it grows
unsigned int dsize = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // hash the word
    int hval = hash(word);

    // Create cursor to point to first node in table at index hval
    node *crsr = table[hval];

    // Traverse the linked list looking for word
    while (crsr != NULL)
    {
        if (strcasecmp(crsr->word, word) == 0)
        {
            return true;
        }
        else
        {
            crsr = crsr->next;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int sum = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        sum += tolower(word[i]);
    }

    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *f = fopen(dictionary, "r");
    if (f == NULL)
    {
        printf("Could not load %s\n.", dictionary);
        return false;
    }

    // Prepare to load dictionary one word at a time
    char word[LENGTH + 1];

    while (fscanf(f, "%s", word) != EOF)
    {
        // Create a node for the current word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Could not load %s from %s\n.", word, dictionary);
            return false;
        }

        strcpy(n->word, word);
        n->next = NULL;

        // Hash the word
        int i = hash(word);

        // Insert word in hash table
        if (table[i] == NULL)
        {
            table[i] = n;
        }
        else
        {
            n->next = table[i]->next;
            table[i]->next = n;
        }

        // Update dictionary size
        dsize ++;
    }

    fclose(f);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dsize;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *crsr = table[i];
        node *tmp = crsr;

        while (crsr != NULL)
        {
            crsr = crsr->next;
            free(tmp);
            tmp = crsr;
        }
    }

    return true;
}
