//
// Created by Olcay Taner YILDIZ on 16.11.2023.
//

#include <stdlib.h>
#include <string.h>
#include <FileUtils.h>
#include <PolarityType.h>
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
                                result->argument = create_argument2(layer_value);
                            } else {
                                if (strcmp(layer_type, "shallowParse") == 0){
                                    result->shallow_parse = str_copy(result->shallow_parse, layer_value);
                                } else {
                                    if (strcmp(layer_type, "universalDependency") == 0){
                                        Array_list_ptr values = str_split(layer_value, '$');
                                        result->universal_dependency = create_universal_dependency_relation(atoi(
                                                array_list_get(values, 0)), array_list_get(values, 1));
                                    } else {
                                        if (strcmp(layer_type, "framenet") == 0 || strcmp(layer_type, "frameNet") == 0){
                                            result->frame_element = create_frame_element2(layer_value);
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
    free_array_list(split_layers, free);
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

Annotated_word_ptr create_annotated_word2() {
    Annotated_word_ptr result = malloc(sizeof(Annotated_word));
    result->name = NULL;
    result->parse = NULL;
    result->metamorphic_parse = NULL;
    result->semantic = NULL;
    result->named_entity_type = NER_NONE;
    result->frame_element = NULL;
    result->polarity = POLARITY_NEUTRAL;
    result->shallow_parse = NULL;
    result->slot = NULL;
    result->ccg = NULL;
    result->pos_tag = NULL;
    result->language = TURKISH;
    return result;
}

/**
 * Converts an AnnotatedWord to string. For each annotation layer, the method puts a left brace, layer name,
 * equal sign and layer value finishing with right brace.
 * @return String form of the AnnotatedWord.
 */
char *annotated_word_to_string(const Annotated_word *word) {
    char tmp[MAX_WORD_LENGTH];
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
        sprintf(tmp, "%s{morphologicalAnalysis=%s}", tmp, s);
        free(s);
    }
    if (word->metamorphic_parse != NULL){
        char* s = metamorphic_parse_to_string(word->metamorphic_parse);
        sprintf(tmp, "%s{metaMorphemes=%s}", tmp, s);
        free(s);
    }
    if (word->semantic != NULL){
        sprintf(tmp, "%s{semantics=%s}", tmp, word->semantic);
    }
    if (word->named_entity_type != NER_NONE){
        sprintf(tmp, "%s{namedEntity=%s}", tmp, named_entity_type_to_string(word->named_entity_type));
    }
    if (word->argument != NULL){
        char* s = argument_to_string(word->argument);
        sprintf(tmp, "%s{propbank=%s}", tmp, s);
        free(s);
    }
    if (word->frame_element != NULL){
        char* s = frame_element_to_string(word->frame_element);
        sprintf(tmp, "%s{framenet=%s}", tmp, s);
        free(s);
    }
    if (word->shallow_parse != NULL){
        sprintf(tmp, "%s{shallowParse=%s}", tmp, word->shallow_parse);
    }
    if (word->universal_dependency != NULL){
        char* s = universal_dependency_relation_to_string(word->universal_dependency);
        sprintf(tmp, "%s{universalDependency=%s}", tmp, s);
        free(s);
    }
    if (word->slot != NULL){
        char* s = slot_to_string(word->slot);
        sprintf(tmp, "%s{slot=%s}", tmp, s);
        free(s);
    }
    if (word->polarity != POLARITY_NEUTRAL){
        sprintf(tmp, "%s{polarity=%s}", tmp, polarity_to_string(word->polarity));
    }
    if (word->ccg != NULL) {
        sprintf(tmp, "%s{ccg=%s}", tmp, word->ccg);
    }
    if (word->pos_tag != NULL) {
        sprintf(tmp, "%s{posTag=%s}", tmp, word->pos_tag);
    }
    char* result = str_copy(result, tmp);
    return result;
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
    free(st);
    st = get_with_list(parse);
    result->metamorphic_parse = create_metamorphic_parse(st);
    free(st);
    return result;
}

/**
 * Returns the value of a given layer.
 * @param viewLayerType Layer for which the value questioned.
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
            if (word->argument != NULL){
                return argument_to_string(word->argument);
            }
            break;
        case FRAMENET:
            if (word->frame_element != NULL){
                return frame_element_to_string(word->frame_element);
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

void free_annotated_word(Annotated_word_ptr word) {
    free(word->name);
    if (word->parse != NULL){
        free_morphological_parse(word->parse);
    }
    if (word->metamorphic_parse != NULL){
        free_metamorphic_parse(word->metamorphic_parse);
    }
    if (word->argument != NULL){
        free_argument(word->argument);
    }
    if (word->frame_element != NULL){
        free_frame_element(word->frame_element);
    }
    if (word->slot != NULL){
        free_slot(word->slot);
    }
    free(word->pos_tag);
    free(word->shallow_parse);
    free(word->ccg);
    free(word->semantic);
    free(word);
}

int compare_annotated_word(const Annotated_word *word1, const Annotated_word *word2) {
    return compare_string(word1->name, word2->name);
}
