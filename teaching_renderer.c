#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "teaching_renderer.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void display_section(TeachingSection* section) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════╗\n");
    printf("  📚 %s\n", section->section_title);
    printf("╚═══════════════════════════════════════════════════════╝\n\n");
    
    printf("📖 %s\n\n", section->content);
    
    printf("💡 EXAMPLE: %s\n\n", section->example);
    
    printf("📝 SOLUTION STEPS:\n");
    for (int i = 0; i < section->num_steps; i++) {
        printf("   Step %d: %s\n", i + 1, section->steps[i]);
    }
    printf("\n");
}

void display_teaching_content(TeachingSection sections[], int num_sections, Level level) {
    const char* level_name;
    switch(level) {
        case BEGINNER: level_name = "BEGINNER"; break;
        case INTERMEDIATE: level_name = "INTERMEDIATE"; break;
        case ADVANCED: level_name = "ADVANCED"; break;
    }
    
    printf("\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("          %s LEVEL - TEACHING MODE\n", level_name);
    printf("═══════════════════════════════════════════════════════\n");
    printf("We'll go through %d sections to help you learn.\n", num_sections);
    printf("Press ENTER to continue, or type 'skip' to skip ahead.\n");
    printf("═══════════════════════════════════════════════════════\n");

    for (int i = 0; i < num_sections; i++) {
        display_section(&sections[i]);
        
        if (i < num_sections - 1) {
            printf("─────────────────────────────────────────────────────\n");
            printf("Section %d of %d complete.\n", i + 1, num_sections);
            printf("Press ENTER to continue to next section, or type 'skip' to finish: ");
            
            char input[100];
            fgets(input, sizeof(input), stdin);
            
            if (strncmp(input, "skip", 4) == 0) {
                printf("\nSkipping remaining sections...\n");
                break;
            }
        }
    }
    
    printf("\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("            TEACHING COMPLETE!\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("You're now ready to take the test again.\n");
    printf("Press ENTER to continue...");
    getchar();
}