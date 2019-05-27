#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct text
{
    int size;
    char * restrict body;
}Text;

typedef struct node
{
    int data;
    struct node * restrict next;
    struct node * restrict prev;
}Node;

typedef struct queue
{
    int size;
    Node * restrict bottom;
    Node * restrict top;
}Queue;

static Queue * restrict queue = NULL;
static int key[3][3] = { {1, 2, 3},	{0, 4, 5}, {1, 0, 6} };

Queue *createStack();
void enqueue(int);
int dequeue();
Text *readFile(FILE *);
void charToInt(Text *);
int *cipherText();

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		Text * restrict ptr = readFile(fopen("lorem.txt", "rb"));
    	queue = createStack();
		int * restrict tmp = NULL;

    	charToInt(ptr);
	
		while (queue -> size > 0)
		{
			tmp = cipherText();

			for (int idx = 0; idx < 3; idx++)
				printf("%c", (char)(tmp[idx]) + 65);
		
			free(tmp);
			tmp = NULL;
		}

		printf("\n");
    	free(ptr -> body);
    	free(ptr);
    	return EXIT_SUCCESS;
	}

	else
	{
		printf("Especifica el archivo a cifrar\n");
		return EXIT_FAILURE;
	}
	
}

Text *readFile(FILE * restrict ap)
{
    Text * restrict block = (Text * restrict)calloc(1, sizeof(Text));

    fseek(ap, 0, SEEK_END);
    block -> size = ftell(ap);
    rewind(ap);
    block -> body = (char * restrict)calloc(block -> size, sizeof(char));
    fread(block -> body, sizeof(char), block -> size, ap);
    fclose(ap);
    return block;
}

void charToInt(Text * restrict block)
{
    for (register int idx = 0; idx < block -> size; idx++)
    {
        if (block -> body[idx] < 0 && block -> body[idx + 1] < 0)
            enqueue(13);
	
        else if (block -> body[idx] > 64 && block -> body[idx] < 91)
            enqueue((int)(block -> body[idx] - 65));
        
        else if (block -> body[idx] > 96 && block -> body[idx] < 123)
            enqueue((int)(block -> body[idx] - 97));
    }
}

Queue *createStack()
{
    return (Queue * restrict)calloc(1, sizeof(Queue));
}

void enqueue(int val)
{
    Node * restrict new = (Node * restrict)calloc(1, sizeof(Node));
    new -> data = val;

    if (!queue -> bottom)
    {
        queue -> bottom = new;
        queue -> top = queue -> bottom;
    }

    else
    {
        new -> prev = queue -> top;
        queue -> top -> next = new;
        queue -> top = queue -> top -> next;
    }

    queue -> size++;
}

int dequeue()
{
	if (queue -> size > 0)
	{
		int val = queue -> bottom -> data;
    	Node * restrict tmp = queue -> bottom;

    	queue -> bottom = queue -> bottom -> next;
    	queue -> size--;
    	free(tmp);

    	return val;
	}

	else
		return 23;
}

int *cipherText()
{
	int tmp[3] = {0, 0, 0};
	int *ret = (int * restrict)calloc(3, sizeof(int));

	for (int idx = 0; idx < 3; idx++)
		tmp[idx] = dequeue();

	omp_set_num_threads(3);
	#pragma omp parallel
	{
		for(int idx = 0; idx < 3; idx++)
		{
			#pragma omp for
			for (int jdx = 0; jdx < 3; jdx++)
				ret[idx] += key[idx][jdx] * tmp[jdx];
		
			ret[idx] = ret[idx] % 26;
		}
	}

	return ret;
}