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

###Functions

####cdll_init (CDLL *list, void *data) => void
This function initialize a list if it is empty and gets a data as it's first data.
You rarely have to use this function because when you use other functions to manipulate the list, they know when to invoke this function.

This function does nothing if list is not empty.

```C
CDLL list = NULL;

int *a = malloc(sizeof(int));
*a = 10;
cdll_init (&list, a);

// => list = [10]

int *b = malloc(sizeof(int));
*b = 20;
// This time nothing happens because our list
// is already initialized
cdll_init (&list, b);

// => list = [10]
```

####cdll_push (CDLL *list, void *data) => void
Adds a node with given data to end of list.

If list is empty and not initialized yet (== NULL), this functions invokes cdll_init and initializes it.

```C
CDLL list = NULL;

int *a = malloc(sizeof(int));
*a = 10;
cdll_push (&list, a);

// => list = [10]

int *b = malloc(sizeof(int));
*b = 20;
cdll_push (&list, b);

// => list = [10, 20]
```
