#ifndef __TRIE_H__
#define __TRIE_H__
#include <stdbool.h>
#include <stdio.h>

#define TRIE_CHARSET_SIZE 27

/* Opaque definition of the Trie type*/
typedef struct s_Trie Trie;

/**
 * @brief Allocate a Trie.
 */
Trie* trie_create();

/**
 * @brief Delete a Trie.
 * @param root the trie to delete
 */
void trie_delete(Trie** root);

/**
 * @brief Insert a word in a Trie at the right position.
 * @param root the Trie
 * @param key the word
 */
void trie_add(Trie* root, const char* key);

/* Operator for map operation */
typedef void (*TrieMapOperator)(const Trie* root, char key, void* userdata);

/** @brief Mapping on the Trie
 * @param root the Trie
 * @param f mapping function for key processing
 * @param userdata user environment
 */
void trie_map(const Trie* root, TrieMapOperator f, void* userdata);

/**
 * @brief Search a specific word in the Trie.
 * Return true if the value is found, false otherwise
 * @param root the Trie
 * @param key the word
 */
bool trie_search(const Trie* root, const char* key);

/* Operator for visit operation*/
typedef void (*TrieVisitFunctor)(const char* key, void* userdata);

/**
 * @brief Visit on the Trie
 * @param root the Trie
 * @param f Visit function
 * @param userdata user environment
 */
void trie_visit(const Trie* root, TrieVisitFunctor f, void* userdata);

/**
 * @brief print the Trie
 * @param root the Trie
 */
void trie_print(const Trie* root);

#endif
