/*
 *        Name: solve_rc.c
 *      Credit: This code is based on "Thistlethwaite 3x3 Solver" written in C++ 
 *              by Stefan Pochmann. The following is the original description of
 *              his original program.
 *                "My C++ implementation of the Thistlethwaite algorithm (except
 *                 a little modification to make it easier for me) which won the
 *                 "second place and judge's price" in "Tomas Rokicki's Cube Solver
 *                 Contest" in 2004. The way Tom counted, its source code is 1311
 *                 bytes and for the cubes in Tom's test set it took 16.72 turns
 *                 on average to solve them (those weren't all random, though,
 *                 for random cubes expect 30-35 turns on average)."
 *              Stefan Poochmann's web site is "stefan-pochmann.info"
 *      Author: Thumrongsak Kosiyatrakul
 *       Email: tkosiyat@cs.pitt.edu
 * Description: This code was converted from Stefan's C++ code explained above.
 *              Blah Blah Blah...
 *   Changelog: (11/11/19) After some investigations, the bottleneck are at the
 *                         mapping from an array of integers into either an array
 *                         of integers or an integer. The previous version uses
 *                         linked lists for simplicity. Hash tables with a hash
 *                         function for arrays of integers are used in this version.
 *                         The result is a working version that takes less than a
 *                         second to calculate a solution.
 *              (10/11/19) The first working version. Unfortunately, it is so
 *                         DAMN slow. For a test case, it takes almost a minute
 *                         to calculate a solution. Stefan's code only takes about
 *                         one second. UNACCEPTABLE!!!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************
 * Data Structures *
 *******************/

/* simple_alist
 * ============
 *
 * An simple array implementation of an ADT list of integers.
 */

struct simple_alist
{
    int *data;
    int size;
    int number_of_entries;
};

struct simple_alist *construct_simple_alist(int size)
{
    struct simple_alist *result = (struct simple_alist *) malloc(sizeof(struct simple_alist));
    result->data = (int *) malloc(sizeof(int) * size);
    result->size = size;
    result->number_of_entries = 0;
    return result;
}

int simple_alist_size(struct simple_alist *a_list)
{
    return a_list->number_of_entries;
}

void simple_alist_set(struct simple_alist *a_list, int index, int item)
{
    a_list->data[index] = item;
}

int simple_alist_get(struct simple_alist *a_list, int index)
{
    return a_list->data[index];
}

struct simple_alist *simple_alist_copy(struct simple_alist *a_list)
{
    struct simple_alist *result = (struct simple_alist *) malloc(sizeof(struct simple_alist));
    result->data = (int *) malloc(sizeof(int) * a_list->size);
    result->size = a_list->size;
    result->number_of_entries = a_list->number_of_entries;
    int i;
    for(i = 0; i < a_list->size; i++)
	result->data[i] = a_list->data[i];
    return result;
}

void ensureCapacity(struct simple_alist *a_list)
{
    int *new_array = (int *) malloc(sizeof(int) * (a_list->size * 2));
    int i;
    for(i = 0; i < a_list->number_of_entries; i++)
	new_array[i] = a_list->data[i];
    free(a_list->data);
    a_list->data = new_array;
    a_list->size = a_list->size * 2;
}

// add to the end of the list
void simple_alist_add(struct simple_alist *a_list, int new_item)
{
    if(a_list->number_of_entries == a_list->size)
	ensureCapacity(a_list);
    a_list->data[a_list->number_of_entries] = new_item;
    a_list->number_of_entries++;
}

// insert an item at an index
void simple_alist_insert(struct simple_alist *a_list, int index, int new_item)
{
    if(a_list->number_of_entries == a_list->size)
	ensureCapacity(a_list);
    int i;
    for(i = a_list->number_of_entries - 1; i >= index; i--)
	a_list->data[i + 1] = a_list->data[i];
    a_list->data[index] = new_item;
    a_list->number_of_entries++;
}

int simple_alist_equals(struct simple_alist *a, struct simple_alist *b)
{
    if(a->number_of_entries != b->number_of_entries)
	return 0;

    int result = 1;
    int i;
    for(i = 0; i < a->number_of_entries; i++)
    {
	if(a->data[i] != b->data[i])
	{
	    result = 0;
	    break;
	}
    }
    return result;
}

void simple_alist_print(struct simple_alist *a_list)
{
    int i;
    for(i = 0; i < a_list->number_of_entries; i++)
	printf("%i ", a_list->data[i]);
    printf("\n");
}

/* queue
 * =====
 *
 * A linked-list implementation of the ADT queue.
 */

struct q_node
{
    struct simple_alist *data;
    struct q_node *next;
};

struct queue
{
    struct q_node *firstNode;
    struct q_node *lastNode;
};

struct queue *construct_queue()
{
    struct queue *result = (struct queue *) malloc(sizeof(struct queue));
    result->firstNode = NULL;
    result->lastNode = NULL;
    return result;
}

void queue_addLast(struct queue *a_queue, struct simple_alist *new_item)
{
    struct q_node *new_node = (struct q_node *) malloc(sizeof(struct q_node));
    new_node->data = new_item;
    new_node->next = NULL;

    if(a_queue->firstNode == NULL)
    {
	a_queue->firstNode = new_node;
	a_queue->lastNode = new_node;
    }
    else
    {
	a_queue->lastNode->next = new_node;
	a_queue->lastNode = new_node;
    }
}

struct simple_alist *queue_pop(struct queue *a_queue)
{
    if(a_queue->firstNode == NULL)
	return NULL;
    else
    {
	struct simple_alist *result = a_queue->firstNode->data;
	struct q_node *remove_node = a_queue->firstNode;
	a_queue->firstNode = remove_node->next;
	free(remove_node);
	return result;
    }
}

/* unsigned int simple_alist_hash(struct simple_alist *a_list)
 *
 * A simple hash function for simple_alist.
 */

#define TABLE_SIZE 41777

unsigned int simple_alist_hash(struct simple_alist *a_list)
{
    int key[] = {1069, 113, 251, 337, 491, 509, 677, 827, 953};

    unsigned hash = 17;
    int i;
    for(i = 0; i < a_list->number_of_entries; i++)
	hash = (hash * 19) + (a_list->data[i] * key[i % 9]);
    
    return hash + a_list->data[0];
}

/* m_irir
 * ======
 *
 * A hash table where the key is the hash of a simple_alist mod by TABLE_SIZE
 * and the value is a simple_alist. The table is an array of linked-list of
 * type m_irir_node. A linked list is used in case of collisions.
 */

struct m_irir_node
{
    struct simple_alist *key;
    struct simple_alist *value;
    struct m_irir_node *next;
};

struct m_irir
{
    struct m_irir_node *table[TABLE_SIZE];
};

struct m_irir *construct_m_irir()
{
    struct m_irir *result = (struct m_irir *) malloc(sizeof(struct m_irir));
    int i;
    for(i = 0; i < TABLE_SIZE; i++)
	result->table[i] = NULL;
    return result;
}

struct m_irir_node *add_irir_node(struct simple_alist *a_key, struct simple_alist *a_value)
{
    struct m_irir_node *new_node = (struct m_irir_node *) malloc(sizeof(struct m_irir_node));
    new_node->key = a_key;
    new_node->value = a_value;
    new_node->next = NULL;
    return new_node;
}

void m_irir_set(struct m_irir *a_m_irir, struct simple_alist *a_key, struct simple_alist *a_value)
{
    unsigned int index = simple_alist_hash(a_key) % TABLE_SIZE;

    if(a_m_irir->table[index] == NULL)
	a_m_irir->table[index] = add_irir_node(a_key, a_value);
    else
    {
	struct m_irir_node *currentNode = a_m_irir->table[index];

	while(currentNode != NULL)
	{
	    if(simple_alist_equals(currentNode->key, a_key))
	    {
		currentNode->value = a_value;
		break;
	    }
	    currentNode = currentNode->next;
	}

	if(currentNode == NULL)
	{
	    struct m_irir_node *new_node = add_irir_node(a_key, a_value);
	    new_node->next = a_m_irir->table[index];
	    a_m_irir->table[index] = new_node;
	}
    }
}

struct simple_alist *m_irir_getOrPut(struct m_irir *a_m_irir, struct simple_alist *a_key, struct simple_alist *a_value)
{
    unsigned int index = simple_alist_hash(a_key) % TABLE_SIZE;

    if(a_m_irir->table[index] == NULL)
    {
	a_m_irir->table[index] = add_irir_node(a_key, a_value);
	return a_value;
    }
    else
    {
	struct m_irir_node *currentNode = a_m_irir->table[index];
	struct simple_alist *return_value = a_value;

	while(currentNode != NULL)
	{
	    if(simple_alist_equals(currentNode->key, a_key))
	    {
		return_value = currentNode->value;
		break;
	    }
	    currentNode = currentNode->next;
	}

	if(currentNode == NULL)
	{
	    struct m_irir_node *new_node = add_irir_node(a_key, a_value);
	    new_node->next = a_m_irir->table[index];
	    a_m_irir->table[index] = new_node;
	}
	return return_value;
    }
}

int m_irir_size(struct m_irir *a_m_irir)
{
    int result = 0;
    int i;
    for(i = 0; i < TABLE_SIZE; i++)
    {
	if(a_m_irir->table[i] == NULL)
	    continue;

	struct m_irir_node *currentNode = a_m_irir->table[i];
	while(currentNode != NULL)
	{
	    result++;
	    currentNode = currentNode->next;
	}
    }
    return result;
}

/*
void m_irir_clear(struct m_irir *a_m_irir)
{
    int i;
    struct m_irir_node *currentNode;
    struct m_irir_node *removeNode;
    for(i = 0; i < TABLE_SIZE; i++)
    {
	if(a_m_irir->table[i] == NULL)
	    continue;

	currentNode = a_m_irir->table[i];
	while(currentNode != NULL)
	{
	    removeNode = currentNode;
	    currentNode = currentNode->next;
	    free(removeNode->key);
	    free(removeNode->value);
	    free(removeNode);
	}
    }
}
*/


/* m_iri
 * =====
 *
 * A hash table where the key is the hash of a simple_alist mod by TABLE_SIZE
 * and the value is an integer. The table is an array of linked-list of
 * type m_iri_node. A linked list is used in case of collisions.
 */

struct m_iri_node
{
    struct simple_alist *key;
    int value;
    struct m_iri_node *next;
};

struct m_iri
{
    struct m_iri_node *table[TABLE_SIZE];
};

struct m_iri *construct_m_iri()
{
    struct m_iri *result = (struct m_iri *) malloc(sizeof(struct m_iri));
    int i;
    /*
    for(i = 0; i < TABLE_SIZE; i++)
	result->table[i] = NULL;
    */
    memset(result->table, 0, sizeof(struct m_iri_node *) * TABLE_SIZE);
    return result;
}

struct m_iri_node *add_iri_node(struct simple_alist *a_key, int a_value)
{
    struct m_iri_node *new_node = (struct m_iri_node *) malloc(sizeof(struct m_iri_node));
    new_node->key = a_key;
    new_node->value = a_value;
    new_node->next = NULL;
    return new_node;
}

void m_iri_set(struct m_iri *a_m_iri, struct simple_alist *a_key, int a_value)
{
    unsigned int index = simple_alist_hash(a_key) % TABLE_SIZE;

    if(a_m_iri->table[index] == NULL)
	a_m_iri->table[index] = add_iri_node(a_key, a_value);
    else
    {
	struct m_iri_node *currentNode = a_m_iri->table[index];

	while(currentNode != NULL)
	{
	    if(simple_alist_equals(currentNode->key, a_key))
	    {
		currentNode->value = a_value;
		break;
	    }
	    currentNode = currentNode->next;
	}

	if(currentNode == NULL)
	{
	    struct m_iri_node *new_node = add_iri_node(a_key, a_value);
	    new_node->next = a_m_iri->table[index];
	    a_m_iri->table[index] = new_node;
	}
    }
}

int m_iri_getOrPut(struct m_iri *a_m_iri, struct simple_alist *a_key, int a_value)
{
    unsigned int index = simple_alist_hash(a_key) % TABLE_SIZE;

    if(a_m_iri->table[index] == NULL)
    {
	a_m_iri->table[index] = add_iri_node(a_key, a_value);
	return a_value;
    }
    else
    {
	struct m_iri_node *currentNode = a_m_iri->table[index];
	int return_value = a_value;

	while(currentNode != NULL)
	{
	    if(simple_alist_equals(currentNode->key, a_key))
	    {
		return_value = currentNode->value;
		break;
	    }
	    currentNode = currentNode->next;
	}

	if(currentNode == NULL)
	{
	    struct m_iri_node *new_node = add_iri_node(a_key, a_value);
	    new_node->next = a_m_iri->table[index];
	    a_m_iri->table[index] = new_node;
	}

	return return_value;
    }
}

int m_iri_size(struct m_iri *a_m_iri)
{
    int result = 0;
    int i;
    for(i = 0; i < TABLE_SIZE; i++)
    {
	if(a_m_iri->table[i] == NULL)
	    continue;

	struct m_iri_node *currentNode = a_m_iri->table[i];
	while(currentNode != NULL)
	{
	    result++;
	    currentNode = currentNode->next;
	}
    }
    return result;
}

void m_iri_clear(struct m_iri *a_m_iri)
{
    int i;
    struct m_iri_node *currentNode;
    struct m_iri_node *removeNode;
    for(i = 0; i < TABLE_SIZE; i++)
    {
	if(a_m_iri->table[i] == NULL)
	    continue;

	currentNode = a_m_iri->table[i];
	while(currentNode != NULL)
	{
	    removeNode = currentNode;
	    currentNode = currentNode->next;
	    free(removeNode->key);
	    free(removeNode);
	}
    }
}

/*************************************************
 * r_string                                      *
 * ========                                      *
 *                                               *
 * A string representation of a state of a rubik *
 *************************************************/

char r_string_solved[49] = "UFURUBULDFDRDBDLFRFLBRBLUFRURBUBLULFDRFDFLDLBDBR";
char r_string[49] = "UFURUBULDFDRDBDLFRFLBRBLUFRURBUBLULFDRFDFLDLBDBR";

void r_string_reset()
{
    strcpy(r_string, r_string_solved);
}

char *r_string_get()
{
    char *result = (char *) malloc(sizeof(char) * 49);
    strcpy(result, r_string);
    return result;
}

void r_string_front()
{
    char c = r_string[25];
    r_string[25] = r_string[35];
    r_string[35] = r_string[40];
    r_string[40] = r_string[38];
    r_string[38] = c;
    c = r_string[1];
    r_string[1] = r_string[18];
    r_string[18] = r_string[9];
    r_string[9] = r_string[16];
    r_string[16] = c;
    c = r_string[26];
    r_string[26] = r_string[33];
    r_string[33] = r_string[41];
    r_string[41] = r_string[36];
    r_string[36] = c;
    c = r_string[24];
    r_string[24] = r_string[34];
    r_string[34] = r_string[39];
    r_string[39] = r_string[37];
    r_string[37] = c;
    c = r_string[0];
    r_string[0] = r_string[19];
    r_string[19] = r_string[8];
    r_string[8] = r_string[17];
    r_string[17] = c;
}

void r_string_right()
{
    char c = r_string[26];
    r_string[26] = r_string[37];
    r_string[37] = r_string[47];
    r_string[47] = r_string[28];
    r_string[28] = c;
    c = r_string[3];
    r_string[3] = r_string[17];
    r_string[17] = r_string[11];
    r_string[11] = r_string[21];
    r_string[21] = c;

    c = r_string[25];
    r_string[25] = r_string[36];
    r_string[36] = r_string[46];
    r_string[46] = r_string[27];
    r_string[27] = c;
    c = r_string[38];
    r_string[38] = r_string[45];
    r_string[45] = r_string[29];
    r_string[29] = r_string[24];
    r_string[24] = c;
    c = r_string[16];
    r_string[16] = r_string[10];
    r_string[10] = r_string[20];
    r_string[20] = r_string[2];
    r_string[2] = c;
}

void r_string_up()
{
    char c = r_string[33];
    r_string[33] = r_string[24];
    r_string[24] = r_string[27];
    r_string[27] = r_string[30];
    r_string[30] = c;
    c = r_string[0];
    r_string[0] = r_string[2];
    r_string[2] = r_string[4];
    r_string[4] = r_string[6];
    r_string[6] = c;

    c = r_string[32];
    r_string[32] = r_string[35];
    r_string[35] = r_string[26];
    r_string[26] = r_string[29];
    r_string[29] = c;
    c = r_string[34];
    r_string[34] = r_string[25];
    r_string[25] = r_string[28];
    r_string[28] = r_string[31];
    r_string[31] = c;
    c = r_string[7];
    r_string[7] = r_string[1];
    r_string[1] = r_string[3];
    r_string[3] = r_string[5];
    r_string[5] = c;
}

void r_string_left()
{
    char c = r_string[34];
    r_string[34] = r_string[32];
    r_string[32] = r_string[43];
    r_string[43] = r_string[41];
    r_string[41] = c;
    c = r_string[19];
    r_string[19] = r_string[7];
    r_string[7] = r_string[23];
    r_string[23] = r_string[15];
    r_string[15] = c;

    c = r_string[42];
    r_string[42] = r_string[40];
    r_string[40] = r_string[33];
    r_string[33] = r_string[31];
    r_string[31] = c;
    c = r_string[39];
    r_string[39] = r_string[35];
    r_string[35] = r_string[30];
    r_string[30] = r_string[44];
    r_string[44] = c;
    c = r_string[14];
    r_string[14] = r_string[18];
    r_string[18] = r_string[6];
    r_string[6] = r_string[22];
    r_string[22] = c;
}

void r_string_back()
{
    char c = r_string[31];
    r_string[31] = r_string[29];
    r_string[29] = r_string[46];
    r_string[46] = r_string[44];
    r_string[44] = c;
    c = r_string[5];
    r_string[5] = r_string[20];
    r_string[20] = r_string[13];
    r_string[13] = r_string[22];
    r_string[22] = c;

    c = r_string[30];
    r_string[30] = r_string[28];
    r_string[28] = r_string[45];
    r_string[45] = r_string[43];
    r_string[43] = c;
    c = r_string[27];
    r_string[27] = r_string[47];
    r_string[47] = r_string[42];
    r_string[42] = r_string[32];
    r_string[32] = c;
    c = r_string[4];
    r_string[4] = r_string[21];
    r_string[21] = r_string[12];
    r_string[12] = r_string[23];
    r_string[23] = c;
}

void r_string_down()
{
    char c = r_string[39];
    r_string[39] = r_string[42];
    r_string[42] = r_string[45];
    r_string[45] = r_string[36];
    r_string[36] = c;
    c = r_string[8];
    r_string[8] = r_string[14];
    r_string[14] = r_string[12];
    r_string[12] = r_string[10];
    r_string[10] = c;

    c = r_string[44];
    r_string[44] = r_string[47];
    r_string[47] = r_string[38];
    r_string[38] = r_string[41];
    r_string[41] = c;
    c = r_string[46];
    r_string[46] = r_string[37];
    r_string[37] = r_string[40];
    r_string[40] = r_string[43];
    r_string[43] = c;
    c = r_string[13];
    r_string[13] = r_string[11];
    r_string[11] = r_string[9];
    r_string[9] = r_string[15];
    r_string[15] = c;
}

/* The thistlethwaite algorithm starts here
 * ========================================
 */

int applicableMoves[5] = {0, 262143, 259263, 74943, 74898};

int affectedCubies[6][8] = {{0,  1, 2,  3, 0, 1, 2, 3},  // U
			    {4,  7, 6,  5, 4, 5, 6, 7},  // D
			    {0,  9, 4,  8, 0, 3, 5, 4},  // F
			    {2, 10, 6, 11, 2, 1, 7, 6},  // B
			    {3, 11, 7,  9, 3, 2, 6, 5},  // L
			    {1,  8, 5, 10, 1, 0, 4, 7}}; // R

int phase;

int string_array_index_of(char arr[][4], int size, char *str)
{
    int result = -1;
    int i;
    for(i = 0; i < size; i++)
    {
	if(strcmp(arr[i], str) == 0)
	{
	    result = i;
	    break;
	}
    }
    return result;
}

void rotate_string(char *a_string)
{
    int length = strlen(a_string);
    int i;
    char start = a_string[0];
    for(i = 0; i < length - 1; i++)
	a_string[i] = a_string[i + 1];
    a_string[length - 1] = start;
}

struct simple_alist *applyMove(int move, struct simple_alist *state)
{
    int turns = move % 3 + 1;
    int face = move / 3;
    int i;
    struct simple_alist *state2 = simple_alist_copy(state);

    while(turns--)
    {
	struct simple_alist *oldState = simple_alist_copy(state2);

	for(i = 0; i < 8; i++)
	{
	    int isCorner = (i > 3);
	    int target = affectedCubies[face][i] + isCorner * 12;
	    int temp;
	    if((i & 3) == 3)
		temp = i - 3;
	    else
		temp = i + 1;
	    int killer = affectedCubies[face][temp] + isCorner * 12;
	    int orientationDelta;
	    if(i < 4)
	    {
		orientationDelta = (face > 1 && face < 4);
	    }
	    else
	    {
		if(face < 2)
		    orientationDelta = 0;
		else
		    orientationDelta = 2 - (i & 1);
	    }
	    simple_alist_set(state2, target, simple_alist_get(oldState, killer));
	    simple_alist_set(state2, target + 20, simple_alist_get(oldState, killer + 20) + orientationDelta);
	    if(!turns)
		simple_alist_set(state2, target + 20, simple_alist_get(state2, target + 20) % (2 + isCorner));
	}
    }

    return state2;
}

int inverse(int move)
{
    return move + 2 - 2 * (move % 3);
}

struct simple_alist *id(struct simple_alist *state)
{
    int i, j, e, c;

    if(phase < 2)
    {
	struct simple_alist *result = construct_simple_alist(12);
	for(i = 0; i < 12; i++)
	    simple_alist_add(result, simple_alist_get(state, 20 + i));
	return result;
    }

    if(phase < 3)
    {
	struct simple_alist *result = construct_simple_alist(9);
	
	for(i = 0; i < 9; i++)
	    simple_alist_add(result, simple_alist_get(state, 31 + i));

	for(e = 0; e < 12; e++)
	    simple_alist_set(result, 0,
			     simple_alist_get(result, 0) |
			     (simple_alist_get(state, e) / 8) << e);

	return result;
    }

    if(phase < 4)
    {
	struct simple_alist *result = construct_simple_alist(3);
	for(i = 0; i < 3; i++)
	    simple_alist_add(result, 0);

	for(e = 0; e < 12; e++)
	{
	    int temp;
	    if(simple_alist_get(state, e) > 7)
		temp = 2;
	    else
		temp = simple_alist_get(state, e) & 1;
	    simple_alist_set(result, 0, simple_alist_get(result, 0) | temp << (2 * e));
	}

	for(c = 0; c < 8; c++)
	{
	    simple_alist_set(result, 1,
			     simple_alist_get(result, 1) |
			     ((simple_alist_get(state, c + 12) - 12) & 5) << (3 * c));
	}

	for(i = 12; i < 20; i++)
	{
	    for(j = i + 1; j < 20; j++)
	    {
		simple_alist_set(result, 2,
				 simple_alist_get(result, 2) ^
				 simple_alist_get(state, i) > simple_alist_get(state, j));
	    }
	}

	return result;
    }

    return state;
}

char *solve_rc(void)
{
    char *solution = (char *) malloc(sizeof(char) * 200);
    solution[0] = '\0';
    int d_counter = 0;
    int aggregateMoves = 0;
    char goal[20][4] = {"UF", "UR", "UB", "UL", "DF", "DR", "DB", "DL", "FR", "FL",
			"BR", "BL", "UFR", "URB", "UBL", "ULF", "DRF", "DFL", "DLB", "DBR"};
    //char inputs[20][4] = {"RU", "LF", "UB", "DR", "DL", "BL", "UL", "FU", "BD", "RF", "BR", "FD",
//			  "LDF", "LBD", "FUL", "RFD", "UFR", "RDB", "UBL", "RBU"};
    //char inputs[20][4] = {"UL", "DL", "RF", "UB", "FD", "BR", "DB", "UF", "DR", "UR",
//			  "BL", "FL", "FDR", "BLU", "DLB", "URB", "RUF", "FLD", "BRD", "FUL"};

    //char args[] = "RULFUBDRDLBLULFUBDRFBRFDLDFLBDFULRFDUFRRDBUBLRBU";
    //char args[] = "ULDLRFUBFDBRDBUFDRURBLFLFDRBLUDLBURBRUFFLDBRDFUL";

    int i;
    char inputs[20][4];
    for(i = 0; i < 12; i++)
    {
	strncpy(inputs[i], &r_string[i * 2], 2);
	inputs[i][2] = '\0';
    }

    for(i = 0; i < 8; i++)
    {
	strncpy(inputs[i + 12], &r_string[24 + (i * 3)], 3);
	inputs[i + 12][3] = '\0';
    }

    phase = 0;

    struct simple_alist *currentState = construct_simple_alist(40);
    struct simple_alist *goalState = construct_simple_alist(40);

    for(i = 0; i < 40; i++)
    {
	simple_alist_add(currentState, 0);
	simple_alist_add(goalState, 0);
    }

    for(i = 0; i < 20; i++)
    {
	simple_alist_set(goalState, i, i);

	char cubie[4];
	strcpy(cubie, inputs[i]);

	while(1)
	{
	    int idx = string_array_index_of(goal, 20, cubie);
	    if(idx >= 0)
		simple_alist_set(currentState, i, idx);
	    else
		simple_alist_set(currentState, i, 20);

	    if(simple_alist_get(currentState, i) != 20)
		break;

	    rotate_string(cubie);
	    simple_alist_set(currentState, i + 20, simple_alist_get(currentState, i + 20) + 1);
	}
    }

    while(++phase < 5)
    {
	struct simple_alist *currentId = id(currentState);
	struct simple_alist *goalId = id(goalState);

	if(simple_alist_equals(currentId, goalId))
	    continue;

	struct queue *q = construct_queue();
	queue_addLast(q, currentState);
	queue_addLast(q, goalState);

	struct m_irir *predecessor = construct_m_irir();
	struct m_iri *direction = construct_m_iri();
	struct m_iri *lastMove = construct_m_iri();

	m_iri_set(direction, currentId, 1);
	m_iri_set(direction, goalId, 2);

	while(1)
	{
	    struct simple_alist *oldState = queue_pop(q);
	    struct simple_alist *oldId = id(oldState);
	    int oldDir = m_iri_getOrPut(direction, oldId, 0);

	    int move;

	    for(move = 0; move < 18; move++)
	    {
		if((applicableMoves[phase] & (1 << move)))
		{
		    struct simple_alist *newState = applyMove(move, oldState);
		    struct simple_alist *newId = id(newState);
		    int newDir = m_iri_getOrPut(direction, newId, 0);
		    
		    if(newDir && newDir != oldDir)
		    {
			if(oldDir > 1)
			{
			    struct simple_alist *temp = newId;
			    newId = oldId;
			    oldId = temp;
			    move = inverse(move);
			}

			struct simple_alist *algorithm = construct_simple_alist(20);
			simple_alist_add(algorithm, move);
			    
			while(!simple_alist_equals(oldId, currentId))
			{
			    int tempI = m_iri_getOrPut(lastMove, oldId, 0);
			    simple_alist_insert(algorithm, 0, tempI);

			    struct simple_alist *tempAI = m_irir_getOrPut(predecessor, oldId, construct_simple_alist(20));
			    oldId = tempAI;
			}

			while(!simple_alist_equals(newId, goalId))
			{
			    int tempI = m_iri_getOrPut(lastMove, newId, 0);
			    simple_alist_add(algorithm, inverse(tempI));
			    struct simple_alist *tempAI = m_irir_getOrPut(predecessor, newId, construct_simple_alist(20));
			    newId = tempAI;
			}

			for(i = 0; i < simple_alist_size(algorithm); i++)
			{
			    char m[] = "UDFBLR";
			    /*
			    printf("%c", m[simple_alist_get(algorithm, i) / 3]);
			    printf("%i", simple_alist_get(algorithm, i) % 3 + 1);
			    */
			    char buffer[3];
			    sprintf(buffer, "%c%i", m[simple_alist_get(algorithm, i) / 3], simple_alist_get(algorithm, i) % 3 + 1);
			    strcat(solution, buffer);
			    currentState = applyMove(simple_alist_get(algorithm, i), currentState);
			}

			// jump to nextPhase
			goto nextPhasePlease;
		    }

		    if(!newDir)
		    {
			queue_addLast(q, newState);
			m_iri_set(direction, newId, oldDir);
			m_iri_set(lastMove, newId, move);
			m_irir_set(predecessor, newId, oldId);
		    }
		}
	    }
	}
    nextPhasePlease:
	/*
	m_iri_clear(direction);
	free(direction);
	m_iri_clear(lastMove);
	free(lastMove);
	*/
	;
	
    }
    return solution;
}
