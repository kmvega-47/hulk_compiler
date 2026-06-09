#include "user_type_descriptor.h"

UserTypeDescriptor *user_type_create(const char *name, TypeDescriptor *parent, List *param_types)
{
    UserTypeDescriptor *type = (UserTypeDescriptor *)malloc(sizeof(UserTypeDescriptor));
    if (!type)
        return NULL;

    type->base.tag = HULK_USER_DEFINED;
    type->base.name = name ? strdup(name) : NULL;
    type->base.parent = parent;

    type->param_types = param_types;

    type->attribute_names = list_create(0, NULL, free, NULL, NULL);
    type->attribute_types = list_create(0, NULL, NULL, NULL, NULL);
    type->method_names = list_create(0, NULL, free, NULL, NULL);

    type->struct_type = NULL;
    type->constructor = NULL;
    type->constructor_type = NULL;

    if (parent)
    {
        UserTypeDescriptor *parent_user = type_to_user_defined(parent);
        if (parent_user)
            type->offset = parent_user->offset + (int)list_count(parent_user->attribute_names);
        else
            type->offset = 0;
    }
    else
    {
        type->offset = 0;
    }

    return type;
}

void user_type_destroy(UserTypeDescriptor *type)
{
    if (!type)
        return;

    free(type->base.name);
    list_destroy(type->param_types);
    list_destroy(type->attribute_names);
    list_destroy(type->attribute_types);
    list_destroy(type->method_names);
    free(type);
}

UserTypeDescriptor *type_to_user_defined(TypeDescriptor *type)
{
    if (!type || type->tag != HULK_USER_DEFINED)
        return NULL;

    return (UserTypeDescriptor *)type;
}

List *user_type_get_param_types(const UserTypeDescriptor *type)
{
    if (!type)
        return NULL;

    return type->param_types;
}

void user_type_update_param_type(UserTypeDescriptor *type, int param_idx, TypeDescriptor *new_param_type)
{
    if (!type || !type->param_types)
        return;

    list_set(type->param_types, param_idx, new_param_type);
}

TypeDescriptor *user_type_lookup_attribute(const UserTypeDescriptor *type, const char *name)
{
    if (!type || !name)
        return NULL;

    for (size_t i = 0; i < list_count(type->attribute_names); i++)
    {
        char *attr_name = (char *)list_get(type->attribute_names, i);
        if (attr_name && strcmp(attr_name, name) == 0)
            return (TypeDescriptor *)list_get(type->attribute_types, i);
    }

    return NULL;
}

void user_type_add_attribute(UserTypeDescriptor *type, const char *name, TypeDescriptor *attr_type)
{
    if (!type || !name || !attr_type)
        return;

    char *name_copy = strdup(name);
    if (!name_copy)
        return;

    list_append(type->attribute_names, name_copy);
    list_append(type->attribute_types, attr_type);
}

bool user_type_has_method(const UserTypeDescriptor *type, const char *name)
{
    if (!type || !name)
        return false;

    for (size_t i = 0; i < list_count(type->method_names); i++)
    {
        char *method_name = (char *)list_get(type->method_names, i);
        if (method_name && strcmp(method_name, name) == 0)
            return true;
    }

    if (type->base.parent)
    {
        UserTypeDescriptor *parent_user = type_to_user_defined(type->base.parent);
        if (parent_user)
            return user_type_has_method(parent_user, name);
    }

    return false;
}

void user_type_add_method(UserTypeDescriptor *type, const char *name)
{
    if (!type || !name)
        return;

    char *name_copy = strdup(name);
    if (!name_copy)
        return;

    list_append(type->method_names, name_copy);
}