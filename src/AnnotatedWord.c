//
// Created by Olcay Taner YILDIZ on 16.11.2023.
//

#include <stdlib.h>
#include <string.h>
#include <FileUtils.h>
#include <PolarityType.h>
#include <Memory/Memory.h>
#include "AnnotatedWord.h"

/**
 * Constructor for the AnnotatedWord class. Gets the word with its annotation layers as input and sets the
 * corresponding layers.
 * @param word Input word with annotation layers
 */
Annotated_word_ptr create_annotated_word(const char *word) {
    Annotated_word_ptr result = create_annotated_word2();
    Array_list_ptr split_layers = str_split3(word, "[{}]");
    for (int i = 0; i < split_layers->size; i++){
        char* layer = array_list_get(split_layers, i);
        if (strlen(layer) == 0){
            continue;
        }
        if (!str_contains(layer, "=")){
            result->name = str_copy(result->name, layer);
            continue;
        }
        String_ptr st1 = substring(layer, 0, str_find_c(layer, "="));
        String_ptr st2 = substring2(layer, str_find_c(layer, "=") + 1);
        char* layer_type = st1->s;
        char* layer_value = st2->s;
        if (string_in_list(layer_type, (char*[]) {"turkish", "english", "persian"}, 3)){
            result->name = str_copy(result->name, layer_value);
            result->language = get_language_from_string(layer_type);
        } else {
            if (strcmp(layer_type, "morphologicalAnalysis") == 0){
                result->parse = create_morphological_parse(layer_value);
            } else {
                if (strcmp(layer_type, "metaMorphemes") == 0){
                    result->metamorphic_parse = create_metamorphic_parse(layer_value);
                } else {
                    if (strcmp(layer_type, "semantics") == 0){
                        result->semantic = str_copy(result->semantic, layer_value);
                    } else {
                        if (strcmp(layer_type, "namedEntity") == 0){
                            result->named_entity_type = get_named_entity_type(layer_value);
                        } else {
                            if (strcmp(layer_type, "propbank") == 0 || strcmp(layer_type, "propBank") == 0){
                                result->argument_list = create_argument_list(layer_value);
                            } else {
                                if (strcmp(layer_type, "shallowParse") == 0){
                                    result->shallow_parse = str_copy(result->shallow_parse, layer_value);
                                } else {
                                    if (strcmp(layer_type, "universalDependency") == 0){
                                        Array_list_ptr values = str_split(layer_value, '$');
                                        result->universal_dependency = create_universal_dependency_relation(atoi(
                                                array_list_get(values, 0)), array_list_get(values, 1));
                                        free_array_list(values, free_);
                                    } else {
                                        if (strcmp(layer_type, "framenet") == 0 || strcmp(layer_type, "frameNet") == 0){
                                            result->frame_element_list = create_frame_element_list(layer_value);
                                        } else {
                                            if (strcmp(layer_type, "slot") == 0){
                                                result->slot = create_slot2(layer_value);
                                            } else {
                                                if (strcmp(layer_type, "polarity") == 0){
                                                    result->polarity = get_polarity_type(layer_value);
                                                } else {
                                                    if (strcmp(layer_type, "ccg") == 0) {
                                                        result->ccg = str_copy(result->ccg, layer_value);
                                                    } else {
                                                        if (strcmp(layer_type, "posTag") == 0) {
                                                            result->pos_tag = str_copy(result->pos_tag, layer_value);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        free_string_ptr(st1);
        free_string_ptr(st2);
    }
    free_array_list(split_layers, free_);
    return result;
}

/**
 * Converts a language string to language.
 * @param language String defining the language name.
 * @return Language corresponding to the languageString.
 */
Language get_language_from_string(const char *language) {
    if (string_in_list(language, (char*[]) {"turkish", "Turkish"}, 2)) {
        return TURKISH;
    } else {
        if (string_in_list(language, (char*[]) {"english", "English"}, 2)) {
            return ENGLISH;
        } else {
            if (string_in_list(language, (char*[]) {"persian", "Persian"}, 2)) {
                return PERSIAN;
            }
        }
    }
    return TURKISH;
}

/**
 * Empty constructor for AnnotatedWord.
 * @return Allocated new annotated word.
 */
Annotated_word_ptr create_annotated_word2() {
    Annotated_word_ptr result = malloc_(sizeof(Annotated_word), "create_annotated_word2");
    result->name = NULL;
    result->parse = NULL;
    result->metamorphic_parse = NULL;
    result->argument_list = NULL;
    result->frame_element_list = NULL;
    result->slot = NULL;
    result->universal_dependency = NULL;
    result->pos_tag = NULL;
    result->shallow_parse = NULL;
    result->ccg = NULL;
    result->semantic = NULL;
    result->named_entity_type = NER_NONE;
    result->polarity = POLARITY_NEUTRAL;
    result->language = TURKISH;
    return result;
}

/**
 * Frees memory to be dealloacated for a word.
 * @param word Word to be deallocated.
 */
void free_annotated_word(Annotated_word_ptr word) {
    free_(word->name);
    if (word->parse != NULL){
        free_morphological_parse(word->parse);
    }
    if (word->metamorphic_parse != NULL){
        free_metamorphic_parse(word->metamorphic_parse);
    }
    if (word->argument_list != NULL){
        free_argument_list(word->argument_list);
    }
    if (word->frame_element_list != NULL){
        free_frame_element_list(word->frame_element_list);
    }
    if (word->slot != NULL){
        free_slot(word->slot);
    }
    if (word->universal_dependency != NULL){
        free_universal_dependency_relation(word->universal_dependency);
    }
    free_(word->pos_tag);
    free_(word->shallow_parse);
    free_(word->ccg);
    free_(word->semantic);
    free_(word);
}

/**
 * Converts an AnnotatedWord to string. For each annotation layer, the method puts a left brace, layer name,
 * equal sign and layer value finishing with right brace.
 * @return String form of the AnnotatedWord.
 */
char *annotated_word_to_string(const Annotated_word *word) {
    char tmp[MAX_WORD_LENGTH], tmp1[MAX_WORD_LENGTH];
    switch (word->language) {
        case TURKISH:
            sprintf(tmp, "{turkish=%s}", word->name);
            break;
        case ENGLISH:
            sprintf(tmp, "{english=%s}", word->name);
            break;
        case PERSIAN:
            sprintf(tmp, "{persian=%s}", word->name);
            break;
    }
    if (word->parse != NULL){
        char* s = morphological_parse_to_string(word->parse);
        sprintf(tmp1, "%s{morphologicalAnalysis=%s}", tmp, s);
        strcpy(tmp, tmp1);
        free_(s);
    }
    if (word->metamorphic_parse != NULL){
        char* s = metamorphic_parse_to_string(word->metamorphic_parse);
        sprintf(tmp1, "%s{metaMorphemes=%s}", tmp, s);
        strcpy(tmp, tmp1);
        free_(s);
    }
    if (word->semantic != NULL){
        sprintf(tmp1, "%s{semantics=%s}", tmp, word->semantic);
        strcpy(tmp, tmp1);
    }
    if (word->named_entity_type != NER_NONE){
        sprintf(tmp1, "%s{namedEntity=%s}", tmp, named_entity_type_to_string(word->named_entity_type));
        strcpy(tmp, tmp1);
    }
    if (word->argument_list != NULL){
        char* s = argument_list_to_string(word->argument_list);
        sprintf(tmp1, "%s{propbank=%s}", tmp, s);
        strcpy(tmp, tmp1);
        free_(s);
    }
    if (word->frame_element_list != NULL){
        char* s = frame_element_list_to_string(word->frame_element_list);
        sprintf(tmp1, "%s{framenet=%s}", tmp, s);
        strcpy(tmp, tmp1);
        free_(s);
    }
    if (word->shallow_parse != NULL){
        sprintf(tmp1, "%s{shallowParse=%s}", tmp, word->shallow_parse);
        strcpy(tmp, tmp1);
    }
    if (word->universal_dependency != NULL){
        char* s = universal_dependency_relation_to_string(word->universal_dependency);
        sprintf(tmp1, "%s{universalDependency=%s}", tmp, s);
        strcpy(tmp, tmp1);
        free_(s);
    }
    if (word->slot != NULL){
        char* s = slot_to_string(word->slot);
        sprintf(tmp1, "%s{slot=%s}", tmp, s);
        strcpy(tmp, tmp1);
        free_(s);
    }
    if (word->polarity != POLARITY_NEUTRAL){
        sprintf(tmp1, "%s{polarity=%s}", tmp, polarity_to_string(word->polarity));
        strcpy(tmp, tmp1);
    }
    if (word->ccg != NULL) {
        sprintf(tmp1, "%s{ccg=%s}", tmp, word->ccg);
        strcpy(tmp, tmp1);
    }
    if (word->pos_tag != NULL) {
        sprintf(tmp1, "%s{posTag=%s}", tmp, word->pos_tag);
        strcpy(tmp, tmp1);
    }
    return clone_string(tmp);
}

/**
 * Another constructor for AnnotatedWord. Gets the word and a namedEntityType and sets two layers.
 * @param name Lemma of the word.
 * @param named_entity_type Named entity of the word.
 */
Annotated_word_ptr create_annotated_word3(const char *name, Named_entity_type named_entity_type) {
    Annotated_word_ptr result = create_annotated_word2();
    result->name = str_copy(result->name, name);
    result->named_entity_type = named_entity_type;
    return result;
}

/**
 * Another constructor for AnnotatedWord. Gets the word and morphological parse and sets two layers.
 * @param name Lemma of the word.
 * @param parse Morphological parse of the word.
 */
Annotated_word_ptr create_annotated_word4(const char *name, Morphological_parse_ptr parse) {
    Annotated_word_ptr result = create_annotated_word2();
    result->name = str_copy(result->name, name);
    result->parse = parse;
    return result;
}

/**
 * Another constructor for AnnotatedWord. Gets the word and morphological parse and sets two layers.
 * @param name Lemma of the word.
 * @param parse Morphological parse of the word.
 */
Annotated_word_ptr create_annotated_word5(const char *name, Fsm_parse_ptr parse) {
    Annotated_word_ptr result = create_annotated_word2();
    result->name = str_copy(result->name, name);
    char* st = fsm_parse_to_string(parse);
    result->parse = create_morphological_parse(st);
    free_(st);
    st = get_with_list(parse);
    result->metamorphic_parse = create_metamorphic_parse(st);
    free_(st);
    return result;
}

/**
 * Returns the value of a given layer.
 * @param word Annotated word
 * @param view_layer_type Layer for which the value questioned.
 * @return The value of the given layer.
 */
char *get_layer_info(const Annotated_word *word, View_layer_type view_layer_type) {
    switch (view_layer_type){
        case INFLECTIONAL_GROUP:
            if (word->parse != NULL){
                return morphological_parse_to_string(word->parse);
            }
            break;
        case META_MORPHEME:
            if (word->metamorphic_parse != NULL){
                return metamorphic_parse_to_string(word->metamorphic_parse);
            }
            break;
        case SEMANTICS:
            return word->semantic;
        case NER:
            if (word->named_entity_type != NER_NONE){
                return named_entity_type_to_string(word->named_entity_type);
            }
            break;
        case SHALLOW_PARSE:
            return word->shallow_parse;
        case TURKISH_WORD:
        default:
            return word->name;
        case PROPBANK:
            if (word->argument_list != NULL){
                return argument_list_to_string(word->argument_list);
            }
            break;
        case FRAMENET:
            if (word->frame_element_list != NULL){
                return frame_element_list_to_string(word->frame_element_list);
            }
            break;
        case DEPENDENCY:
            if (word->universal_dependency != NULL){
                return universal_dependency_relation_to_string(word->universal_dependency);
            }
            break;
        case SLOT:
            if (word->slot != NULL){
                return slot_to_string(word->slot);
            }
            break;
        case POLARITY:
            if (word->polarity != POLARITY_NEUTRAL){
                return polarity_to_string(word->polarity);
            }
            break;
        case CCG:
            return word->ccg;
        case POS_TAG:
            return word->pos_tag;
    }
    return NULL;
}

/**
 * Comparator method for twp words.
 * @param word1 First word to compare.
 * @param word2 Second word to compare.
 * @return 0 if they are equal. -1 if the first comes before the second one in lexicographical order. 1, otherwise.
 */
int compare_annotated_word(const Annotated_word *word1, const Annotated_word *word2) {
    return compare_string(word1->name, word2->name);
}
