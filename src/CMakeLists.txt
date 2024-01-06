cmake_minimum_required(VERSION 3.23)
project(AnnotatedSentence)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-O3")

find_package(dependency_parser_c REQUIRED)
find_package(named_entity_recognition_c REQUIRED)
find_package(prop_bank_c REQUIRED)
find_package(frame_net_c REQUIRED)
find_package(senti_net_c REQUIRED)
find_package(word_net_c REQUIRED)

add_library(AnnotatedSentence Language.h ViewLayerType.h AnnotatedWord.c AnnotatedWord.h AnnotatedPhrase.c AnnotatedPhrase.h AnnotatedSentence.c AnnotatedSentence.h AnnotatedCorpus.c AnnotatedCorpus.h ViewLayerType.c)
target_link_libraries(AnnotatedSentence dependency_parser_c::dependency_parser_c named_entity_recognition_c::named_entity_recognition_c prop_bank_c::prop_bank_c frame_net_c::frame_net_c senti_net_c::senti_net_c word_net_c::word_net_c)
