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
			SetDlgItemText(hwnd, IDC_RESULT, "0");
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
					InsertStringNumber('0', strValue);
					CalculateOperand();
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_ONE:
					InsertStringNumber('1', strValue);
					CalculateOperand();
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_TWO:
					InsertStringNumber('2', strValue);
					CalculateOperand();
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_THREE:
					InsertStringNumber('3', strValue);
					CalculateOperand();
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_FOUR:
					InsertStringNumber('4', strValue);
					CalculateOperand();
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_FIVE:
					InsertStringNumber('5', strValue);
					CalculateOperand();
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_SIX:
					InsertStringNumber('6', strValue);
					CalculateOperand();
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_SEVEN:
					InsertStringNumber('7', strValue);
					CalculateOperand();
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_EIGHT:
					InsertStringNumber('8', strValue);
					CalculateOperand();
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_NINE:
					InsertStringNumber('9', strValue);
					CalculateOperand();
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_ADDITION:
					if (!FinalCalcFlag)
						CalculateDoubleResult();
					OperatorType = ADD;
					InsertOperator('+', strValue);
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_SUBTRACTION:
					if (!FinalCalcFlag)
						CalculateDoubleResult();
					OperatorType = SUB;
					InsertOperator('-', strValue);
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_MULTIPLICATION:
					if (!FinalCalcFlag)
						CalculateDoubleResult();
					OperatorType = MUL;
					InsertOperator('x', strValue);
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
				break;
				case IDC_DIVISION:
					if (!FinalCalcFlag)
						CalculateDoubleResult();
					OperatorType = DIV;
					InsertOperator('/', strValue);
					SetDlgItemText(hwnd, IDC_RESULT, strValue);
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
		strValue += sprintf(strValue, "%s %c", strValue, Value);
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