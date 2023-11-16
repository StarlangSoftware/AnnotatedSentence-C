//
// Created by Olcay Taner YILDIZ on 16.11.2023.
//

#ifndef ANNOTATEDSENTENCE_ANNOTATEDWORD_H
#define ANNOTATEDSENTENCE_ANNOTATEDWORD_H

#include <MorphologicalParse.h>
#include <MetamorphicParse.h>
#include <NamedEntityType.h>
#include <Argument.h>
#include <FrameElement.h>
#include <PolarityType.h>
#include <Slot.h>
#include <Universal/UniversalDependencyRelation.h>
#include <FsmParse.h>
#include "Language.h"
#include "ViewLayerType.h"

struct annotated_word{
    char* name;
    Morphological_parse_ptr parse;
    Metamorphic_parse_ptr metamorphic_parse;
    char* semantic;
    Named_entity_type named_entity_type;
    Argument_ptr argument;
    Frame_element_ptr frame_element;
    Polarity_type polarity;
    char* shallow_parse;
    Slot_ptr slot;
    char* ccg;
    char* pos_tag;
    Language language;
    Universal_dependency_relation_ptr universal_dependency;
};

typedef struct annotated_word Annotated_word;

typedef Annotated_word *Annotated_word_ptr;

Annotated_word_ptr create_annotated_word(const char* word);

Annotated_word_ptr create_annotated_word2();

Annotated_word_ptr create_annotated_word3(const char* name, Named_entity_type named_entity_type);

Annotated_word_ptr create_annotated_word4(const char* name, Morphological_parse_ptr parse);

Annotated_word_ptr create_annotated_word5(const char* name, Fsm_parse_ptr parse);

Language get_language_from_string(const char* language);

char* annotated_word_to_string(const Annotated_word* word);

char* get_layer_info(const Annotated_word* word, View_layer_type view_layer_type);

void free_annotated_word(Annotated_word_ptr word);

int compare_annotated_word(const Annotated_word* word1, const Annotated_word* word2);

#endif //ANNOTATEDSENTENCE_ANNOTATEDWORD_H
