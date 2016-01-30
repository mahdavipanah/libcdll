# libcdll
This was project created as part of my data structures course in university. <br/ >
I really tried to write my codes as clean as I can, so everyone can read and understand it quikly, no matter he is a hard worker C programmer or some college student who wants to learn C.

I'm a perfectionist and this project like all creations in the world is not flawless, so feel free to commit! :)

## Usage

To use libcdll you have to compile it with a compiler that supports c99 standard.

An example command for compiling libcdll with your program using gcc or clang:

```Shell
gcc -std=c99 main.c libcdll.c
```
```Shell
clang -std=c99 main.c libcdll.c
```

## How to use it? (or API!)

First of all you need to define a CDLL object (frankly it is a pointer to a struct).

```C
// Don't forget to initialize it with NULL
// so cdll functions can find out your list
// is empty at the moment.

CDLL list = NULL;
```

### Functions

#### cdll_init (CDLL *list, void *data) => void
This function initialize a list if it is empty and gets a data as it's first data.
You rarely have to use this function because when you use other functions to manipulate the list, they know when to invoke this function.

This function does nothing if list is not empty.

```C
CDLL list = NULL;

int *a = malloc(sizeof(int));
*a = 10;
cdll_init (&list, a);

// => list: [10]

int *b = malloc(sizeof(int));
*b = 20;
// This time nothing happens because our list
// is already initialized
cdll_init (&list, b);

// => list: [10]
```

#### cdll_push (CDLL *list, void *data) => void
Adds a node with given data to end of list.

If list is empty and not initialized yet (== NULL), this functions invokes cdll_init and initializes it.

```C
CDLL list = NULL;

int *a = malloc(sizeof(int));
*a = 10;
cdll_push (&list, a);

// => list: [10]

int *b = malloc(sizeof(int));
*b = 20;
cdll_push (&list, b);

// => list: [10, 20]
```

#### cdll_pop (CDLL *list) => void *
Removes and frees the last node in the list and returns it's data.

This function does not free the last node's data's memory, so it is on you to
do whatever you want with it such as freeing it's dynamic memory.

```C
CDLL list = NULL;

for (int i = 1; i <= 5; ++i) {
        int *num = malloc(sizeof(int));
        *num = i;
        cdll_push(&list, num);
}

// => list: [1, 2, 3, 4, 5]

int *data = (int *) cdll_pop(&list);

// => *data: 5
// => list: [1, 2, 3, 4]

free(data);

```

#### cdll_add (CDLL *list, void *data) => void
Adds a node to first of list with the given data.

If list is empty and not initialized yet (== NULL), this functions invokes cdll_init and initializes it.

```C
CDLL list = NULL;

int *a = malloc(sizeof(int));
*a = 10;
cdll_add (&list, a);

// => list: [10]

int *b = malloc(sizeof(int));
*b = 20;
cdll_add (&list, b);

// => list: [20, 10]
```

#### cdll_remove_first (CDLL *list) => void *
Removes and frees the first node in the list and returns it's data.

This function does not free the last node's data's memory, so it is on you to
do whatever you want with it such as freeing it's dynamic memory.

```C
CDLL list = NULL;

for (int i = 1; i <= 5; ++i) {
        int *num = malloc(sizeof(int));
        *num = i;
        cdll_push(&list, num);
}

// => list: [1, 2, 3, 4, 5]

int *data = (int *) cdll_remove_first(&list);

// => *data: 1
// => list: [2, 3, 4, 5]

free(data);

```

#### cdll_length (const CDLL *list) => size_t
Returns the length of the list. (Number of nodes (elements)).

```C
CDLL list = NULL;

for (int i = 1; i <= 5; ++i) {
        int *num = malloc(sizeof(int));
        *num = i;
        cdll_push(&list, num);
}

// => list: [1, 2, 3, 4, 5]

size_t len = cdll_length(&list);

// => len: 5

```

#### cdll_first (const CDLL *list) => CDLL
Returns the first node in the list.

```C
CDLL list = NULL;

for (int i = 1; i <= 5; ++i) {
        int *num = malloc(sizeof(int));
        *num = i;
        cdll_push(&list, num);
}

// => list: [1, 2, 3, 4, 5]

CDLL first_node = cdll_first(&list);

int *data = (int *) first_node->data;
int *second_data = (int *) first_node->next->data;

// => *data: 1
// => *second_data: 2

```

#### cdll_last (const CDLL *list) => CDLL
Returns the last node in the list.

```C
CDLL list = NULL;

for (int i = 1; i <= 5; ++i) {
        int *num = malloc(sizeof(int));
        *num = i;
        cdll_push(&list, num);
}

// => list: [1, 2, 3, 4, 5]

CDLL last_node = cdll_last(&list);

int *data = (int *) last_node->data;
int *before_last_data = (int *) last_node->previous->data;

// => *data: 5
// => *before_last_data: 4

```

#### cdll_foreach (const CDLL *list, void (*function) (void*, void*), void *user_data) => void
Iterates over list's nodes starting from the first node to the last one and calls the given handler function with these two parameters :
* The node's data (is probably different in each handler's invocation)
* The given user_data (is same in every handler's invocation)

```C
void func (void *data, void *user_data)
{
        int * number     = (int *) data;
        int * sum_number = (int *) user_data;

        *number += *sum_number;
}
```

```C
CDLL list = NULL;

for (int i = 1; i <= 5; ++i) {
        int *num = malloc(sizeof(int));
        *num = i;
        cdll_push(&list, num);
}

// => list: [1, 2, 3, 4, 5]

int * number = malloc(sizeof(int));
*number = 2;

cdll_foreach(&list, func, number);

// => list: [3, 4, 5, 6, 7]

```

#### cdll_reverse_foreach (const CDLL *list, void (*function) (void*, void*), void *user_data) => void
Iterates over list's nodes starting from the last node to the first one and calls the given handler function with these two parameters :
* The node's data (is probably different in each handler's invocation)
* The given user_data (is same in every handler's invocation)

#### cdll_find (const CDLL *list, const void *data) => CDLL
Returns the node in the list that it's data is the same as the the second argument. In other word, the node that it's data is pointing to the same place that given data argument points to.

Returns NULL if no node found.

```C
CDLL list = NULL;

int *data_3;

for (int i = 1; i <= 5; ++i) {
        int *num = malloc(sizeof(int));
        *num = i;
        cdll_push(&list, num);

        if (i == 3)
                data_3 = num;
}

// => list: [1, 2, 3, 4, 5]

CDLL node = cdll_find(&list, data_3);

int *data = (int *) node->data;
int *next_node = (int *) node->next->data;

// => *data: 3
// => *next_node: 4

```

#### cdll_deep_find (const CDLL *list, const void *data, int (*compare)(const void *, const void *)) => CDLL
Returns the node in the list that it's data is the same as the the second argument. In other word, the node that it's data is pointing to the same place that given data argument points to.
But you have to provide a function that tells when two data valus are equal. compare function should return 0 if two values are equal.

Returns NULL if no node found.

```C
int int_ptr_compare (const void *a, const void *b)
{
	int m = *((int *) a);
	int n = *((int *) n);
	if (m == n)
		return 0;
}
```

```C
CDLL list = NULL;

for (int i = 1; i <= 5; ++i) {
        int *num = malloc(sizeof(int));
        *num = i;
        cdll_push(&list, num);
}

// => list: [1, 2, 3, 4, 5]

int * number = malloc(sizeof(int));
*number = 3;

CDLL node = cdll_find(&list, number, int_ptr_compare);

int *data = (int *) node->data;
int *next_node = (int *) node->next->data;

// => *data: 3
// => *next_node: 4

```

#### cdll_position (const CDLL *list, const CDLL node) => int
Returns index of node in the list starting from 0.

Returns -1 if node not found.


#### cdll_data_position (const CDLL *list, const void *data) => int
Returns index of first node with data equal to the data argument.

Returns -1 if no node with given data found.


#### cdll_deep_data_position (const CDLL *list, const void *data,  int (*compare)(const void *, const void *)) => int
Returns index of first node with data equal to the data argument. But you have to provide a function that tells when two data valus are equal. compare function should return 0 if two values are equal.

Returns -1 if no node with given data found.
