#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "cbinem.h"

#ifndef CBINEM_FILE_SUFFIX
#define CBINEM_FILE_SUFFIX "cbindat"
#endif

#define CBINEM_FILE_SUFFIX_H "."CBINEM_FILE_SUFFIX".h"
#define CBINEM_FILE_SUFFIX_C "."CBINEM_FILE_SUFFIX".c"

#ifndef CBINEM_FUNC_PREFIX
#define CBINEM_FUNC_PREFIX CBINEM_FILE_SUFFIX
#endif

void cbinem_generate(const uint8_t* buffer, size_t buffer_size, const char* file_name, size_t bytes_per_line, int pretty_formatting) {
    char* filename = strdup(file_name);
    char* ext = strrchr(filename, '.');
	if (ext) *ext = '\0';
    char* fs = strrchr(filename, '/');
    char* bs = strrchr(filename, '\\');
    char* id = (fs || bs) ? ((fs > bs) ? fs : bs)+1 : filename;

    size_t namelen = strlen(filename) + strlen(CBINEM_FILE_SUFFIX_H);

    char filename_h[namelen + 1];
    strcpy(filename_h, filename);
    strcat(filename_h, CBINEM_FILE_SUFFIX_H);
    FILE* file = fopen(filename_h, "w");
    if (!file) {
		fprintf(stderr, "Failed to open file: %s\n", filename_h);
        return;
    }

    fprintf(file,
        "#ifndef %s_"CBINEM_FILE_SUFFIX"_H\n"
        "#define %s_"CBINEM_FILE_SUFFIX"_H\n"
        "\n"
        "#include <stddef.h>\n"
        "#include <stdint.h>\n"
        "\n"
        "uint8_t* "CBINEM_FUNC_PREFIX"_%s_malloc(size_t* size);\n"
        "void     "CBINEM_FUNC_PREFIX"_%s_free(uint8_t** p);\n"
        "\n"
        "#endif", id, id, id, id);
    fclose(file);

    static const char hex[] = "0123456789ABCDEF";
    char* datacontents = (char*)malloc((buffer_size * 5) + (buffer_size / bytes_per_line) + 1); // '0x00,' for every byte + '\t' for every line + '\0' at the end.
    char* cptr = datacontents;
    const uint8_t* bptr = buffer;
    if (pretty_formatting) {
        for (size_t i = 0; i < buffer_size; ++i) {
            *cptr++ = '0';
            *cptr++ = 'x';
            *cptr++ = hex[*bptr >> 4];
            *cptr++ = hex[*bptr & 0x0F];
            *cptr++ = ',';
            if(i % bytes_per_line == bytes_per_line - 1)
                *cptr++ = '\n';
            bptr++;
        }
    } else {
        for (size_t i = 0; i < buffer_size; ++i) {
            *cptr++ = '0';
            if (*bptr >> 4)
            {
                *cptr++ = 'x';
                *cptr++ = hex[*bptr >> 4];
                *cptr++ = hex[*bptr & 0x0F];
            }
            else if (*bptr & 0x0F)
            {
                *cptr++ = 'x';
                *cptr++ = hex[*bptr & 0x0F];
            }
            *cptr++ = ',';
            if(i % bytes_per_line == bytes_per_line - 1)
                *cptr++ = '\n';
            bptr++;
        }
    }
    *--cptr = '\0';

    char filename_c[namelen + 1];
    strcpy(filename_c, filename);
    strcat(filename_c, CBINEM_FILE_SUFFIX_C);
    file = fopen(filename_c, "w");
    if (!file) {
		fprintf(stderr, "Failed to open file: %s\n", filename_c);
        return;
    }

    fprintf(file,
        "#include <stdlib.h>\n"
        "#include <string.h>\n"
        "#include \"%s"CBINEM_FILE_SUFFIX_H"\"\n"
        "\n"
        "uint8_t* "CBINEM_FUNC_PREFIX"_%s_malloc(size_t* size) {\n"
        "\tconst size_t data_size = %zu;\n"
        "\tconst uint8_t data[data_size] = {\n"
        "%s\n"
        "\t};\n"
        "\t*size = data_size;\n"
        "\tuint8_t* p = (uint8_t*)malloc(data_size);\n"
        "\tif (!p) return 0;\n"
        "\treturn memcpy(p, data, data_size);\n"
        "}\n"
        "\n"
        "void "CBINEM_FUNC_PREFIX"_%s_free(uint8_t** p) {\n"
        "\tfree(*p);\n"
        "\t*p = 0;\n"
        "}", id, id, buffer_size, datacontents, id);
    fclose(file);

    free(filename);
    free(datacontents);
}

void cbinem_generate_from_file(const char* file_path, size_t bytes_per_line, int pretty_formatting) {
	FILE* file = fopen(file_path, "rb");
	if (!file) {
		fprintf(stderr, "Failed to open file: %s\n", file_path);
		return;
	}
	
	fseek(file, 0, SEEK_END);
	size_t buffer_size = (size_t)ftell(file);
	fseek(file, 0, SEEK_SET);

	uint8_t* buffer = (uint8_t *)malloc(buffer_size);
	if (!buffer) {
		fprintf(stderr, "Failed to allocate memory (%zu bytes) for file: %s", buffer_size, file_path);
        fclose(file);
        return;
    }
	else
	    fread(buffer, buffer_size, 1, file);
	fclose(file);

    cbinem_generate(buffer, buffer_size, file_path, bytes_per_line, pretty_formatting);

	free(buffer);
}