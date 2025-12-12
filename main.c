#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structures.h"
#include "question_loader.h"
#include "teaching_loader.h"
#include "quiz_engine.h"
#include "teaching_renderer.h"
#include "level_rules.h"


void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


void print_welcome() {
    printf("\n");
    printf("===========================================================\n");
    printf("=                                                         =\n");
    printf("=          WELCOME TO CALCULUS 1 LEARNING SYSTEM          =\n");
    printf("=                                                         =\n");
    printf("===========================================================\n");
    printf("\n");
    printf("This program will help you learn Calculus 1 step by step!\n");
    printf("\n");
    printf("How it works:\n");
    printf("  1. You'll start with a Beginner test (5 questions)\n");
    printf("  2. If you pass, you move to Intermediate level\n");
    printf("  3. If you don't pass, we'll teach you and you can retry\n");
    printf("  4. After 2 failed attempts, hints will be provided\n");
    printf("  5. Complete all 3 levels to finish!\n");
    printf("\n");
    printf("Passing criteria:\n");
    printf("  - Beginner: 5/5 (100%%)\n");
    printf("  - Intermediate: 4/5 (80%%)\n");
    printf("  - Advanced: 3/5 (60%%)\n");
    printf("\n");

    int valid = 0;
    while (!valid) {
        printf("Press ENTER to begin...");
        
        int c = getchar();
        
        if (c == '\n') {
            valid = 1;

        } else {
            // User typed something before Enter - clear the rest and re-prompt
            while ((c = getchar()) != '\n' && c != EOF);  // Clear the buffer
            printf("Please press only ENTER.\n\n");
        }
    }
}


void print_congratulations() {
    printf("\n");
    printf("===========================================================\n");
    printf("=                                                         =\n");
    printf("=                    CONGRATULATIONS!                     =\n");
    printf("=                                                         =\n");
    printf("=      You have completed all levels of Calculus 1!       =\n");
    printf("=                                                         =\n");
    printf("===========================================================\n");
    printf("\n");
    printf("You've mastered:\n");
    printf("-- Beginner concepts\n");
    printf("-- Intermediate techniques\n");
    printf("-- Advanced applications\n");
    printf("\n");
    printf("Keep practicing and good luck with your studies!\n");
    printf("\n");
}


int handle_level(StudentProgress* progress) {
    Question all_questions[MAX_QUESTIONS];
    int questions_to_show = 5;
    Question selected_questions[questions_to_show];
    TeachingSection teaching[10];
    
    const char* question_file = get_question_filename(progress->current_level);
    const char* teaching_file = get_teaching_filename(progress->current_level);
    
    // Load all available questions for this level
    int total_questions = load_questions(question_file, all_questions, MAX_QUESTIONS);
    
    if (total_questions == 0) {
        printf("Error: Could not load questions for %s level.\n", 
               get_level_name(progress->current_level));
        return -1;
    }
    
    printf("Loaded %d questions for this level.\n", total_questions);
    
    while (1) {
        int num_selected;
        
        // First 5 questions will be displayed until user retries twice. This happens in all levels. 
        if (progress->retry_count < 2) {

            num_selected = (total_questions >= questions_to_show) ? questions_to_show : total_questions;
            for (int i = 0; i < num_selected; i++) {
                selected_questions[i] = all_questions[i];
            }

        } else {

            num_selected = select_random_questions(all_questions, total_questions, 
                                                   selected_questions, questions_to_show, progress);

        }
        
        if (num_selected == 0) {
            printf("Error: Could not select questions.\n");
            return -1;

        }
        

        int score = run_quiz(selected_questions, questions_to_show, progress);
        

        // Check if passed
        if (check_pass(score, questions_to_show, progress->current_level)) {

            printf("\nCongratulations! You passed the %s level!\n", 
                   get_level_name(progress->current_level));
            return 1; // Passed

        }
        
        // Failed
        progress->retry_count++;
        
        printf("\n");
        printf("You need %d/%d to pass this level.\n", 
               get_passing_score(progress->current_level), 5);
        
        if (progress->retry_count >= 2) {
            // After 2 failures, enable hint mode
            printf("\nDon't worry! We'll help you with hints from now on.\n");
            progress->hint_mode = 1;
            
            printf("Let's review the teaching material first.\n");
            int valid1 = 0;
            while (!valid1) {
                printf("Press ENTER to continue...");
                
                int c = getchar();
                
                if (c == '\n') {
                    valid1 = 1;

                } else {
                    // User typed something before Enter - clear the rest and re-prompt
                    while ((c = getchar()) != '\n' && c != EOF);  // Clear the buffer
                    printf("Please press only ENTER.\n\n");
                }
            }
            getchar(); // Extra getchar for buffer
            
            // Load and display teaching content
            int num_sections = load_teaching_content(teaching_file, teaching, 10);
            if (num_sections > 0) {
                display_teaching_content(teaching, num_sections, progress->current_level);
            }
            
            printf("\nYou can now retry with hints enabled!\n");
            printf("You'll also receive a new set of questions.\n");
            int valid2 = 0;
            while (!valid2) {
                printf("Press ENTER to retry the test...");
                
                int c = getchar();
                
                if (c == '\n') {
                    valid2 = 1;

                } else {
                    // User typed something before Enter - clear the rest and re-prompt
                    while ((c = getchar()) != '\n' && c != EOF);  // Clear the buffer
                    printf("Please press only ENTER.\n\n");
                }
            }
            
        } else {
            // First failure - offer teaching
            printf("\nWould you like to:\n");
            printf("  1. Review teaching material\n");
            printf("  2. Retry the test immediately\n");
            
            int choice;
            int valid_input = 0;
            while(!valid_input) {
                printf("\nYour choice (1 or 2): ");
                scanf("%d", &choice);
                 
                if ( choice == 1 || choice == 2 ) {
                    valid_input = 1;
                } else {
                    printf("Invalid input! Please enter 1 or 2.\n");
                    clear_buffer();
                }
            }
            
            if (choice == 1) {
                int num_sections = load_teaching_content(teaching_file, teaching, 10);
                if (num_sections > 0) {
                    display_teaching_content(teaching, num_sections, progress->current_level);
                }
            }
            
            printf("\nPress ENTER to retry the test...");
            getchar();
            getchar();
        }
    }
}


int main() {
    // Seed random number generator
    srand(time(NULL));
    
    StudentProgress progress = {
        .current_level = BEGINNER,
        .current_score = 0,
        .retry_count = 0,
        .total_questions = 5,
        .hint_mode = 0,
        .num_used = 0
    };
    
    print_welcome();
    
    // Process each level
    for (int level = BEGINNER; level <= ADVANCED; level++) {
        progress.current_level = level;
        progress.retry_count = 0;
        progress.hint_mode = 0;
        progress.num_used = 0;  // Reset used questions for new level
        
        printf("\n");
        printf("===================================================\n");
        printf("           Starting %s Level\n", get_level_name(level));
        printf("===================================================\n");
        int valid = 0;
        while (!valid) {
            printf("Press ENTER to begin the test...");
            
            int c = getchar();
            
            if (c == '\n') {
                valid = 1;

            } else {
                // User typed something before Enter - clear the rest and re-prompt
                while ((c = getchar()) != '\n' && c != EOF);  // Clear the buffer
                printf("Please press only ENTER.\n\n");
            }
        }
        
        int result = handle_level(&progress);
        
        if (result < 0) {
            printf("An error occurred. Exiting program.\n");
            return 1;
        }
    }
    
    // All levels completed!
    print_congratulations();
    
    return 0;
}