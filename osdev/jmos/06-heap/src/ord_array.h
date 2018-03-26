/*
 * ord_array.h 
 *
 * Interface for creating, inserting, and deleting from ordered arrays.
 *
 * Taken from JamesM's kernel development tutorials.
 */
#ifndef ORD_ARRAY_H
#define ORD_ARRAY_H

#include "common.h"

// Ordered arrays in this context will be insertion sorted - it should always
// be ordered between calls.
// The array should store anything that can be casted to a void*.
typedef void* type_t;

// A predicate should return non-zero if the first argument is less than the second.
typedef int8_t (*lessthan_predicate_t)(type_t, type_t);

/*
 * ordered_array_t
 */
typedef struct 
{
    type_t               *array;
    uint32_t             size;
    uint32_t             max_size;
    lessthan_predicate_t less_than;
} ordered_array_t;

// Standard less-than predicate
int8_t standard_lessthan_predicate(type_t a, type_t b);

// create_ordered_array creates an ordered array using kmalloc et al.
ordered_array_t create_ordered_array(uint32_t max_size, lessthan_predicate_t less_than);
// place_ordered_array creates an ordered array in a specific memory addres.
ordered_array_t place_ordered_array(void *addr, uint32_t max_size, lessthan_predicate_t less_than);

// destroy_ordered_array.
void destroy_ordered_array(ordered_array_t *array);

// insert_ordered_array inserts an item into the ordered array.
void insert_ordered_array(type_t item, ordered_array_t *array);

// lookup_ordered_array returns the item at given index from array.
type_t lookup_ordered_array(uint32_t i, ordered_array_t *array);

// remove_ordered_array removes item at index from ordered array.
void remove_ordered_array(uint32_t i, ordered_array_t *array);


#endif // ORD_ARRAY_H
