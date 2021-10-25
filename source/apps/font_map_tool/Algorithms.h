#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "CharacterBounds.h"

void Algorithm_DetectRows(std::list<CharacterBounds> * characters, std::vector<int> * lineHeights);
void Algorithm_SortAndFix(std::list<CharacterBounds> * characters, std::vector<int> lineHeights);
int Algorithm_GetMaxCharactersWidth(std::list<CharacterBounds> * characters);

#endif