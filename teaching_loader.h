#ifndef TEACHING_LOADER_H
#define TEACHING_LOADER_H

#include "structures.h"

// Load teaching content from file based on level
int load_teaching_content(const char* filename, TeachingSection sections[], int max_sections);

// Get filename for specific level teaching content
const char* get_teaching_filename(Level level);

#endif