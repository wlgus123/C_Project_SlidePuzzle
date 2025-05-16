#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "windows.h"
#include "conio.h"

#include "mymtg.h"



// ���� ����
//char g_Key;
char g_Buffer[CMD_HEIGHT][CMD_WIDTH] = { 0, };


char _GetKey()
{
	char key = KEYBOARDNONECLICK;
	if (TRUNBASELOOP ||
		_kbhit())
	{
		key = _getch();
		if (key == -32)
		{
			key = _getch();
			if (key == 'H')
			{
				key = UPARROW;
			}
			if (key == 'M')
			{
				key = RIGHTARROW;
			}
			if (key == 'K')
			{
				key = LEFTARROW;
			}
			if (key == 'P')
			{
				key = DOWNARROW;
			}
		}
	}
	return key;
}


int IS_CMDInSide(int p_x, int p_y)
{
	if (p_x < 0 || p_x > CMD_WIDTH)
		return FALSE;
	if (p_y < 0 || p_y > CMD_HEIGHT)
		return FALSE;

	return TRUE;
}
void DrawCharFN(int p_x, int p_y, char p_char)
{
	if (IS_CMDInSide(p_x, p_y) == FALSE)
		return;

	g_Buffer[p_y][p_x] = p_char;
}
void DrawTextFN(int p_x, int p_y, const char* p_char)
{
	if (IS_CMDInSide(p_x, p_y) == FALSE)
		return;

	int xpos = 0;
	for (size_t i = 0; ; i++)
	{
		xpos = p_x + i;
		if (p_char[i] == '\0')
			break;
		if (IS_CMDInSide(p_x + i, p_y) == FALSE)
			break;

		g_Buffer[p_y][p_x + i] = p_char[i];
	}


}


void Render()
{
	COORD pos = { 0, 0 };
	HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hconsole, pos);

	for (size_t i = 0; i < CMD_HEIGHT; i++)
	{
		printf("%s\n", g_Buffer[i]);
	}
	printf("����: w a s d, �� �� �� ��\n\n");
	printf("u: ���� ���, r: �ٽ� ����, esc: ����\n\n\n");

	char* buffpos = &g_Buffer[0][0];
	for (size_t i = 0; i < CMD_HEIGHT * CMD_WIDTH; i++)
	{
		*(buffpos + i) = ' ';
		if (i % CMD_WIDTH)
			*(buffpos + i) = '\0';
	}

}