#include <stdio.h>

#define MAXR 20
#define MAXC 20
#define MAXT (MAXR * MAXC + 1)

/* Convert matrix into triplet form */
int compact(int a[][MAXC], int m, int n, int b[][3])
{
    int count = 0;

    b[0][0] = m;
    b[0][1] = n;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (a[i][j] != 0) {
                count++;
                b[count][0] = i;
                b[count][1] = j;
                b[count][2] = a[i][j];
            }
        }
    }

    b[0][2] = count;
    return count;
}

/* FAST transpose using S and T arrays */
void fast_transpose(int a[][3], int b[][3], int s[], int t[])
{
    int rows = a[0][0];
    int cols = a[0][1];
    int terms = a[0][2];
    int pos[MAXC];

    /* Initialize S */
    for (int i = 0; i < cols; i++)
        s[i] = 0;

    /* Count non-zero elements in each column */
    for (int i = 1; i <= terms; i++)
        s[a[i][1]]++;

    /* Calculate starting positions T */
    t[0] = 1;

    for (int i = 1; i < cols; i++)
        t[i] = t[i - 1] + s[i - 1];

    /* Copy T so the original starting positions remain unchanged */
    for (int i = 0; i < cols; i++)
        pos[i] = t[i];

    /* Set transpose header */
    b[0][0] = cols;
    b[0][1] = rows;
    b[0][2] = terms;

    /* Place elements in transposed positions */
    for (int i = 1; i <= terms; i++) {
        int col = a[i][1];
        int p = pos[col]++;

        b[p][0] = a[i][1];
        b[p][1] = a[i][0];
        b[p][2] = a[i][2];
    }
}

/* Add two sparse matrices in triplet form */
int sparse_add(int a[][3], int b[][3], int c[][3])
{
    int i = 1, j = 1, k = 0;
    int ta = a[0][2];
    int tb = b[0][2];

    while (i <= ta && j <= tb) {
        /* Compare row first, then column */
        if (a[i][0] == b[j][0] &&
            a[i][1] == b[j][1]) {

            int value = a[i][2] + b[j][2];

            if (value != 0) {
                k++;
                c[k][0] = a[i][0];
                c[k][1] = a[i][1];
                c[k][2] = value;
            }

            i++;
            j++;

        } else if (a[i][0] < b[j][0] ||
                  (a[i][0] == b[j][0] &&
                   a[i][1] < b[j][1])) {

            k++;
            c[k][0] = a[i][0];
            c[k][1] = a[i][1];
            c[k][2] = a[i][2];
            i++;

        } else {
            k++;
            c[k][0] = b[j][0];
            c[k][1] = b[j][1];
            c[k][2] = b[j][2];
            j++;
        }
    }

    /* Copy remaining elements of A */
    while (i <= ta) {
        k++;
        c[k][0] = a[i][0];
        c[k][1] = a[i][1];
        c[k][2] = a[i][2];
        i++;
    }

    /* Copy remaining elements of B */
    while (j <= tb) {
        k++;
        c[k][0] = b[j][0];
        c[k][1] = b[j][1];
        c[k][2] = b[j][2];
        j++;
    }

    c[0][0] = a[0][0];
    c[0][1] = a[0][1];
    c[0][2] = k;

    return k;
}

/* Provided functions - do not modify */
void print_triplets(const char *name, int b[][3])
{
    printf("%s = %d %d %d\n",
           name, b[0][0], b[0][1], b[0][2]);

    for (int k = 1; k <= b[0][2]; k++)
        printf("%d %d %d\n",
               b[k][0], b[k][1], b[k][2]);
}

void print_array(const char *name, int x[], int n)
{
    printf("%s =", name);

    for (int i = 0; i < n; i++)
        printf(" %d", x[i]);

    printf("\n");
}

void read_matrix(int a[][MAXC], int m, int n)
{
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            if (scanf("%d", &a[i][j]) != 1)
                a[i][j] = 0;
}

int main(void)
{
    static int A[MAXR][MAXC], B[MAXR][MAXC];
    static int ta[MAXT][3], tb[MAXT][3];
    static int tr[MAXT][3], sum[2 * MAXT][3];
    int S[MAXC], T[MAXC], m, n;

    if (scanf("%d %d", &m, &n) != 2 ||
        m < 1 || n < 1 || m > MAXR || n > MAXC)
        return 1;

    read_matrix(A, m, n);
    read_matrix(B, m, n);

    compact(A, m, n, ta);
    compact(B, m, n, tb);
    fast_transpose(ta, tr, S, T);
    sparse_add(ta, tb, sum);

    print_triplets("A", ta);
    print_triplets("B", tb);
    print_array("S", S, n);
    print_array("T", T, n);
    print_triplets("TRANSPOSE", tr);
    print_triplets("SUM", sum);

    return 0;
}

