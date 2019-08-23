#include <windowsx.h>
#include <Windows.h>
#include <stdlib.h>

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define C_TO_F 4
#define F_TO_C 5
#define CONVERT_BUTTON 6

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWMD);
void AddControls(HWND);
float TemperatureConversionCtoF(float temperature);
float TemperatureConversionFtoC(float temperature);
int IntToStr(int x, char str[], int d);
void FloatToString(float n, char *res, int afterpoint);
void Reverse(char *str, int len);

HMENU hMenu;
HWND hTemperature, hOut, hCtoFButton, hFtoCButton;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
	WNDCLASSW wc = { 0 };

	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;

	if((!RegisterClassW(&wc)))
		return -1;

	CreateWindowW(L"myWindowClass", L"Temperature Converter", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100,100,250,500,
					NULL,NULL,NULL,NULL);
	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_COMMAND:
		
		switch (wp) {
		case FILE_MENU_EXIT:
			DestroyWindow(hWnd);
			break;
		case FILE_MENU_NEW:
			MessageBeep(MB_ICONINFORMATION);
			break;
		case C_TO_F:
			break;
		case F_TO_C:
			break;
		case CONVERT_BUTTON:
		{
			char temperature[16];
			char resultTemperature[16];
			float resultingTemp;

			GetWindowTextA(hTemperature, temperature, 16);
			float enteredTemperature = strtof(temperature, NULL);

			if(Button_GetCheck(hCtoFButton) != 0)
				resultingTemp = TemperatureConversionCtoF(enteredTemperature);
			else if(Button_GetCheck(hFtoCButton) != 0)
				resultingTemp = TemperatureConversionFtoC(enteredTemperature);
			
			FloatToString(resultingTemp, resultTemperature, 1);

			SetWindowTextA(hOut, resultTemperature);
		}
			break;
		}

		break;
	case WM_CREATE:
		AddMenus(hWnd);
		AddControls(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}

void AddMenus(HWND hWnd) {
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hSubMenu = CreateMenu();

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"Exit");

	SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd) {

	CreateWindowW(L"static", L"TEMPERATURE CONVERTER", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 17, 50, 200, 20, hWnd, NULL, NULL, NULL);

	CreateWindowW(L"static", L"Enter Temperature Here: ", WS_VISIBLE | WS_CHILD | SS_CENTER, 17, 100, 200, 20, hWnd, NULL, NULL, NULL);
	hTemperature = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 90, 125, 50, 20, hWnd, NULL, NULL, NULL);

	hCtoFButton = CreateWindowW(L"button", L"Celsius to Fahrenheit", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 17, 160, 200, 20, hWnd, (HMENU)C_TO_F, NULL, NULL);
	SendMessage(hCtoFButton, BM_SETCHECK, BST_CHECKED, 1);
	hFtoCButton = CreateWindowW(L"button", L"Fahrenheit to Celsius", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 17, 185, 200, 20, hWnd, (HMENU)F_TO_C, NULL, NULL);

	CreateWindowW(L"button", L"Convert", WS_VISIBLE | WS_CHILD, 17, 215, 200, 20, hWnd, (HMENU)CONVERT_BUTTON , NULL, NULL);

	CreateWindowW(L"static", L"Converted Temperature is:", WS_VISIBLE | WS_CHILD | SS_CENTER, 17, 250, 200, 20, hWnd, NULL, NULL, NULL);
	hOut = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 65, 275, 100, 20, hWnd, NULL, NULL, NULL);
}

float TemperatureConversionCtoF(float temperature){
	
	float result = 0;

	result = (temperature * (9.0/5.0)) + 32;

	return result;
}

float TemperatureConversionFtoC(float temperature) {

	float result = 0;

	result = (temperature - 32) * (5.0 / 9.0);

	return result;
}


int IntToStr(int x, char str[], int d) {
	int i = 0;
	while (x) {
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}

	while (i < d) {
		str[i++] = '0';
	}

	Reverse(str, i);
	str[i] = '\0';
	return i;
}

void FloatToString(float n, char *res, int afterpoint) {
	int ipart = (int)n;

	float fpart = n - (float)ipart;

	int i = IntToStr(ipart, res, 0);

	if (afterpoint != 0) {
		res[i] = '.';
	}

	fpart = fpart * pow(10, afterpoint);

	IntToStr((int)fpart, res + i + 1, afterpoint);
}

void Reverse(char *str, int len) {
	int i = 0, j = len - 1, temp;
	while (i < j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}