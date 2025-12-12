#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_QUESTION_LENGTH 500
#define MAX_OPTION_LENGTH 200
#define MAX_ANSWER_LENGTH 100
#define MAX_HINT_LENGTH 300
#define MAX_QUESTIONS 20

// Question types
typedef enum {
    MULTIPLE_CHOICE,
    NUMERICAL
} QuestionType;

// Difficulty levels
typedef enum {
    BEGINNER,
    INTERMEDIATE,
    ADVANCED
} Level;

// Multiple choice question structure
typedef struct {
    char question[MAX_QUESTION_LENGTH];
    char options[4][MAX_OPTION_LENGTH];  // A, B, C, D
    char correct_answer;  
    char hint[MAX_HINT_LENGTH];
} MCQuestion;

// Numerical question structure
typedef struct {
    char question[MAX_QUESTION_LENGTH];
    double correct_answer;
    double tolerance;  // Usually 0.01
    char hint[MAX_HINT_LENGTH];
} NumQuestion;

// Generic question structure
typedef struct {
    QuestionType type;
    union {
        MCQuestion mc;
        NumQuestion num;
    } data;
} Question;

// Student progress for current session
typedef struct {
    Level current_level;
    int current_score;
    int retry_count;  
    int total_questions;
    int hint_mode;  // 0 = no hints, 1 = hints shown after 2 failures
    int used_questions[MAX_QUESTIONS];  // For tracking which questions have been used
    int num_used;                       // Number of questions already used
} StudentProgress;

// Teaching content structure
typedef struct {
    char section_title[200];
    char content[1000];
    char example[500];
    char steps[5][300];  // Up to 5 steps
    int num_steps;
} TeachingSection;

#endif