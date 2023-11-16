//
// Created by Olcay Taner YILDIZ on 16.11.2023.
//

#ifndef ANNOTATEDSENTENCE_ANNOTATEDCORPUS_H
#define ANNOTATEDSENTENCE_ANNOTATEDCORPUS_H

#include <ParserEvaluationScore.h>
#include <Corpus.h>

Corpus_ptr create_annotated_corpus(const char* folder);

Parser_evaluation_score_ptr compare_corpus_parses(Corpus_ptr corpus1, Corpus_ptr corpus2);

void free_annotated_corpus(Corpus_ptr corpus);

#endif //ANNOTATEDSENTENCE_ANNOTATEDCORPUS_H
