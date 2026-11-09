#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>

struct memory_info{
    size_t total_ram;
    size_t free_ram;
};

struct memory_info get_memory_info();

#endif