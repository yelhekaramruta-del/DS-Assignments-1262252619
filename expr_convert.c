#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

char stack[MAX];
int top = -1;
char postfix[MAX];
int k = 0;

/* ---------------- provided - do not modify ---------------- */
void print_step(char sym)
{
    printf("%c ", sym);
    if (top == -1) printf("-");
    for (int i = 0; i <= top; i++) printf("%c", stack[i]);
    postfix[k] = '\0';
    printf(" %s\n", k ? postfix : "-");
}

/* ---------------- your part ---------------- */
int icp(char c)
{
    if (c == '(') return 5;
    if (c == '^') return 4;
    if (c == '*' || c == '/' || c == '%') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}

int isp(char c)
{
    if (c == '^') return 3;
    if (c == '*' || c == '/' || c == '%') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}

void push(char c)
{
    if (top < MAX - 1)
        stack[++top] = c;
}

char pop(void)
{
    if (top >= 0)
        return stack[top--];

    return '\0';
}

/* Convert infix to postfix and print the stack after each symbol */
void infix_to_postfix(const char *in)
{
    top = -1;
    k = 0;

    for (int i = 0; in[i] != '\0'; i++) {
        char c = in[i];

        if (isspace((unsigned char)c))
            continue;

        if (isdigit((unsigned char)c)) {
            postfix[k++] = c;
        }
        else if (c == '(') {
            push(c);
        }
        else if (c == ')') {
            while (top >= 0 && stack[top] != '(')
                postfix[k++] = pop();

            if (top >= 0 && stack[top] == '(')
                pop();
        }
        else {
            while (top >= 0 && stack[top] != '(' &&
                   isp(stack[top]) >= icp(c))
                postfix[k++] = pop();

            push(c);
        }

        print_step(c);
    }

    while (top >= 0) {
        if (stack[top] == '(') {
            pop();
        } else {
            postfix[k++] = pop();
        }
    }

    postfix[k] = '\0';
}

/* Evaluate the postfix expression using an operand stack */
int eval_postfix(const char *p)
{
    int values[MAX];
    int vtop = -1;

    for (int i = 0; p[i] != '\0'; i++) {
        char c = p[i];

        if (isdigit((unsigned char)c)) {
            values[++vtop] = c - '0';
        }
        else {
            if (vtop < 1)
                return 0;

            int b = values[vtop--];
            int a = values[vtop--];
            int result = 0;

            switch (c) {
                case '+':
                    result = a + b;
                    break;

                case '-':
                    result = a - b;
                    break;

                case '*':
                    result = a * b;
                    break;

                case '/':
                    if (b == 0) return 0;
                    result = a / b;
                    break;

                case '%':
                    if (b == 0) return 0;
                    result = a % b;
                    break;

                case '^': {
                    if (b < 0) return 0;

                    result = 1;
                    for (int j = 0; j < b; j++)
                        result *= a;

                    break;
                }

                default:
                    return 0;
            }

            values[++vtop] = result;
        }
    }

    return vtop == 0 ? values[vtop] : 0;
}

/* ---------------- provided - do not modify ---------------- */
int main(void)
{
    char in[MAX];

    if (!fgets(in, sizeof in, stdin))
        return 1;

    in[strcspn(in, "\r\n")] = '\0';

    printf("SYMBOL STACK POSTFIX\n");
    infix_to_postfix(in);

    printf("POSTFIX: %s\n", postfix);
    printf("VALUE: %d\n", eval_postfix(postfix));

    return 0;
}

