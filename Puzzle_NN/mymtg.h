#pragma once

#define TRUE 1
#define FALSE 0
#define CMD_WIDTH 80
#define CMD_HEIGHT 15

#define TRUNBASELOOP 1
#define KEYBOARDNONECLICK -1


#define UPARROW -100
#define RIGHTARROW -101
#define DOWNARROW -102
#define LEFTARROW -103


/// <summary>
/// 범위지정
/// </summary>
/// <param name="p_x">위치x값</param>
/// <param name="p_y">위치y값</param>
/// <returns>1, 0 반환한다</returns>
int IS_CMDInSide(int p_x, int p_y);

void DrawCharFN(int p_x, int p_y, char p_char);

void DrawTextFN(int p_x, int p_y, const char* p_char);

void Render();