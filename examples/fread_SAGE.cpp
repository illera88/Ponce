// SAGE_fread.cpp : Defines the entry point for the console application.
//
#include <stdio.h>

bool reading_input_from_file(char *buffer)
{
	FILE *f = fopen("input.txt", "rb");
	if (f == NULL)
	{
		printf("input.txt not found!\n");
		return 0;
	}
	if (fread(buffer, 1, 4, f) != 4)
	{
		printf("file too small!\n");
		fclose(f);
		return 0;
	}
	fclose(f);
	return 1;
}

void check(const char *buf)
{
	int n = 0;
	if (buf[0] == 'b') n++;
	if (buf[1] == 'a') n++;
	if (buf[2] == 'd') n++;
	if (buf[3] == '!') n++;
	if (n == 4)
	{
		printf("Win!\n");
	}
	else
	{
		printf("Fail\n");
	}
}

int main(int argc, char* argv[])
{
	char buffer[256];
	if (reading_input_from_file(buffer))
		check(buffer);
	return 0;
}

