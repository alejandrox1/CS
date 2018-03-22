/*
 * ord_array.h
 *
 * Interface for creating, inserting, and deleting from ordered arrays.
 * Taken from JamesM' kernel development tutorials.
 */
#ifndef ORDARRAY_H
#define ORDARRAY_H

#include "common.h"

/*
 * his array is insertion sorted.
 * It can store anything that can be casted to void*.
 */
typedef void* type_t;

/* 
 * A predicate should return non-zero if the first argument is less than the
 * second. Else, return zero.
 * This will be use to order by sizes as opposed to pointer addresses.
 */
typedef int8_t (*lessthan_predicate_t)(type_t, type_t);

/*
 * ordered_aray_t
 */
typedef struct ordered_array_struct
{
    type_t               *array;
    uint32_t             size;
    uint32_t             max_size;
    lessthan_predicate_t less_than;
} ordered_array_t;


// standard_lessthan_predicate.
int8_t standard_lessthan_predicate(type_t a, type_t b);


// create_ordered_array allocates memory using kmalloc().
ordered_array_t create_ordered_array(uint32_t max_size, lessthan_predicate_t less_than);

// place_ordered_array creates an ordered array given a starting lcoation.
ordered_array_t place_ordered_array(void *addr, uint32_t max_size, lessthan_predicate_t less_than);

// destroy_ordered_array.
void destroy_ordered_array(ordered_array_t *arr);

// insert_ordered_array inserts an item into array.
void insert_ordered_array(type_t item, ordered_array_t *arr);

// lookup_ordered_array.
type_t lookup_ordered_array(uint32_t i, ordered_array_t *arr);

// remove_ordered_array removes item from array.
void remove_ordered_array(uint32_t i, ordered_array_t *arr);


#endif
