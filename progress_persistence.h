#ifndef PROGRESS_PERSISTENCE_H
#define PROGRESS_PERSISTENCE_H

#include "structures.h"

#define PROGRESS_FILE "user_progress.txt"

// Saved progress structure (extends StudentProgress with more info)
typedef struct {
    Level current_level;
    int retry_count[3];           // Retry count for each level (0=Beginner, 1=Inter, 2=Adv)
    int level_completed[3];       // 1 if completed, 0 if not
    int best_score[3];            // Best score achieved for each level
    int hint_mode[3];             // Whether hints are enabled for each level
} SavedProgress;

// Load progress from file
// Returns 1 if file exists and loaded successfully, 0 otherwise
int load_progress(SavedProgress* progress);

// Save progress to file
// Returns 1 if saved successfully, 0 otherwise
int save_progress(SavedProgress* progress);

// Reset all progress
void reset_all_progress(SavedProgress* progress);

// Reset progress for a specific level
void reset_level_progress(SavedProgress* progress, Level level);

// Check if any progress exists
int progress_exists();

#endif