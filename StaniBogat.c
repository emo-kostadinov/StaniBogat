#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>


typedef struct question_t
{
    unsigned difficulty, answer;
    char* answers[4];
    char* text;
}question_t;

typedef struct all_t
{
    question_t** arr;
    unsigned size, capacity;
}all_t;

void encryptFile(const char* filename)
{
    FILE* file = fopen(filename, "r+");
    char character = 0;

    while (character != EOF)
    {
        character = fgetc(file);
        if (character == '\r' || character == '\n')
            continue;
        if (character == -1)
            break;
        fseek(file, -1, SEEK_CUR);
        fputc(character + 10, file);
        fseek(file, 0, SEEK_CUR);
    }

    fclose(file);
}

void decryptFile(const char* filename)
{
    FILE* file = fopen(filename, "r+");
    char character = 0;

    while (character != EOF)
    {
        character = fgetc(file);
        if (character == '\r' || character == '\n')
            continue;
        if (character == -1)
            break;
        fseek(file, -1, SEEK_CUR);
        fputc(character - 10, file);
        fseek(file, 0, SEEK_CUR);
    }
    fclose(file);
}

void purgeQuestion(question_t* question)
{
    for (size_t i = 0; i < 4; ++i)
        free(question->answers[i]);

    free(question->text);
    free(question);
}

void purgeAllQuestions(all_t* allQuestion)
{
    for (size_t i = 0; i < allQuestion->size; ++i)
        purgeQuestion(allQuestion->arr[i]);

    free(allQuestion->arr);
    free(allQuestion);
}

void getAnswer(question_t* question, size_t i, char* buffer, FILE* file)
{
    fgets(buffer, 256, file);
    question->answers[i] = (char*)malloc(strlen(buffer) + 1);
    assert(question->answers[i]);
    strncpy(question->answers[i], buffer, strlen(buffer) - 1);
    question->answers[i][strlen(buffer) - 1] = '\0';
}

all_t* readFromFile(const char* fileName)
{
    decryptFile(fileName);
    FILE* file = fopen(fileName, "r");
    char buffer[256] = { 0 };

    all_t* allQuestions = (all_t*)calloc(1, sizeof * allQuestions);
    allQuestions->capacity = 16;
    allQuestions->size = 0;

    allQuestions->arr = (question_t**)calloc(allQuestions->capacity, sizeof * allQuestions->arr[0]);

    size_t i = 0;
    while (feof(file) == 0)
    {
        i++;
        question_t* question = (question_t*)calloc(1, sizeof * question);
        assert(question);

        fgets(buffer, 256, file);

        question->text = (char*)malloc(strlen(buffer) + 1);
        assert(question->text);
        strncpy(question->text, buffer, strlen(buffer) - 1);
        question->text[strlen(buffer) - 1] = '\0';

        fgets(buffer, 256, file);
        question->difficulty = atoi(buffer);

        getAnswer(question, 0, buffer, file);
        getAnswer(question, 1, buffer, file);
        getAnswer(question, 2, buffer, file);
        getAnswer(question, 3, buffer, file);

        fgets(buffer, 256, file);
        question->answer = atoi(buffer);

        if (allQuestions->size >= allQuestions->capacity)
            allQuestions->arr = realloc(allQuestions->arr, allQuestions->capacity *= 2);

        allQuestions->arr[allQuestions->size++] = question;

        fgets(buffer, 256, file); // Remove empty spaces between lines
    }

    encryptFile(fileName);
    return allQuestions;
}


void writeQuestionsToFile(all_t* questions, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error with opening the file!\n");
        return;
    }

    for (int i = 0; i < questions->size - 1; i++)
    {
        fprintf(file, "%s\n", questions->arr[i]->text);
        fprintf(file, "%d\n", questions->arr[i]->difficulty);
        for (int j = 0; j < 4; j++)
            fprintf(file, "%s\n", questions->arr[i]->answers[j]);
        fprintf(file, "%d\n", questions->arr[i]->answer);
        fprintf(file, "\n");
    }
    fprintf(file, "%s\n", questions->arr[questions->size - 1]->text);
    fprintf(file, "%d\n", questions->arr[questions->size - 1]->difficulty);
    for (int j = 0; j < 4; j++)
        fprintf(file, "%s\n", questions->arr[questions->size - 1]->answers[j]);
    fprintf(file, "%d", questions->arr[questions->size - 1]->answer);

    fclose(file);
}

question_t* addQuestion()
{
    char buffer[256] = { 0 };
    while ((getchar()) != '\n'); // Clear stdin (buffer

    question_t* new_question = malloc(sizeof(question_t));
    if (new_question == NULL)
    {
        printf("Error with memory allocation for new question!\n");
        return NULL;
    }

    printf("Write a question: ");
    fgets(buffer, 256, stdin);
    new_question->text = malloc(strlen(buffer) + 1);
    strncpy(new_question->text, buffer, strlen(buffer) - 1);
    new_question->text[strlen(buffer) - 1] = '\0';

    printf("Difficulty (from 1 to 10): ");
    fgets(buffer, 256, stdin);
    new_question->difficulty = atoi(buffer);

    for (unsigned i = 0; i < 4; i++)
    {
        printf("Answer %d: ", i + 1);
        fgets(buffer, 256, stdin);
        new_question->answers[i] = malloc(strlen(buffer) + 1);
        strncpy(new_question->answers[i], buffer, strlen(buffer) - 1);
        new_question->answers[i][strlen(buffer) - 1] = '\0';
    }

    printf("The right answer (1-4): ");
    fgets(buffer, 256, stdin);
    new_question->answer = atoi(buffer);
    if (new_question->answer < 1 || new_question->answer > 4)
    {
        printf("Invalid correct answer!\n");
        return NULL;
    }

    printf("The question is successfully added\n");
    return new_question;
}

void editQuestion(all_t* questions) {
    unsigned chosen_question;

    printf("Choose the question that you want to edit (1-%d): ", questions->size);
    scanf("%d", &chosen_question);
    if (chosen_question < 1 || chosen_question > questions->size) {
        printf("Invalid number of question_t!\n");
        return;
    }

    question_t* edited_question = addQuestion();
    if (edited_question == NULL) {
        printf("Error with adding the question_t!\n");
        return;
    }

    purgeQuestion(questions->arr[chosen_question - 1]);
    questions->arr[chosen_question - 1] = edited_question;
}

void printMenu()
{
    printf("Chose an option!\n");
    printf("1. Start the game\n");
    printf("2. Add a question\n");
    printf("3. Edit a question\n");
    printf("4. Quit\n");
}

unsigned getQuestionsForDifficulty(question_t** availableQuestions, int difficulty, all_t* allQuestions)
{
    unsigned availableQuestionsSize = 0;

    for (unsigned i = 0; i < allQuestions->size; i++)
    {
        if (allQuestions->arr[i]->difficulty == difficulty)
        {
            availableQuestions[availableQuestionsSize] = allQuestions->arr[i];
            availableQuestionsSize++;
        }
    }
    return availableQuestionsSize;
}

void generateRandomQuestions(question_t** questions, all_t* allQuestions)
{
    question_t** availableQuestions = malloc(sizeof(question_t*) * allQuestions->size);

    for (unsigned i = 0; i < 10; i++)
    {
        unsigned availableQuestionsSize = getQuestionsForDifficulty(availableQuestions, i + 1, allQuestions);
        question_t* q = availableQuestions[rand() % availableQuestionsSize];

        questions[i] = malloc(sizeof(question_t)); //Copy question so that the original one is not changed when using hints
        questions[i]->text = malloc(strlen(q->text) + 1);
        strcpy(questions[i]->text, q->text);
        questions[i]->difficulty = q->difficulty;
        for (unsigned j = 0; j < 4; j++)
        {
            questions[i]->answers[j] = malloc(strlen(q->answers[j]) + 1);
            strcpy(questions[i]->answers[j], q->answers[j]);
        }
        questions[i]->answer = q->answer;


        availableQuestions[i] = NULL;
    }

    free(availableQuestions);
}

void printQuestion(question_t* question)
{
    printf("%s\n", question->text);
    printf("A) %s\n", question->answers[0]);
    printf("B) %s\n", question->answers[1]);
    printf("C) %s\n", question->answers[2]);
    printf("D) %s\n", question->answers[3]);
}

void printHints(const char** hints)
{
    printf("1. %s	", hints[0]);
    printf("2. %s	", hints[1]);
    printf("3. %s \n", hints[2]);
}

int chancePercantage(question_t* question, int percentage)
{
    int i = rand() % 100;

    if (i < percentage) return question->answer - 1;
    else
    {
        while (i != question->answer) i = rand() % 4;
        return i;
    }
}

unsigned help(char buffer, question_t* question)
{
    switch (buffer)
    {
    case '1':
    {
        unsigned i = 0, j = 0;
        while ((i == question->answer - 1 || j == question->answer - 1) || i == j)
        {
            i = rand() % 4;
            j = rand() % 4;
        }
        question->answers[i] = "";
        question->answers[j] = "";
        return 4;
    }

    case '2':
    {
        if (question->difficulty < 5) return chancePercantage(question, 80);

        if (4 < question->difficulty && question->difficulty < 8) return chancePercantage(question, 60);

        if (7 < question->difficulty) return chancePercantage(question, 30);
    }

    case '3':
    {
        if (question->difficulty < 5) return chancePercantage(question, 80);

        if (4 < question->difficulty && question->difficulty < 8) return chancePercantage(question, 60);

        if (7 < question->difficulty) return chancePercantage(question, 30);
    }
    }
}

void game(char answer, all_t* allQuestions)
{
    question_t* questions[10] = { NULL };
    generateRandomQuestions(questions, allQuestions);
    char* hints[3] = { "50/50", "Help from friend", "Help from audience" };
    int i = 0;


    while (i < 10)
    {
        printQuestion(questions[i]);
        printHints(hints);
        char choice = 'A';
        getchar();
        scanf(" %c", &choice);
        if ('A' <= choice && choice <= 'D')
        {
            if (choice - 'A' + 1 != questions[i]->answer)
            {
                printf(" Wrong answer!\n");
                return;
            }
            printf(" Correct answer!\n");
            questions[i++] = NULL;
            continue;
        }

        if ('1' <= choice && choice <= '3' && hints[choice - '1'] != "")
        {
            unsigned res = help(choice, questions[i]);
            if (res != 4)
                printf(" Answer: %c\n", res + 'A');
            hints[choice - '1'] = "";
            continue;
        }
        printf("Invalid input!");
    }
    purgeQuestion(questions);

    printf("You won the game!\n");
}


void Menu(const char* filename)
{
    char buffer;
    all_t* allQuestions = readFromFile(filename);

    srand(time(NULL)); //Seed randomness

    while (true)
    {
        printMenu();
        scanf("%c", &buffer);

        switch (buffer)
        {
        case '1':
        {
            game(buffer, allQuestions);
            break;
        }

        case '2':
        {
            question_t* new_question = addQuestion();
            if (new_question == NULL) {
                printf("Error adding question!\n");
                break;
            }

            allQuestions->arr[allQuestions->size++] = new_question;

            if (allQuestions->size >= allQuestions->capacity)
                allQuestions->arr = realloc(allQuestions->arr, allQuestions->capacity * 2 * sizeof(question_t*));

            break;
        }

        case '3':
        {
            editQuestion(allQuestions);
            break;
        }

        case '4':
        {
            writeQuestionsToFile(allQuestions, filename);
            purgeAllQuestions(allQuestions);

            encryptFile(filename);
            return;
        }

        default:
        {
            printf("Invalid input!\n");
            break;
        }
        }
    }
}


int main()
{
    //encryptFile("input.txt");
    Menu("input.txt");
    return 0;
}