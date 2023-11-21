cmake_minimum_required(VERSION 3.23)
project(AnnotatedSentence C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-O3")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(AnnotatedSentence src/Language.h src/ViewLayerType.h src/AnnotatedWord.c src/AnnotatedWord.h src/AnnotatedPhrase.c src/AnnotatedPhrase.h src/AnnotatedSentence.c src/AnnotatedSentence.h src/AnnotatedCorpus.c src/AnnotatedCorpus.h)
target_link_libraries(AnnotatedSentence ${CONAN_LIBS})
add_executable(AnnotatedSentenceTest src/Language.h src/ViewLayerType.h src/AnnotatedWord.c src/AnnotatedWord.h src/AnnotatedPhrase.c src/AnnotatedPhrase.h src/AnnotatedSentence.c src/AnnotatedSentence.h src/AnnotatedCorpus.c src/AnnotatedCorpus.h Test/AnnotatedSentenceTest.c)
target_link_libraries(AnnotatedSentenceTest ${CONAN_LIBS})
