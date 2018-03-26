/*
 * timer.c
 *
 * Defines interface for all PIT-related functions.
 * Taken JamesM' kernel development tutorials.
 */
#ifndef TIMER_H
#define TIMER_H 

#include "common.h"

void init_timer(uint32_t frequency);

#endif
