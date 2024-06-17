//
// Created by Olcay Taner YILDIZ on 16.11.2023.
//

#include <dirent.h>
#include <Memory/Memory.h>
#include "AnnotatedCorpus.h"
#include "AnnotatedSentence.h"

/**
 * Compares the corpus with the given corpus and returns a parser evaluation score for this comparison. The result
 * is calculated by summing up the parser evaluation scores of sentence by sentence dependency relation comparisons.
 * @param corpus1 First corpus to be compared.
 * @param corpus2 Second corpus to be compared.
 * @return A parser evaluation score object.
 */
Parser_evaluation_score_ptr compare_corpus_parses(Corpus_ptr corpus1, Corpus_ptr corpus2) {
    Parser_evaluation_score_ptr score = create_parser_evaluation_score2();
    for (int i = 0; i < corpus1->sentences->size; i++){
        Sentence_ptr sentence1 = array_list_get(corpus1->sentences, i);
        Sentence_ptr sentence2 = array_list_get(corpus2->sentences, i);
        add_score(score, compare_sentence_parses(sentence1, sentence2));
    }
    return score;
}

/**
 * A constructor of AnnotatedCorpus class which reads all AnnotatedSentence files with the file
 * name satisfying the given pattern inside the given folder. For each file inside that folder, the constructor
 * creates an AnnotatedSentence and puts in inside the list parseTrees.
 * @param folder Folder where all sentences reside.
 * @param fileList File list containing all sentences.
 */
Corpus_ptr create_annotated_corpus(const char *folder) {
    DIR *directory_ptr;
    struct dirent *file_entry;
    Array_list_ptr files = create_array_list();
    Corpus_ptr corpus = create_corpus();
    directory_ptr = opendir(folder);
    while ((file_entry = readdir(directory_ptr)) != NULL){
        if (ends_with(file_entry->d_name, "dev") || ends_with(file_entry->d_name, "train") || ends_with(file_entry->d_name, "test")){
            char* file_name = malloc_(strlen(folder) + strlen(file_entry->d_name) + 2, "create_annotated_corpus");
            sprintf(file_name, "%s/%s", folder, file_entry->d_name);
            array_list_add(files, file_name);
        }
    }
    closedir(directory_ptr);
    array_list_sort(files, (int (*)(const void *, const void *)) compare_string);
    for (int i = 0; i < files->size; i++){
        FILE* sentence_file = fopen(array_list_get(files, i), "r");
        if (sentence_file != NULL){
            array_list_add(corpus->sentences, create_annotated_sentence(sentence_file));
            fclose(sentence_file);
        }
    }
    free_array_list(files, free_);
    return corpus;
}

/**
 * Frees memory allocated to the AnnotatedCorpus object. Deallocates memory for word_list hash map, paragraphs and
 * sentences array lists.
 */
void free_annotated_corpus(Corpus_ptr corpus) {
    free_counter_hash_map(corpus->word_list);
    free_array_list(corpus->paragraphs, NULL);
    free_array_list(corpus->sentences, (void (*)(void *)) free_annotated_sentence);
    free_(corpus);
}
