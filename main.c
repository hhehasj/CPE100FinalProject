#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "question_loader.h"
#include "teaching_loader.h"
#include "quiz_engine.h"
#include "teaching_renderer.h"
#include "level_rules.h"
#include "progress_persistence.h"

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/* * A-STYLE VISUALS:
 * Reverted to simple ASCII borders (=) and removed emojis.
 * Removed the internal input loop because Code B's main() handles the pause.
 */
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
    printf("NOTE: Your progress is automatically saved after each question.\n");
    printf("\n");
}

/* * A-STYLE VISUALS:
 * Reverted to simple ASCII borders (=) and removed emojis.
 */
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

/* * HYBRID MENU:
 * Logic from Code B, Visuals adapted to match Code A.
 * Replaced '═' with '=' and '✓' with '[X]'.
 */
void show_progress_status(SavedProgress* saved_progress) {
    printf("\n");
    printf("=======================================================\n");
    printf("                     YOUR PROGRESS\n");
    printf("=======================================================\n");
    
    const char* level_names[] = {"Beginner", "Intermediate", "Advanced"};
    
    for (int i = 0; i < 3; i++) {
        printf("%s Level: ", level_names[i]);
        
        if (saved_progress->level_completed[i]) {
            printf("[X] COMPLETED (Best: %d/5)\n", saved_progress->best_score[i]);
        } else if (saved_progress->retry_count[i] > 0) {
            printf("In Progress (Attempts: %d, Best: %d/5)\n", 
                   saved_progress->retry_count[i], 
                   saved_progress->best_score[i]);
        } else {
            printf("Not Started\n");
        }
    }
    
    printf("=======================================================\n");
}

/* * HYBRID MENU:
 * Logic from Code B, Visuals adapted to match Code A.
 */
void show_reset_menu(SavedProgress* saved_progress) {
    printf("\n");
    printf("=======================================================\n");
    printf("                    RESET PROGRESS\n");
    printf("=======================================================\n");
    printf("1. Reset Beginner level only\n");
    printf("2. Reset Intermediate level only\n");
    printf("3. Reset Advanced level only\n");
    printf("4. Reset ALL progress\n");
    printf("5. Cancel (go back)\n");
    printf("=======================================================\n");
    printf("Your choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    switch(choice) {
        case 1:
            reset_level_progress(saved_progress, BEGINNER);
            break;
        case 2:
            reset_level_progress(saved_progress, INTERMEDIATE);
            break;
        case 3:
            reset_level_progress(saved_progress, ADVANCED);
            break;
        case 4:
            printf("\nAre you sure? This will delete ALL progress! (y/n): ");
            char confirm;
            scanf(" %c", &confirm);
            if (confirm == 'y' || confirm == 'Y') {
                reset_all_progress(saved_progress);
            } else {
                printf("Reset cancelled.\n");
            }
            break;
        case 5:
            printf("Cancelled.\n");
            break;
        default:
            printf("Invalid choice.\n");
    }
    
    printf("Press ENTER to continue...");
    getchar();
    getchar();
}

/* * HYBRID MENU:
 * Logic from Code B, Visuals adapted to match Code A.
 */
int show_start_menu(SavedProgress* saved_progress) {
    printf("\n");
    printf("=======================================================\n");
    printf("                       MAIN MENU\n");
    printf("=======================================================\n");
    printf("1. Continue from where you left off\n");
    printf("2. Start new (current progress will be lost)\n");
    printf("3. View progress\n");
    printf("4. Reset progress\n");
    printf("5. Exit\n");
    printf("=======================================================\n");
    printf("Your choice: ");
    
    int choice;
    scanf("%d", &choice);
    getchar(); // Clear buffer
    
    switch(choice) {
        case 1:
            return 1; // Continue
        case 2:
            printf("\nAre you sure? Current progress will be lost! (y/n): ");
            char confirm;
            scanf(" %c", &confirm);
            getchar();
            if (confirm == 'y' || confirm == 'Y') {
                reset_all_progress(saved_progress);
                return 1; // Start new
            }
            return show_start_menu(saved_progress); // Ask again
        case 3:
            show_progress_status(saved_progress);
            printf("Press ENTER to continue...");
            getchar();
            return show_start_menu(saved_progress); // Back to menu
        case 4:
            show_reset_menu(saved_progress);
            return show_start_menu(saved_progress); // Back to menu
        case 5:
            return 0; // Exit
        default:
            printf("Invalid choice. Please try again.\n");
            return show_start_menu(saved_progress);
    }
}

/* * CODE B LOGIC:
 * Handles persistence, but prints messages in A-style (no emojis).
 */
int handle_level(StudentProgress* progress, SavedProgress* saved_progress) {
    Question questions[MAX_QUESTIONS];
    TeachingSection teaching[10];
    
    const char* question_file = get_question_filename(progress->current_level);
    const char* teaching_file = get_teaching_filename(progress->current_level);
    
    int num_questions = load_questions(question_file, questions, 5);
    
    if (num_questions == 0) {
        printf("Error: Could not load questions for %s level.\n", 
               get_level_name(progress->current_level));
        return -1;
    }
    
    // Restore retry count and hint mode from saved progress
    int level_index = progress->current_level;
    progress->retry_count = saved_progress->retry_count[level_index];
    progress->hint_mode = saved_progress->hint_mode[level_index];
    
    while (1) {
        // Run the quiz with auto-save enabled
        int score = run_quiz(questions, num_questions, progress);
        
        // Update best score
        if (score > saved_progress->best_score[level_index]) {
            saved_progress->best_score[level_index] = score;
        }
        
        // Save progress after quiz (auto-save)
        saved_progress->retry_count[level_index] = progress->retry_count;
        saved_progress->hint_mode[level_index] = progress->hint_mode;
        save_progress(saved_progress);
        
        // Check if passed
        if (check_pass(score, num_questions, progress->current_level)) {
            // Replaced emoji with standard text
            printf("\nCongratulations! You passed the %s level!\n", 
                   get_level_name(progress->current_level));
            
            // Mark level as completed
            saved_progress->level_completed[level_index] = 1;
            
            // Move to next level
            if (progress->current_level < ADVANCED) {
                saved_progress->current_level = progress->current_level + 1;
            }
            
            // Save completion
            save_progress(saved_progress);
            
            return 1; // Passed
        }
        
        // Failed
        progress->retry_count++;
        saved_progress->retry_count[level_index] = progress->retry_count;
        
        printf("\n");
        printf("You need %d/%d to pass this level.\n", 
               get_passing_score(progress->current_level), num_questions);
        
        if (progress->retry_count >= 2) {
            // After 2 failures, enable hint mode
            // Replaced emoji with standard text
            printf("\nDon't worry! We'll help you with hints from now on.\n");
            progress->hint_mode = 1;
            saved_progress->hint_mode[level_index] = 1;
            save_progress(saved_progress);
            
            printf("Let's review the teaching material first.\n");
            printf("Press ENTER to continue...");
            getchar();
            getchar(); // Extra getchar for buffer
            
            // Load and display teaching content
            int num_sections = load_teaching_content(teaching_file, teaching, 10);
            if (num_sections > 0) {
                display_teaching_content(teaching, num_sections, progress->current_level);
            }
            
            printf("\nYou can now retry with hints enabled!\n");
            printf("Press ENTER to retry the test...");
            getchar();
            
        } else {
            // First failure - offer teaching
            printf("\nWould you like to:\n");
            printf("  1. Review teaching material\n");
            printf("  2. Retry the test immediately\n");
            printf("\nYour choice (1 or 2): ");
            
            int choice;
            scanf("%d", &choice);
            
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
        
        // Save progress before retrying
        save_progress(saved_progress);
    }
}

int main() {
    SavedProgress saved_progress;
    
    // Check if progress file exists (Code B Logic)
    if (progress_exists()) {
        load_progress(&saved_progress);
        
        // Replaced emoji with standard text
        printf("\n[Saved progress found!]\n");
        
        // Show menu
        int menu_choice = show_start_menu(&saved_progress);
        if (menu_choice == 0) {
            printf("\nGoodbye!\n");
            return 0;
        }
    } else {
        // No saved progress - initialize new
        reset_all_progress(&saved_progress);
    }
    
    StudentProgress progress = {
        .current_level = saved_progress.current_level,
        .current_score = 0,
        .retry_count = 0,
        .total_questions = 5,
        .hint_mode = 0
    };
    
    print_welcome();
    
    // Manual input wait to match Code A's behavior but fit Code B's logic
    int valid = 0;
    while (!valid) {
        printf("Press ENTER to begin...");
        int c = getchar();
        if (c == '\n') {
            valid = 1;
        } else {
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Please press only ENTER (no other keys).\n\n");
        }
    }
    
    // Check if all levels are already completed
    if (saved_progress.level_completed[0] && 
        saved_progress.level_completed[1] && 
        saved_progress.level_completed[2]) {
        print_congratulations();
        printf("\nYou've already completed all levels!\n");
        printf("You can reset progress from the main menu to try again.\n");
        return 0;
    }
    
    // Process each level starting from current level
    for (int level = saved_progress.current_level; level <= ADVANCED; level++) {
        // Skip if already completed
        if (saved_progress.level_completed[level]) {
            printf("\n%s level already completed! Moving to next level...\n", 
                   get_level_name(level));
            continue;
        }
        
        progress.current_level = level;
        
        // Code A Style Header
        printf("\n");
        printf("===================================================\n");
        printf("           Starting %s Level\n", get_level_name(level));
        printf("===================================================\n");
        
        printf("Press ENTER to begin the test...");
        getchar();
        
        int result = handle_level(&progress, &saved_progress);
        
        if (result < 0) {
            printf("An error occurred. Exiting program.\n");
            return 1;
        }
    }
    
    // All levels completed!
    print_congratulations();
    
    return 0;
}