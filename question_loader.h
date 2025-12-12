#ifndef QUESTION_LOADER_H
#define QUESTION_LOADER_H

#include "structures.h"

// Load questions from file based on level
int load_questions(const char* filename, Question questions[], int max_questions);

// Get filename for specific level
const char* get_question_filename(Level level);

int select_random_questions(Question all_questions[], int total_questions, Question selected_questions[], int num_to_select, StudentProgress *progress);

#endif