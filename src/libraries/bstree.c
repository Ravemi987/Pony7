#include "bstree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

/*------------------------   PROTOTYPES   -----------------------------*/

void bstree_remove_node(ptrBinarySearchTree *t, ptrBinarySearchTree current);
void bstree_free_node(const BinarySearchTree *t, void *userData);
void printNode(const BinarySearchTree *n, void *out);
void leftrotate(BinarySearchTree *x);
void rightrotate(BinarySearchTree *y);
ptrBinarySearchTree grandparent(ptrBinarySearchTree n);
ptrBinarySearchTree uncle(ptrBinarySearchTree n);
ptrBinarySearchTree fixredblack_insert(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_insert_case0(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_insert_case1(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_insert_case2(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_insert_case2_left(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_insert_case2_right(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_remove(ptrBinarySearchTree p, ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_remove_case1_left(ptrBinarySearchTree p, ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_remove_case1_right(ptrBinarySearchTree p, ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_remove_case1(ptrBinarySearchTree p, ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_remove_case2_left(ptrBinarySearchTree p, ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_remove_case2_right(ptrBinarySearchTree p, ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_remove_case2(ptrBinarySearchTree p, ptrBinarySearchTree x);

/*------------------------   RBT ENUM   -----------------------------*/

typedef enum {red, black} NodeColor;

/*------------------------  BSTreeType  -----------------------------*/

struct _bstree {
    BinarySearchTree *parent;
    BinarySearchTree *left;
    BinarySearchTree *right;
    NodeColor color;
    int root;
};

/*------------------------  BaseBSTree  -----------------------------*/

BinarySearchTree *bstree_create(void) {
    return NULL;
}

/* This constructor is private so that we can maintain the oredring invariant on
 * nodes. The only way to add nodes to the tree is with the bstree_add function
 * that ensures the invariant.
 */
BinarySearchTree *bstree_cons(BinarySearchTree *left, BinarySearchTree *right, int root) {
    BinarySearchTree *t = malloc(sizeof(struct _bstree));
    t->parent = NULL;
    t->left = left;
    t->right = right;
    t->color = red; /* Lorsque l'on créé un noeud, celui-ci prend la couleur rouge */
    if (t->left != NULL)
        t->left->parent = t;
    if (t->right != NULL)
        t->right->parent = t;
    t->root = root;
    return t;
}

/* Fonction permettant de libérer la mémoire occupée par un noeud */
void bstree_free_node(const BinarySearchTree *t, void *userData) {
    (void) userData;
    free((BinarySearchTree *)t);
}

/* Fonction qui supprime et libère la mémoire occupée par un arbre,
 * en effectuant un parcours postfixe en profondeur d'abord avec la fonction bstree_free_node
 */
void bstree_delete(ptrBinarySearchTree *t) {
    bstree_depth_postfix(*t, bstree_free_node, NULL);
}

bool bstree_empty(const BinarySearchTree *t) {
    return t == NULL;
}

int bstree_root(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->root;
}

BinarySearchTree *bstree_left(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->left;
}

BinarySearchTree *bstree_right(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->right;
}

BinarySearchTree *bstree_parent(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->parent;
}

/*------------------------  BSTreeDictionary  -----------------------------*/

/* Obligation de passer l'arbre par référence pour pouvoir le modifier */
void bstree_add(ptrBinarySearchTree *t, int v) {
	ptrBinarySearchTree *cur = t;
    BinarySearchTree *par = NULL;
    /* Recherche de l'endroit où le nouveau noeud doit être inséré */
    while (*cur != NULL) {
        par = *cur;
        if ((*cur)->root == v) {
            return;
        }
        cur = ((*cur)->root > v) ? &((*cur)->left) : &((*cur)->right);
    }

    *cur = bstree_cons(NULL, NULL, v);
    (*cur)->parent = par;

    /* Restauration des proriétés après insertion selon l'algorithme donné */
    BinarySearchTree *stop = fixredblack_insert(*cur);
    /* stop est le noeud auquel s'arrête la coloration. 
       Il peut devenir la nouvelle racine de l'arbre */
    if (stop->parent == NULL)
        *t = stop;
}

bool bstree_search(const BinarySearchTree *t, int v) {
    while (!bstree_empty(t)) {
        if (t->root == v)
            return true;
        t = (t->root > v) ? t->left : t->right;
    }
    return false;
}

BinarySearchTree *bstree_successor(const BinarySearchTree *x) {
    assert(!bstree_empty(x));

    BinarySearchTree *cur = x->right;
    if (cur) {
        /* Le noeud 'x' a un fils droit, on va le plus à gauche possible */
        while (cur->left)
            cur = cur->left;
    } else {
        cur = x->parent;
        /* Sinon, il faut remonter dans l'arbre tant qu'il a une valeur supérieure à son parent */
        while(cur && (x == cur->right)) {
            x = cur;
            cur = cur->parent;
        }
    }
    return cur;
}

BinarySearchTree *bstree_predecessor(const BinarySearchTree *x) {
    assert(!bstree_empty(x));

    BinarySearchTree *cur = x->left;
    if (cur) {
        /* Le noeud 'x' a un fils gauche, on va le plus à droite possible */
        while (cur->right)
            cur = cur->right;
    } else {
        cur = x->parent;
        /* Sinon, il faut remonter dans l'arbre tant qu'il a une valeur inférieure à son parent */
        while (cur && (x == cur->left)) {
            x = cur;
            cur = cur->parent;
        }
    }
    return cur;
}

void bstree_swap_nodes(ptrBinarySearchTree *tree, ptrBinarySearchTree from, ptrBinarySearchTree to) {
    assert(!bstree_empty(*tree) && !bstree_empty(from) && !bstree_empty(to));
    /* Ne pas faire d'opérations inutiles si on essaye d'échanger un noeud avec lui-même */
    if (from == to)
        return;

    /* Mémorisation des liens vers les fils des noeuds à échanger (pas obligatoire) */
    ptrBinarySearchTree fromLeft = from->left;
    ptrBinarySearchTree fromRight = from->right;
    ptrBinarySearchTree toLeft = to->left;
    ptrBinarySearchTree toRight = to->right;

    /* Le noeud 'to' devient le parent des fils du noeud 'from' et inversement */
    if (fromLeft) fromLeft->parent = to;
    if (fromRight) fromRight->parent = to;
    if(toLeft) toLeft->parent = from;
    if(toRight) toRight->parent = from;

    /* Les fils du noeud 'from' deviennent ceux du noeud 'to' et inversement */
    to->left = fromLeft;
    to->right = fromRight;
    from->left = toLeft;
    from->right = toRight;
    
    /* Le parent du noeud 'from' a maintenant comme fils le noeud 'to' */
    if (from->parent) {
        if (from->parent->left == from) {
            from->parent->left = to;
        } else {
            from->parent->right = to;
        }
    } else {
        *tree = to; /* Si 'from' n'a pas de parent, 'to' devient la racine de l'arbre */
    }

    /* Le parent du noeud 'to' a maintenant comme fils le noeud 'from' */
    if (to->parent) {
        if (to->parent->left == to) {
            to->parent->left = from;
        } else {
            to->parent->right = from;
        }
    } else {
        *tree = from; /* Si 'to' n'a pas de parent, 'from' devient la racine de l'arbre */
    }

    /* Le parent du noeud 'from' est maintenant celui du noeud 'to' et inversement */
    ptrBinarySearchTree temp = from->parent;
    from->parent = to->parent;
    to->parent = temp;
}

// t -> the tree to remove from, current -> the node to remove
void bstree_remove_node(ptrBinarySearchTree *t, ptrBinarySearchTree current) {
    assert(!bstree_empty(*t) && !bstree_empty(current));
    ptrBinarySearchTree substitute;
 
    /* Identification du noeud qui remplacera le noeud supprimé selon le cas */
    if (current->left == current->right) { /* current est une feuille */
        substitute = bstree_create();
    } else if (bstree_empty(current->left)) { /* current a un fils droit */
        substitute = current->right;
    } else if (bstree_empty(current->right)) { /* current a un fils gauche */
        substitute = current->left;
    } else { /* current a deux fils */
        ptrBinarySearchTree leaf = bstree_successor(current);
        if (leaf) {
            bstree_swap_nodes(t, current, leaf);/* échange entre le noeud et son successeur */
            /* Dans ce cas précis, seule la clé du noeud doit être échangée, pas la couleur (provoque des bugs sinon) */
            NodeColor tmp = current->color;
            current->color = leaf->color;
            leaf->color = tmp;
            substitute = current->right; /* le noeud remplacant est le fils droit du successeur s'il existe */
        }
    }
    /* Modification des liens */
    if (substitute != NULL) {
        substitute->parent = current->parent;
    }
    if (bstree_empty(current->parent)) {
        *t = substitute; /* Si le noeud supprimé est la racine, substitute devient la racine de l'arbre */
    } else if (current->parent->left == current) {
        current->parent->left = substitute;
    } else {
        current->parent->right = substitute;
    }
    /* Restauration (correction) des propriétés après suppression selon l'algorithme donné */
    if (current->color == black) { /* Si le noeud supprimé est noir, la propriété 3 est rompue */
        if ((substitute == NULL) || (substitute->color == black)) {
            ptrBinarySearchTree subtreeroot = fixredblack_remove(current->parent, substitute);
            if (subtreeroot->parent == NULL)
                *t = subtreeroot;
        } else {
            substitute->color = black;
        }
    }
    free(current);
}

void bstree_remove(ptrBinarySearchTree *t, int v) {
    ptrBinarySearchTree current = *t;
    /* Recherche du noeud avec la valeur v */
    while (current && current->root != v)
        current = (current->root > v) ? current->left : current->right;
    /* Si ce noeud existe, suppression du noeud */
    if (current)
        bstree_remove_node(t, current);
}

/*------------------------  BSTreeVisitors  -----------------------------*/

void bstree_depth_prefix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    if (bstree_empty(t))
        return;
    f(t, userData);
    bstree_depth_prefix(t->left, f, userData);
    bstree_depth_prefix(t->right, f, userData);
}

void bstree_depth_infix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    if (bstree_empty(t))
        return;
    bstree_depth_infix(t->left, f, userData);
    f(t, userData);
    bstree_depth_infix(t->right, f, userData);
}

void bstree_depth_postfix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    if (bstree_empty(t))
        return;
    bstree_depth_postfix(t->left, f, userData);
    bstree_depth_postfix(t->right, f, userData);
    f(t, userData);
}

void bstree_iterative_depth_infix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    BinarySearchTree *current = (BinarySearchTree*)t;
    BinarySearchTree *prev = t->parent; /* prev est le dernier noeud visité */
    BinarySearchTree *next = t->parent; /* next est le prochain noeud qui doit être visité */

    while (!bstree_empty(current)) {
        /* 1ere visite : si on vient du parent, on doit aller sur le fils gauche */
        if (current->parent == prev) {
            prev = current; 
            next = current->left;
        }
        /* 2eme visite : si pas de fils gauche OU qu'on vient du fils gauche, on doit aller sur le fils droit */
        if (bstree_empty(next) || current->left == prev) {
            f(current, userData);
            prev = current; 
            next = current->right;
        }
        /* 3eme visite : si pas de fils droit OU qu'on vient du fils droit, on doit retourner sur le parent */
        if (bstree_empty(next) || current->right == prev) {
            prev = current; 
            next = current->parent;
        }
        current = next;
    }
}

void bstree_iterative_breadth_prefix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    Queue *q = createQueue(); /* Création d'une file */

    q = queuePush(q, t); /* On push la racine de l'arbre dans la file  */
    while (!queueEmpty(q)) {
        BinarySearchTree *cur = (BinarySearchTree*)queueTop(q);
        f(cur, userData); /* On effectue le traitement */
        /* Tant qu'il reste des noeuds à visiter, on met les fils gauche et droit dans la file */
        q = queuePop(q);
        if (!bstree_empty(cur->left))
            q = queuePush(q, cur->left);
        if (!bstree_empty(cur->right))
            q =queuePush(q, cur->right);
    }

    deleteQueue(&q); /* Suppression de la file*/
}

/*------------------------  BSTreeIterator  -----------------------------*/

struct _BSTreeIterator {
    /* the collection the iterator is attached to */
    const BinarySearchTree *collection;
    /* the first element according to the iterator direction */
    const BinarySearchTree *(*begin)(const BinarySearchTree *);
    /* the current element pointed by the iterator */
    const BinarySearchTree *current;
    /* function that goes to the next element according to the iterator direction */
    BinarySearchTree *(*next)(const BinarySearchTree *);
};

/* minimum element of the collection */
const BinarySearchTree *goto_min(const BinarySearchTree *e) {
    while (e->left)
        e = e ->left;
    return e;
}

/* maximum element of the collection */
const BinarySearchTree *goto_max(const BinarySearchTree *e) {
    while (e->right)
        e = e->right;
    return e;
}

/* constructor */
BSTreeIterator *bstree_iterator_create(const BinarySearchTree *collection, IteratorDirection direction) {
	BSTreeIterator *i = malloc(sizeof(BSTreeIterator));
    if (i == NULL) return NULL;

    i->collection = collection;

    if (direction == forward) {
        i->begin = goto_min;
        i->next = bstree_successor;
    } else {
        i->begin = goto_max;
        i->next = bstree_predecessor;
    }

    i->current = i->begin(collection);
    return i;
}

/* destructor */
void bstree_iterator_delete(ptrBSTreeIterator *i) {
    free(*i);
    *i = NULL;
}

BSTreeIterator *bstree_iterator_begin(BSTreeIterator *i) {
    i->current = i->begin(i->collection);
    return i;
}

bool bstree_iterator_end(const BSTreeIterator *i) {
    return i->current == NULL;
}

BSTreeIterator *bstree_iterator_next(BSTreeIterator *i) {
    i->current = i->next(i->current);
    return i;
}

const BinarySearchTree *bstree_iterator_value(const BSTreeIterator *i) {
    return i->current;
}

/*------------------------  RedBlackTrees  -----------------------------*/

void printNode(const BinarySearchTree *n, void *out) {
    FILE *file = (FILE *) out;
    char *color = (n->color == red) ? "red" : "grey";

    printf("%d ", bstree_root(n));
    fprintf(file, "\tn%d [style=filled, fillcolor=%s, label=\"{{<parent>}|%d|{<left>|<right>}}\"];\n",
            bstree_root(n), color, bstree_root(n));

    if (bstree_left(n)) {
        fprintf(file, "\tn%d:left:c -> n%d:parent:c [headclip=false, tailclip=false]\n",
                bstree_root(n), bstree_root(bstree_left(n)));
    } else {
        fprintf(file, "\tlnil%d [style=filled, fillcolor=grey, label=\"NIL\"];\n", bstree_root(n));
        fprintf(file, "\tn%d:left:c -> lnil%d:n [headclip=false, tailclip=false]\n",
                bstree_root(n), bstree_root(n));
    }
    if (bstree_right(n)) {
        fprintf(file, "\tn%d:right:c -> n%d:parent:c [headclip=false, tailclip=false]\n",
                bstree_root(n), bstree_root(bstree_right(n)));
    } else {
        fprintf(file, "\trnil%d [style=filled, fillcolor=grey, label=\"NIL\"];\n", bstree_root(n));
        fprintf(file, "\tn%d:right:c -> rnil%d:n [headclip=false, tailclip=false]\n",
                bstree_root(n), bstree_root(n));
    }
}

void rbtree_export_dot(const BinarySearchTree *t, FILE *file) {
    fprintf(file, "digraph RedBlackTree {\n\tgraph [ranksep =0.5];\n\tnode [shape = record];\n\n");
    bstree_iterative_depth_infix(t, printNode, file);
    fprintf(file, "\n}\n");
}

void leftrotate(BinarySearchTree *x) {
    assert(!bstree_empty(x) && !bstree_empty(x->right));
    BinarySearchTree *gamma;
    BinarySearchTree *alpha;

    /* Memorisation de alpha et gamma */
    gamma = x->right->right;
    alpha = x->left;

    /* Déplacement de beta a la place de gamma */
    if (x->right->left)
        x->right->right = x->right->left;
    else
        x->right->right = NULL;

    /* Déplacement de alpha a l'ancienne place de beta */
    if (alpha) {
        x->right->left = alpha;
        alpha->parent = x->right;
    } else {
        x->right->left = NULL;
    }

    /* La branche de droite devient celle de gauche */
    x->left = x->right;

    /* Deplacement de gamma sur la branche de droite */
    if (gamma) {
        x->right = gamma;
        gamma->parent = x;
    } else {
        x->right = NULL;
    }

    /* Echange de x et y */
    bstree_swap_nodes(&x, x, x->left);
}

void rightrotate(BinarySearchTree *y) {
    assert(!bstree_empty(y) && !bstree_empty(y->left));
    BinarySearchTree *gamma;
    BinarySearchTree *alpha;

    /* Memorisation de alpha et gamma */
    alpha = y->left->left;
    gamma = y->right;

    /* Déplacement de beta a la place de alpha */
    if (y->left->right)
        y->left->left = y->left->right;
    else
        y->left->left = NULL;

    /* Déplacement de gamma a l'ancienne place de beta */
    if (gamma) {
        y->left->right = gamma;
        gamma->parent = y->left;
    } else {
        y->left->right = NULL;
    }

    /* La branche de gauche devient celle de droite */
    y->right = y->left;

    /* Déplacement de alpha sur la branche de gauche */
    if (alpha) {
        y->left = alpha;
        alpha->parent = y;
    } else {
        y->left = NULL;
    }

    /* Echange de x et y */
    bstree_swap_nodes(&y, y, y->right);
}

void testrotateleft(BinarySearchTree *t) {
    leftrotate(t);
}

void testrotateright(BinarySearchTree *t) {
    rightrotate(t);
}

ptrBinarySearchTree grandparent(ptrBinarySearchTree n) {
    if (n && n->parent)
        return n->parent->parent;
    else
        return NULL;
}

ptrBinarySearchTree uncle(ptrBinarySearchTree n) {
    ptrBinarySearchTree gp = grandparent(n);
    if (!gp) 
        return NULL;
    if (n->parent == gp->left)
        return gp->right;
    else
        return gp->left;
}

/* ---------- Les fonctions suivantes suivent les algorithmes décrits dans le sujet ---------- */

/* x et son père p sont tous les deux rouges */
ptrBinarySearchTree fixredblack_insert(ptrBinarySearchTree x) {
    if (x->parent && x->parent->color == red)
        return fixredblack_insert_case0(x);
    else
        return x;
}

/* Cas 0 : le noeud père p de x est la racine de l'arbre */
ptrBinarySearchTree fixredblack_insert_case0(ptrBinarySearchTree x) {
    ptrBinarySearchTree p = x->parent;
    if (p->parent == NULL) {
        p->color = black;
        return p;
    } else {
        return fixredblack_insert_case1(x);
    }
}

/* Cas 1 : l'oncle f de x est rouge */
ptrBinarySearchTree fixredblack_insert_case1(ptrBinarySearchTree x) {
    ptrBinarySearchTree f = uncle(x);
    ptrBinarySearchTree pp = grandparent(x);
    if (f && f->color == red) {
        f->color = x->parent->color = black;
        pp->color = red;
        return fixredblack_insert(pp);
    } else {
        return fixredblack_insert_case2(x);
    }
}

/* Cas 2 : 'l'oncle de x est noir */
ptrBinarySearchTree fixredblack_insert_case2(ptrBinarySearchTree x) {
    ptrBinarySearchTree pp = grandparent(x);
    if (x->parent == pp->left)
        return fixredblack_insert_case2_left(x);
    else
        return fixredblack_insert_case2_right(x); 
}

/* Cas 2 (le père p de x est le fils gauche de son propre père pp) */
ptrBinarySearchTree fixredblack_insert_case2_left(ptrBinarySearchTree x) {
    ptrBinarySearchTree p = x->parent;
    ptrBinarySearchTree pp = grandparent(x);
    if (x == p->left) {
        rightrotate(pp);
        p->color = black;
        pp->color = red;
        return p; /* Nouvelle racine à cause de la rotation */
    } else {
        leftrotate(p);
        rightrotate(pp);
        x->color = black;
        pp->color = red;
        return x;
    }
}

/* Cas 2 (le père p de x est le fils droit de son propre père pp) */
ptrBinarySearchTree fixredblack_insert_case2_right(ptrBinarySearchTree x) {
    ptrBinarySearchTree p = x->parent;
    ptrBinarySearchTree pp = grandparent(x);
    if (x == p->right) {
        leftrotate(pp);
        p->color = black;
        pp->color = red;
        return p; /* Nouvelle racine à cause de la rotation */
    } else {
        rightrotate(p);
        leftrotate(pp);
        x->color = black;
        pp->color = red;
        return x;
    }
}

ptrBinarySearchTree fixredblack_remove(ptrBinarySearchTree p, ptrBinarySearchTree x) {
    if (p == NULL) { /* Cas 0 : x est la racine de l'arbre */
        return x; /* x (subtitute) est NULL ou déjà de couleur noire */
    } else {
        ptrBinarySearchTree f = (p->left == x) ? p->right : p->left;
        if (f->color == black)
            return fixredblack_remove_case1(p, x);
        else
            return fixredblack_remove_case2(p, x);
    }
}

/* Cas 1 : le frère f de x est noir, x est le fils gauche de son père p */
ptrBinarySearchTree fixredblack_remove_case1_left(ptrBinarySearchTree p, ptrBinarySearchTree x) {
    ptrBinarySearchTree f = p->right;
    if ((!f->left || (f->left->color == black)) && (!f->right || (f->right->color == black))) {
        if (x) x->color = black;
        f->color = red;
        if (p->color == red) { /* cas 1.1 */
            p->color = black;
            return p;
        }
        else { /* cas 1.2 */
            return fixredblack_remove(p->parent, p);
        }
    } else if (f->right && f->right->color == red) {
        leftrotate(p);
        f->color = p->color;
        if (x) x->color = black;
        p->color = f->right->color = black;
        return f;
    } else { /* cas 1.3 : f->right est noir et f->left est rouge */
        f->left->color = black; f->color = red;
        rightrotate(f);
        leftrotate(p);
        f->color = p->color;
        return f->parent;
    }
}

/* Cas 1 : le frère f de x est noir, x est le fils droit de son père p */
ptrBinarySearchTree fixredblack_remove_case1_right(ptrBinarySearchTree p, ptrBinarySearchTree x) {
    ptrBinarySearchTree f = p->left;
    if ((!f->left || (f->left->color == black)) && (!f->right || (f->right->color == black))) {
        if (x) x->color = black;
        f->color = red;
        if (p->color == red) { /* cas 1.1 */
            p->color = black;
            return p;
        }
        else { /* cas 1.2 */
            return fixredblack_remove(p->parent, p);
        }
    } else if (f->left && f->left->color == red) {
        rightrotate(p);
        f->color = p->color;
        if (x) x->color = black;
        p->color = f->left->color = black;
        return f;
    } else { /* cas 1.3 : f->left est noir et f->right est rouge */
        f->right->color = black; f->color = red;
        leftrotate(f);
        rightrotate(p);
        f->color = p->color;
        return f->parent;
    }
}

/* Cas 1 : le frère f de x est noir */
ptrBinarySearchTree fixredblack_remove_case1(ptrBinarySearchTree p, ptrBinarySearchTree x) {
    if (p->left == x)
        return fixredblack_remove_case1_left(p, x);
    else
        return fixredblack_remove_case1_right(p, x);
}

/* Cas 2 : le frère f de x est rouge, x est le fils gauche de son père p; */
ptrBinarySearchTree fixredblack_remove_case2_left(ptrBinarySearchTree p, ptrBinarySearchTree x) {
    ptrBinarySearchTree f = p->right;
    leftrotate(p);
    p->color = red;
    f->color = black;
    fixredblack_remove_case1_left(p, x);
    return f;
}

/* Cas 2 : le frère f de x est rouge, x est le fils droit de son père p */
ptrBinarySearchTree fixredblack_remove_case2_right(ptrBinarySearchTree p, ptrBinarySearchTree x) {
    ptrBinarySearchTree f = p->left;
    rightrotate(p);
    p->color = red;
    f->color = black;
    fixredblack_remove_case1_right(p, x);
    return f;
}

/* Cas 2 : le frère f de x est rouge */
ptrBinarySearchTree fixredblack_remove_case2(ptrBinarySearchTree p, ptrBinarySearchTree x) {
    if (p->left == x)
        return fixredblack_remove_case2_left(p, x);
    else
        return fixredblack_remove_case2_right(p, x);
}
