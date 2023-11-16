//
// Created by Olcay Taner YILDIZ on 16.11.2023.
//

#ifndef ANNOTATEDSENTENCE_ANNOTATEDSENTENCE_H
#define ANNOTATEDSENTENCE_ANNOTATEDSENTENCE_H

#include <Sentence.h>
#include <FramesetList.h>
#include <FrameNet.h>
#include <ParserEvaluationScore.h>

Sentence_ptr create_annotated_sentence(FILE* input_file);

Sentence_ptr create_annotated_sentence2(const char* sentence);

void free_annotated_sentence(Sentence_ptr string);

bool contains_predicate(Sentence_ptr sentence);

bool contains_frame_predicate(Sentence_ptr sentence);

bool update_connected_predicate(Sentence_ptr sentence, const char* previous_id, const char* current_id);

Array_list_ptr get_shallow_parse_groups(Sentence_ptr sentence);

Array_list_ptr predicate_candidates(Sentence_ptr sentence, Frameset_list_ptr framesetList);

Array_list_ptr predicate_frame_candidates(Sentence_ptr sentence, Frame_net_ptr frameNet);

char* get_predicate(Sentence_ptr sentence, int index);

char* to_stems(Sentence_ptr sentence);

Parser_evaluation_score_ptr compare_sentence_parses(Sentence_ptr sentence1, Sentence_ptr sentence2);

#endif //ANNOTATEDSENTENCE_ANNOTATEDSENTENCE_H
