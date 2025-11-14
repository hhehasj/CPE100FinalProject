#ifndef LEVEL_RULES_H
#define LEVEL_RULES_H

#include "structures.h"

// Check if student passed the current level
int check_pass(int score, int total, Level level);

// Get passing score for a level
int get_passing_score(Level level);

// Get level name as string
const char* get_level_name(Level level);

#endif