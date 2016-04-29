/*
 * Perya Kayou Typing Patcher
 * Copyright(C) 2010 peryaudo.All rights reserved.
 *
 * This program is licensed under MIT License.
 *
 * Last Modified: 2010/05/16
 *
 */
#include <windows.h>
#include <tlhelp32.h>

#define WM_TRAYICON WM_USER + 1
#define PROGRAM_TITLE "Peryaudo Kayou Typing Patcher"

HINSTANCE m_hInstance;

int RegisterNotifyIcon(HWND hWnd){
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = 0;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_TRAYICON;
	nid.hIcon = LoadIcon(m_hInstance, "PERYAICO");
	lstrcpy((LPSTR)&nid.szTip,PROGRAM_TITLE);

	Shell_NotifyIcon(NIM_ADD, &nid);
	
	return 0;
}

int UnregisterNotifyIcon(HWND hWnd){
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = 0;
	nid.uFlags = 0;

	Shell_NotifyIcon(NIM_DELETE, &nid);
	return 0;
}

int CompareArray(int len, unsigned char *a, unsigned char *b){
	int i = 0;
	for(i = 0; i < len; i++){
		if(*(a + i) != *(b + i)) return 0;
	}
	return 1;
}

int PatchProcess(DWORD dwProcessId){
	/* This code will rewrite following instructions: 
	 *
	 *   004307AD  |. 0F85 E3000000  JNZ Typing.00430896
	 *   004307BA  |. 0F85 D6000000  JNZ Typing.00430896
	 *
	 */
	HANDLE hProcess;
	unsigned char current[6];
	unsigned char patched[6] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	unsigned char firstJNZ[6] = { 0x0F, 0x85, 0xE3, 0x00, 0x00, 0x00 };
	unsigned char secondJNZ[6] = { 0x0F, 0x85, 0xD6, 0x00, 0x00, 0x00 };
	const unsigned int firstJNZAddress = 0x004307AD;
	const unsigned int secondJNZAddress = 0x004307BA;

	if((hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwProcessId)) == NULL){
		MessageBox(NULL, "OpenProcess failed", PROGRAM_TITLE, MB_OK | MB_ICONERROR);
		ExitProcess(-1);
		return -1;
	}

	/* first JNZ */
	if(!ReadProcessMemory(hProcess, (LPCVOID)firstJNZAddress, (LPVOID)current, 6, NULL)){
		MessageBox(NULL, "ReadProcessMemory failed", PROGRAM_TITLE, MB_OK | MB_ICONERROR);
		ExitProcess(-1);
		return -1;
	}

	if(!CompareArray(6, firstJNZ, current)){
		MessageBox(NULL, "Typing.exe has changed since the patcher was written.", PROGRAM_TITLE, MB_OK | MB_ICONERROR);
		ExitProcess(-1);
		return -1;
	}

	/* second JNZ */
	if(!ReadProcessMemory(hProcess, (LPCVOID)secondJNZAddress, (LPVOID)current, 6, NULL)){
		MessageBox(NULL, "ReadProcessMemory failed", PROGRAM_TITLE, MB_OK | MB_ICONERROR);
		ExitProcess(-1);
		return -1;
	}

	if(!CompareArray(6, secondJNZ, current)){
		MessageBox(NULL, "Typing.exe has changed since the patcher was written.", PROGRAM_TITLE, MB_OK | MB_ICONERROR);
		ExitProcess(-1);
		return -1;
	}

	if(!WriteProcessMemory(hProcess, (LPVOID)firstJNZAddress, (LPVOID)patched, 6, NULL)){
		MessageBox(NULL, "WriteProcessMemory failed", PROGRAM_TITLE, MB_OK | MB_ICONERROR);
		ExitProcess(-1);
		return -1;
	}

	if(!WriteProcessMemory(hProcess, (LPVOID)secondJNZAddress, (LPVOID)patched, 6, NULL)){
		MessageBox(NULL, "WriteProcessMemory failed", PROGRAM_TITLE, MB_OK | MB_ICONERROR);
		ExitProcess(-1);
		return -1;
	}

	CloseHandle(hProcess);

	return 0;
}
int SearchProcesses(HWND hWnd){
	HANDLE hSnap;
	PROCESSENTRY32 pe;
	DWORD dwProcessId = 0;
	static DWORD dwProcessIdLast;
	BOOL bResult;

	if((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE){
		MessageBox(NULL, "CreateToolhelp32Snapshot() failed", PROGRAM_TITLE, MB_OK | MB_ICONERROR);
		ExitProcess(-1);
		return 0;
	}

	pe.dwSize = sizeof(pe);

	bResult = Process32First(hSnap, &pe);

	while(bResult){
		if(!lstrcmpi(pe.szExeFile, "Typing.exe")){
			dwProcessId = pe.th32ProcessID;
			break;
		}
		bResult = Process32Next(hSnap, &pe);
	}

	CloseHandle(hSnap);

	if(dwProcessId != 0 && dwProcessId != dwProcessIdLast){
		dwProcessIdLast = dwProcessId;
		PatchProcess(dwProcessId);
	}



	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			RegisterNotifyIcon(hWnd);
			SetTimer(hWnd, /* id */0, 1/* msecs */, NULL);
			break;
		case WM_TIMER:
			SearchProcesses(hWnd);
			break;
		case WM_TRAYICON:
			switch(lParam){
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
					if(MessageBox(hWnd, "I—¹‚µ‚Ü‚·‚©H", PROGRAM_TITLE, MB_YESNO | MB_SYSTEMMODAL) == IDYES){
						DestroyWindow(hWnd);
					}
					break;
			}
			break;
		case WM_DESTROY:
			UnregisterNotifyIcon(hWnd);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPTSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wndClassEx;
	HWND hWnd;
	MSG msg;

	if(FindWindow("PeryaKTPatcher", NULL) != NULL) return 0;

	m_hInstance = hInstance;

	/* Create a window class */
	wndClassEx.cbSize = sizeof(WNDCLASSEX);
	wndClassEx.style = 0;
	wndClassEx.lpfnWndProc = WndProc;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = hInstance;
	wndClassEx.hIcon = LoadIcon(hInstance, "PERYAICO");
	wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClassEx.lpszMenuName = NULL;
	wndClassEx.lpszClassName = "PeryaKTPatcher";
	wndClassEx.hIconSm = wndClassEx.hIcon;
	if(!RegisterClassEx(&wndClassEx)) return 0;

	hWnd = CreateWindow("PeryaKTPatcher", PROGRAM_TITLE, 0,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
		NULL, NULL, hInstance, NULL);
	if(hWnd == NULL) return 0;

	/* Message loop */
	while(GetMessage(&msg, NULL, 0, 0) != FALSE){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DestroyWindow(hWnd);
	UnregisterClass(wndClassEx.lpszClassName, hInstance);

	return msg.wParam;
}
