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

typedef struct all
{
	question** arr;
	unsigned size, capacity;
}all;

void encryptFile(FILE* file)
{
	char character = "";

	while (character != EOF)
	{
		character = fgetc(file);
		if (character == 13)
			continue;
		if (character == -1)
			break;
		fseek(file, -1, SEEK_CUR);
		fputc(character + 10, file);
		fseek(file, 0, SEEK_CUR);
	}
}

void decryptFile(FILE* file)
{
	char character = "";

	while (character != EOF)
	{
		character = fgetc(file);
		if (character == 13)
			continue;
		if (character == -1)
			break;
		fseek(file, -1, SEEK_CUR);
		fputc(character - 10, file);
		fseek(file, 0, SEEK_CUR);
	}
}

all* read(char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Failed to open the file: %s\n", filename);
		perror("fopen");
		return NULL;
	}

	char* title = malloc(sizeof(char) * 100);
	int* diff = malloc(sizeof * diff);
	char** ansers = malloc(sizeof * ansers * 4);
	for (int i = 0; i < 4; i++)
		ansers[i] = malloc(sizeof * ansers[i]);
	int* cor = malloc(sizeof * cor);


	all* all_questions = malloc(sizeof(all));
	all_questions->arr = malloc(sizeof * all_questions->arr);
	all_questions->capacity = 10;
	all_questions->size = 0;

	for (; fscanf(file, "%[^\r]\r%d\r%[^\r]\r%[^\r]\r%[^\r]\r%[^\r]\r%d\r", title, diff, ansers[0], ansers[1], ansers[2], ansers[3], cor) == 7;)
	{
		question* _new = malloc(sizeof * _new);
		_new->text = malloc(sizeof(*_new->text) * 50);
		strcpy(_new->text, title);
		_new->difficulty = malloc(sizeof(int));
		_new->difficulty = *diff;
		_new->answer = malloc(sizeof(int));
		_new->answer = *cor;
		_new->answers[0] = malloc(sizeof * _new->answers[0] * 50);
		_new->answers[1] = malloc(sizeof * _new->answers[1] * 50);
		_new->answers[2] = malloc(sizeof * _new->answers[2] * 50);
		_new->answers[3] = malloc(sizeof * _new->answers[3] * 50);
		strcpy(_new->answers[0], ansers[0]);
		strcpy(_new->answers[1], ansers[1]);
		strcpy(_new->answers[2], ansers[2]);
		strcpy(_new->answers[3], ansers[3]);
		all_questions->arr[all_questions->size++] = _new;
	}
	return all_questions;
}

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

void addQuestion(question** questions, size_t questions_num)
{
	FILE* file = fopen("the_text.txt", "a");
	if (file == NULL)
	{
		printf("Error opening file for writing!\n");
		return;
	}
	questions_num++;
	*questions = (question*)realloc(*questions, questions_num * sizeof(question));
	if (*questions == NULL)
	{
		printf("Error with the memory allocation!\n");
		questions_num--;
		return;
	}
	(*questions)[questions_num - 1].text = (char*)malloc(100 * sizeof(char));
	if ((*questions)[questions_num - 1].text == NULL)
	{
		printf("Error with the memory allocation!\n");
		questions_num--;
		return;
	}
	for (int i = 0; i < 4; i++)
	{
		(*questions)[questions_num - 1].answers[i] = (char*)malloc(50 * sizeof(char));
		if ((*questions)[questions_num - 1].answers[i] == NULL) {
			printf("Error with memory allocation for answers!\n");
			return;
		}
	}
	printf("Write a question: ");
	fgets((* questions)[questions_num - 1].text, sizeof((* questions)[questions_num - 1].text), stdin);
	getchar();
		
	printf("Difficulty (from 1 to 10): ");
	scanf(" %*[^\n]\n %d", &(*questions)[questions_num - 1].difficulty);
	getchar();

	printf("Answer 1: ");
	fgets((*questions)[questions_num - 1].answers[0], sizeof((*questions)[questions_num - 1].answers[0]), stdin);
	getchar();

	printf("Answer 2: ");
	fgets((*questions)[questions_num - 1].answers[1], sizeof((*questions)[questions_num - 1].answers[1]), stdin);
	getchar();

	printf("Answer 3: ");
	fgets((*questions)[questions_num - 1].answers[2], sizeof((*questions)[questions_num - 1].answers[2]), stdin);
	getchar();

	printf("Answer 4: ");
	fgets((*questions)[questions_num - 1].answers[3], sizeof((*questions)[questions_num - 1].answers[3]), stdin);
	getchar();

	printf("The right answer (1-4): ");
	scanf("%d", &(* questions)[questions_num - 1].answer);
	getchar();

	printf("The question is successfully added\n");
	fputc('\n', file);
	fprintf(file,"%s", (*questions)[questions_num - 1].text);
	fprintf(file,"%d", (*questions)[questions_num - 1].difficulty);
	for (int i = 0; i < 4; i++)
	{
		fprintf(file, "%s", (*questions)[questions_num - 1].answers[i]);
	}
	fprintf(file, "%d", (*questions)[questions_num - 1].answer);
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

int inQuestions(question** questions, question *q, unsigned total) {
	for (int i = 0; i < total; i++) {
		if (questions[i] == q)
			return 1;
	}
	return 0;
}

void generateRandomQuestions(question** questions, all* allQuestions)
{
	for (int i = 0; i < 10; i++) {
		question *q = allQuestions->arr[rand() % allQuestions->size];
		if (inQuestions(questions, q, i))
		{
			i--;
			continue;
		}
		questions[i] = q;
	}
}

size_t getEasiestQuestion(question** questions, size_t size)
{
	size_t result = 0;
	for (int i = 0, minDifficulty = 10; i < size; i++)
		if (questions[i] != NULL && minDifficulty > questions[i]->difficulty)
		{
			minDifficulty = questions[i]->difficulty;
			result = i;
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

void printHints(const char** hints)
{
	printf("1. %s	", hints[0]);
	printf("2. %s	", hints[1]);
	printf("3. %s \n", hints[2]);
}

unsigned help(static char buffer, question* question)
{
	switch (buffer)
	{
	case '1':
	{
		unsigned i = 0, j = 0;
		while ((i == question->answer-1 || j == question->answer-1) || i == j)
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

void game(static char buffer, all* allQuestions)
{
	question** questions=malloc(sizeof * questions *10);
	generateRandomQuestions(questions, allQuestions);
	char* hints[3] = { "50/50", "Help from friend", "Help from audience" };
	int i = -1;

	for (int j = 1; j < 10; )
	{
		if (i == -1)
			i = getEasiestQuestion(questions, allQuestions->size);

		printQuestion(questions[i]);
		printHints(hints);
		char choice = 'A';
		getchar();
		scanf(" %c", &choice);
		if ('A' <= choice && choice <= 'D')
		{
			if (choice - 'A'+1 != questions[i]->answer)
			{
				printf(" Wrong answer!\n");
				return;
			}
			printf(" Correct answer!\n");
			questions[i] = NULL;  i = -1; j++;
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
}

void Menu(const char * file_name)
{
	static char buffer;
	all* allQuestions = read(file_name);



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
			addQuestion(&allQuestions, allQuestions->size);
			break;
		}

		case '3':
		{
			editQuestion(allQuestions, allQuestions->size);
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

void main()
{
	FILE* file;

	file = fopen("the_text.txt", "r+");
	decryptFile(file);
	fclose(file);
}
