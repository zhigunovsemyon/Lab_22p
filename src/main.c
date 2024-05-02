#include "main.h"
/*
В заданном тексте удалить все части текста, заключенные в	*
скобки (вместе со скобками).								*
Текст вида (123(456)789) преобразуется в 789)

Дан текст. Определить наибольшее количество подряд идущих	*
пробелов в нем.												*

Исключить из строки группы символов, расположенные между	*
символами «*//*», «*//*» включая границы.Предполагается,	* 
что нет вложенных скобок.									*
Если в тексте стоит незакрытый блок, то от удаляется
*/
int main(void)
{
	char String[MAXLEN];
	*String = 0; //Установка первым символом терминального
	do
	{	//Функция выбора действия пользователем
		switch (Switcher(String))
		{
		case QUIT: //q
			return 0;

		case KEYBOARD://k
			puts("Введите текст:");
			fgets(String, MAXLEN, stdin);	//Ввод пользователем
			if (*String == '\n')			//Если пользователь нажал Enter, не введя ничего
				*String = 0;				//строка зануляется
			break;

		case READ_FILE: //f
			GetAndReadFile(String);	//Ввод пользователем названия файла, открытие его, чтение
			break;

		case REMOVE: //d
			*String = 0; //Установка первым символом терминального
			break;

		case RM_PARENTHESES: //( )
			RemoveParentheses(String);
			break;

		case COUNT_SPACE: //s
			printf("Максимальное число пробелов: %u\n", 
				CountMaxSpace(String));
			break;

		case RM_COMMENT_BLOCK: //c
			RemoveCommentBlock(String);
			break;

		case PRINT: //p
			puts(String);
			break;

		default:
			break;
		}
		putchar('\n');
	} while (1);
}
//Удаление блока /**/
void RemoveCommentBlock(char String[])
{	//Если строка закончилась, функция завершается
	if (!(*String))
		return;

	/*Сравнение с символами открытия блока*/
	if (strncmp(String, "/*", 2)/*!= 0*/)	//Пока символы не встречены
		return RemoveCommentBlock(++String);//Осуществляется повторный запуск со сдвигом на символ

	uint16_t i = 2;	//Номер символа перематывается до момента, пока String[i] не будет равна *
	while(String[i] && strncmp(String + i, "*/", 2)/*!=0*/)
		i++;
		
	if (String[i])	//Если String[i] = '\0', то memmove скопирует этот терминальный символ
		i += 2;		//Если секция кончилась */ то i переставляется на символ после блока

	//Копирование символов, идущих после String[i] на место String[0...]
	memmove(String, String + i, strlen(String + i) + 1);

	//Повторное удаление следующего блока
	return RemoveCommentBlock(String);
}

//Удаление блока в скобках ()
void RemoveParentheses(char String[])
{	
	char* pDest = strchr(String, '(');		//Поиск начала сегмента в скобках

	//Поиск конца сегмента в скобках, +1 переключает на следующий за ) символ
	char* pSrc = strchr(String, ')') + 1;	
	int32_t len = pSrc - pDest;				//Выяснение длинны блока

	//Если блок меньше пары скобок, например они стоят в обратном порядке, либо если открывающей скобки не было
	if (len < 2 || !pDest)	
		return;				//осуществляется выход из программы
	memmove(pDest, pSrc, strlen(pSrc) + 1);			//Копирование строки
	RemoveParentheses(pDest);				//Повторный вызов функции для проверки на ещё одни пары скобок 
}

//Подсчёт максимальной последовательности пробелов в строке String
uint8_t CountMaxSpace(char String[])
{
	uint32_t max = 0, count = 0;
	for (uint32_t i = 0; String[i]; ++i)//Цикл перебирает строку, пока она не закончится
	{
		if (String[i] != ' ')	//Перебор i до первого пробела
			continue;

		/*else*/
		do//Если символ оказался пробелом,
		{// перебор осуществляется этим циклом, вместо внешнего
			count++;	//Временный счётчик увеличивается
			i++;		//Счётчик строки также увеличивается
		} while (String[i] == ' ');

		//После окончание цикла счёта пробелов, сравнивается с максимальным,
		if (count > max)
			max = count;// присваивается, если больше
		count = 0;//Временный счётчик сбрасывается
	}
	return max;//После всех циклов, максимальное значение возвращается
}

//Подсчёт длинны файла, сравнение с максимальным
uint32_t FileLen(FILE* f, uint32_t maxLen)
{
	fseek(f, SEEK_SET, SEEK_END);//Перемотка в конец
	uint32_t len = ftell(f);	 //Чтение размера файла
	rewind(f);					 //Перемотка обратно в начало
	return (len < maxLen) ? 	 //Если полученный размер больше максимального
		len : maxLen;			 //передаётся максимальный
}

//Процедура спрашивает у пользователя файл, читает его в строку String
void GetAndReadFile(char String[])
{
	FILE* f;
	printf("Введите открываемый файл: ");
	scanf(SCANF_SPEC_S, String);
	f = fopen(String, "rb");
	if (!f)	//Если не удалось открыть файл
	{
		puts("Не удалось открыть файл!");
		*String = 0; //Установка первым символом терминального
		return;
	}
	//Определение размера загружаемой строки между размером файла и максимальным
	uint32_t len = FileLen(f, MAXLEN - 1);	
	fread(String, sizeof(char), len, f);	//Чтение в буфер
	String[len] = 0;						//Зануление конца
	fclose(f);								//Закрытие файла
	return;
}

//Выбор режима работы
uint8_t Switcher(char String[])
{
	printf("%s%s%s",
		"Для выхода нажмите q\n",
		"Чтобы ввести текст вручную, введите k\n",
		"Чтобы прочитать текст из файла, введите f\n");
	if (*String)	//Если в памяти есть строка
	{
		printf("%s%s%s%s%s",
			"Чтобы вывести текст, введите p\n",
			"Чтобы удалить весь текст, введите d\n",
			"Чтобы удалить текст внутри скобок, введите ( или )\n",
			"Чтобы удалить текст внутри блока /* */, введите c\n",
			"Чтобы сосчитать наибольшую последовательность пробелов, введите s\n");
	}
	do
	{	//Ввод пользователем символа
		switch (getchar())
		{
		case 'q':
		case 'Q':
			getchar();//Удаление новой строки из ввода
			return QUIT;
		case 'k':
		case 'K':
			getchar();//Удаление новой строки из ввода
			return KEYBOARD;
		case 'f':
		case 'F':
			getchar();//Удаление новой строки из ввода
			return READ_FILE;
		case 'd':
		case 'D':
			getchar();//Удаление новой строки из ввода
			if (*String)
				return REMOVE;
			break;
		case 'p':
		case 'P':
			getchar();//Удаление новой строки из ввода
			if (*String)
				return PRINT;
			break;
		case '(':
		case ')':
			getchar();//Удаление новой строки из ввода
			if (*String)
				return RM_PARENTHESES;
			break;
		case 's':
		case 'S':
			getchar();//Удаление новой строки из ввода
			if (*String)
				return COUNT_SPACE;
			break;
		case 'c':
		case 'C':
			getchar();//Удаление новой строки из ввода
			if (*String)
				return RM_COMMENT_BLOCK;
			break;
		default:
			break;
		}
	} while (1);
}