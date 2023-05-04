#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <locale.h>
#include <uchar.h>
#include <string.h>

#ifdef _MSC_VER
#include <windows.h>
#endif

#include "common.h"
#include "utf8.h"

STRUCT(buffer_t)
{
    char *mem;
    size_t cap, size;
};

buffer_t create_buffer()
{
    char *mem = malloc(1);
    if (mem == NULL)
    {
        bail("Failed to allocate memory");
    }
    return (buffer_t){mem, 1, 0};
}

void push_back_to_buffer(buffer_t *buffer, char value)
{
    if (buffer->size == buffer->cap)
    {
        buffer->cap *= 2;
        buffer->mem = realloc(buffer->mem, buffer->cap);
        if (buffer->mem == NULL)
        {
            bail("Failed to re-allocate memory");
        }
    }

    buffer->mem[buffer->size] = value;
}

void free_buffer(buffer_t *buffer)
{
    free(buffer->mem);
}

char *allocate_from_buffer(buffer_t *buffer, size_t n)
{
    if ((buffer->size + n) > buffer->cap)
    {
        buffer->cap = (buffer->cap + n) * 2;
        buffer->mem = realloc(buffer->mem, buffer->cap);
        if (buffer->mem == NULL)
        {
            bail("Failed to re-allocate memory");
        }
    }

    char *result = (buffer->mem + buffer->size);
    buffer->size += n;
    return result;
}

void truncate_and_shrink_buffer(buffer_t *buffer, size_t new_size)
{
    if (new_size > buffer->size)
    {
        bail("New size for truncation has to be less than buffer size");
    }

    buffer->size = new_size;
    buffer->cap = new_size;
    buffer->mem = realloc(buffer->mem, new_size);
    if (buffer->mem == NULL)
    {
        bail("Failed to re-allocate memory");
    }
}

int main(int argc, char **argv)
{

#ifdef _MSC_VER
    SetConsoleOutputCP(CP_UTF8);
#endif

    if (argc != 2)
    {
        puts("Expected arguments: source.cy");
        return 0;
    }

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL)
    {
        bail("Failed to open file");
    }

    // Read entire source code into memory
    buffer_t buffer = create_buffer();
    size_t total_read_bytes = 0;
    while (true)
    {
        char *dst = allocate_from_buffer(&buffer, 1024);
        size_t num_read_bytes = fread(dst, 1, 1024, file);
        if (num_read_bytes == 0)
        {
            break;
        }
        total_read_bytes += num_read_bytes;
    }
    truncate_and_shrink_buffer(&buffer, total_read_bytes);
    push_back_to_buffer(&buffer, '\0');

    printf("Parsing source:\n%s\n", buffer.mem);

    // Iterate source code one UTF-8 code point at a time
    char *current_token_start = buffer.mem;
    char *current_token_end = current_token_start;
    while (true)
    {
        decode_code_point(&current_token_end);

        if (memcmp(current_token_start, "import", current_token_end - current_token_start) == 0)
        {
            puts("Found import token");
            current_token_start = current_token_end;
        }

        // for (char *iter = current_token_start; iter < current_token_end; iter++)
        // {
        //     putchar(*iter);
        // }
        // putchar('\n');

        if (current_token_end == buffer.mem + total_read_bytes)
        {
            break;
        }
    }

    free_buffer(&buffer);
    return 0;
}