#ifndef QUESTION_LOADER_H
#define QUESTION_LOADER_H

#include "structures.h"

// Load questions from file based on level
int load_questions(const char* filename, Question questions[], int max_questions);

// Get filename for specific level
const char* get_question_filename(Level level);

#endif