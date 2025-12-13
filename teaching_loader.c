#include <stdio.h>
#include <string.h>
#include "teaching_loader.h"

const char* get_teaching_filename(Level level) {
    switch(level) {
        case BEGINNER: 
            return "teaching_beginner.txt";
            
        case INTERMEDIATE: 
            return "teaching_intermediate.txt";

        case ADVANCED: 
            return "teaching_advanced.txt";

        default: 
            return NULL;
    }
}

int load_teaching_content(const char* filename, TeachingSection sections[], int max_sections) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return 0;
    }

    int count = 0;
    char line[1000];

    while (count < max_sections && fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;

        // Read section title
        if (sscanf(line, "SECTION:%[^\n]", sections[count].section_title) == 1) {
            // Read content
            fgets(line, sizeof(line), file);
            line[strcspn(line, "\n")] = 0;
            sscanf(line, "CONTENT:%[^\n]", sections[count].content);
            
            // Read example
            fgets(line, sizeof(line), file);
            line[strcspn(line, "\n")] = 0;
            sscanf(line, "EXAMPLE:%[^\n]", sections[count].example);
            
            // Read steps
            sections[count].num_steps = 0;
            for (int i = 0; i < 5; i++) {
                if (!fgets(line, sizeof(line), file)) break;
                line[strcspn(line, "\n")] = 0;
                
                // Check if it's a step or end of section
                if (strncmp(line, "STEP", 4) == 0) {
                    sscanf(line, "STEP%*d:%[^\n]", sections[count].steps[i]);
                    sections[count].num_steps++;
                } else if (strcmp(line, "---") == 0) {
                    break;
                }
            }
            
            count++;
        }
    }

    fclose(file);
    return count;
}