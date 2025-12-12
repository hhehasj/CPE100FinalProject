#include <stdio.h>
#include <string.h>
#include "progress_persistence.h"
#include "level_rules.h"  // For get_level_name() function

int progress_exists() {
    FILE* file = fopen(PROGRESS_FILE, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

int load_progress(SavedProgress* progress) {
    FILE* file = fopen(PROGRESS_FILE, "r");
    if (!file) {
        return 0;
    }

    // Read current level
    int level;
    fscanf(file, "CURRENT_LEVEL:%d\n", &level);
    progress->current_level = (Level)level;

    // Read retry counts for each level
    fscanf(file, "RETRY_COUNTS:%d,%d,%d\n", 
           &progress->retry_count[0],
           &progress->retry_count[1],
           &progress->retry_count[2]);

    // Read completion status
    fscanf(file, "COMPLETED:%d,%d,%d\n",
           &progress->level_completed[0],
           &progress->level_completed[1],
           &progress->level_completed[2]);

    // Read best scores
    fscanf(file, "BEST_SCORES:%d,%d,%d\n",
           &progress->best_score[0],
           &progress->best_score[1],
           &progress->best_score[2]);

    // Read hint modes
    fscanf(file, "HINT_MODES:%d,%d,%d\n",
           &progress->hint_mode[0],
           &progress->hint_mode[1],
           &progress->hint_mode[2]);

    fclose(file);
    return 1;
}

int save_progress(SavedProgress* progress) {
    FILE* file = fopen(PROGRESS_FILE, "w");
    if (!file) {
        printf("Error: Could not save progress\n");
        return 0;
    }

    // Write current level
    fprintf(file, "CURRENT_LEVEL:%d\n", progress->current_level);

    // Write retry counts
    fprintf(file, "RETRY_COUNTS:%d,%d,%d\n",
            progress->retry_count[0],
            progress->retry_count[1],
            progress->retry_count[2]);

    // Write completion status
    fprintf(file, "COMPLETED:%d,%d,%d\n",
            progress->level_completed[0],
            progress->level_completed[1],
            progress->level_completed[2]);

    // Write best scores
    fprintf(file, "BEST_SCORES:%d,%d,%d\n",
            progress->best_score[0],
            progress->best_score[1],
            progress->best_score[2]);

    // Write hint modes
    fprintf(file, "HINT_MODES:%d,%d,%d\n",
            progress->hint_mode[0],
            progress->hint_mode[1],
            progress->hint_mode[2]);

    fclose(file);
    return 1;
}

void reset_all_progress(SavedProgress* progress) {
    progress->current_level = BEGINNER;
    
    for (int i = 0; i < 3; i++) {
        progress->retry_count[i] = 0;
        progress->level_completed[i] = 0;
        progress->best_score[i] = 0;
        progress->hint_mode[i] = 0;
    }
    
    // Save the reset progress
    save_progress(progress);
    printf("\nAll progress has been reset!\n");
}

void reset_level_progress(SavedProgress* progress, Level level) {
    progress->retry_count[level] = 0;
    progress->level_completed[level] = 0;
    progress->best_score[level] = 0;
    progress->hint_mode[level] = 0;
    
    // If resetting current or previous level, move back to that level
    if (level <= progress->current_level) {
        progress->current_level = level;
    }
    
    // Save the updated progress
    save_progress(progress);
    printf("\nProgress for %s level has been reset!\n", get_level_name(level));
}