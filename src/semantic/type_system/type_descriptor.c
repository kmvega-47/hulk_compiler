#include "type_descriptor.h"

TypeDescriptor *type_create(HulkType tag, const char *name, TypeDescriptor *parent)
{
    TypeDescriptor *type = (TypeDescriptor *)malloc(sizeof(TypeDescriptor));
    if (!type)
        return NULL;

    type->tag = tag;
    type->name = name ? strdup(name) : NULL;

    if (type->name)
        str_toupper(type->name);

    type->parent = parent;
    return type;
}

void type_destroy(TypeDescriptor *type)
{
    if (!type)
        return;

    free(type->name);
    free(type);
}

bool type_conforms_to(const TypeDescriptor *a, const TypeDescriptor *b)
{
    if (!a || !b)
        return false;

    if (a == b)
        return true;

    const TypeDescriptor *current = a->parent;
    while (current)
    {
        if (current == b)
            return true;

        current = current->parent;
    }

    return false;
}

static int get_type_depth(TypeDescriptor *type)
{
    int depth = 0;

    while (type && type->parent)
    {
        depth++;
        type = type->parent;
    }

    return depth;
}

static TypeDescriptor *ascend(TypeDescriptor *type, int n)
{
    for (int i = 0; i < n && type && type->parent; i++)
    {
        type = type->parent;
    }

    return type;
}

TypeDescriptor *type_lca(TypeDescriptor *a, TypeDescriptor *b)
{
    if (!a || !b)
        return NULL;

    if (a == b)
        return a;

    int depth_a = get_type_depth(a);
    int depth_b = get_type_depth(b);

    if (depth_a > depth_b)
        a = ascend(a, depth_a - depth_b);
        
    else if (depth_b > depth_a)
        b = ascend(b, depth_b - depth_a);

    while (a && b && a != b)
    {
        a = a->parent;
        b = b->parent;
    }

    return a;
}

int type_cmp(const TypeDescriptor *a, const TypeDescriptor *b)
{
    if (a == b)
        return 0;

    return -1;
}