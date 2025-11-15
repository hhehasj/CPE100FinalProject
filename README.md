# Calculus 1 Learning System - CPE100 Group Project

## Project Overview
An interactive C-based learning application for Calculus 1 that guides students through beginner, intermediate, and advanced levels with adaptive testing and teaching materials.

## Features
- ✅ Progressive difficulty system (Beginner → Intermediate → Advanced)
- ✅ Multiple choice and numerical questions
- ✅ 2-retry limit with hint system after failures
- ✅ Step-by-step teaching materials
- ✅ No data persistence (session-based only)
- ✅ Skip option for teaching content

## File Structure

```
project/
├── main.c                      # Main program entry point
├── structures.h                # Core data structures
├── question_loader.c/.h        # Question file loading
├── teaching_loader.c/.h        # Teaching content loading
├── quiz_engine.c/.h            # Quiz logic and validation
├── teaching_renderer.c/.h      # Teaching content display
├── level_rules.c/.h            # Level progression rules
├── questions_beginner.txt      # Beginner level questions
├── questions_intermediate.txt  # Intermediate level questions
├── questions_advanced.txt      # Advanced level questions
├── teaching_beginner.txt       # Beginner teaching content
├── teaching_intermediate.txt   # Intermediate teaching content
├── teaching_advanced.txt       # Advanced teaching content
├── Makefile                    # Build configuration
└── README.md                   # This file
```

## How to Compile

### Using Makefile (Recommended)
```bash
# Compile the project
make

# Compile and run
make run

# Clean build files
make clean

# Rebuild from scratch
make rebuild
```

### Manual Compilation
```bash
gcc main.c question_loader.c teaching_loader.c quiz_engine.c teaching_renderer.c level_rules.c -o CPE100FINALPROJECT -lm -Wall -std=c99
```

## How to Run
```bash
./CPE100FINALPROJECT
```

## Passing Criteria
- **Beginner Level**: 5/5 questions correct (100%)
- **Intermediate Level**: 4/5 questions correct (80%)
- **Advanced Level**: 3/5 questions correct (60%)

## Retry System
1. First attempt: Take the test
2. If failed: Option to review teaching or retry immediately
3. After 2 failures: Hints automatically enabled for all questions
4. Unlimited retries available after hints are activated

## Question File Format

### Multiple Choice Question
```
TYPE:MC
QUESTION:What is the derivative of x^2?
A:2x
B:x^2
C:2
D:x
ANSWER:A
HINT:Use the power rule: d/dx(x^n) = nx^(n-1)
```

### Numerical Question
```
TYPE:NUM
QUESTION:Find the derivative of f(x) = 3x
ANSWER:3
TOLERANCE:0.01
HINT:The derivative of x is 1, multiply by the constant 3
```

## Teaching File Format
```
SECTION:Section Title
CONTENT:Explanation of the concept...
EXAMPLE:Example problem statement
STEP1:First step of solution
STEP2:Second step of solution
STEP3:Third step of solution
---
```

## Adding New Questions

1. Open the appropriate question file (beginner/intermediate/advanced)
2. Add new questions following the format above
3. Make sure to include hints for all questions
4. Leave blank lines between questions for readability

## Adding New Teaching Content

1. Open the appropriate teaching file
2. Add new sections with title, content, example, and steps
3. End each section with `---`
4. You can have up to 5 steps per section

## System Requirements
- GCC compiler with C99 support
- Math library (-lm flag)
- Unix-like system (Linux/Mac) or Windows with MinGW

## Testing the Application

To test if everything works:
1. Compile the project
2. Run the executable
3. Try answering questions correctly and incorrectly
4. Test the teaching material skip functionality
5. Test the hint system by failing twice

## Troubleshooting

### "Cannot open file" error
- Ensure all .txt files are in the same directory as the executable
- Check file permissions

### Compilation errors
- Make sure all .c and .h files are present
- Verify GCC is installed: `gcc --version`

### Math library errors
- Ensure you're using the `-lm` flag when compiling

## Future Enhancements (Optional)
- Add more questions per level
- Add timer functionality
- Add scoring statistics
- Add more calculus topics (integration, limits, etc.)
- Add difficulty scaling within levels

## Team Members
[Add your team member names here]

## Course Information
- Course: CPE100
- Project: Calculus 1 Learning System
- Language: C