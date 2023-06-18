#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct question 
{
	unsigned difficulty, answer;
	char* answers[4];
	char* text;
}question;

void writeQuestionsToFile(char** questions, size_t* questions_num, const char* file_name)
{
	FILE* file = fopen(file_name, "w");
	if (file == NULL)
	{
		printf("Error with opening the file!\n");
		return;
	}
	for (int i = 0; i < questions_num; i++)
	{
		fprintf(file, "%s\n", questions[i]);
	}
	fclose(file);
}

char** saveQuestionFromFile(const char* file_name, size_t* questions_num)
{
	FILE* file = fopen(file_name, "r");
	if (file == NULL)
	{
		printf("Error with opening the file!\n");
		return NULL;
	}
	*questions_num = 0;
	char c;
	while (c = fgetc(file) != EOF)
	{
		if (c == '?') (*questions_num)++;
	}
	fseek(file, 0, SEEK_SET);
	char** questions = (char**)malloc((*questions_num) * sizeof(char*));
	if (questions == NULL)
	{
		printf("Error with the memeory allocation!");
		fclose(file);
		return NULL;
	}

	size_t buffer_size = 256;
	char* buffer = (char*)malloc(buffer_size * sizeof(char));
	if (buffer == NULL) {
		printf("Error with the memory allocation!\n");
		fclose(file);
		free(questions);
		return NULL;
	}
	for (int i = 0; fgetc(buffer, buffer_size, file); i++)
	{
		size_t len = strlen(buffer);
		questions[i] = (char*)malloc((len + 1) * sizeof(char));
		if (questions[i] == NULL)
		{
			printf("Error with the memory allocation!\n");
			fclose(file);
			free(buffer);
			for (int j = 0; j < i; j++)
			{
				free(questions[j]);
			}
			free(questions);
			return NULL;
		}
		strcpy(questions[i], buffer);
	}
	fclose(file);
	free(buffer);
}

void addQuestion(question* questions, size_t* questions_num)
{
	(*questions_num)++;
	questions = (question*)realloc(questions, (*questions_num) * sizeof(question));
	if (questions == NULL) 
	{
		printf("Error with the memory allocation!\n");
		(*questions_num)--;
		return;
	}

	int size = sizeof(questions[*questions_num - 1].answers[0]);
	printf("Write a question: ");
	fgets(questions[*questions_num - 1].text, sizeof(questions[*questions_num - 1].text), stdin);

	printf("Difficulty (from 1 to 10): ");
	scanf("%d", &questions[*questions_num - 1].difficulty);
	getchar();

	printf("Answer 1: ");
	fgets(questions[*questions_num - 1].answers[0], size, stdin);

	printf("Answer 2: ");
	fgets(questions[*questions_num - 1].answers[1], size, stdin);

	printf("Answer 3: ");
	fgets(questions[*questions_num - 1].answers[2], size, stdin);

	printf("Answer 4: ");
	fgets(questions[*questions_num - 1].answers[3], size, stdin);

	printf("The right answer (1-4): ");
	scanf("%d", &questions[*questions_num - 1].answer);
	getchar();

	printf("The question is successfully added\n");
}

void editQuestion(question* questions, size_t* questions_num)
{
	printf("Choose the question that you want to edit (1-%d): ", *questions_num);
	int chosen_question;
	scanf("%d", &chosen_question);
	getchar();
	if (chosen_question < 1 || chosen_question >(*questions_num))
	{
		printf("Invalid number of question!\n");
		return;
	}
	for (int i = chosen_question - 1; i < *questions_num - 1; i++)
	{
		questions[i] = questions[i + 1];
	}
	(*questions_num)--;
	addQuestion(chosen_question, questions_num);
}

void printMenu()
{
	printf("Chose an option!\n");
	printf("1. Start the game\n");
	printf("2. Add a question\n");
	printf("3. Edit a question\n");
	printf("4. Quit\n");
}

void generateRandomQuestions(question** questions, question** allQuestions, size_t size)
{
	for (int i = 0; i < 10; i++)
		questions[i] = allQuestions[rand() % size];
}

question* getEasiestQuestion(question** questions, size_t size)
{
	question* result = NULL;
	for (int i = 0, minDifficulty = 0; i < size; i++)
		if (questions[i] != NULL && minDifficulty > questions[i]->difficulty)
		{
			minDifficulty = questions[i]->difficulty;
			result = questions[i];
		}
	return result;
}

void printQuestion(question* question)
{
	printf("%s\n", question->text);
	printf("A) %s	", question->answers[0]);
	printf("	B) %s\n", question->answers[1]);
	printf("C) %s	", question->answers[2]);
	printf("	D) %s\n", question->answers[3]);
}

void printHints(const char* hints)
{
	printf("1. %s	", hints[0]);
	printf("2. %s	", hints[1]);
	printf("3. %s", hints[2]);
}

unsigned help(static char buffer, question* question)
{
	switch (buffer)
	{
		case '1':
		{
			unsigned i = 0, j = 0;
			while (i != j && i != question->answer && j != question->answer)
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
			unsigned i = rand() % 100;

			if (question->difficulty < 5)
			{
				if (i < 80) return question->answer - 1;
				else 
				{
					while (i != question->answer) i = rand() % 4;
					return i;
				}
			}

			if (4 < question->difficulty && question->difficulty < 8)
			{
				if (i < 60) return question->answer - 1;
				else
				{
					while (i != question->answer) i = rand() % 4;
					return i;
				}
			}

			if (7 < question->difficulty)
			{
				if (i < 30) return question->answer - 1;
				else
				{
					while (i != question->answer) i = rand() % 4;
					return i;
				}
			}
		}

		case '3':
		{
			unsigned i = rand() % 100;

			if (question->difficulty < 5)
			{
				if (i < 80) return question->answer - 1;
				else
				{
					while (i != question->answer) i = rand() % 4;
					return i;
				}
			}

			if (4 < question->difficulty && question->difficulty < 8)
			{
				if (i < 60) return question->answer - 1;
				else
				{
					while (i != question->answer) i = rand() % 4;
					return i;
				}
			}

			if (7 < question->difficulty)
			{
				if (i < 30) return question->answer - 1;
				else
				{
					while (i != question->answer) i = rand() % 4;
					return i;
				}
			}
		}
	}
}

void game(static char buffer, question* allQuestions, size_t size)
{
	question* questions[10];
	generateRandomQuestions(questions, allQuestions, size);
	char* hints[3] = { "50/50", "Help from friend", "Help from audience" };
	question* currentQuestion = NULL;

	while (true)
	{
		if (currentQuestion == NULL)
			currentQuestion = getEasiestQuestion(questions, size);

		printQuestion(currentQuestion);
		printHints(hints);

		scanf("%c", &buffer);

		if ('A' <= buffer && buffer <= 'D')
		{
			if (buffer - 'A' != currentQuestion->answer) return;
			currentQuestion = NULL;
			continue;
		}
		
		if ('1' <= buffer && buffer <= '3' && hints[buffer-'1'] != "")
		{
			help(buffer, currentQuestion);
			hints[buffer - '1'] = "";
			continue;
		}
		printf("Invalid input!");
	}
}

void Menu()
{
	static char buffer;
	question* allQuestions = NULL;
	size_t size = 0;

	while (true)
	{
		printMenu();
		scanf("%c", &buffer);

		switch (buffer)
		{
			case '1':
			{
				game(buffer, allQuestions, size);
				break;
			}
		
			case '2':
			{
				addQuestion(allQuestions, size);
				break;
			}

			case '3':
			{
				editQuestion(allQuestions, size);
				break;
			}

			case '4':
			{
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

void encryptFile(FILE* file)
{
	int character = 0;

	while (!feof(file))
	{
		character = fgetc(file);
		if (character == -1)
			break;
		fseek(file, -1, SEEK_CUR);
		fputc(character + 10, file);
		fseek(file, 0, SEEK_CUR);
	}
}

void decryptFile(FILE* file)
{
	int character = 0;

	while (!feof(file))
	{
		character = fgetc(file);
		if (character == -1)
			break;
		fseek(file, -1, SEEK_CUR);
		fputc(character - 10, file);
		fseek(file, 0, SEEK_CUR);
	}
}

void main()
{
	srand(time(NULL));
	Menu();

	FILE* file;

	file = fopen("the_text.txt", "r+");
	encryptFile(file);
	fclose(file);

	file = fopen("the_text.txt", "r+");
	decryptFile(file);
	fclose(file);
}