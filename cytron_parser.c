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
    puts(buffer.mem);

    free_buffer(&buffer);

    printf("\xF0\x9F\x98\x80\n"); // UTF-8 encoding of 😀 emoji
    printf("🥺\n");
    return 0;
}