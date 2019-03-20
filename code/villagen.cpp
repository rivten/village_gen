#include <iostream>

struct village
{
	char* Name;
};

#include "rivten.h"
#include "math.h"
#include "random.h"

char HiraganaVowels[5] = {'a', 'i', 'u', 'e', 'o'};
char HiraganaConsonant[15] = {0, 'k', 'g', 's', 'z', 't', 'd', 'n', 'h', 'b', 'p', 'm', 'y', 'r', 'w'};
char HiraganaSingular = 'n';

#define MAX_KANA_INDEX (12 * 5 + 11 + 33)

internal char*
GetKanaFromIndex(u32 KanaIndex)
{
	char* Result = 0;

	if(KanaIndex < 12 * 5)
	{
		// NOTE(hugo): exceptions
		if(KanaIndex == 16)
		{
			buf_push(char, Result, 's');
			buf_push(char, Result, 'h');
			buf_push(char, Result, 'i');
		}
		else if(KanaIndex == 26)
		{
			buf_push(char, Result, 'c');
			buf_push(char, Result, 'h');
			buf_push(char, Result, 'i');
		}
		else if(KanaIndex == 27)
		{
			buf_push(char, Result, 't');
			buf_push(char, Result, 's');
			buf_push(char, Result, 'u');
		}
		else if(KanaIndex == 42)
		{
			buf_push(char, Result, 'f');
			buf_push(char, Result, 'u');
		}
		else if(KanaIndex == 21)
		{
			buf_push(char, Result, 'j');
			buf_push(char, Result, 'i');
		}
		else if(KanaIndex == 31)
		{
			buf_push(char, Result, 'j');
			buf_push(char, Result, 'i');
		}
		else if(KanaIndex == 32)
		{
			buf_push(char, Result, 'z');
			buf_push(char, Result, 'u');
		}
		else
		{
			if((KanaIndex / 5) != 0)
			{
				buf_push(char, Result, HiraganaConsonant[KanaIndex / 5]);
			}
			buf_push(char, Result, HiraganaVowels[KanaIndex % 5]);
		}
	}
	else if(KanaIndex < (12 * 5 + 11))
	{
		KanaIndex -= 12 * 5;
		if(KanaIndex < 3)
		{
			buf_push(char, Result, 'y');
			if(KanaIndex == 0)
			{
				buf_push(char, Result, 'a');
			}
			else if(KanaIndex == 1)
			{
				buf_push(char, Result, 'u');
			}
			else
			{
				buf_push(char, Result, 'o');
			}
		}
		else
		{
			KanaIndex -= 3;
			if(KanaIndex < 5)
			{
				buf_push(char, Result, 'r');
				buf_push(char, Result, HiraganaVowels[KanaIndex]);
			}
			else
			{
				KanaIndex -= 5;
				Assert(KanaIndex <= 2);
				if(KanaIndex == 0)
				{
					buf_push(char, Result, 'w');
					buf_push(char, Result, 'a');
				}
				else if(KanaIndex == 1)
				{
					buf_push(char, Result, 'w');
					buf_push(char, Result, 'o');
				}
				else
				{
					buf_push(char, Result, 'n');
				}
			}
		}
	}
	else
	{
		KanaIndex -= 12 * 5 + 11;
		Assert(KanaIndex < 33);
		if(KanaIndex == 0)
		{
			buf_push(char, Result, 's');
			buf_push(char, Result, 'h');
			buf_push(char, Result, 'a');
		}
		else if(KanaIndex == 1)
		{
			buf_push(char, Result, 's');
			buf_push(char, Result, 'h');
			buf_push(char, Result, 'u');
		}
		else if(KanaIndex == 2)
		{
			buf_push(char, Result, 's');
			buf_push(char, Result, 'h');
			buf_push(char, Result, 'o');
		}
		else if(KanaIndex == 3)
		{
			buf_push(char, Result, 'c');
			buf_push(char, Result, 'h');
			buf_push(char, Result, 'a');
		}
		else if(KanaIndex == 4)
		{
			buf_push(char, Result, 'c');
			buf_push(char, Result, 'h');
			buf_push(char, Result, 'u');
		}
		else if(KanaIndex == 5)
		{
			buf_push(char, Result, 'c');
			buf_push(char, Result, 'h');
			buf_push(char, Result, 'o');
		}
		else if(KanaIndex == 6)
		{
			buf_push(char, Result, 'j');
			buf_push(char, Result, 'a');
		}
		else if(KanaIndex == 7)
		{
			buf_push(char, Result, 'j');
			buf_push(char, Result, 'u');
		}
		else if(KanaIndex == 8)
		{
			buf_push(char, Result, 'j');
			buf_push(char, Result, 'o');
		}
		else
		{
			KanaIndex -= 9;
			char Cons[] = {'k', 'n', 'h', 'm', 'r', 'g', 'b', 'p'};
			char Voy[] = {'a', 'u', 'o'};
			Assert(KanaIndex / 3 < ArrayCount(Cons));
			buf_push(char, Result, Cons[KanaIndex / 3]);
			buf_push(char, Result, 'y');
			buf_push(char, Result, Voy[KanaIndex % 3]);
		}
	}

	buf_push(char, Result, 0);
	return(Result);
}

#define VILLAGE_NAME_MAX_SIZE 5
#define VILLAGE_NAME_MIN_SIZE 2

char* GenerateRandomJapaneseName(random_series* Series, u32 MinSize, u32 MaxSize)
{
	Assert(MaxSize >= MinSize);
	u32 NameSize = RandomChoice(Series, MaxSize - MinSize) + MinSize;
	//printf("%u\n", NameSize);
	char* Result = 0;
	for(u32 i = 0; i < NameSize; ++i)
	{
		u32 KanaIndex = RandomChoice(Series, MAX_KANA_INDEX);
		char* Kana = GetKanaFromIndex(KanaIndex);
		//printf("%u - %s\n", KanaIndex, Kana);
		if(i == 0)
		{
			while((Kana[0] == 'n') && (Kana[1] == 0))
			{
				KanaIndex = RandomChoice(Series, MAX_KANA_INDEX);
				buf_free(Kana);
				Kana = GetKanaFromIndex(KanaIndex);
				//printf("Retry. %u - %s\n", KanaIndex, Kana);
			}
		}
		for(char* C = Kana; *C != 0; ++C)
		{
			buf_push(char, Result, *C);
		}

		buf_free(Kana);

	}
	buf_push(char, Result, 0);
	return(Result);
}

int main(int ArgumentCount, char** Arguments)
{
	random_series NameEntropy = RandomSeed(12345, 12879);
	for(u32 i = 0; i < 200; ++i)
	{
		char* Name = GenerateRandomJapaneseName(&NameEntropy, VILLAGE_NAME_MIN_SIZE, VILLAGE_NAME_MAX_SIZE);
		printf("%u. %s\n", i, Name);
		buf_free(Name);
	}
	return(0);
}
