/* PROGRAM 5: POLYNOMIAL ADDITION USING HEADER-NODE SCLLs
   a. Read the degree and nonzero terms (coefficient, power).
   b. Represent each polynomial in descending order of power.
   c. Add POLY1 and POLY2 and store the result in POLYSUM.
 Support the program with appropriate functions for each of the above operations. 
*/
#include <stdio.h>
#include <stdlib.h>

/* Each term stores its coefficient, power and next-node address. */
typedef struct node {
    long coef;
    int power;
    struct node *next;
} Node;

/* Create a node. A new header initially points to itself. */
Node *createNode(long coef, int power)
{
    Node *newNode;
    newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    newNode->coef = coef;
    newNode->power = power;
    newNode->next = newNode;
    return newNode;
}

/* Add a term at the end, keeping the last node linked to header. */
Node *append(Node *header, Node *last, long coef, int power)
{
    Node *newNode;
    if (coef == 0)
        return last;               /* Do not store zero terms. */
    newNode = createNode(coef, power);
    last->next = newNode;
    newNode->next = header;
    return newNode;                 /* This becomes the new last node. */
}

Node *readPoly(const char *name)
{
    Node *header, *last;
    int degree, terms, i, power, previous = 0;
    long coef;

    header = createNode(0, -1);     /* Dummy header, not a real term. */
    last = header;
    printf("%s: Enter degree and number of nonzero terms: ", name);
    if (scanf("%d%d", &degree, &terms) != 2 || degree < 0 ||
        degree > 32767 || terms < 0 || terms > 100 ||
        (terms == 0 && degree != 0) || (long)terms > (long)degree + 1) {
        printf("Invalid degree or term count.\n");
        exit(1);
    }

    printf("Enter coefficient and power in descending power order:\n");
    for (i = 0; i < terms; i++) {
        if (scanf("%ld%d", &coef, &power) != 2 || coef == 0 ||
            coef < -1000000L || coef > 1000000L || power < 0 ||
            power > degree || (i == 0 && power != degree) ||
            (i > 0 && power >= previous)) {
            printf("Invalid term or power order.\n");
            exit(1);
        }
        last = append(header, last, coef, power);
        previous = power;
    }
    return header;
}

Node *addPoly(Node *poly1, Node *poly2)
{
    Node *p, *q, *sum, *last;
    p = poly1->next;                /* First term of POLY1. */
    q = poly2->next;                /* First term of POLY2. */
    sum = createNode(0, -1);        /* Header of POLYSUM. */
    last = sum;

    /* Compare terms until one polynomial has no terms left. */
    while (p != poly1 && q != poly2) {
        if (p->power > q->power) {
            last = append(sum, last, p->coef, p->power);
            p = p->next;
        } else if (q->power > p->power) {
            last = append(sum, last, q->coef, q->power);
            q = q->next;
        } else {                   /* Equal powers: add coefficients. */
            last = append(sum, last, p->coef + q->coef, p->power);
            p = p->next;
            q = q->next;
        }
    }

    /* Copy any remaining terms from POLY1. */
    while (p != poly1) {
        last = append(sum, last, p->coef, p->power);
        p = p->next;
    }

    /* Copy any remaining terms from POLY2. */
    while (q != poly2) {
        last = append(sum, last, q->coef, q->power);
        q = q->next;
    }
    return sum;
}

void display(Node *header, const char *name)
{
    Node *p;
    p = header->next;
    printf("%s = ", name);
    if (p == header)
        printf("0");               /* No terms means zero polynomial. */

    while (p != header) {
        if (p == header->next) {   /* First term needs no plus sign. */
            if (p->coef < 0)
                printf("-");
        } else {
            if (p->coef < 0)
                printf(" - ");
            else
                printf(" + ");
        }
        printf("%ld", labs(p->coef));
        if (p->power > 0)
            printf("x^%d", p->power);
        p = p->next;
    }
    printf("\n");
}

/* Free every term, then free the header. */
void freePoly(Node *header)
{
    Node *p;
    while (header->next != header) {
        p = header->next;
        header->next = p->next;
        free(p);
    }
    free(header);
}

int main(void)
{
    Node *POLY1, *POLY2, *POLYSUM;
    POLY1 = readPoly("POLY1");
    POLY2 = readPoly("POLY2");
    POLYSUM = addPoly(POLY1, POLY2);

    display(POLY1, "POLY1");
    display(POLY2, "POLY2");
    display(POLYSUM, "POLYSUM");

    freePoly(POLY1);
    freePoly(POLY2);
    freePoly(POLYSUM);
    return 0;
}

/*
STUDENT TEST CASES AND EXPECTED OUTPUTS
Run the program separately for each test. Enter the lines exactly as shown.
For each polynomial, the first line is: degree number_of_nonzero_terms.
Following lines are: coefficient power. Input prompts are omitted below.
The notation x^1 means x. Zero coefficients in a sum are not stored.
Addition: O(m+n) time and at most O(m+n) result space.
Reading: O(m+n), provided the terms follow the stated input order.
*/

/* TEST 1: EQUAL POWERS AND PARTIAL CANCELLATION
Input:
3 3
5 3
4 1
2 0
3 3
3 3
-4 1
7 0
Expected output (excluding prompts):
POLY1 = 5x^3 + 4x^1 + 2
POLY2 = 3x^3 - 4x^1 + 7
POLYSUM = 8x^3 + 9
*/

/* TEST 2: DIFFERENT DEGREES AND UNMATCHED POWERS
Input:
5 2
2 5
3 1
4 2
4 4
-6 0
Expected output (excluding prompts):
POLY1 = 2x^5 + 3x^1
POLY2 = 4x^4 - 6
POLYSUM = 2x^5 + 4x^4 + 3x^1 - 6
*/

/* TEST 3: COMPLETE CANCELLATION
Input:
2 2
7 2
-5 0
2 2
-7 2
5 0
Expected output (excluding prompts):
POLY1 = 7x^2 - 5
POLY2 = -7x^2 + 5
POLYSUM = 0
*/

/* TEST 4: ZERO POLYNOMIAL PLUS NONZERO POLYNOMIAL
Input:
0 0
2 2
3 2
1 0
Expected output (excluding prompts):
POLY1 = 0
POLY2 = 3x^2 + 1
POLYSUM = 3x^2 + 1
*/

/* TEST 5: BOTH POLYNOMIALS ZERO
Input:
0 0
0 0
Expected output (excluding prompts):
POLY1 = 0
POLY2 = 0
POLYSUM = 0
*/

/* TEST 6: LARGE CONSTANT COEFFICIENTS
Input:
0 1
1000000 0
0 1
1000000 0
Expected output (excluding prompts):
POLY1 = 1000000
POLY2 = 1000000
POLYSUM = 2000000
*/

/* TEST 7: REPEATED POWER REJECTED
Input:
2 2
3 2
4 2
Expected output (excluding prompts):
Invalid term or power order.
*/
