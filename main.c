#include "main.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBox(hInst, MAKEINTRESOURCE(IDC_CALCULATOR), NULL, (DLGPROC)DlgProc);

	return 0;
}

LRESULT CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
		{
			strcpy(strValue, "0");
			SetDlgItemText(hwnd, IDC_RESULT, strValue);
			SetFocus(GetDlgItem(hwnd, IDC_RESULT));
			SetWindowPos(hwnd, HWND_TOP, 200, 200, 0, 0, SWP_NOSIZE);
			return 0;
		}
		break;
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDC_ZERO:
					PrintStringNumber(hwnd, IDC_ZERO);
				break;
				case IDC_ONE:
					PrintStringNumber(hwnd, IDC_ONE);
				break;
				case IDC_TWO:
					PrintStringNumber(hwnd, IDC_TWO);
				break;
				case IDC_THREE:
					PrintStringNumber(hwnd, IDC_THREE);
				break;
				case IDC_FOUR:
					PrintStringNumber(hwnd, IDC_FOUR);
				break;
				case IDC_FIVE:
					PrintStringNumber(hwnd, IDC_FIVE);
				break;
				case IDC_SIX:
					PrintStringNumber(hwnd, IDC_SIX);
				break;
				case IDC_SEVEN:
					PrintStringNumber(hwnd, IDC_SEVEN);
				break;
				case IDC_EIGHT:
					PrintStringNumber(hwnd, IDC_EIGHT);
				break;
				case IDC_NINE:
					PrintStringNumber(hwnd, IDC_NINE);
				break;
				case IDC_ADDITION:
					OperatorType = ADD;
					PrintStringOperator(hwnd, IDC_ADDITION); 
				break;
				case IDC_SUBTRACTION:
					OperatorType = SUB;
					PrintStringOperator(hwnd, IDC_SUBTRACTION);
				break;
				case IDC_MULTIPLICATION:
					OperatorType = MUL;
					PrintStringOperator(hwnd, IDC_MULTIPLICATION);
				break;
				case IDC_DIVISION:
					OperatorType = DIV;
					PrintStringOperator(hwnd, IDC_DIVISION);
				break;
				case IDC_NEGATIVE:
					if (!OperatorFlag)
						ValueConversion();
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_EQUAL:
					if (!FinalCalcFlag && !FinalValueConversion) {
						CalculateDoubleResult();
						CalculateStringResult();
					}
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_DECIMAL:
					InsertDecimalPoint();
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_CLEAR:
					Reset();
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_DELETE:
					if (FinalCalcFlag)
						Reset();
					DeleteStringNumber(strValue);
					CalculateOperand();
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
			}
		}
		break;
		case WM_DESTROY:
		case WM_CLOSE:
			EndDialog(hwnd, 0);
		break;
		default:
			return FALSE;
	}
	return TRUE;
}

void InsertStringNumber(char Value, char *strValue)
{
	if (FinalCalcFlag)
		Reset();

	OperatorFlag = FALSE;

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
	if (FinalCalcFlag == FALSE && OperatorFlag == FALSE) {
		if (strValue[--NumDigits] == '.')
			DecimalFlag = FALSE;
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
		DecimalFlag = TRUE;
	}
}

void ValueConversion(void)
{
	if (FinalCalcFlag) {
		FinalValueConversion = TRUE;
		LeftOperand *= -1;
		ChangeNumDigits(LeftOperand);
		gcvt(LeftOperand, MAX_DIGITS, strValue);
	} else {
		if (OperandType == LEFT) {
			LeftOperand *= -1;
			ChangeNumDigits(LeftOperand);
			gcvt(LeftOperand, MAX_DIGITS, strValue);
		} 
		if (OperandType == RIGHT) {
			RightOperand *= -1;
			ChangeNumDigits(RightOperand);
			gcvt(RightOperand, MAX_DIGITS, strValue);
		}		
	}
	RemoveStringDecimal();
}

void InsertOperator(char Value, char *strValue)
{
	ChangeOperand();
	FinalCalcFlag = FALSE;
	FinalValueConversion = FALSE;

	if (LeftOperand == 0) {
		strValue[0] = '0';
		strValue[1] = ' ';
		strValue[2] = Value;
		strValue[3] = '\0';
	} else {
		gcvt(LeftOperand, MAX_DIGITS, strValue);
		RemoveStringDecimal();
		strValue += strlen(strValue);
		sprintf(strValue, " %c", Value);
	}
	OperatorFlag = TRUE;
}

void ChangeOperand(void)
{
	OperandType = RIGHT;
	NumDigits = 0;
	DecimalFlag = FALSE;
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
	RemoveStringDecimal();
	FinalCalcFlag = TRUE;
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
	DecimalFlag = FALSE;
	OperatorFlag = FALSE;
	ContinueCalcFlag = FALSE;
	FinalCalcFlag = FALSE;
	FinalValueConversion = FALSE;
}

void ChangeNumDigits(double Operand)
{
	if (Operand < 0)
		++NumDigits;
	if (Operand > 0)
		--NumDigits;
}

void RemoveStringDecimal(void)
{
	char *PtrStrValue = strValue;
	while (*PtrStrValue) {
		PtrStrValue++;
	}
	if (*(PtrStrValue-1) == '.') {
		*(PtrStrValue-1) = '\0';
	}
}

void PrintStringNumber(HWND hwnd, int n)
{
	if (strValue[0] == '0' && n == IDC_ZERO)
		;
	else {
		InsertStringNumber(n - IDC_ZERO + '0', strValue);
		CalculateOperand();
		SetDlgItemText(hwnd, IDC_RESULT, strValue);
	}
}

void PrintStringOperator(HWND hwnd, int n)
{
	if (!FinalCalcFlag)
		CalculateDoubleResult();
	InsertOperator(n - OPERATOR_ASCII + '0', strValue);
	SetDlgItemText(hwnd, IDC_RESULT, strValue);
}