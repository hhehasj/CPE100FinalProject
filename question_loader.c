#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "question_loader.h"

const char* get_question_filename(Level level) {
    switch(level) {
        case BEGINNER: return "questions_beginner.txt";
        case INTERMEDIATE: return "questions_intermediate.txt";
        case ADVANCED: return "questions_advanced.txt";
        default: return NULL;
    }
}

int load_questions(const char* filename, Question questions[], int max_questions) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return 0;
    }

    int count = 0;
    char line[1000];
    char type[10];

    while (count < max_questions && fgets(line, sizeof(line), file)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        // Skip empty lines
        if (strlen(line) == 0) continue;

        // Read question type
        if (sscanf(line, "TYPE:%s", type) == 1) {
            if (strcmp(type, "MC") == 0) {
                questions[count].type = MULTIPLE_CHOICE;
                
                // Read question
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = 0;
                sscanf(line, "QUESTION:%[^\n]", questions[count].data.mc.question);
                
                // Read options A, B, C, D
                for (int i = 0; i < 4; i++) {
                    fgets(line, sizeof(line), file);
                    line[strcspn(line, "\n")] = 0;
                    sscanf(line, "%*c:%[^\n]", questions[count].data.mc.options[i]);
                }
                
                // Read answer
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = 0;
                sscanf(line, "ANSWER:%c", &questions[count].data.mc.correct_answer);
                
                // Read hint
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = 0;
                sscanf(line, "HINT:%[^\n]", questions[count].data.mc.hint);
                
                count++;
            }
            else if (strcmp(type, "NUM") == 0) {
                questions[count].type = NUMERICAL;
                
                // Read question
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = 0;
                sscanf(line, "QUESTION:%[^\n]", questions[count].data.num.question);
                
                // Read answer
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = 0;
                sscanf(line, "ANSWER:%lf", &questions[count].data.num.correct_answer);
                
                // Read tolerance
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = 0;
                sscanf(line, "TOLERANCE:%lf", &questions[count].data.num.tolerance);
                
                // Read hint
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = 0;
                sscanf(line, "HINT:%[^\n]", questions[count].data.num.hint);
                
                count++;
            }
        }
    }

    fclose(file);
    return count;
}