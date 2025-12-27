//
// Created by Olcay Taner YILDIZ on 16.11.2023.
//

#include <FileUtils.h>
#include <string.h>
#include "AnnotatedSentence.h"
#include "AnnotatedWord.h"
#include "AnnotatedPhrase.h"
#include <FrameNet.h>
#include <Memory/Memory.h>

/**
 * Reads an annotated sentence from a text file.
 * @param input_file File containing the annotated sentence.
 */
Sentence_ptr create_annotated_sentence(FILE *input_file) {
    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, input_file);
    line[strcspn(line, "\n")] = 0;
    return create_annotated_sentence2(line);
}

/**
 * Frees memory allocated to a sentence object.
 * @param sentence Sentence to be dellocated.
 */
void free_annotated_sentence(Sentence_ptr sentence) {
    free_array_list(sentence->words, (void (*)(void *)) free_annotated_word);
    free_(sentence);
}

/**
 * Converts a simple sentence to an annotated sentence
 * @param string Simple sentence
 */
Sentence_ptr create_annotated_sentence2(const char *string) {
    Sentence_ptr sentence = create_sentence();
    Array_list_ptr word_array = str_split(string, ' ');
    for (int i = 0; i < word_array->size; i++){
        array_list_add(sentence->words, create_annotated_word(array_list_get(word_array, i)));
    }
    free_array_list(word_array, free_);
    return sentence;
}

/**
 * The method checks all words in the sentence and returns true if at least one of the words is annotated with
 * PREDICATE tag.
 * @param sentence Sentence object.
 * @return True if at least one of the words is annotated with PREDICATE tag; false otherwise.
 */
bool contains_predicate(Sentence_ptr sentence) {
    for (int i = 0; i < sentence->words->size; i++){
        Annotated_word_ptr word = array_list_get(sentence->words, i);
        if (word->argument_list != NULL){
            if (contains_predicate_in_argument_list(word->argument_list)) {
                return true;
            }
        }
    }
    return false;
}

/**
 * The method checks all words in the sentence and returns true if at least one of the words is annotated with
 * PREDICATE tag.
 * @param sentence Sentence object.
 * @return True if at least one of the words is annotated with PREDICATE tag; false otherwise.
 */
bool contains_frame_predicate(Sentence_ptr sentence) {
    for (int i = 0; i < sentence->words->size; i++){
        Annotated_word_ptr word = array_list_get(sentence->words, i);
        if (word->frame_element_list != NULL){
            if (contains_predicate_in_frame_element_list(word->frame_element_list)) {
                return true;
            }
        }
    }
    return false;
}

bool update_connected_predicate(Sentence_ptr sentence, const char *previous_id, const char *current_id) {
    bool modified = false;
    for (int i = 0; i < sentence->words->size; i++){
        Annotated_word_ptr word = array_list_get(sentence->words, i);
        Argument_list_ptr argument_list = word->argument_list;
        if (argument_list != NULL){
            if (contains_predicate_with_id_in_argument_list(argument_list, previous_id)) {
                update_connected_id_in_argument_list(argument_list, previous_id, current_id);
                modified = true;
            }
        }
        Frame_element_list_ptr frame_element_list = word->frame_element_list;
        if (frame_element_list != NULL){
            if (contains_predicate_with_id_in_frame_element_list(frame_element_list, previous_id)) {
                update_connected_id_in_frame_element_list(frame_element_list, previous_id, current_id);
                modified = true;
            }
        }
    }
    return modified;
}

/**
 * The method constructs all possible shallow parse groups of a sentence.
 * @param sentence Sentence object.
 * @return Shallow parse groups of a sentence.
 */
Array_list_ptr get_shallow_parse_groups(Sentence_ptr sentence) {
    Array_list_ptr result = create_array_list();
    Annotated_word_ptr previous_word = NULL;
    Annotated_phrase_ptr current = NULL;
    for (int i = 0; i < sentence->words->size; i++){
        Annotated_word_ptr word = array_list_get(sentence->words, i);
        if (previous_word == NULL){
            current = create_annotated_phrase(i, word->shallow_parse);
        } else {
            if (previous_word->shallow_parse != NULL && (word->shallow_parse == NULL || strcmp(previous_word->shallow_parse, word->shallow_parse) != 0)){
                array_list_add(result, current);
                current = create_annotated_phrase(i, word->shallow_parse);
            }
        }
        array_list_add(current->words, word);
        previous_word = word;
    }
    array_list_add(result, current);
    return result;
}

/**
 * The method returns all possible words, which is
 * 1. Verb
 * 2. Its semantic tag is assigned
 * 3. A frameset exists for that semantic tag
 * @param sentence Sentence object.
 * @param framesetList Frameset list that contains all frames for Turkish
 * @return An array of words, which are verbs, semantic tags assigned, and framesetlist assigned for that tag.
 */
Array_list_ptr predicate_candidates(Sentence_ptr sentence, Frameset_list_ptr framesetList) {
    Array_list_ptr candidate_list = create_array_list();
    for (int i = 0; i < sentence->words->size; i++){
        Annotated_word_ptr word = array_list_get(sentence->words, i);
        if (word->parse != NULL && is_parse_verb(word->parse) && word->semantic != NULL && frame_exists(framesetList, word->semantic)){
            array_list_add(candidate_list, word);
        }
    }
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < sentence->words->size - i - 1; j++){
            Annotated_word_ptr word = array_list_get(sentence->words, j);
            Annotated_word_ptr next_word = array_list_get(sentence->words, j + 1);
            if (!array_list_contains(candidate_list, word,
                                     (int (*)(const void *, const void *)) compare_annotated_word) && array_list_contains(candidate_list, next_word,
                                                                                                                          (int (*)(const void *, const void *)) compare_annotated_word) && word->semantic != NULL && strcmp(word->semantic, next_word->semantic) == 0){
                array_list_add(candidate_list, word);
            }
        }
    }
    return candidate_list;
}

/**
 * The method returns all possible words, which is
 * 1. Verb
 * 2. Its semantic tag is assigned
 * 3. A frameset exists for that semantic tag
 * @param sentence Sentence object.
 * @param frameNet Frameset list that contains all frames for Turkish
 * @return An array of words, which are verbs, semantic tags assigned, and framesetlist assigned for that tag.
 */
Array_list_ptr predicate_frame_candidates(Sentence_ptr sentence, Frame_net_ptr frameNet) {
    Array_list_ptr candidate_list = create_array_list();
    for (int i = 0; i < sentence->words->size; i++){
        Annotated_word_ptr word = array_list_get(sentence->words, i);
        if (word->parse != NULL && is_parse_verb(word->parse) && word->semantic != NULL && frame_net_lexical_unit_exists(frameNet, word->semantic)){
            array_list_add(candidate_list, word);
        }
    }
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < sentence->words->size - i - 1; j++){
            Annotated_word_ptr word = array_list_get(sentence->words, j);
            Annotated_word_ptr next_word = array_list_get(sentence->words, j + 1);
            if (!array_list_contains(candidate_list, word,
                                     (int (*)(const void *, const void *)) compare_annotated_word) && array_list_contains(candidate_list, next_word,
                                                                                                                          (int (*)(const void *, const void *)) compare_annotated_word) && word->semantic != NULL && strcmp(word->semantic, next_word->semantic) == 0){
                array_list_add(candidate_list, word);
            }
        }
    }
    return candidate_list;
}

/**
 * Returns the i'th predicate in the sentence.
 * @param sentence Sentence object.
 * @param index Predicate index
 * @return The predicate with index index in the sentence.
 */
char *get_predicate(Sentence_ptr sentence, int index) {
    int count1  = 0, count2 = 0;
    char* data = NULL;
    Array_list_ptr word_list = create_array_list();
    Array_list_ptr parse_list = create_array_list();
    if (index < sentence->words->size){
        for (int i = 0; i < sentence->words->size; i++) {
            Annotated_word_ptr word = array_list_get(sentence->words, i);
            array_list_add(word_list, word);
            array_list_add(parse_list, word->parse);
        }
        for (int i = index; i >= 0; i--) {
            Morphological_parse_ptr parse = array_list_get(parse_list, i);
            char* root_pos = get_root_pos(parse);
            char* pos = get_pos(parse);
            if (parse != NULL && root_pos != NULL && pos != NULL && strcmp(root_pos, "VERB") == 0 && strcmp(pos, "VERB") == 0){
                free_(root_pos);
                free_(pos);
                count1 = index - i;
                break;
            }
            free_(root_pos);
            free_(pos);
        }
        for (int i = index; i < sentence->words->size - index; i++) {
            Morphological_parse_ptr parse = array_list_get(parse_list, i);
            char* root_pos = get_root_pos(parse);
            char* pos = get_pos(parse);
            if (parse != NULL && root_pos != NULL && pos != NULL && strcmp(root_pos, "VERB") == 0 && strcmp(pos, "VERB") == 0){
                free_(root_pos);
                free_(pos);
                count2 = i - index;
                break;
            }
            free_(root_pos);
            free_(pos);
        }
        if (count1 > count2){
            data = ((Annotated_word_ptr) array_list_get(word_list, count1))->name;
        }
        else{
            data = ((Annotated_word_ptr) array_list_get(word_list, count2))->name;
        }
    }
    free_array_list(word_list, NULL);
    free_array_list(parse_list, NULL);
    return data;
}

/**
 * The toStems method returns an accumulated string of each word's stems in words ArrayList.
 * If the parse of the word does not exist, the method adds the surfaceform to the resulting string.
 *
 * @param sentence Sentence object.
 * @return String result which has all the stems of each item in words ArrayList.
 */
char *to_stems(Sentence_ptr sentence) {
    char tmp[MAX_WORD_LENGTH], tmp1[MAX_WORD_LENGTH];
    Annotated_word_ptr annotatedWord;
    if (sentence->words->size != 0) {
        annotatedWord = array_list_get(sentence->words, 0);
        if (annotatedWord->parse != NULL){
            sprintf(tmp, "%s", annotatedWord->parse->root->name);
        } else {
            sprintf(tmp, "%s", annotatedWord->name);
        }
        for (int i = 1; i < sentence->words->size; i++) {
            annotatedWord = array_list_get(sentence->words, i);
            if (annotatedWord->parse != NULL){
                sprintf(tmp1, "%s %s", tmp, annotatedWord->parse->root->name);
                strcpy(tmp, tmp1);
            } else {
                sprintf(tmp1, "%s %s", tmp, annotatedWord->name);
                strcpy(tmp, tmp1);
            }
        }
        char* result = malloc_((strlen(tmp) + 1) * sizeof(char));
        strcpy(result, tmp);
        return result;
    } else {
        return NULL;
    }
}

/**
 * Compares the sentence with the given sentence and returns a parser evaluation score for this comparison. The result
 * is calculated by summing up the parser evaluation scores of word by word dpendency relation comparisons.
 * @param sentence1 First sentence to be compared.
 * @param sentence2 Second sentence to be compared.
 * @return A parser evaluation score object.
 */
Parser_evaluation_score_ptr compare_sentence_parses(Sentence_ptr sentence1, Sentence_ptr sentence2) {
    Parser_evaluation_score_ptr score = create_parser_evaluation_score2();
    for (int i = 0; i < sentence1->words->size; i++){
        Universal_dependency_relation_ptr relation1 = ((Annotated_word_ptr)array_list_get(sentence1->words, i))->universal_dependency;
        Universal_dependency_relation_ptr relation2 = ((Annotated_word_ptr)array_list_get(sentence2->words, i))->universal_dependency;
        if (relation1 != NULL && relation2 != NULL){
            add_score(score, compare_relations(relation1, relation2));
        }
    }
    return score;
}
