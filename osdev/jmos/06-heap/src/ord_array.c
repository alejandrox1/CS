/*
 * ord_map.c
 *
 * Implementation for creating, inserting, and deleting ordered arrays.
 * Taken from JamesM's kernel development tutorials.
 */
#include "common.h"
#include "ord_array.h"
#include "memalloc.h"
#include "common-io.h"



int8_t standard_lessthan_predicate(type_t a, type_t b)
{
    return (a<b)?1:0;
}

/*
 * create_ordered_array using kmalloc.
 */
ordered_array_t create_ordered_array(uint32_t max_size, lessthan_predicate_t less_than)
{
    ordered_array_t ret;
    ret.array = (void *)kmalloc(max_size*sizeof(type_t));
    memset((uint8_t *)ret.array, 0, max_size*sizeof(type_t));
    ret.size      = 0;
    ret.max_size  = max_size;
    ret.less_than = less_than;
    return ret;
}

/* 
 * place_ordered_array creates an ordered array given a starting address.
 */
ordered_array_t place_ordered_array(void *addr, uint32_t max_size, lessthan_predicate_t less_than)
{
    ordered_array_t ret;
    ret.array = (type_t *)addr;
    memset((uint8_t *)ret.array, 0 , max_size*sizeof(type_t));
    ret.size      = 0;
    ret.max_size  = max_size;
    ret.less_than = less_than;
    return ret;
}

/*
 * destroy_ordered_array.
 */
//void destroy_ordered_array(ordered_array_t *arr);
//{
//    kfree(array->array);
//}

/*
 * insert_ordered_arrays inserts items into an ordered array.
 */
void insert_ordered_array(type_t item, ordered_array_t *array)
{
    ASSERT(array->less_than);

    uint32_t iterator = 0;
    while (iterator < array->size && array->less_than(array->array[iterator], item))
        iterator++;

    if (iterator == array->size)
        array->array[array->size++] = item;
    else
    {
        type_t tmp = array->array[iterator];
        array->array[iterator] = item;
        while (iterator < array->size)
        {
            iterator++;
            type_t tmp2 = array->array[iterator];
            array->array[iterator] = tmp;
            tmp = tmp2;
        }
        array->size++;
    }
}

/*
 * lookup_ordered_array find item in ordered array.
 */
type_t lookup_ordered_array(uint32_t i, ordered_array_t *array)
{
    ASSERT(i < array->size);
    return array->array[i];
}

/*
 * remove_ordered_array removes an item from an ordered array.
 */
void remove_ordered_array(uint32_t i, ordered_array_t *array)
{
    while (i < array->size)
    {
        array->array[i] = array->array[i+1];
        array++;
    }
    array->size--;
}
