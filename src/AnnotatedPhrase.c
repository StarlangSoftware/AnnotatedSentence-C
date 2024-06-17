//
// Created by Olcay Taner YILDIZ on 16.11.2023.
//

#include <stdlib.h>
#include <StringUtils.h>
#include <Memory/Memory.h>
#include "AnnotatedPhrase.h"
#include "AnnotatedWord.h"

/**
 * Constructor for AnnotatedPhrase. AnnotatedPhrase stores information about phrases such as
 * Shallow Parse phrases or named entity phrases.
 * @param wordIndex Starting index of the first word in the phrase w.r.t. original sentence the phrase occurs.
 * @param tag Tag of the phrase. Corresponds to the shallow parse or named entity tag.
 */
Annotated_phrase_ptr create_annotated_phrase(int word_index, const char *tag) {
    Annotated_phrase_ptr result = malloc_(sizeof(Annotated_phrase), "create_annotated_phrase");
    result->words = create_array_list();
    result->word_index = word_index;
    result->tag = str_copy(result->tag, tag);
    return result;
}

/**
 * Frees memory allocated to the phrase. Deallocates memory for words array list.
 * @param phrase Phrase to be dealloacated.
 */
void free_annotated_phrase(Annotated_phrase_ptr phrase) {
    free_array_list(phrase->words, NULL);
    free_(phrase->tag);
    free_(phrase);
}

/**
 * Adds a new word to the given phrase.
 * @param phrase Phrase to which a new word to be added.
 * @param word Word to be added.
 */
void phrase_add_word(Annotated_phrase_ptr phrase, Annotated_word_ptr word) {
    array_list_add(phrase->words, word);
}
