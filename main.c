#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"

// Global vars
const char g_szClassName[] = "MainWindowClass";
char strValue[MAX_DIGITS+3];

int OperatorType = ADD;
int OperandType = LEFT;
int NumDigits = 0;
double LeftOperand = 0;
double RightOperand = 0;

// Flags
bool DecimalFlag = false;
bool OperatorFlag = false;
bool ContinueCalcFlag = false;
bool FinalCalcFlag = false;
bool FinalValueConversion = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void InsertStringNumber(char Value, char *strValue);
void DeleteStringNumber(char *strValue);
void InsertDecimalPoint(void);
void ValueConversion(void);
void InsertOperator(char Value, char *strValue);
void ChangeOperand(void);
void CalculateOperand(void);
void CalculateDoubleResult(void);
void CalculateStringResult(void);
void Reset(void);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg;

	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance 	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName	 = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"Calculator v. 1.0",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, CALC_WIDTH, CALC_HEIGHT,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndStatic;

	switch (message) {
		case WM_CREATE:
		{
			hwndStatic = CreateWindowEx(
				WS_EX_STATICEDGE, "STATIC", "0",
				WS_VISIBLE | WS_CHILD | SS_RIGHT,
				RESULT_X + ROW_GAP, RESULT_Y, RESULT_WIDTH, RESULT_HEIGHT,
				hwnd, (HMENU) IDC_STATIC_RESULT, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "7",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 0 + ROW_GAP, BUTTON_ROW_Y * 1, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_SEVEN, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "8",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 1 + ROW_GAP, BUTTON_ROW_Y * 1, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_EIGHT, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "9",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 2 + ROW_GAP, BUTTON_ROW_Y * 1, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_NINE, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "+/-",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 3 + ROW_GAP, BUTTON_ROW_Y * 1, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_NEGATIVE, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "DEL",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 4 + ROW_GAP, BUTTON_ROW_Y * 1, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_DEL, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "4",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 0 + ROW_GAP, BUTTON_ROW_Y * 2, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_FOUR, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "5",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 1 + ROW_GAP, BUTTON_ROW_Y * 2, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_FIVE, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "6",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 2 + ROW_GAP, BUTTON_ROW_Y * 2, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_SIX, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "x",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 3 + ROW_GAP, BUTTON_ROW_Y * 2, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_MULTIPLICATION, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "/",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 4 + ROW_GAP, BUTTON_ROW_Y * 2, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_DIVISION, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "1",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 0 + ROW_GAP, BUTTON_ROW_Y * 3, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_ONE, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "2",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 1 + ROW_GAP, BUTTON_ROW_Y * 3, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_TWO, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "3",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 2 + ROW_GAP, BUTTON_ROW_Y * 3, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_THREE, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "-",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 3 + ROW_GAP, BUTTON_ROW_Y * 3, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_SUBTRACTION, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "=",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 4 + ROW_GAP, BUTTON_ROW_Y * 3, BUTTON_WIDTH, BUTTON_HEIGHT * 2 + 3,
				hwnd, (HMENU) IDC_BUTTON_EQUAL, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "C",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 0 + ROW_GAP, BUTTON_ROW_Y * 4, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_CLEAR, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "0",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 1 + ROW_GAP, BUTTON_ROW_Y * 4, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_ZERO, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", ".",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 2 + ROW_GAP, BUTTON_ROW_Y * 4, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_DECIMAL, GetModuleHandle(NULL), NULL);

			CreateWindow(
				"BUTTON", "+",
				WS_VISIBLE | WS_CHILD,
				BUTTON_ROW_X * 3 + ROW_GAP, BUTTON_ROW_Y * 4, BUTTON_WIDTH, BUTTON_HEIGHT,
				hwnd, (HMENU) IDC_BUTTON_ADDITION, GetModuleHandle(NULL), NULL);
		}
		break;
		case WM_COMMAND:
			if (HIWORD(wParam) == BN_CLICKED) {
				switch (LOWORD(wParam)) {
					case IDC_BUTTON_ZERO:
						InsertStringNumber('0', strValue);
						CalculateOperand();
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_ONE:
						InsertStringNumber('1', strValue);
						CalculateOperand();
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_TWO:
						InsertStringNumber('2', strValue);
						CalculateOperand();
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_THREE:
						InsertStringNumber('3', strValue);
						CalculateOperand();
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_FOUR:
						InsertStringNumber('4', strValue);
						CalculateOperand();
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_FIVE:
						InsertStringNumber('5', strValue);
						CalculateOperand();
						SetWindowText(hwndStatic, strValue);					
					break;
					case IDC_BUTTON_SIX:
						InsertStringNumber('6', strValue);
						CalculateOperand();
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_SEVEN:
						InsertStringNumber('7', strValue);
						CalculateOperand();
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_EIGHT:
						InsertStringNumber('8', strValue);
						CalculateOperand();
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_NINE:
						InsertStringNumber('9', strValue);
						CalculateOperand();
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_ADDITION:
						if (!FinalCalcFlag)
							CalculateDoubleResult();
						OperatorType = ADD;
						InsertOperator('+', strValue);
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_SUBTRACTION:
						if (!FinalCalcFlag)
							CalculateDoubleResult();
						OperatorType = SUB;
						InsertOperator('-', strValue);
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_MULTIPLICATION:
						if (!FinalCalcFlag)
							CalculateDoubleResult();
						OperatorType = MUL;
						InsertOperator('x', strValue);
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_DIVISION:
						if (!FinalCalcFlag)
							CalculateDoubleResult();
						OperatorType = DIV;
						InsertOperator('/', strValue);
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_NEGATIVE:
						if (!OperatorFlag)
							ValueConversion();
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_EQUAL:
						if (!FinalCalcFlag && !FinalValueConversion) {
							CalculateDoubleResult();
							CalculateStringResult();
						}
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_DECIMAL:
						InsertDecimalPoint();
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_CLEAR:
						Reset();
						SetWindowText(hwndStatic, strValue);
					break;
					case IDC_BUTTON_DEL:
						if (FinalCalcFlag)
							Reset();
						DeleteStringNumber(strValue);
						CalculateOperand();
						SetWindowText(hwndStatic, strValue);
					break;
				}
			}
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

void InsertStringNumber(char Value, char *strValue)
{
	if (FinalCalcFlag)
		Reset();

	OperatorFlag = false;

	if (DecimalFlag) {
		if (NumDigits < MAX_DIGITS+1) {
			strValue[NumDigits++] = Value;
			strValue[NumDigits] = '\0';
		}
	} else {
		if (NumDigits < MAX_DIGITS) {
			strValue[NumDigits++] = Value;
			strValue[NumDigits] = '\0';
		}
	}
}

void DeleteStringNumber(char *strValue)
{
	if (FinalCalcFlag == false && OperatorFlag == false) {
		if (strValue[--NumDigits] == '.')
			DecimalFlag = false;
		if (NumDigits < 1) {
			strValue[0] = '0';
			NumDigits = 0;
		}
		else
			strValue[NumDigits] = '\0';
	}
}

void InsertDecimalPoint(void)
{
	if (!DecimalFlag) {
		if (NumDigits == 0) {
			InsertStringNumber('0', strValue);
			InsertStringNumber('.', strValue);
		} else
			InsertStringNumber('.', strValue);
		DecimalFlag = true;
	}
}

void ValueConversion(void)
{
	if (FinalCalcFlag) {
		FinalValueConversion = true;
		LeftOperand *= -1;
		if (LeftOperand < 0)
			++NumDigits;
		if (LeftOperand > 0)
			--NumDigits;
		gcvt(LeftOperand, MAX_DIGITS, strValue);
	} else {
		if (OperandType == LEFT) {
			LeftOperand *= -1;
			if (LeftOperand < 0)
				++NumDigits;
			if (LeftOperand > 0)
				--NumDigits;
			gcvt(LeftOperand, MAX_DIGITS, strValue);
		} 
		if (OperandType == RIGHT) {
			RightOperand *= -1;
			if (RightOperand < 0)
				++NumDigits;
			if (LeftOperand > 0)
				--NumDigits;
			gcvt(RightOperand, MAX_DIGITS, strValue);
		}		
	}

	char *PtrStrValue = strValue;
	while (*PtrStrValue) {
		PtrStrValue++;
	}
	if (*(PtrStrValue-1) == '.') {
		*(PtrStrValue-1) = '\0';
	}
}

void InsertOperator(char Value, char *strValue)
{
	ChangeOperand();
	FinalCalcFlag = false;
	FinalValueConversion = false;

	if (LeftOperand == 0) {
		strValue[0] = '0';
		strValue[1] = ' ';
		strValue[2] = Value;
		strValue[3] = '\0';
	} else {
		gcvt(LeftOperand, MAX_DIGITS, strValue);
		char *PtrStrValue = strValue;
		while (*PtrStrValue) {
			PtrStrValue++;
		}
		if (*(PtrStrValue-1) == '.') {
			*(PtrStrValue-1) = '\0';
		}
		strValue += sprintf(strValue, "%s %c", strValue, Value);
	}
	OperatorFlag = true;
}

void ChangeOperand(void)
{
	OperandType = RIGHT;
	NumDigits = 0;
	DecimalFlag = false;
}

void CalculateOperand()
{
	if (OperandType == LEFT) {
		LeftOperand = atof(strValue);
	} else {
		RightOperand = atof(strValue);
	}
}

void CalculateDoubleResult()
{
	if (!OperatorFlag) {
		if (OperatorType == ADD)
			LeftOperand += RightOperand;
		if (OperatorType == SUB)
			LeftOperand -= RightOperand;
		if (OperatorType == MUL)
			LeftOperand *= RightOperand;
		if (OperatorType == DIV)
			LeftOperand /= RightOperand;
	}
}

void CalculateStringResult()
{
	gcvt(LeftOperand, MAX_DIGITS, strValue);
	char *PtrStrValue = strValue;
	while (*PtrStrValue) {
		PtrStrValue++;
	}
	if (*(PtrStrValue-1) == '.') {
		*(PtrStrValue-1) = '\0';
	}

	FinalCalcFlag = true;
}

void Reset(void)
{
	strValue[0] = '0';
	strValue[1] = '\0';

	// Global vars
	OperatorType = ADD;
	OperandType = LEFT;
	NumDigits = 0;
	LeftOperand = 0;
	RightOperand = 0;

	// Flags
	DecimalFlag = false;
	OperatorFlag = false;
	ContinueCalcFlag = false;
	FinalCalcFlag = false;
	FinalValueConversion = false;
}