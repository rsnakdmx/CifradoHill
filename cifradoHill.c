#include <stdio.h>
#include <stdlib.h>

typedef struct text
{
    int size;
    char *body;
}Text;

typedef struct node
{
    int data;
    struct node *next;
    struct node *prev;
}Node;

typedef struct stack
{
    int size;
    Node *bottom;
    Node *top;
}Stack;

Stack *createStack();
void push(Stack *, int);
int pop(Stack *);
Text* readFile(FILE *);
void charToInt(Stack *, Text *);

int main()
{
    Text *ptr = readFile(fopen("lorem.txt", "rb"));
    Stack *stack = NULL;
    stack = createStack();
    charToInt(stack, ptr);

    printf(" %d\n", pop(stack));
    printf(" %d\n", pop(stack));
    // for (int i = 0; i < ptr -> size; i++)
    //     printf("%d ", toX[i]);
    //printf("File size: %d\n", ptr -> size);
    //printf("%s", ptr -> body);
    free(ptr -> body);
    free(ptr);
    return 0x0;
}

Text *readFile(FILE *ap)
{
    Text *block = (Text*)calloc(0x1, sizeof(Text));

    fseek(ap, 0, SEEK_END);
    block -> size = ftell(ap);
    rewind(ap);
    block -> body = (char*)calloc(block -> size, sizeof(char));
    fread(block -> body, sizeof(char), block -> size, ap);
    fclose(ap);
    return block;
}

void charToInt(Stack *stack, Text *block)
{
    for (int idx = 0; idx < block -> size; idx++)
    {
        if (block -> body[idx] < 0 && block -> body[idx + 1] < 0)
            push(stack, 13);

        else if (block -> body[idx] > 64 && block -> body[idx] < 91)
            push(stack, (int)(block -> body[idx] - 65));
        
        else if (block -> body[idx] > 96 && block -> body[idx] < 123)
            push(stack, (int)(block -> body[idx] - 97));
    }
}

Stack *createStack()
{
    return (Stack*)calloc(0x1, sizeof(Stack));
}

void push(Stack *stack, int val)
{
    Node *new = (Node*)calloc(0x1, sizeof(Node));
    new -> data = val;

    if (!stack -> bottom)
    {
        stack -> bottom = new;
        stack -> top = stack -> bottom;
    }

    else
    {
        stack -> top -> next = new;
        stack -> top -> prev = stack -> top;
        stack -> top = stack -> top -> next;
    }

    stack -> size++;
}

int pop(Stack *stack)
{
    Node *tmp = stack -> top;
    int val = stack -> top -> data;

    stack -> size--;
    printf("%p", stack -> top);
    free(tmp); 
    return val;
}