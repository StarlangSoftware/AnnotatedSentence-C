//
// Created by Olcay Taner YILDIZ on 16.11.2023.
//

#ifndef ANNOTATEDSENTENCE_ANNOTATEDPHRASE_H
#define ANNOTATEDSENTENCE_ANNOTATEDPHRASE_H

#include <ArrayList.h>
#include "AnnotatedWord.h"

struct annotated_phrase{
    Array_list_ptr words;
    int word_index;
    char* tag;
};

typedef struct annotated_phrase Annotated_phrase;

typedef Annotated_phrase *Annotated_phrase_ptr;

Annotated_phrase_ptr create_annotated_phrase(int word_index, const char* tag);

void free_annotated_phrase(Annotated_phrase_ptr phrase);

void phrase_add_word(Annotated_phrase_ptr phrase, Annotated_word_ptr word);

#endif //ANNOTATEDSENTENCE_ANNOTATEDPHRASE_H
