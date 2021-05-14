#include <windows.h>
#include <stdio.h>
#include "resource.h"

#define MAX_DIGITS 15
#define ADD 1
#define SUB 2
#define MUL 3
#define DIV 4
#define LEFT 1
#define RIGHT 2

// Global vars
const char g_szClassName[] = "MainWindowClass";
char strValue[MAX_DIGITS+3];

int OperatorType = ADD;
int OperandType = LEFT;
int NumDigits = 0;
double LeftOperand = 0;
double RightOperand = 0;

// Flags
BOOL DecimalFlag = FALSE;
BOOL OperatorFlag = FALSE;
BOOL ContinueCalcFlag = FALSE;
BOOL FinalCalcFlag = FALSE;
BOOL FinalValueConversion = FALSE;

LRESULT CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

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
void ChangeNumDigits(double Operand);
void RemoveStringDecimal(void);