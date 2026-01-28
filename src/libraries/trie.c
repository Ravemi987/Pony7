#include "trie.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>

#include <stdio.h>

const char to_index_trie[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 26, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18,
    19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

struct s_Trie {
    Trie* children[TRIE_CHARSET_SIZE];
    bool is_terminal;
};

/* Allow us to extend the trie for every charset.
 * Return the children index of the char given in argument
 */
int char_to_index(char c) {
    return to_index_trie[(int) c];
}

/* Allow us to extend the trie for every charset.
 * Return the char of the children index given in argument
 */
char index_to_char(int index) {
    if (0 <= index && index < 26) {
        return 'a' + index;
    } else if (index == 26) {
        return '-';
    } else {
        return '\0';
    }
}

Trie* trie_create_node() {
    Trie* t = malloc(sizeof(struct s_Trie));
    t->is_terminal = false;

    for (int i = 0; i < TRIE_CHARSET_SIZE; i++)
        t->children[i] = NULL;

    return t;
}

Trie* trie_create() {
    Trie* t = malloc(sizeof(struct s_Trie));
    t->is_terminal = true;

    for (int i = 0; i < TRIE_CHARSET_SIZE; i++)
        t->children[i] = NULL;

    return t;
}

void trie_free_node(Trie** n) {
    free(*n);
    *n = NULL;
}

void trie_delete(Trie** root) {
    Trie* current = *root;
    Trie* child;
    for (int i = 0; i < TRIE_CHARSET_SIZE; ++i) {
        if ((child = current->children[i]) != NULL) {
            trie_delete(&child);
        }
    }
    trie_free_node(root);
}

void trie_add(Trie* root, const char* key) {
    Trie* current = root;
    for (size_t i = 0; i < strlen(key); i++) {
        int index = char_to_index(key[i]);
        if (current->children[index] == NULL) {
            current->children[index] = trie_create_node();
        }
        current = current->children[index];
    }
    current->is_terminal = true;
}

void trie_map_rec(const Trie* root, TrieMapOperator f, void* userdata) {
    Trie* current = (Trie*) root;
    Trie* child;
    for (int i = 0; i < TRIE_CHARSET_SIZE; i++) {
        char c = index_to_char(i);
        if ((child = current->children[i]) != NULL) {
            f(child, c, userdata);
            trie_map_rec(child, f, userdata);
        }
    }
}

void trie_map(const Trie* root, TrieMapOperator f, void* userdata) {
    f(root, '\0', userdata);
    trie_map_rec(root, f, userdata);
}

bool trie_search(const Trie* root, const char* key) {
    Trie* current = (Trie*) root;
    for (size_t i = 0; i < strlen(key); i++) {
        int index = char_to_index(key[i]);
        if (current->children[index] == NULL) {
            return false;
        }
        current = current->children[index];
    }
    return (current != NULL && current->is_terminal);
}

void trie_visit_helper(const Trie* root, const char* prefix, TrieVisitFunctor f, void* userdata) {
    size_t prefix_length = strlen(prefix);
    char new_prefix[prefix_length + 2];
    strcpy(new_prefix, prefix);
    new_prefix[prefix_length + 1] = '\0';

    Trie* current = (Trie*) root;
    Trie* child;

    for (int i = 0; i < TRIE_CHARSET_SIZE; i++) {
        char c = index_to_char(i);
        if ((child = current->children[i]) != NULL) {
            new_prefix[prefix_length] = c;
            if (child->is_terminal)
                f(new_prefix, userdata);
            trie_visit_helper(child, new_prefix, f, userdata);
        }
    }
}

void trie_visit(const Trie* root, TrieVisitFunctor f, void* userdata) {
    if (root->is_terminal)
        f("", userdata);
    trie_visit_helper(root, "", f, userdata);
}

void trie_print_word(const char* key, void* userdata) {
    (void) userdata;
    printf("%s\n", key);
}

void trie_print(const Trie* root) {
    trie_visit(root, trie_print_word, NULL);
}
