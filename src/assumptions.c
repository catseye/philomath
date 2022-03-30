#include <stdio.h>
#include <stdlib.h>

#include "formula.h"
#include "assumptions.h"

struct assumptions *
assume(int label, struct formula *formula, struct assumptions *next) {
    struct assumptions *a = malloc(sizeof(struct assumptions));
    a->label = label;
    a->formula = formula;
    a->next = next;
    return a;
}

struct formula *
lookup(int label, struct assumptions *assumptions) {
    struct assumptions *a = assumptions;
    while (a != NULL) {
        if (a->label == label) {
            return a->formula;
        }
        a = a->next;
    }
    return NULL;
}

struct assumptions *
discharge(int label, struct assumptions *assumptions) {
    /* Returns a copy of the assumptions, without
       the assumption with the given label. */
    struct assumptions *a = assumptions;
    struct assumptions *result = NULL;
    while (a != NULL) {
        if (a->label != label) {
            result = assume(a->label, a->formula, result);
        }
        a = a->next;
    }
    return result;
}

struct assumptions *
merge(struct assumptions *a, struct assumptions *b) {
    struct assumptions *k = a;
    struct assumptions *c = NULL;
    struct formula *f;

    for (k = a; k != NULL; k = k->next) {
        f = lookup(k->label, b);
        if (f == NULL || formula_eq(f, k->formula)) {
            c = assume(k->label, k->formula, c);
        } else {
            fprintf(stderr, "Inconsistent assumptions with same label (%d)", k->label);
            exit(1);
        }
    }

    return c;
}

void
assumptions_fprint(FILE *f, struct assumptions *a) {
    struct assumptions *k = a;

    for (k = a; k != NULL; k = k->next) {
        fprintf(f, "(%d) ", k->label);
        formula_fprint(f, k->formula);
        fprintf(f, "\n");
    }
}
