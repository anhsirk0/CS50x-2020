// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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
const unsigned int N = 1000;
int num_words = 0;
bool dict_loaded = false;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
	char check_word[strlen(word)];
	strcpy(check_word, word);
	for (int i = 0; check_word[i] != '\0'; i++)
	{
	    check_word[i] = tolower(word[i]);
	}
	int index = hash(check_word);
	if (table[index] != NULL)
	{
		for (node* ptr = table[index]; ptr != NULL; ptr = ptr->next)
		{
			if (strcasecmp(ptr->word,check_word) == 0)
			{
				return true;
			}
		}
	}   
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // int n = 0;
    // int hash = 0;
// 
    // for (int i = 0; word[i] != '\0'; i++)
    // {
    	// n = word[i] - word[0] + 3;
    	// hash = ((hash << 3) + n )%N;
    // }
    // 
    // return hash;
    int hash = 0;
    
    for (int i = 0; word[i] != '\0'; i++)
    {
	    hash = (hash << 2) ^ word[i];
    }
    return hash%N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    
    FILE* inptr = fopen(dictionary,"r");
    
    if (inptr == NULL)
    {
    	printf("Dictionary can't be loaded\n");
    	return dict_loaded;
    }
    
    for (unsigned int i = 0; i < N; i++)
    {
    	table[i] = NULL;
    }

    char word[LENGTH + 1];
    while (fscanf(inptr, "%s", word) != EOF)
    {
    	num_words++;
    	node* new = malloc(sizeof(node));
    	if (new == NULL)
    	{
    		free(new);
    	}

    	strcpy(new->word, (char*)word);
    	unsigned int index = hash(word);
    	// if (table[index] == NULL)
    	// {
    		// new->next = NULL;
    		// table[index] = new;
    	// }
    	// else
    	// {
    		new->next = table[index];
    		table[index] = new;
    	// }
    }
    fclose(inptr);
    dict_loaded = true;
   	return dict_loaded;

}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    
    if (!dict_loaded)
    {
    	return 0;
    }
    else
    {
    	return num_words;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    
    if (!dict_loaded)
    {
    	return false;
    }
    for (unsigned int i = 0; i < N; i++)
    {
    	if (table[i] != NULL)
    	{
    		node* ptr = table[i];
    		while (ptr != NULL)
    		{
    			node* pre_ptr = ptr;
    			ptr = ptr->next;
    			free(pre_ptr);
    		}
    	}
    }
    return true;
}
