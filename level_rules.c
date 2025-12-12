#include "level_rules.h"

int get_passing_score(Level level) {
    switch(level) {
        case BEGINNER: return 5;      // 5/5
        case INTERMEDIATE: return 4;  // 4/5
        case ADVANCED: return 3;      // 3/5
        default: return 0;
    }
}

int check_pass(int score, int total, Level level) {
    (void)total; // Mark as intentionally unused
    int required = get_passing_score(level);
    return score >= required;
}

const char* get_level_name(Level level) {
    switch(level) {
        case BEGINNER: return "Beginner";
        case INTERMEDIATE: return "Intermediate";
        case ADVANCED: return "Advanced";
        default: return "Unknown";
    }
}