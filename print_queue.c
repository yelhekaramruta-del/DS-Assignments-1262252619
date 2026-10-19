#include <stdio.h>
#include <string.h>

#define MAXN 20

struct job {
    int id;
    char title[32];
};

struct job q[MAXN];
int n, front = 0, rear = 0;

int is_full(void)
{
    return (rear + 1) % n == front;
}

int is_empty(void)
{
    return front == rear;
}

/* Add a job at the rear */
int enqueue(int id, const char *title)
{
    if (is_full())
        return 0;

    rear = (rear + 1) % n;

    q[rear].id = id;
    strcpy(q[rear].title, title);

    return 1;
}

/* Remove the job at the front */
int dequeue(struct job *out)
{
    if (is_empty())
        return 0;

    front = (front + 1) % n;

    out->id = q[front].id;
    strcpy(out->title, q[front].title);

    return 1;
}

/* Display all pending jobs from front to rear */
void display(void)
{
    if (is_empty()) {
        printf("No pending print jobs.\n");
        return;
    }

    printf("Pending:");

    int i = (front + 1) % n;

    while (1) {
        printf(" %d:%s", q[i].id, q[i].title);

        if (i == rear)
            break;

        i = (i + 1) % n;
    }

    printf("\n");
}

/* ---------------- provided - do not modify ---------------- */
int main(void)
{
    char cmd[8], title[32];
    int id;

    if (scanf("%d", &n) != 1 || n < 2 || n > MAXN)
        return 1;

    printf("Print queue of size %d created. FRONT=%d REAR=%d\n",
           n, front, rear);

    while (scanf("%7s", cmd) == 1) {
        if (cmd[0] == 'E') {
            scanf("%d %31s", &id, title);

            if (enqueue(id, title))
                printf("Job %d (%s) added. FRONT=%d REAR=%d\n",
                       id, title, front, rear);
            else
                printf("Queue Overflow! Print queue is full. FRONT=%d REAR=%d\n",
                       front, rear);

        } else if (cmd[0] == 'D') {
            struct job j;

            if (dequeue(&j))
                printf("Printing job %d (%s). FRONT=%d REAR=%d\n",
                       j.id, j.title, front, rear);
            else
                printf("Queue Underflow! No print jobs available.\n");

        } else if (cmd[0] == 'P') {
            display();
        }
    }

    return 0;
}

