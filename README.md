# cbinem

A small <b>C</b> <b>Bin</b>ary Data <b>Em</b>bedding library.

## Example
To generate the C source for a given binary file simply call `cbinem_generate_from_file`:
```C
// Specify the number of hex-bytes we want printed per line in the final output.
size_t bytes_per_line = 64;

// Enable prettier formatting for easier humnan-readability of hex data.
int b_pretty_formatting = 1;

cbinem_generate_from_file("somedata.bin", bytes_per_line, b_pretty_formatting);
```
The generator will create two new files: `somedata.cbindat.h` and `somedata.cbindat.c`.

`somedata.cbindat.h`:
```C
#ifndef somedata_cbindat_H
#define somedata_cbindat_H

#include <stddef.h>
#include <stdint.h>

uint8_t* cbindat_somedata_malloc(size_t* size);
void     cbindat_somedata_free(uint8_t** p);

#endif
```
`somedata.cbindat.c`
```C
#include <stdlib.h>
#include <string.h>
#include "somedata.cbindat.h"

uint8_t* cbindat_somedata_malloc(size_t* size) {
	const size_t data_size = 3;
	const uint8_t data[data_size] = {
0x00, 0x01, 0x02
	};
	*size = data_size;
	uint8_t* p = (uint8_t*)malloc(data_size);
	if (!p) return 0;
	return memcpy(p, data, data_size);
}

void cbindat_somedata_free(uint8_t** p) {
	free(*p);
	*p = 0;
}
```
To access the data at runtime simply call `cbinem_somedata_malloc` and remember to release the memory with `cbinem_somedata_free`:
```C
size_t data_size = 0;
uint8_t* data = cbindat_somedata_malloc(&data_size);

// ...

cbindat_somedata_free(&data);
```
