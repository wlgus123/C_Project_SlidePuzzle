#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "windows.h"
#include "conio.h"

#include "mymtg.h"

#define PUZZLESIZE 3
#define HISTORYSIZE 50

typedef struct MYPoint
{
	int x;
	int y;
}MYPoint;

typedef struct History
{
	int index; // ���� ����ǰ� �ִ� �����丮 ũ��
	int key[HISTORYSIZE]; // �����丮 key ��
	int isUndoRedo; // undo, redo ���� ����
} History;

// ���� ����
char g_Key;
int g_PuzzleData[PUZZLESIZE][PUZZLESIZE] = {
	{1, 2, 3},
	{4, 5, 6},
	{7, 8, 0},
};
MYPoint g_EmptyPos = { 2, 2 };
History g_His = { -1, { 0, }, FALSE };

void Init();
void ChangeData(int* p_src, int* p_dest);
void Undo();
void Input();
void InputControlKey();
void DrawBG();
void DrawNumber();
void FindZeroPos();
int ISComplete();
void ShuffleRandom();
void GetHistory();
void ClearHistory(int p_src);

void main()
{
	// 1. �����Լ� �����
	// 2. mtg Ȯ��
	// 3. while�� �̿��ؼ� �ݺ� ó��
	// 4. key �޾ƿ���
	// 5. ȭ�� ��� ó��
	// 6. ��ȣ 2���� �迭 ó��
	// 7. ��ȣ ȭ�� ���
	// 8. Ű���� �� �޾Ƽ� 2���� �迭 �� ġȯ ó��
	// 9. ����ó��

	Init();

	while (TRUE)
	{
		g_Key = _GetKey();

		Input();
		InputControlKey(); // w, a, s, d �� Ű �Է� �޴� �Լ�

		DrawNumber();

		Render();

		if (ISComplete() == TRUE)
		{
			printf("���� ����\n");
			system("pause");
			return;
		}

	}
}

void Init()
{
	srand(time(NULL));

	FindZeroPos(); // 0�� ��ġ ã��
	ShuffleRandom(); // ����ó��

	// ����ó�� �� �̵� ��� �ʱ�ȭ
	ClearHistory(0);
	g_His.index = -1;

	DrawNumber(); // ���� �׸���
	Render();
}

// w, a, s, d Ű �Է� ó�� �Լ�
void Input()
{
	switch (g_Key)
	{
	case 'w': case 'W': case UPARROW:
		if (g_EmptyPos.y > 0)
		{
			ChangeData(
				&g_PuzzleData[g_EmptyPos.y][g_EmptyPos.x],
				&g_PuzzleData[g_EmptyPos.y - 1][g_EmptyPos.x]
			);
			g_EmptyPos.y -= 1;
			GetHistory();
		}
		break;
	case 's': case 'S': case DOWNARROW:
		if (g_EmptyPos.y < PUZZLESIZE - 1)
		{
			ChangeData(
				&g_PuzzleData[g_EmptyPos.y][g_EmptyPos.x],
				&g_PuzzleData[g_EmptyPos.y + 1][g_EmptyPos.x]
			);
			g_EmptyPos.y += 1;
			GetHistory();
		}
		break;
	case 'a': case 'A': case LEFTARROW:
		if (g_EmptyPos.x > 0)
		{
			ChangeData(
				&g_PuzzleData[g_EmptyPos.y][g_EmptyPos.x]
				, &g_PuzzleData[g_EmptyPos.y][g_EmptyPos.x - 1]
			);
			g_EmptyPos.x -= 1;
			GetHistory();
		}
		break;
	case 'd': case 'D': case RIGHTARROW:
		if (g_EmptyPos.x < PUZZLESIZE - 1)
		{
			ChangeData(
				&g_PuzzleData[g_EmptyPos.y][g_EmptyPos.x]
				, &g_PuzzleData[g_EmptyPos.y][g_EmptyPos.x + 1]
			);
			g_EmptyPos.x += 1;
			GetHistory();
		}
		break;
	}
}

// ��� �ʱ�ȭ (p_src���� HISTORYSIZE����)
void ClearHistory(int p_src)
{
	for (size_t i = p_src; i < HISTORYSIZE; i++)
	{
		g_His.key[i] = '\0';
	}
}

// ��� �߰�
void GetHistory()
{
	// history�� overflow�� ��� �� �տ� �ִ� history�� �����ϰ� �ֱ� history ��������
	if (g_His.index >= HISTORYSIZE - 1)
	{
		for (size_t i = 1; i < HISTORYSIZE; i++)
		{
			g_His.key[i - 1] = g_His.key[i];
		}
		g_His.index--;
	}

	// undo, redo ���°� �ƴϸ� history�� �ְ� History �� ��� �ʱ�ȭ
	if (g_His.isUndoRedo == FALSE)
	{
		g_His.key[++g_His.index] = g_Key;
		
		ClearHistory(g_His.index + 1);
	}
	else
	{
		g_His.isUndoRedo = FALSE;
	}
}

void ChangeData(int* p_src, int* p_dest)
{
	int tempval = *p_src;
	*p_src = *p_dest;
	*p_dest = tempval;
}

// ���� ���
void Undo()
{
	// ����� ���� ��� ��ȯ
	if (g_His.index < 0) return;

	switch (g_His.key[g_His.index--])
	{
	case 'w': case 'W': case UPARROW:
		g_Key = 's';
		break;
	case 'a': case 'A': case LEFTARROW:
		g_Key = 'd';
		break;
	case 's': case 'S': case DOWNARROW:
		g_Key = 'w';
		break;
	case 'd': case 'D': case RIGHTARROW:
		g_Key = 'a';
		break;
	}
	g_His.isUndoRedo = TRUE;

	Input();
}

// �ٽ� ����
void Redo()
{
	// �ڿ� ����� ���� ��� ��ȯ
	if (g_His.key[g_His.index + 1] == '\0') return;

	g_Key = g_His.key[++g_His.index];
	g_His.isUndoRedo = TRUE;

	Input();
}

// ESC Ű �Է� �� ���� ���� Ȯ�� �޼��� ����
void ExitProgram()
{
	int code = -1;
	while (1)
	{
		system("cls");
		printf("������ �����Ͻðڽ��ϱ�? (y, n)\n");

		code = _getch(); // Ű �Է�
		switch (code)
		{
		case 'y': case 'Y':
			exit(0);
		case 'n': case 'N':
			system("cls"); // ȭ�� �����
			return;
		default:
			system("cls");
			printf("�ٽ� �Է����ּ���.\n");
			Sleep(500);
			break;
		}
	}

	return;
}

void InputControlKey()
{
	switch (g_Key)
	{
	case 'u': case 'U':
		Undo();
		break;
	case 'r': case 'R':
		Redo();
		break;
	case '\033':
		ExitProgram();
		break;
	}
}

void DrawNumber()
{
	DrawBG();

	// ȭ�� �׷��ֱ��
	for (size_t y = 0; y < PUZZLESIZE; y++)
	{
		for (size_t x = 0; x < PUZZLESIZE; x++)
		{
			// int -> char
			char c = g_PuzzleData[y][x] + '0';
			if (0 == g_PuzzleData[y][x])
			{
				c = '*';
			}
			DrawCharFN((x * 6) + 4, (y * 4) + 2, c);
		}
	}
}

void FindZeroPos()
{
	int isfine = 0;
	for (size_t y = 0; y < PUZZLESIZE; y++)
	{
		for (size_t x = 0; x < PUZZLESIZE; x++)
		{
			if (0 == g_PuzzleData[y][x])
			{
				g_EmptyPos.x = x;
				g_EmptyPos.y = y;
				isfine = 1;
				return;
			}
		}
		if (isfine)
		{
			break;
		}
	}
}

int ISComplete()
{
	int val = 1;
	for (size_t y = 0; y < PUZZLESIZE; y++)
	{
		for (size_t x = 0; x < PUZZLESIZE; x++)
		{
			//int tval = ((y * PUZZLESIZE) + x) + 1;
			if (g_PuzzleData[y][x] != (val % 9))
			{
				return FALSE;
			}
			++val;
		}
	}

	return TRUE;
}

void ShuffleRandom()
{
	for (size_t i = 0; i < 100; i++)
	{
		// w, s, a, d
		int direction = rand() % 4;
		switch (direction)
		{
		case 1: // ����
			g_Key = 'd';
			break;
		case 2: // �ϴ�
			g_Key = 's';
			break;
		case 3: // ��
			g_Key = 'a';
			break;
		default: // ��
			g_Key = 'w';
			break;
		}

		Input();
	}

	g_Key = '\0';
}

void DrawBG()
{
	DrawTextFN(0, 0, "��������������������������������");
	DrawTextFN(0, 1, "��    ��    ��    ��");
	DrawTextFN(0, 2, "��    ��    ��    ��");
	DrawTextFN(0, 3, "��    ��    ��    ��");
	DrawTextFN(0, 4, "��������������������������������");
	DrawTextFN(0, 5, "��    ��    ��    ��");
	DrawTextFN(0, 6, "��    ��    ��    ��");
	DrawTextFN(0, 7, "��    ��    ��    ��");
	DrawTextFN(0, 8, "��������������������������������");
	DrawTextFN(0, 9, "��    ��    ��    ��");
	DrawTextFN(0, 10, "��    ��    ��    ��");
	DrawTextFN(0, 11, "��    ��    ��    ��");
	DrawTextFN(0, 12, "��������������������������������");
}