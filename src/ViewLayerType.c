//
// Created by Olcay Taner YILDIZ on 9.12.2023.
//
#include <Memory/Memory.h>
#include "ViewLayerType.h"

int compare_view_layer_type(const View_layer_type *layer_type1, const View_layer_type *layer_type2) {
    return (int)*layer_type1 - (int)*layer_type2;
}

unsigned int hash_function_view_layer_type(const View_layer_type *layer_type, int N) {
    return (*layer_type) % N;
}

View_layer_type_ptr create_view_layer_type(View_layer_type layer_type) {
    View_layer_type_ptr result = malloc_(sizeof(View_layer_type), "create_view_layer_type");
    *result = layer_type;
    return result;
}
