#include <windows.h> 
#include <sstream>

#include "Equation_Y1.h"

#define ID_LIST 1
#define ID_LIST_2 2

#define ID_BTN_1 1
#define ID_BTN_2 2

LONG WINAPI WndProc(HWND, UINT, WPARAM, LPARAM); // ôóíêöèÿ îáðàáîòêè ñîîáùåíèé îêíà
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM); //ôóíêöèÿ îáðàáîòêè ñîîáùåíèé äèàëîãà

/*----------------
Ñòàðòîâàÿ ôóíêöèÿ
-----------------*/
int  PASCAL  WinMain(HINSTANCE  hInstance,
	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd; // äåñêðèïòîð îêíà
	MSG msg;   // ñòðóêòóðà ñîîáùåíèÿ
	WNDCLASS w; // ñòðóêòóðà êëàññà îêíà
	RECT desctopRect;
	GetWindowRect(GetDesktopWindow(), &desctopRect);
	COLORREF backGroundColor = RGB(23, 72, 104);
	memset(&w, 0, sizeof(WNDCLASS)); // î÷èñòêà ïàìÿòè äëÿ ñòðóêòóðû
	w.style = CS_HREDRAW | CS_VREDRAW;
	w.lpfnWndProc = WndProc;
	w.hInstance = hInstance;
	w.hbrBackground = CreateSolidBrush(backGroundColor);
	w.lpszClassName = "MyClass";
	RegisterClass(&w); // ðåãèñòðàöèÿ êëàññà îêíà
	// Ñîçäàíèå îêíà
	hwnd = CreateWindow("MyClass", "Runge_Kutta",
		WS_OVERLAPPEDWINDOW,
		desctopRect.left, desctopRect.top, desctopRect.right, desctopRect.bottom - 40,
		NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow); // îòîáðàæåíèå îêíà
	UpdateWindow(hwnd);         // ïåðåðèñîâêà îêíà

	// Öèêë îáðàáîòêè ñîîáùåíèé
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

/*-------------------------
Ôóíêöèÿ îáðàáîòêè ñîîáùåíèé
--------------------------*/
LONG WINAPI WndProc(HWND hwnd, UINT Message,
	WPARAM wparam, LPARAM lparam)
{
	static HINSTANCE hInst;
	PAINTSTRUCT ps;
	HDC hdc;

	static HWND hBtn1, hBtn2; // äåñêðèïòîðû êíîïîê

	//Äåñêðèïòîðû ñòàòè÷åñêèõ ïîëåé íà÷àëüíûõ çíà÷åíèé
	static HWND
		hStaticXP_0,		//
		hStaticX1_0,		//
		hStaticYP_0,		//
		hStaticY1_0,		//
		hStaticL,			//
		hStaticL_0,			//
		hStatic_nu,			//
		hStatic_mu,			//
		hStatic_p,			//
		hStatic_q,			//
		hStatic_Ftrait,		//
		hStatic_tfirst,		//
		hStatic_tlast;		//

	//Äåñêðèïòîðû ïîëåé ââîäà íà÷àëüíûõ çíà÷åíèé
	static HWND
		hEditXP_0,		//
		hEditX1_0,		//
		hEditYP_0,		//
		hEditY1_0,		//
		hEditL,			//
		hEditL_0,		//
		hEdit_nu,		//
		hEdit_mu,		//
		hEdit_p,		//
		hEdit_q,		//
		hEdit_Ftrait,	//
		hEdit_tfirst,	//
		hEdit_tlast;	//





	static HWND hList1, hList2; // äåñêðèïòîðû ñïèñêîâ çíà÷åíèé ôóíêöèé
	RECT rekt;

	//Êîîðäèíàòû ýëåìåòîâ îêíà:
	int ListY = 10,							//âûñîòà ñïèñêîâ çíà÷åíèé ôóêöèé
		ListWidth = LOWORD(lparam) / 8,		//èõ øèðèíà
		ListHeight = HIWORD(lparam),		//èõ âûñîòà

		tauY = 10,							//Y-êîîðäèíàòà ïîëåé ââîäà òàó

		tauStaticFirst_X = 400,				//X-êîîðäèíàòà ñòàòè÷åñêîãî ïîëÿ òàó ïåðâîãî
		tauEditFirst_X = 450,				//X-êîîðäèíàòà ïîëÿ ââîäà òàó ïåðâîãî

		tauStaticLast_X = 520,				//X-êîîðäèíàòà ñòàòè÷åñêîãî ïîëÿ òàó ïîñëåäíåãî
		tauEditLast_X = 570,				//X-êîîðäèíàòà ïîëÿ ââîäà òàó ïîñëåäíåãî


		initialValuesX = 450,				//X-êîîðäèíàòà ïîëåé ââîäà äðóãèõ íà÷àëüíûõ çíà÷åíèé 
		initialValuesWidth = 60,			//øèðèíà ïîëåé ââîäà äðóãèõ íà÷àëüíûõ çíà÷åíèé
		initialValuesHeight = 22,			//âûñîòà ïîëåé ââîäà äðóãèõ íà÷àëüíûõ çíà÷åíèé

		initialValueStaticWidth = 50,		//øèðèíà ñòàòè÷åñêèõ ïîëåé íà÷àëüíûõ çíà÷åíèé

		initialValuesStaticX = 400,			//X-êîîðäèíàòà ñòàòè÷åñêèõ ïîëåé íà÷àëüíûõ çíà÷åíèé

		//Y-êîîðäèíàòû äëÿ ñîîòâåòñòâóþùèõ ïîëåé ââîäà
		XP_0_Ypos = 40,
		X1_0_Ypos = 70,
		YP_0_Ypos = 100,
		Y1_0_Ypos = 130,
		L_Ypos = 160,
		L_0_Ypos = 190,
		nu_Ypos = 220,
		mu_Ypos = 250,
		p_Ypos = 280,
		q_Ypos = 310,
		Ftrait_Ypos = 340;

	switch (Message)
	{
	case WM_CREATE: // ñîîáùåíèå ñîçäàíèÿ îêíà
		hInst = ((LPCREATESTRUCT)lparam)->hInstance; // äåñêðèïòîð ïðèëîæåíèÿ
		GetWindowRect(hwnd, &rekt);

		//ñîçäàíèå ñïèñêîâ çíà÷åíèé ôóíêöèé
		hList1 = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_USETABSTOPS, 
			10, ListY, ListWidth, ListHeight, hwnd, (HMENU)ID_LIST, hInst, NULL);
		ShowWindow(hList1, SW_SHOWNORMAL);
	
		hList2 = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_USETABSTOPS, 
			rekt.right / 6, ListY, ListWidth, ListHeight, hwnd, (HMENU)ID_LIST_2, hInst, NULL);
		ShowWindow(hList2, SW_SHOWNORMAL);

		//ñîçäàíèå êíîïîê

		hBtn1 = CreateWindow("button", "Ðàññ÷èòàòü", WS_CHILD | WS_VISIBLE | WS_BORDER,
			400, 370, 120, 30, hwnd, (HMENU)ID_BTN_1, hInst, NULL);

		hBtn2 = CreateWindow("button", "Cáðîñ", WS_CHILD | WS_VISIBLE | WS_BORDER,
			400, 410, 120, 30, hwnd, (HMENU)ID_BTN_2, hInst, NULL);

		//ñòàòè÷åñêèå ïîëÿ íà÷àëüíûõ çà÷åíèé

		hStatic_tfirst = CreateWindow("static", NULL, WS_CHILD | WS_VISIBLE | SS_RIGHT,
			tauStaticFirst_X, tauY , initialValueStaticWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hStatic_tfirst, SW_SHOWNORMAL);
		SetWindowText(hStatic_tfirst, "tau_0=");

		hStatic_tlast = CreateWindow("static", NULL, WS_CHILD | WS_VISIBLE | SS_RIGHT,
			tauStaticLast_X, tauY, initialValueStaticWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hStatic_tlast, SW_SHOWNORMAL);
		SetWindowText(hStatic_tlast, "tau_N=");

		hStaticXP_0 = CreateWindow("static", NULL, WS_CHILD | WS_VISIBLE | SS_RIGHT,
			initialValuesStaticX, XP_0_Ypos, initialValueStaticWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hStaticXP_0, SW_SHOWNORMAL);
		SetWindowText(hStaticXP_0, "X_p(0)=");

		hStaticX1_0 = CreateWindow("static", NULL, WS_CHILD | WS_VISIBLE | SS_RIGHT,
			initialValuesStaticX, X1_0_Ypos, initialValueStaticWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hStaticX1_0, SW_SHOWNORMAL);
		SetWindowText(hStaticX1_0, "X_1(0)=");

		hStaticYP_0 = CreateWindow("static", NULL, WS_CHILD | WS_VISIBLE | SS_RIGHT,
			initialValuesStaticX, YP_0_Ypos, initialValueStaticWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hStaticYP_0, SW_SHOWNORMAL);
		SetWindowText(hStaticYP_0, "Y_p(0)=");

		hStaticY1_0 = CreateWindow("static", NULL, WS_CHILD | WS_VISIBLE | SS_RIGHT,
			initialValuesStaticX, Y1_0_Ypos, initialValueStaticWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hStaticY1_0, SW_SHOWNORMAL);
		SetWindowText(hStaticY1_0, "Y_1(0)=");

		hStaticL = CreateWindow("static", NULL, WS_CHILD | WS_VISIBLE | SS_RIGHT,
			initialValuesStaticX, L_Ypos, initialValueStaticWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hStaticL, SW_SHOWNORMAL);
		SetWindowText(hStaticL, "L=");

		hStaticL_0 = CreateWindow("static", NULL, WS_CHILD | WS_VISIBLE | SS_RIGHT,
			initialValuesStaticX, L_0_Ypos, initialValueStaticWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hStaticL_0, SW_SHOWNORMAL);
		SetWindowText(hStaticL_0, "L_0=");

		hStatic_nu = CreateWindow("static", NULL, WS_CHILD | WS_VISIBLE | SS_RIGHT,
			initialValuesStaticX, nu_Ypos, initialValueStaticWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hStatic_nu, SW_SHOWNORMAL);
		SetWindowText(hStatic_nu, "nu=");

		hStatic_mu = CreateWindow("static", NULL, WS_CHILD | WS_VISIBLE | SS_RIGHT,
			initialValuesStaticX, mu_Ypos, initialValueStaticWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hStatic_mu, SW_SHOWNORMAL);
		SetWindowText(hStatic_mu, "mu=");

		hStatic_p = CreateWindow("static", NULL, WS_CHILD | WS_VISIBLE | SS_RIGHT,
			initialValuesStaticX, p_Ypos, initialValueStaticWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hStatic_p, SW_SHOWNORMAL);
		SetWindowText(hStatic_p, "p=");

		hStatic_q = CreateWindow("static", NULL, WS_CHILD | WS_VISIBLE | SS_RIGHT,
			initialValuesStaticX, q_Ypos, initialValueStaticWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hStatic_q, SW_SHOWNORMAL);
		SetWindowText(hStatic_q, "q=");

		hStatic_Ftrait = CreateWindow("static", NULL, WS_CHILD | WS_VISIBLE | SS_RIGHT,
			initialValuesStaticX, Ftrait_Ypos, initialValueStaticWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hStatic_Ftrait, SW_SHOWNORMAL);
		SetWindowText(hStatic_Ftrait, "~F=");

		//ïîëÿ ââîäà íà÷àëüíûõ çíà÷åíèé
		
		hEdit_tfirst = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			tauEditFirst_X, tauY, initialValuesWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hEdit_tfirst, SW_SHOWNORMAL);

		hEdit_tlast = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			tauEditLast_X, tauY, initialValuesWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hEdit_tlast, SW_SHOWNORMAL);

		hEditXP_0 = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			initialValuesX, XP_0_Ypos, initialValuesWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hEditXP_0, SW_SHOWNORMAL);
		
		hEditX1_0 = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			initialValuesX, X1_0_Ypos, initialValuesWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hEditX1_0, SW_SHOWNORMAL);
		
		hEditYP_0 = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			initialValuesX, YP_0_Ypos, initialValuesWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hEditYP_0, SW_SHOWNORMAL);
		
		hEditY1_0 = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			initialValuesX, Y1_0_Ypos, initialValuesWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hEditY1_0, SW_SHOWNORMAL);
		
		hEditL = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 
			initialValuesX, L_Ypos, initialValuesWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hEditL, SW_SHOWNORMAL);

		hEditL_0 = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			initialValuesX, L_0_Ypos, initialValuesWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hEditL_0, SW_SHOWNORMAL);

		hEdit_nu = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			initialValuesX, nu_Ypos, initialValuesWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hEdit_nu, SW_SHOWNORMAL);

		hEdit_mu = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			initialValuesX, mu_Ypos, initialValuesWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hEdit_mu, SW_SHOWNORMAL);

		hEdit_p = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			initialValuesX, p_Ypos, initialValuesWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hEdit_p, SW_SHOWNORMAL);

		hEdit_q = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			initialValuesX, q_Ypos, initialValuesWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hEdit_q, SW_SHOWNORMAL);

		hEdit_Ftrait = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			initialValuesX, Ftrait_Ypos, initialValuesWidth, initialValuesHeight, hwnd, 0, hInst, NULL);
		ShowWindow(hEdit_Ftrait, SW_SHOWNORMAL);

		/*SendMessage(hList1, WM_SETREDRAW, TRUE, 0L);
		InvalidateRect(hList1, NULL, TRUE);*/

		break;
	case WM_COMMAND:  // ñîîáùåíèå î êîìàíäå
		// åñëè íàæàëè êíîïêó "Ðàññ÷èòàòü"
		if (lparam == (LPARAM)hBtn1)
		{
			char* value = new char[7];
			double initialValues[13];

			//èçâëåêàåì çíà÷åèÿ èç êàæäîãî ïîëÿ ââîäà
			GetWindowText(hEdit_tfirst, value, 7);
			initialValues[0] = std::stod(std::string(value));

			GetWindowText(hEdit_tlast, value, 7);
			initialValues[1] = std::stod(std::string(value));

			GetWindowText(hEditXP_0, value, 7);
			initialValues[2] = std::stod(std::string(value));

			GetWindowText(hEditX1_0, value, 7);
			initialValues[3] = std::stod(std::string(value));

			GetWindowText(hEditYP_0, value, 7);
			initialValues[4] = std::stod(std::string(value));

			GetWindowText(hEditY1_0, value, 7);
			initialValues[5] = std::stod(std::string(value));

			GetWindowText(hEditL, value, 7);
			initialValues[6] = std::stod(std::string(value));

			GetWindowText(hEditL_0, value, 7);
			initialValues[7] = std::stod(std::string(value));

			GetWindowText(hEdit_nu, value, 7);
			initialValues[8] = std::stod(std::string(value));

			GetWindowText(hEdit_mu, value, 7);
			initialValues[9] = std::stod(std::string(value));

			GetWindowText(hEdit_p, value, 7);
			initialValues[10] = std::stod(std::string(value));

			GetWindowText(hEdit_q, value, 7);
			initialValues[11] = std::stod(std::string(value));

			GetWindowText(hEdit_Ftrait, value, 7);
			initialValues[12] = std::stod(std::string(value));

			//Ñîçäà¸ì îáúåêò êëàññà óðàâíåíèÿ. Ââîäèì íà÷àëüíûå çíà÷åíèÿ â êîíñòðóêòîð
			Equation_Y1 Y1(initialValues[2], initialValues[3], initialValues[4], initialValues[5], initialValues[6],
				initialValues[7], initialValues[8], initialValues[9], initialValues[10], initialValues[11],
				initialValues[12], initialValues[0], initialValues[1]);

			//Ïðîâîäèì ðàñ÷¸òû
			Y1.Calculation();

			//Çàïîëíÿåì ñïèñêè ðåçóëüòàòàìè
			for (size_t i = 0; i < Y1.XP.size(); i++)
			{
				std::string strbuf = std::to_string(Y1.XP[i]);
				SendMessage(hList1, LB_ADDSTRING, 0, (LPARAM)strbuf.data());
			}

			for (size_t i = 0; i < Y1.X1.size(); i++)
			{
				std::string strbuf = std::to_string(Y1.X1[i]);
				SendMessage(hList2, LB_ADDSTRING, 0, (LPARAM)strbuf.data());
			}

			Y1.X1.clear();
			Y1.XP.clear();
			Y1.Y1.clear();
			Y1.YP.clear();
		}
		// åñëè íàæàëè êíîïêó "Ñáðîñ"
		if (lparam == (LPARAM)hBtn2)
		{
			//Óäàëÿåì äàííûå èç ñïèñêîâ
			SendMessage(hList1, LB_RESETCONTENT, 0, 0L);
			SendMessage(hList2, LB_RESETCONTENT, 0, 0L);
		}
		break;
	case WM_CHARTOITEM:
		if (wparam == VK_DOWN)
		{
			
		}
		if (wparam == VK_UP)
		{
			
		}
		break;
	case WM_PAINT: // ïåðåðèñîâêà îêíà
		hdc = BeginPaint(hwnd, &ps); // íà÷àëî ïåðåðèñîâêè

		EndPaint(hwnd, &ps); // êîíåö ïåðåðèñîâêè
		break;
	case WM_SIZE:
		switch (wparam)
		{
		case SIZE_MAXIMIZED:
			MoveWindow(hList1, 10, ListY, ListWidth, ListHeight, TRUE);
			MoveWindow(hList2, LOWORD(lparam) / 6, ListY, ListWidth, ListHeight, TRUE);

			MoveWindow(hBtn1, 400, 370, 120, 30, TRUE);

			/*MoveWindow(hStaticXP_0, initialValuesStaticX, XP_0_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStaticX1_0, initialValuesStaticX, X1_0_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStaticYP_0, initialValuesStaticX, YP_0_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStaticY1_0, initialValuesStaticX, Y1_0_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStaticL, initialValuesStaticX, L_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStaticL_0, initialValuesStaticX, L_0_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStatic_nu, initialValuesStaticX, nu_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStatic_mu, initialValuesStaticX, mu_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStatic_p, initialValuesStaticX, p_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStatic_q, initialValuesStaticX, q_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStatic_Ftrait, initialValuesStaticX, Ftrait_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);*/

			MoveWindow(hEditXP_0, initialValuesX, XP_0_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEditX1_0, initialValuesX, X1_0_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEditYP_0, initialValuesX, YP_0_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEditY1_0, initialValuesX, Y1_0_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEditL, initialValuesX, L_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEditL_0, initialValuesX, L_0_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEdit_nu, initialValuesX, nu_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEdit_mu, initialValuesX, mu_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEdit_p, initialValuesX, p_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEdit_q, initialValuesX, q_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEdit_Ftrait, initialValuesX, Ftrait_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			break;
		case SIZE_RESTORED:
			MoveWindow(hList1, 10, ListY, ListWidth, ListHeight, TRUE);
			MoveWindow(hList2, LOWORD(lparam) / 6, ListY, ListWidth, ListHeight, TRUE);

			MoveWindow(hBtn1, 400, 370, 120, 30, TRUE);

			/*MoveWindow(hStaticXP_0, initialValuesStaticX, XP_0_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStaticX1_0, initialValuesStaticX, X1_0_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStaticYP_0, initialValuesStaticX, YP_0_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStaticY1_0, initialValuesStaticX, Y1_0_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStaticL, initialValuesStaticX, L_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStaticL_0, initialValuesStaticX, L_0_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStatic_nu, initialValuesStaticX, nu_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStatic_mu, initialValuesStaticX, mu_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStatic_p, initialValuesStaticX, p_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStatic_q, initialValuesStaticX, q_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);
			MoveWindow(hStatic_Ftrait, initialValuesStaticX, Ftrait_Ypos, initialValueStaticWidth, initialValuesHeight, TRUE);*/

			MoveWindow(hEditXP_0, initialValuesX, XP_0_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEditX1_0, initialValuesX, X1_0_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEditYP_0, initialValuesX, YP_0_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEditY1_0, initialValuesX, Y1_0_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEditL, initialValuesX, L_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEditL_0, initialValuesX, L_0_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEdit_nu, initialValuesX, nu_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEdit_mu, initialValuesX, mu_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEdit_p, initialValuesX, p_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEdit_q, initialValuesX, q_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			MoveWindow(hEdit_Ftrait, initialValuesX, Ftrait_Ypos, initialValuesWidth, initialValuesHeight, TRUE);
			break;
		}
		break;

	case WM_DESTROY: // çàêðûòèå îêíà
		PostQuitMessage(0);
		break;

	default: // îáðàáîòêà ñîîáùåíèÿ ïî óìîë÷àíèþ
		return DefWindowProc(hwnd, Message, wparam, lparam);
	}
	return 0;
}
