#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int coef;
    int exp;
    struct node *next;
} pnode;

/* Create an empty circular linked list with a header node */
pnode *new_poly(void)
{
    pnode *head = (pnode *)malloc(sizeof(pnode));

    if (head == NULL)
        exit(1);

    head->coef = 0;
    head->exp = 0;
    head->next = head;

    return head;
}

/* Insert a term in descending exponent order */
void insert_term(pnode *head, int coef, int exp)
{
    if (coef == 0)
        return;

    pnode *prev = head;
    pnode *curr = head->next;

    /* Find the correct position */
    while (curr != head && curr->exp > exp) {
        prev = curr;
        curr = curr->next;
    }

    /* Combine terms with the same exponent */
    if (curr != head && curr->exp == exp) {
        curr->coef += coef;

        /* Remove the term if its coefficient becomes zero */
        if (curr->coef == 0) {
            prev->next = curr->next;
            free(curr);
        }

        return;
    }

    /* Create and insert a new node */
    pnode *newnode = (pnode *)malloc(sizeof(pnode));

    if (newnode == NULL)
        exit(1);

    newnode->coef = coef;
    newnode->exp = exp;
    newnode->next = curr;
    prev->next = newnode;
}

/* Read a polynomial */
pnode *read_poly(void)
{
    int n, coef, exp;

    if (scanf("%d", &n) != 1)
        exit(1);

    pnode *head = new_poly();

    for (int i = 0; i < n; i++) {
        if (scanf("%d %d", &coef, &exp) != 2) {
            free(head);
            exit(1);
        }

        insert_term(head, coef, exp);
    }

    return head;
}

/* Return a new polynomial equal to h1 + h2 */
pnode *poly_add(pnode *h1, pnode *h2)
{
    pnode *sum = new_poly();

    pnode *p = h1->next;
    pnode *q = h2->next;

    while (p != h1 && q != h2) {
        if (p->exp == q->exp) {
            insert_term(sum, p->coef + q->coef, p->exp);
            p = p->next;
            q = q->next;
        }
        else if (p->exp > q->exp) {
            insert_term(sum, p->coef, p->exp);
            p = p->next;
        }
        else {
            insert_term(sum, q->coef, q->exp);
            q = q->next;
        }
    }

    /* Copy remaining terms of the first polynomial */
    while (p != h1) {
        insert_term(sum, p->coef, p->exp);
        p = p->next;
    }

    /* Copy remaining terms of the second polynomial */
    while (q != h2) {
        insert_term(sum, q->coef, q->exp);
        q = q->next;
    }

    return sum;
}

/* Free all nodes, including the header */
void free_poly(pnode *head)
{
    if (head == NULL)
        return;

    pnode *curr = head->next;

    while (curr != head) {
        pnode *temp = curr;
        curr = curr->next;
        free(temp);
    }

    free(head);
}

/* Provided - do not modify */
void print_poly(pnode *head)
{
    pnode *p = head->next;

    if (p == head) {
        printf("0\n");
        return;
    }

    while (p != head) {
        printf("%+dx^%d", p->coef, p->exp);
        p = p->next;

        if (p != head)
            printf(" ");
    }

    printf("\n");
}

int main(void)
{
    pnode *p1 = read_poly();
    pnode *p2 = read_poly();
    pnode *sum = poly_add(p1, p2);

    printf("P1 = ");
    print_poly(p1);

    printf("P2 = ");
    print_poly(p2);

    printf("SUM = ");
    print_poly(sum);

    free_poly(p1);
    free_poly(p2);
    free_poly(sum);

    return 0;
}

