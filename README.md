# Sets-of-sequences-library
(computer architecture and organization project)

## Equivalence Sets of Sequences
The task is to implement a dynamically loaded C library that handles sets of
sequences with an equivalence relation. The elements of the sets are non-empty
sequences consisting of the numbers 0, 1, and 2. Sequences are represented
as strings in the implementation. For example, the sequence {0, 1, 2} is represented as the string "012".
Abstract classes can be named.

## Library Interface
The functions and type name to be provided by the library are declared in the seq.h file attached to the task description.
The following declarations are provided:

typedef struct seq seq_t;
seq_t * seq_new(void);
void seq_delete(seq_t *p); 
int seq_add(seq_t *p, char const *s);
int seq_remove(seq_t *p, char const *s);
int seq_valid(seq_t *p, char const *s);
int seq_set_name(seq_t *p, char const *s, char const *n);
char const * seq_get_name(seq_t *p, char const *s);
int seq_equiv(seq_t *p, char const *s1, char const *s2);

## Sequence representation, operations on sets etc. 
The correct representation of a sequence is a non-empty string composed of the characters 0, 1, or 2, and it is terminated by a null character.
The correct name of an abstract class is a non-empty string terminated by a null character.
Additional details about the library's operation, including information about invalid parameters, can be inferred from the seq_example.c file provided,
which is an integral part of the specification. The term "the set of sequences remains unchanged" means that the observable state of the set of sequences has not changed.
