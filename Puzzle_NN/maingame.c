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
	int index; // 현재 저장되고 있는 히스토리 크기
	int key[HISTORYSIZE]; // 히스토리 key 값
	int isUndoRedo; // undo, redo 가능 여부
} History;

// 전역 변수
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
	// 1. 진입함수 만들기
	// 2. mtg 확인
	// 3. while문 이용해서 반복 처리
	// 4. key 받아오기
	// 5. 화면 출력 처리
	// 6. 번호 2차원 배열 처리
	// 7. 번호 화면 출력
	// 8. 키보드 값 받아서 2차원 배열 값 치환 처리
	// 9. 종료처리

	Init();

	while (TRUE)
	{
		g_Key = _GetKey();

		Input();
		InputControlKey(); // w, a, s, d 외 키 입력 받는 함수

		DrawNumber();

		Render();

		if (ISComplete() == TRUE)
		{
			printf("게임 종료\n");
			system("pause");
			return;
		}

	}
}

void Init()
{
	srand(time(NULL));

	FindZeroPos(); // 0의 위치 찾기
	ShuffleRandom(); // 랜덤처리

	// 랜덤처리 후 이동 기록 초기화
	ClearHistory(0);
	g_His.index = -1;

	DrawNumber(); // 숫자 그리기
	Render();
}

// w, a, s, d 키 입력 처리 함수
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

// 기록 초기화 (p_src부터 HISTORYSIZE까지)
void ClearHistory(int p_src)
{
	for (size_t i = p_src; i < HISTORYSIZE; i++)
	{
		g_His.key[i] = '\0';
	}
}

// 기록 추가
void GetHistory()
{
	// history가 overflow인 경우 맨 앞에 있는 history를 삭제하고 최근 history 가져오기
	if (g_His.index >= HISTORYSIZE - 1)
	{
		for (size_t i = 1; i < HISTORYSIZE; i++)
		{
			g_His.key[i - 1] = g_His.key[i];
		}
		g_His.index--;
	}

	// undo, redo 상태가 아니면 history에 넣고 History 뒤 기록 초기화
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

// 실행 취소
void Undo()
{
	// 기록이 없을 경우 반환
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

// 다시 실행
void Redo()
{
	// 뒤에 기록이 없을 경우 반환
	if (g_His.key[g_His.index + 1] == '\0') return;

	g_Key = g_His.key[++g_His.index];
	g_His.isUndoRedo = TRUE;

	Input();
}

// ESC 키 입력 시 게임 종료 확인 메세지 띄우기
void ExitProgram()
{
	int code = -1;
	while (1)
	{
		system("cls");
		printf("게임을 종료하시겠습니까? (y, n)\n");

		code = _getch(); // 키 입력
		switch (code)
		{
		case 'y': case 'Y':
			exit(0);
		case 'n': case 'N':
			system("cls"); // 화면 지우기
			return;
		default:
			system("cls");
			printf("다시 입력해주세요.\n");
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

	// 화면 그려주기용
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
		case 1: // 오른
			g_Key = 'd';
			break;
		case 2: // 하단
			g_Key = 's';
			break;
		case 3: // 왼
			g_Key = 'a';
			break;
		default: // 위
			g_Key = 'w';
			break;
		}

		Input();
	}

	g_Key = '\0';
}

void DrawBG()
{
	DrawTextFN(0, 0, "┌────┬────┬────┐");
	DrawTextFN(0, 1, "│    │    │    │");
	DrawTextFN(0, 2, "│    │    │    │");
	DrawTextFN(0, 3, "│    │    │    │");
	DrawTextFN(0, 4, "├────┼────┼────┤");
	DrawTextFN(0, 5, "│    │    │    │");
	DrawTextFN(0, 6, "│    │    │    │");
	DrawTextFN(0, 7, "│    │    │    │");
	DrawTextFN(0, 8, "├────┼────┼────┤");
	DrawTextFN(0, 9, "│    │    │    │");
	DrawTextFN(0, 10, "│    │    │    │");
	DrawTextFN(0, 11, "│    │    │    │");
	DrawTextFN(0, 12, "└────┴────┴────┘");
}