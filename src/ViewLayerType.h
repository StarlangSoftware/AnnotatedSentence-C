//
// Created by Olcay Taner YILDIZ on 16.11.2023.
//

#ifndef ANNOTATEDSENTENCE_VIEWLAYERTYPE_H
#define ANNOTATEDSENTENCE_VIEWLAYERTYPE_H

enum view_layer_type{
    PART_OF_SPEECH, INFLECTIONAL_GROUP, META_MORPHEME, META_MORPHEME_MOVED, TURKISH_WORD, PERSIAN_WORD, ENGLISH_WORD, WORD, SEMANTICS, NER, DEPENDENCY, PROPBANK, SHALLOW_PARSE, ENGLISH_PROPBANK, ENGLISH_SEMANTICS, FRAMENET, SLOT, POLARITY, CCG, POS_TAG
};

typedef enum view_layer_type View_layer_type;

typedef View_layer_type *View_layer_type_ptr;

int compare_view_layer_type(const View_layer_type* layer_type1, const View_layer_type* layer_type2);

unsigned int hash_function_view_layer_type(const View_layer_type* layer_type, int N);

View_layer_type_ptr create_view_layer_type(View_layer_type layer_type);

#endif //ANNOTATEDSENTENCE_VIEWLAYERTYPE_H
