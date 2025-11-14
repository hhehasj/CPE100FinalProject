#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "quiz_engine.h"

int validate_mc_answer(char user_answer, char correct_answer) {
    return toupper(user_answer) == toupper(correct_answer);
}

int validate_num_answer(double user_answer, double correct_answer, double tolerance) {
    return fabs(user_answer - correct_answer) <= tolerance;
}

int ask_question(Question* q, int show_hint) {
    if (q->type == MULTIPLE_CHOICE) {
        printf("\n%s\n", q->data.mc.question);
        printf("A: %s\n", q->data.mc.options[0]);
        printf("B: %s\n", q->data.mc.options[1]);
        printf("C: %s\n", q->data.mc.options[2]);
        printf("D: %s\n", q->data.mc.options[3]);
        
        if (show_hint) {
            printf("\n💡 HINT: %s\n", q->data.mc.hint);
        }
        
        char answer;
        printf("\nYour answer (A/B/C/D): ");
        scanf(" %c", &answer);
        
        return validate_mc_answer(answer, q->data.mc.correct_answer);
    }
    else { // NUMERICAL
        printf("\n%s\n", q->data.num.question);
        printf("(Please round your answer to 2 decimal places)\n");
        
        if (show_hint) {
            printf("\n💡 HINT: %s\n", q->data.num.hint);
        }
        
        double answer;
        printf("\nYour answer: ");
        scanf("%lf", &answer);
        
        return validate_num_answer(answer, q->data.num.correct_answer, q->data.num.tolerance);
    }
}

int run_quiz(Question questions[], int num_questions, StudentProgress* progress) {
    int score = 0;
    
    printf("\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("                    QUIZ TIME!\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("You will answer %d questions.\n", num_questions);
    
    // Show passing criteria based on level
    switch(progress->current_level) {
        case BEGINNER:
            printf("Passing score: 5/5 (100%%)\n");
            break;
        case INTERMEDIATE:
            printf("Passing score: 4/5 (80%%)\n");
            break;
        case ADVANCED:
            printf("Passing score: 3/5 (60%%)\n");
            break;
    }
    
    if (progress->hint_mode) {
        printf("🌟 HINT MODE ACTIVATED - Hints will be shown for each question\n");
    }
    
    printf("═══════════════════════════════════════════════════════\n");

    for (int i = 0; i < num_questions; i++) {
        printf("\n--- Question %d of %d ---\n", i + 1, num_questions);
        
        int correct = ask_question(&questions[i], progress->hint_mode);
        
        if (correct) {
            printf("✓ Correct!\n");
            score++;
        } else {
            printf("✗ Incorrect.\n");
            if (!progress->hint_mode) {
                // Show correct answer
                if (questions[i].type == MULTIPLE_CHOICE) {
                    printf("The correct answer is: %c\n", questions[i].data.mc.correct_answer);
                } else {
                    printf("The correct answer is: %.2f\n", questions[i].data.num.correct_answer);
                }
            }
        }
    }

    printf("\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("                  QUIZ RESULTS\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("Your score: %d/%d\n", score, num_questions);
    printf("═══════════════════════════════════════════════════════\n");

    return score;
}