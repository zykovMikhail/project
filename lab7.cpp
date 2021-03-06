#include "stdafx.h"
#include "string.h"
#include "windows.h"
#include "vld.h"

char* InputFile(int podmenu);
int Delimeters(char sym);
char* InputChar();
void SortNameU(char** words, int n);
void SortSoglU(char** words, int n);
int Sogl(char* word, int n);
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	char* text = NULL;
	char** words = NULL;
	int menu = 0;
	int n = 0;
	char check = '\0';
	int sort = 0;
	bool ext = false;
	while (ext != true)
	{
		printf("Меню:\n");
		printf("1.Загрузка данных из файла\n");
		printf("2.Обработка данных\n");
		printf("3.Вывод результата на экран\n");
		printf("4.Выход\n");
		printf("Выбор:");
		while (scanf_s("%d%c", &menu, &check, 1) != 2 || check != '\n' || menu > 4 || menu < 1)
		{
			printf("Ошибка!");
			while (getchar() != '\n');
			printf("Введите число от 1 до 4:");
		}
		if (menu == 1)
		{
			if (text != NULL)
			{
				delete[] text;
			}
			int podmenu = 0;
			printf("1.Загрузить из стандартного файла\n");
			printf("2.Загрузить из выбранного файла(Путь необходимо ввести с двойным обратным слэшем)\n");
			printf("Выбор:");
			while (scanf_s("%d%c", &podmenu, &check, 1) != 2 || check != '\n' || podmenu > 2 || podmenu < 1)
			{
				printf("Ошибка!");
				while (getchar() != '\n');
				printf("Введите число от 1 до 2:");
			}
			text = InputFile(podmenu);
		}
		if (menu == 2)
		{
			if (text == NULL)
			{
				printf("Данные отсутствуют!\n");
			}
			else
			{
				if (n > 0)
				{
					for (int i = 0; i < n; i++)
					{
						delete[] words[i];
					}
				}
				n = 0;
				int pos = 0;
				int count = 0;
				int size = strlen(text) + 1;
				for (int i = 0; i < size + 1; i++)
				{
					count++;
					if (Delimeters(text[i]))
					{
						if (count - pos == 1)
						{
							pos = count;
						}
						else
						{
							char** buf = new char*[n + 1];
							for (int j = 0; j < n; j++)
							{
								buf[j] = words[j];
							}
							delete[] words;
							words = buf;
							words[n] = new char[count - pos];
							int cpy = 0;
							for (int j = pos; j < count; j++)
							{
								words[n][cpy] = text[j];
								cpy++;
							}
							words[n][count - pos - 1] = '\0';
							pos = count;
							n++;
						}

					}
				}
			}
		}
		if (menu == 3)
		{
			if (words == NULL)
			{
				printf("Данные не обработаны!\n");
			}
			else
			{
				printf("1.Три самых длинных слова\n");
				printf("2.Слова в порядке возрастания количества согласных букв в них\n");
				printf("Выбор:");
				while (scanf_s("%d%c", &sort, &check, 1) != 2 || check != '\n' || sort > 2 || sort < 1)
				{
					printf("Ошибка!");
					while (getchar() != '\n');
					printf("Введите число от 1 до 2:");
				}
				if (sort == 1)
				{
					SortNameU(words, n);
					for (int i = 0; i < 3; i++)
					{
						printf("%s\n", words[i]);
					}
				}
				else
				{
					SortSoglU(words, n);
					for (int i = 0; i < n; i++)
					{
						printf("%s\n", words[i]);
					}
				}			
				getchar();
			}
		}
		if (menu == 4)
		{
			ext = true;
		}
	}
	
	delete[] text;
	for (int i = 0; i < n; i++)
	{
		delete[] words[i];
	}
	delete[] words; 
    return 0;
}

char* InputFile(int podmenu)
{
	FILE* myfile;
	if (podmenu == 1)
	{
		fopen_s(&myfile, "D:\\mytext.txt", "r");
	}
	else
	{
		printf("Введите путь(c двойным обратным слэшем):\n");
		char* path = InputChar();
		if (path[strlen(path) - 1] != 't' || path[strlen(path) - 2] != 'x' || path[strlen(path) - 3] != 't')
		{
			printf("Неверный тип файла\n");
			delete[] path;
			return 0;
		}
		fopen_s(&myfile, path , "r");
		delete[] path;
	}
	int size = 0;
	if (myfile == NULL)
	{
		printf("Ошибка чтения!");
		getchar();
		return 0;
	}

	fseek(myfile, 0, SEEK_END);
	size = ftell(myfile);
	fseek(myfile, 0, SEEK_SET);
	char* text = new char[size + 1];
	int sym = 0;
	int count = 0;
	int ncoun = 0;
	while ((sym = fgetc(myfile)) != EOF)
	{
		text[count] = sym;
		count++;
	}
	fclose(myfile);
	text[count] = '\0';
	return text;
}
int Delimeters(char sym)
{
	const char delim[10] = "./,!? ;:\n";
	for (int i = 0; i < 10; i++)
	{
		if (sym == delim[i])
		{
			return 1;
		}
	}
	return 0;
}
char* InputChar()
{
	int n = 2;
	char* strn = new char[n];
	strn[0] = '1';
	strn[1] = '\0';
	char inp;
	while ((inp = getchar()) != '\n')
	{
		char* buf = new char[n];
		for (int i = 0; i < n - 1; i++)
		{
			buf[i] = strn[i];
		}
		buf[n - 1] = '\0';
		buf[n - 2] = inp;
		n++;
		delete[] strn;
		strn = buf;
	}

	return strn;
}
void SortNameU(char** words, int n)
{
	char* sort;
	for (int i = 1; i < n; i++)
	{
		for (int r = 0; r < n - 1; r++)
		{
			if (strlen(words[r]) < strlen(words[r + 1]))
			{
				sort = words[r];
				words[r] = words[r + 1];
				words[r + 1] = sort;
			}
		}
	}
}
void SortSoglU(char** words, int n)
{
	char* sort;
	for (int i = 1; i < n; i++)
	{
		for (int r = 0; r < n - 1; r++)
		{
			if (Sogl(words[r], n) > Sogl(words[r + 1], n))
			{
				sort = words[r];
				words[r] = words[r + 1];
				words[r + 1] = sort;
			}
		}
	}
}
int Sogl(char* word, int n)
{
	int csogl = 0;
	char  tokens[] = "бвгджзйклмнпрстфхцчшщБВГДЖЗЙКЛМНПРСТФХЦЧШЦBCDFGHJKLMNPQRSTVWXYZbcdfghjklmnpqrstvwxyz";
	for (int i = 0; i < strlen(word) + 1; i++)
	{
		for (int j = 0; j < strlen(tokens) + 1; j++)
		{
			if (word[i] == tokens[j])
			{
				csogl++;
			}
		}
	}
	return csogl;
}
