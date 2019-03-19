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

struct kana
{
	char k0;
	char k1;
};

#define MAX_KANA_INDEX (12 * 5 + 11)

internal kana
GetKanaFromIndex(u32 KanaIndex)
{
	kana Result = {};

	if(KanaIndex < 12 * 5)
	{
		Result.k0 = HiraganaConsonant[KanaIndex / 5];
		Result.k1 = HiraganaVowels[KanaIndex % 5];
	}
	else
	{
		KanaIndex -= 12 * 5;
		if(KanaIndex < 3)
		{
			Result.k0 = 'y';
			if(KanaIndex == 0)
			{
				Result.k1 = 'a';
			}
			else if(KanaIndex == 1)
			{
				Result.k1 = 'u';
			}
			else
			{
				Result.k1 = 'o';
			}
		}
		else
		{
			KanaIndex -= 3;
			if(KanaIndex < 5)
			{
				Result.k0 = 'r';
				Result.k1 = HiraganaVowels[KanaIndex];
			}
			else
			{
				KanaIndex -= 5;
				Assert(KanaIndex <= 2);
				if(KanaIndex == 0)
				{
					Result.k0 = 'w';
					Result.k1 = 'a';
				}
				else if(KanaIndex == 1)
				{
					Result.k0 = 'w';
					Result.k1 = 'o';
				}
				else
				{
					Result.k0 = 'n';
					Result.k1 = 0;
				}
			}
		}
	}

	return(Result);
}

#define VILLAGE_NAME_MAX_SIZE 5
#define VILLAGE_NAME_MIN_SIZE 2

char* GenerateRandomJapaneseName(random_series* Series, u32 MinSize, u32 MaxSize)
{
	Assert(MaxSize >= MinSize);
	u32 NameSize = RandomChoice(Series, MaxSize - MinSize) + MinSize;
	char* Result = 0;
	for(u32 i = 0; i < NameSize; ++i)
	{
		u32 KanaIndex = RandomChoice(Series, MAX_KANA_INDEX);
		kana Kana = GetKanaFromIndex(KanaIndex);
		if(i == 0)
		{
			while(Kana.k0 == 'n' && Kana.k1 == 0)
			{
				KanaIndex = RandomChoice(Series, MAX_KANA_INDEX);
				Kana = GetKanaFromIndex(KanaIndex);
			}
		}
		if(Kana.k0 != 0)
		{
			buf_push(char, Result, Kana.k0);
		}
		if(Kana.k1 != 0)
		{
			buf_push(char, Result, Kana.k1);
		}

	}
	buf_push(char, Result, 0);
	return(Result);
}

int main(int ArgumentCount, char** Arguments)
{
	random_series NameEntropy = RandomSeed(12345, 12878);
	for(u32 i = 0; i < 200; ++i)
	{
		char* Name = GenerateRandomJapaneseName(&NameEntropy, VILLAGE_NAME_MIN_SIZE, VILLAGE_NAME_MAX_SIZE);
		printf("%s\n", Name);
		buf_free(Name);
	}
	return(0);
}
