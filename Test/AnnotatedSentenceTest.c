//
// Created by Olcay Taner YILDIZ on 16.11.2023.
//

#include <Corpus.h>
#include <Memory/Memory.h>
#include "../src/AnnotatedCorpus.h"
#include "../src/AnnotatedSentence.h"
#include "../src/AnnotatedPhrase.h"

int main(){
    start_large_memory_check();
    int sizes[10] = {4, 5, 3, 5, 5, 5, 6, 5, 5, 3};
    char* stems[10] = {"devasa ölçek yeni kanun kullan karmaşık ve çetrefil dil kavga bulan .",
                       "gelir art usul komite gel salı gün kanun tasarı hakkında bir duruşma yap .",
                       "reklam ve tanıtım iş yara yara gör üzere .",
                       "bu defa , daha da hız hareket et .",
                       "shearson lehman hutton ınc. dün öğle sonra kadar yeni tv reklam yaz .",
                       "bu kez , firma hazır .",
                       "`` diyalog sür kesinlikle temel önem haiz .",
                       "cuma gün bu üzerine düşün çok geç kal ol .",
                       "bu hakkında önceden düşün gerek . ''",
                       "isim göre çeşit göster birkaç kefaret fon reklam yap için devam et ."};
    Corpus_ptr corpus = create_annotated_corpus("../../sentences");
    for (int i = 0; i < 10; i++){
        Sentence_ptr sentence = array_list_get(corpus->sentences, i);
        char* _stems = to_stems(sentence);
        if (strcmp(stems[i], _stems) != 0){
            printf("Error in sentence %s\n", _stems);
        }
        free_(_stems);
    }
    printf("Predicates:\n");
    for (int i = 0; i < 10; i++){
        Sentence_ptr sentence = array_list_get(corpus->sentences, i);
        if (contains_predicate(sentence)){
            printf("%s\n", get_predicate(sentence, 0));
        }
    }
    for (int i = 0; i < 10; i++){
        Sentence_ptr sentence = array_list_get(corpus->sentences, i);
        Array_list_ptr groups = get_shallow_parse_groups(sentence);
        if (sizes[i] != groups->size){
            printf("Error in size %d\n", get_shallow_parse_groups(sentence)->size);
        }
        free_array_list(groups, (void*)free_annotated_phrase);
    }
    free_annotated_corpus(corpus);
    end_memory_check();
}