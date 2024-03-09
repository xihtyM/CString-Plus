#pragma once

#include <stdlib.h>
#include "types.h"
#include "print.h"

typedef enum error_type {
    success,
    bad_alloc,
    out_of_range,
} error_type;

typedef struct out_of_range_t
{
    u64 attempted_access;
    u64 size;
} out_of_range_t;


typedef struct exception
{
    error_type err;
    union
    {
        u64 size; // for bad allocations

        out_of_range_t out_of_range_struct;
    };
} exception;


void default_bad_alloc_handler(u64 size);
void default_out_of_range_handler(out_of_range_t out_of_range_struct);


typedef void (*bad_alloc_handler_t)(u64);
typedef void (*out_of_range_handler_t)(out_of_range_t);
static bad_alloc_handler_t __bad_alloc_handler = default_bad_alloc_handler;
static out_of_range_handler_t __out_of_range_handler = default_out_of_range_handler;


static inline void
set_bad_alloc_handler(
    bad_alloc_handler_t f)
{ __bad_alloc_handler = f; }

static inline void
set_out_of_range_handler(
    out_of_range_handler_t f)
{ __out_of_range_handler = f; }


void c_throw(exception exc);
