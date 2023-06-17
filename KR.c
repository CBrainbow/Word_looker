#include <stdio.h>
#include <Windows.h>
#include <ctype.h>
#include <malloc.h>
#include <conio.h>
void clrscr();
void SetStandartScreen();
void SetTextBackround(int TextColor, int Background);
void PrintfNormal(char *text, int begin, int end);
void PrintfColor(char *text, int begin, int end);
void Find_Word(char *text, int len, int from, int *begin, int *end);
void Count_Symbol(char *text, int len, int *count_symb);
enum ConsoleColor {		
	Black=0,
	LightGray=7,
	White=15
};
void main (void)
{
	FILE *f_in;
	char name[80],*S;
	int l_str,i,begin,end,count_symb,from;
	
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	printf("Введите имя файла с текстом:\n");
	scanf("%s",name);
	f_in=fopen(name,"rb");
	if(f_in==NULL)	
		{
		clrscr();
		printf("Невозможно открыть файл %s.",name);
		getch();
		exit(1);		
		}
	fseek(f_in,0,SEEK_END);
	l_str=ftell(f_in);			
	if(l_str==0)			
		{
		clrscr();
		printf("В файле %s отсутствует текст.",name);
		getch();
		exit(1);		
		}						
	S=(char*)malloc((l_str+1)*sizeof(char));
	fseek(f_in,0,SEEK_SET);
	
	for (i=0;i<l_str;i++)			
		fscanf(f_in,"%c",S+i);
	*(S+l_str)='\0';			
	fclose(f_in);	

	clrscr();
	printf("Текст:");
	for(i=0;i<l_str;i++)
		{
			if (i%79==0)
				printf("\n");
			putch(*(S+i));
		}
	Count_Symbol(S,l_str,&count_symb);
	if (count_symb==0)
	{
		printf("\n\nВ тексте отсутствуют слова.");
		getch();
		exit(1);		
	}	
	printf("\n\nКоличество символов в самом длинном слове - %2d",count_symb);
	printf("\n\nДля организации поиска нажмите любую клавишу...");
	
	from=0;
	getch();
	while (from+1<l_str)
	{
		Find_Word(S,l_str,from,&begin,&end);
		if(end-begin+1==count_symb)
		{
			clrscr();
			printf("Текст:");	
			PrintfNormal(S,0,begin-1);			
			PrintfColor(S,begin,end);	
			PrintfNormal(S,end+1,l_str);	
			printf("\n\nКоличество символов в самом длинном слове - %2d",count_symb);
			printf("\n\nДля организации поиска нажмите любую клавишу...");
			getch();
		}
		from=end+1;			
	}
	clrscr();
	printf("Текст:");
	for(i=0;i<l_str;i++)
		{
			if (i%79==0)
				printf("\n");
			putch(*(S+i));
		}
	printf("\n\nКоличество символов в самом длинном слове - %2d",count_symb);
	printf("\n\nПоиск завершен.");	
	getch();

	free(S);
}
void clrscr()
{
	system("cls");
	return;
}
void SetStandartScreen()
{
	system("color 0F");			
	return;
}
void SetTextBackground(int TextColor, int BackgroundColor)	
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)
	((BackgroundColor << 4) | TextColor));
	return;
}
void PrintfNormal(char *text, int begin, int end)
{
	int i;
	SetTextBackground(Black,White);
	for (i=begin;i<=end;i++)
	{
		if (i%79==0)
			printf("\n");
		putch(*(text+i));
	}
	return;
}
void PrintfColor(char *text, int begin, int end)
{
	int i;
	SetTextBackground(Black,LightGray);
	for (i=begin;i<=end;i++)
	{
		if (i%79==0)
			printf("\n");
		putch(*(text+i));
	}
	SetTextBackground(White,Black);
	return;
}
void Find_Word(char *text, int len, int from, int *begin, int *end)
{
	int i,j;
	
	i=from;
	*begin=-1;
	while (i<len)
			if (isalpha(*(text+i)))
			{
				*begin=i;	
				*end=i;
				j=i+1;
				while(j<len)
				{
					if ((*(text+j)==' ')||(*(text+j)=='.')||(*(text+j)=='!')||(*(text+j)=='?')||(*(text+j)==',')||(*(text+j)==';')||(*(text+j)==':')||(*(text+j)=='*'))
					{
						*end=j-1;
						return;
					}
					else
						j++;
				}
			}
			else
				i++;
}
void Count_Symbol(char *text, int len, int *count_symb)
{
	int i,from,begin,end,max;
	
	max=0;
	*count_symb=0;
	from=0;
	while (from+1<len)
	{
		Find_Word(text,len,from,&begin,&end);
		if (begin==-1)
			break;
		if(max<end-begin+1)
		{
			max=end-begin+1;
			*count_symb=end-begin+1;			
		}
		from=end+1;
	}
}
