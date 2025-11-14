#ifndef QUIZ_ENGINE_H
#define QUIZ_ENGINE_H

#include "structures.h"

// Run a quiz with given questions and return the score
int run_quiz(Question questions[], int num_questions, StudentProgress* progress);

// Display a single question and get user answer
int ask_question(Question* q, int show_hint);

// Validate multiple choice answer
int validate_mc_answer(char user_answer, char correct_answer);

// Validate numerical answer with tolerance
int validate_num_answer(double user_answer, double correct_answer, double tolerance);

#endif