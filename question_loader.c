#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
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

// Fisher-Yates shuffle algorithm
void shuffle_array(int* array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

int select_random_questions(Question all_questions[], int total_questions, 
                           Question selected_questions[], int num_to_select,
                           StudentProgress* progress) {
    // If we don't have enough questions, return error
    if (total_questions < num_to_select) {
        printf("Warning: Not enough questions in pool (%d available, %d needed)\n", 
               total_questions, num_to_select);
        return 0;
    }
    
    // Create array of available question indices
    int available[MAX_QUESTIONS];
    int num_available = 0;
    
    for (int i = 0; i < total_questions; i++) {
        int is_used = 0;
        for (int j = 0; j < progress->num_used; j++) {
            if (progress->used_questions[j] == i) {
                is_used = 1;
                break;
            }
        }
        if (!is_used) {
            available[num_available++] = i;
        }
    }
    
    // If we've used all questions, reset the pool
    if (num_available < num_to_select) {
        printf("\nYou've seen all available questions! Resetting question pool...\n");
        num_available = 0;
        for (int i = 0; i < total_questions; i++) {
            available[num_available++] = i;
        }
        progress->num_used = 0;
    }
    
    // Shuffle available questions
    shuffle_array(available, num_available);
    
    // Select first num_to_select questions
    for (int i = 0; i < num_to_select; i++) {
        int selected_index = available[i];
        selected_questions[i] = all_questions[selected_index];
        
        // Mark as used
        progress->used_questions[progress->num_used++] = selected_index;
    }
    
    return num_to_select;
}