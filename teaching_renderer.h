#ifndef TEACHING_RENDERER_H
#define TEACHING_RENDERER_H

#include "structures.h"

// Display teaching content with step-by-step progression
void display_teaching_content(TeachingSection sections[], int num_sections, Level level);

// Display a single teaching section
void display_section(TeachingSection* section);

#endif