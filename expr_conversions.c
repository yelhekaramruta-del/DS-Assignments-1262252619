
#include <stdio.h>
#include <string.h>

#define DEPTH 64
#define LEN 256

int icp(char c)
{
    return c == '(' ? 5 : c == '^' ? 4 :
           (c == '*' || c == '/' || c == '%') ? 2 :
           (c == '+' || c == '-') ? 1 : 0;
}

int isp(char c)
{
    return c == '^' ? 3 :
           (c == '*' || c == '/' || c == '%') ? 2 :
           (c == '+' || c == '-') ? 1 : 0;
}

int is_operand(char c)
{
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9');
}

int icp_pre(char c)
{
    return c == '(' ? 5 : c == '^' ? 3 :
           (c == '*' || c == '/' || c == '%') ? 2 :
           (c == '+' || c == '-') ? 1 : 0;
}

int isp_pre(char c)
{
    return c == '^' ? 4 :
           (c == '*' || c == '/' || c == '%') ? 2 :
           (c == '+' || c == '-') ? 1 : 0;
}

/* Infix to postfix */
void infix_to_postfix(const char *in, char *out)
{
    char stack[DEPTH];
    int top = -1, j = 0;

    for (int i = 0; in[i] != '\0'; i++) {
        char c = in[i];

        if (is_operand(c)) {
            out[j++] = c;
        }
        else if (c == '(') {
            stack[++top] = c;
        }
        else if (c == ')') {
            while (top >= 0 && stack[top] != '(')
                out[j++] = stack[top--];

            if (top >= 0 && stack[top] == '(')
                top--;
        }
        else {
            while (top >= 0 && stack[top] != '(' &&
                   isp(stack[top]) >= icp(c))
                out[j++] = stack[top--];

            stack[++top] = c;
        }
    }

    while (top >= 0)
        out[j++] = stack[top--];

    out[j] = '\0';
}

/* Infix to prefix using reverse, swap brackets, convert, reverse */
void infix_to_prefix(const char *in, char *out)
{
    char rev[LEN], temp[LEN], stack[DEPTH];
    int n = (int)strlen(in), top = -1, j = 0;

    for (int i = 0; i < n; i++) {
        char c = in[n - 1 - i];
        if (c == '(')
            c = ')';
        else if (c == ')')
            c = '(';
        rev[i] = c;
    }
    rev[n] = '\0';

    for (int i = 0; rev[i] != '\0'; i++) {
        char c = rev[i];

        if (is_operand(c)) {
            temp[j++] = c;
        }
        else if (c == '(') {
            stack[++top] = c;
        }
        else if (c == ')') {
            while (top >= 0 && stack[top] != '(')
                temp[j++] = stack[top--];

            if (top >= 0 && stack[top] == '(')
                top--;
        }
        else {
            while (top >= 0 && stack[top] != '(' &&
                   isp_pre(stack[top]) > icp_pre(c))
                temp[j++] = stack[top--];

            stack[++top] = c;
        }
    }

    while (top >= 0)
        temp[j++] = stack[top--];

    temp[j] = '\0';

    for (int i = 0; i < j; i++)
        out[i] = temp[j - 1 - i];

    out[j] = '\0';
}

/* Postfix to infix */
void postfix_to_infix(const char *in, char *out)
{
    char stack[DEPTH][LEN];
    int top = -1;

    for (int i = 0; in[i] != '\0'; i++) {
        char c = in[i];

        if (is_operand(c)) {
            stack[++top][0] = c;
            stack[top][1] = '\0';
        }
        else {
            char op2[LEN], op1[LEN];

            strcpy(op2, stack[top--]);
            strcpy(op1, stack[top--]);

            snprintf(stack[++top], LEN, "(%s%c%s)", op1, c, op2);
        }
    }

    strcpy(out, stack[top]);
}

/* Postfix to prefix */
void postfix_to_prefix(const char *in, char *out)
{
    char stack[DEPTH][LEN];
    int top = -1;

    for (int i = 0; in[i] != '\0'; i++) {
        char c = in[i];

        if (is_operand(c)) {
            stack[++top][0] = c;
            stack[top][1] = '\0';
        }
        else {
            char op2[LEN], op1[LEN];

            strcpy(op2, stack[top--]);
            strcpy(op1, stack[top--]);

            snprintf(stack[++top], LEN, "%c%s%s", c, op1, op2);
        }
    }

    strcpy(out, stack[top]);
}

/* Prefix to infix */
void prefix_to_infix(const char *in, char *out)
{
    char stack[DEPTH][LEN];
    int top = -1, n = (int)strlen(in);

    for (int i = n - 1; i >= 0; i--) {
        char c = in[i];

        if (is_operand(c)) {
            stack[++top][0] = c;
            stack[top][1] = '\0';
        }
        else {
            char op1[LEN], op2[LEN];

            strcpy(op1, stack[top--]);
            strcpy(op2, stack[top--]);

            snprintf(stack[++top], LEN, "(%s%c%s)", op1, c, op2);
        }
    }

    strcpy(out, stack[top]);
}

/* Prefix to postfix */
void prefix_to_postfix(const char *in, char *out)
{
    char stack[DEPTH][LEN];
    int top = -1, n = (int)strlen(in);

    for (int i = n - 1; i >= 0; i--) {
        char c = in[i];

        if (is_operand(c)) {
            stack[++top][0] = c;
            stack[top][1] = '\0';
        }
        else {
            char op1[LEN], op2[LEN];

            strcpy(op1, stack[top--]);
            strcpy(op2, stack[top--]);

            snprintf(stack[++top], LEN, "%s%s%c", op1, op2, c);
        }
    }

    strcpy(out, stack[top]);
}

/* ---------------------------------------------------------------- do not change below */
int main(void)
{
    char cmd[16], e[LEN], out[LEN];

    while (scanf("%15s %255s", cmd, e) == 2) {
        out[0] = '\0';

        if (strcmp(cmd, "IN-POST") == 0)
            infix_to_postfix(e, out);
        else if (strcmp(cmd, "IN-PRE") == 0)
            infix_to_prefix(e, out);
        else if (strcmp(cmd, "POST-IN") == 0)
            postfix_to_infix(e, out);
        else if (strcmp(cmd, "POST-PRE") == 0)
            postfix_to_prefix(e, out);
        else if (strcmp(cmd, "PRE-IN") == 0)
            prefix_to_infix(e, out);
        else if (strcmp(cmd, "PRE-POST") == 0)
            prefix_to_postfix(e, out);
        else
            continue;

        printf("%s %s = %s\n", cmd, e, out);
    }

    return 0;
}

