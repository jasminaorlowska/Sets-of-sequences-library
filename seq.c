#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include "seq.h"
#include "str.h"

struct node {
    struct node *next;
    char *name;
    size_t num_of_el;
}; typedef struct node node;

struct tree {
    struct tree *zero;
    struct tree *one;
    struct tree *two;
    node *class;
}; typedef struct tree tree;

struct seq {
    tree *root;
    node *classes;
};

node *new_node() {
    node *new_node = malloc(sizeof(node));

    if (new_node == NULL) {
        return NULL;
    }

    new_node->next = NULL;
    new_node->name = NULL;
    new_node->num_of_el = 0;

    return new_node;
};

//Adds a class to the set's list of equiv classes
void add_class(seq_t *p, node * new_class) {
    if (p == NULL || new_class == NULL) { //equiv class doesnt exist
        return;
    }

    node * curr = p->classes;
    while (curr && curr->next) {
        curr = curr->next;
    }
    curr->next = new_class;

    return;
}

//Removes specific equivalence class from the list
void delete_class(node *list, node *class) {
    if (list == NULL) return;

    node *curr = list->next;
    node *prev = list;

    while (curr && curr != class) {
        prev = curr; 
        curr = curr->next;
    }

    if (curr == NULL) {
        return;
    }
    else {
        prev->next = curr->next;
        if (curr->name) free(curr->name);
        free(curr);
    }
}

/*Moving all elements from one equiv class to another*/
void change_class(tree *t, node *change_from, node *change_to) {
    if (t == NULL) return; 

    if (t->class) {
        if (t->class == change_from) {
            t->class = change_to;
        }
    }

    change_class(t->zero, change_from, change_to);
    change_class(t->one, change_from, change_to);
    change_class(t->two, change_from, change_to);
}

/*Moving all elements from one equiv class to another
and deleting the first class*/
void move_from_class_to_class(seq_t *p, node * move_from, node * move_to) {
    if (p == NULL || move_from == NULL || move_to == NULL) return;

    move_to->num_of_el = ((move_to->num_of_el) + (move_from->num_of_el));

    change_class(p->root->zero, move_from, move_to);
    change_class(p->root->one, move_from, move_to);
    change_class(p->root->two, move_from, move_to);

    delete_class(p->classes, move_from);
}

//Removes all empty equivalence classes
void remove_empty_classes(seq_t *p) {
    node *prev = p->classes;
    node *curr = p->classes->next;

    while (curr) {
        if (curr->num_of_el == 0) {
            if (curr->name) free(curr->name);
            prev->next = curr->next;
            free(curr);
            curr = prev->next;
        }
        else {
            prev = curr;
            curr = curr->next;
        }
    }
}

//Deletes list of equivalence classes.
void delete_class_list(node *list) {
    if (list == NULL) return;

    node *tmp = list->next;   
    free(list->name);
    free(list);
    delete_class_list(tmp);
}

tree *new_tree_node() {
    tree *new_tree_node = malloc(sizeof(tree));

    if (new_tree_node == NULL) {
        return NULL;
    }

    new_tree_node->zero = NULL;
    new_tree_node->one = NULL;
    new_tree_node->two = NULL;
    new_tree_node->class = NULL;
    
    return new_tree_node;
}

 tree *get_tree_child(tree *t, int i) {
        if (t == NULL) return NULL;
        if (i == 0) return t->zero;
        if (i == 1) return t->one;
        if (i == 2) return t->two;
        return NULL;
}

 tree **get_tree_child_address(tree *t, int i) {
        if (t == NULL) return NULL;
        if (i == 0) return &t->zero;
        if (i == 1) return &t->one;
        if (i == 2) return &t->two;
        return NULL;
}

/*Adds a child to a tree node, child_num specifies
which child (zero, one or two)*/
bool add_tree_child(tree *t, int child_num) {
    if (t == NULL) {
        return false;
    }

    tree *new_t_node = new_tree_node();
    if (new_t_node == NULL) return false;

    tree ** t_child = get_tree_child_address(t, child_num);
    *t_child = new_t_node;

    return true;
}

/*Deletes a tree, final means that equiv classes
have already been freed or dont exist */
void delete_tree(seq_t *p, tree *root, bool final) {
    if (root == NULL) return;

    tree *t0 = root->zero;
    tree *t1 = root->one;
    tree *t2 = root->two;

    if (!final) {           //equivalence classes exist
        node *t_class = root->class;
        if (t_class != NULL) {
            if ((t_class->num_of_el) == 1) {
                delete_class(p->classes, t_class);
            }
            else {
                t_class->num_of_el = (t_class->num_of_el) - 1;
            }
        }
    }
    else {  //equivalence classes dont exist
        root->class = NULL;
    }

    free(root);
    delete_tree(p, t0, final);
    delete_tree(p, t1, final);
    delete_tree(p, t2, final);
}

/*Removes the subtree rooted at the specific child node
from the given parent node.
meaning of bool final explained in delete_tree function*/
void delete_subtree(seq_t *p, tree *parent, int num, bool final) {
    if (parent == NULL) return;
 
    tree *to_delete = get_tree_child(parent, num);
    tree **to_delete_address = get_tree_child_address(parent, num);

    delete_tree(p, to_delete, final);
    *to_delete_address = NULL;
}

seq_t * seq_new() {
    node *c = new_node();
    if (c == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    tree *r = new_tree_node();
    if (r == NULL) {
        free(c);
        errno = ENOMEM;
        return NULL;
    }

    seq_t *new_seq = malloc(sizeof(seq_t));
    if (new_seq == NULL) {
        free(r);
        free(c);
        errno = ENOMEM;
        return NULL;
    }

    new_seq->root = r;
    new_seq->classes = c;

    return new_seq;
}

//Deletes a set of sequences
void seq_delete(seq_t *p) {
    if (p == NULL) return;

    delete_class_list(p->classes);
    delete_tree(p, p->root, true);
    free(p);
}

/*Adds a sequence to set*/
int seq_add(seq_t *p, char const *s) {
    if (p == NULL || !valid_string(s)) {
        errno = EINVAL;
        return -1;
    }
    
    //To track first added element if malloc is NULL
    tree *first_added_parent = NULL;
    int first_added_num = -1;

    tree *curr = p->root;
    int pos = 0;

    while (*(s+pos) != '\0') {
        tree *next_t; 
        int i =  *(pos+s) - '0';
        next_t = get_tree_child(curr, i);

        if ((next_t) == NULL) {
            if (!add_tree_child(curr, i)) {
                delete_subtree(p, first_added_parent, first_added_num, true); 
                errno = ENOMEM;
                return -1;
            }
            //Checking if element is the first one added.
            if (first_added_parent == NULL) {
                first_added_parent = curr;
                first_added_num = i;
            }
        }

        curr = get_tree_child(curr, i);
        pos++;   
    }

    if (first_added_parent == NULL) return 0;
    return 1;
}

/*Removes from the set a specific sequence s
and all sequences prefixed by it*/
int seq_remove(seq_t *p, const char * s) {
    if (p == NULL || !valid_string(s)) {
        errno = EINVAL;
        return -1;
    }

    tree *prev = p->root;
    tree *curr = p->root;
    int pos = 0;

    while (*(s+pos) != '\0') {
        char curr_char = *(s+pos);
        tree *next_t = get_tree_child(curr, curr_char - '0');

        if (next_t == NULL) {
            pos++;
            return 0;
        }

        prev = curr;
        curr = next_t;
        pos++;
    }

    int child_num = *(s + pos - 1) - '0';
    delete_subtree(p, prev, child_num, false);
    remove_empty_classes(p);

    return 1;
}

/*Checks whether a given sequence belongs to the set p*/
int seq_valid(seq_t *p, char const *s) {
    if (p == NULL || !valid_string(s)) {
        errno = EINVAL;
        return -1;
    }

    tree * curr = p -> root;
    int pos = 0; 

    while (*(s+ pos) != '\0') {
        tree * t_next = get_tree_child(curr, *(s+pos) - '0');
        if (t_next != NULL) {
            pos++;
            curr = t_next;
        }
        else return 0;
    }

    return 1;
}

/*Returns last tree node of a tree described by
sequence s if s belongs to set p; otherwise, returns NULL.*/
tree *seq_get(seq_t *p, char const *s) {
    if (p == NULL || !valid_string(s)) {
        return NULL;
    }

    tree *curr = p -> root;
    int pos = 0; 

    while (*(s+ pos) != '\0') {
        tree * t_next = get_tree_child(curr, *(s+pos) - '0');
        if (t_next != NULL) {
            pos++;
            curr = t_next;
        }
        else return NULL;
    }

    return curr;
}

//Sets the name of equiv class of the given seq s to n
int seq_set_name(seq_t *p, char const *s, char const *n){
    if (p == NULL || !valid_string(s) || n == NULL || *n == '\0') {
        errno = EINVAL;
        return -1;
    }

    tree *t = seq_get(p, s);

    if (t == NULL) { //seq doesnt exists in set
        return 0;
    }

    else { //seq exists

        node *t_class = t->class;

        //seq is the only one in its equiv class and it doesnt have a name
        if (t_class == NULL) {

            node *new_class = new_node();
            if (new_class == NULL) {
                errno = ENOMEM;
                return -1;
            }
            char *new_name = create_char(n); 
            if (new_name == NULL) {
                free(new_class);
                errno = ENOMEM;
                return -1;
            }

            new_class->num_of_el = 1;
            new_class->name = new_name;
            t->class = new_class;
            add_class(p, new_class);
            return 1;
        }

        /*seq's equiv class has a name or the seq is not alone in the class */
        else {          

            char *t_class_name = t_class->name;  

             //the name is already set to n
            if (t_class_name && compare_strings(t_class_name, n)) {
                return 0;
            }
            //changing/setting seq's equiv class' name
            else {
                char *new_name = create_char(n);
                if (new_name == NULL) {
                    errno = ENOMEM;
                    return -1;
                }
                if (t_class_name != NULL) {
                    free(t_class->name);
                }
                t_class->name = new_name;
                return 1;
            }

        }
    }
    return 1;
};

/*Return a pointer to the character pointing
 at the name of the equivalence class of sequence s.*/
char const *seq_get_name(seq_t *p, char const *s){
    if (p == NULL || !valid_string(s)) {
        errno = EINVAL;
        return NULL;
    }

    tree *t = seq_get(p, s);

    if (t == NULL) {    //seq doesnt exist in a tree
        errno = 0;
        return NULL;
    }
    if (t->class == NULL || 
        (t->class && ((t->class->name) == NULL))) { //seq's class doesnt have a name
        errno = 0;
        return NULL;
    }

    return (const char *) t->class->name;
}

/*Combines into a single equiv class the equiv classes represented by
given sequences s1 and s2*/
int seq_equiv(seq_t *p, char const *s1, char const *s2){
    if (p == NULL || !valid_string(s1) || !valid_string(s2)) {
        errno = EINVAL;
        return -1;
    }

    tree * t1 = seq_get(p, s1);
    tree * t2 = seq_get(p, s2); 

    //at least one seq doesnt belong to the set
    if (t1 == NULL || t2 == NULL)  {  
        return 0;
    }

    if (t1 == t2) return 0;

    node *t1_class = t1->class;
    node *t2_class = t2->class;

    //both seqs are alone in theirs equiv classes
    if (t1_class == NULL && t2_class == NULL) { 
        node *new_class = new_node(); 
        if (new_class == NULL) {
            errno = ENOMEM;
            return -1;
        }
        new_class->num_of_el = 2;
        t1->class = new_class; 
        t2->class = new_class;
        add_class(p, new_class);
        return 1;
    }
    //seqs belong to the same equiv classes
    else if (t1_class == t2_class) {
        return 0;
    }
    //seq s1 is alone in its class
    else if (t1_class == NULL) {    
        t2_class->num_of_el = (t2_class->num_of_el) + 1;
        t1->class = t2_class;
        return 1;
    }
     //seq s2 is alone in its class
    else if (t2_class == NULL) {  
        t1_class->num_of_el = (t1_class->num_of_el) + 1;
        t2->class = t1_class;
        return 1;
    }

    else { /*both s1 and s2 are not alone in their
             classes or their classes have names*/

        char * t1_class_name = t1->class->name;
        char * t2_class_name = t2->class->name;

        //their classes dont have a name 
        if ((t1_class_name == NULL && t2_class_name == NULL) || t2_class_name == NULL) {
            move_from_class_to_class(p, t2_class, t1_class);
            return 1;
        }
        //s1's class doesnt have a name
        else if (t1_class_name == NULL) {  
            move_from_class_to_class(p, t1_class, t2_class);
            return 1;
        }
        else {  //s1's and s2's classes have a name

            //the names are the same
            if (compare_strings2(t1_class_name, t2_class_name)) {
                move_from_class_to_class(p, t1_class, t2_class);
                return 1;
            }
            //the names arent the same
            else {
                char * new_name = concat_strings(t1_class_name, t2_class_name);
                if (new_name == NULL) {
                    errno = ENOMEM;
                    return -1;
                }
                node * new_class = new_node();
                if (new_class == NULL) {
                    free(new_name);
                    errno = ENOMEM;
                    return -1;
                }
                new_class -> name = new_name;
                move_from_class_to_class(p, t1_class, new_class);
                move_from_class_to_class(p, t2_class, new_class);
                return 1; 
            }
        }
    }
    return -1;
};



