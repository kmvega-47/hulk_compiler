#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double _hulk_sqrt(double x)
{
    return sqrt(x);
}

double _hulk_sin(double x)
{
    return sin(x);
}

double _hulk_cos(double x)
{
    return cos(x);
}

double _hulk_exp(double x)
{
    return exp(x);
}

double _hulk_pow(double base, double exp)
{
    return pow(base, exp);
}

double _hulk_log(double base, double x)
{
    return log(x) / log(base);
}

double _hulk_rand(void)
{
    return (double)rand() / RAND_MAX;
}

void _hulk_print_number(double x)
{
    printf("%g\n", x);
}

void _hulk_print_string(const char *s)
{
    printf("%s\n", s);
}

void _hulk_print_bool(int b)
{
    printf("%s\n", b ? "true" : "false");
}

char *_hulk_concat(const char *a, const char *b, int with_space)
{
    size_t len = strlen(a) + strlen(b) + 1;

    if (with_space)
        len++;

    char *result = (char *)malloc(len);

    if (result)
    {
        strcpy(result, a);

        if (with_space)
            strcat(result, " ");

        strcat(result, b);
    }

    return result;
}

void *_hulk_alloc(size_t size)
{
    return malloc(size);
}

void _hulk_free(void *ptr)
{
    free(ptr);
}

char *_hulk_number_to_string(double x)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%g", x);
    return strdup(buffer);
}