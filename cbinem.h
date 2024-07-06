#ifndef CBINEM_H
#define CBINEM_H

#include <stddef.h>
#include <stdint.h>

void cbinem_generate(const uint8_t* buffer, size_t buffer_size, const char* file_name, size_t bytes_per_line, int pretty_formatting);
void cbinem_generate_from_file(const char* filepath, size_t bytes_per_line, int pretty_formatting);

#endif
