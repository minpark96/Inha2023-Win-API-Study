// WinAPI_Proj.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
//

#include "framework.h"
#include "WinAPI_Proj.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIPROJ, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIPROJ));

    MSG msg;

    // �⺻ �޽��� �����Դϴ�:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  �뵵: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIPROJ));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPIPROJ);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   �뵵: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   �ּ�:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

#define timer_ID_1 1

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    using namespace std;

    PAINTSTRUCT ps;
    HDC hdc;

    static list<CObject*> objs;
    static POINT ptMousePos;
    static RECT rectView;
    static bool bFlag = false;
    enum mode { REFLECTION, COMBINATION, DECOMPOSITION };
    static int seletedMode = REFLECTION;
    enum type { NONE, CIRCLE, RECTANGLE, STAR };

    switch (message)
    {
    case WM_CREATE:
        srand(time(NULL));
        GetClientRect(hWnd, &rectView);
        SetTimer(hWnd, timer_ID_1, 20, NULL);
        break;
    case WM_TIMER:
        if (wParam == timer_ID_1)
        {
            switch (seletedMode)
            {
            case REFLECTION:
            {
                for (auto it1 = objs.begin(); it1 != objs.end(); ++it1)
                {
                    auto it2 = it1;
                    for (++it2; it2 != objs.end(); ++it2)
                    {
                        if ((*it1)->Collision(**it2))
                        {
                            (*it1)->SetPosition(**it2);
                            do
                            {
                                (*it1)->Update(&rectView);
                                (*it2)->Update(&rectView);
                            } while ((*it1)->Collision(**it2));
                        }
                    }

                    (*it1)->Update(&rectView);
                }
            }
            break;
            case COMBINATION:
            {
                auto it1 = objs.begin();

                while (it1 != objs.end())
                {
                    bool it1Erased = false;
                    auto it2 = it1;
                    ++it2;
                    while (it2 != objs.end())
                    {
                        if ((*it1)->Collision(**it2))
                        {
                            if ((*it1)->GetType() == (*it2)->GetType())
                            {
                                if((*it1)->Combination(**it2) == 0)
                                {
                                    objs.erase(it2);
                                    it1 = objs.erase(it1);
                                    it1Erased = true;
                                    break;
                                }
                                else if ((*it1)->Combination(**it2) == 1)
                                {
                                    it1 = objs.erase(it1);
                                    it1Erased = true;
                                    break;
                                }
                                else if ((*it1)->Combination(**it2) == 2)
                                {
                                    it2 = objs.erase(it2);
                                    continue;
                                }
                            }
                            else
                            {
                                (*it1)->SetPosition(**it2);
                                do
                                {
                                    (*it1)->Update(&rectView);
                                    (*it2)->Update(&rectView);
                                } while ((*it1)->Collision(**it2));
                            }
                        }
                        
                        ++it2;
                    }

                    (*it1)->Update(&rectView);

                    if (!it1Erased)
                    {
                        ++it1;
                    }
                }
            }
            break;
            case DECOMPOSITION:
            {
                for (auto it1 = objs.begin(); it1 != objs.end(); ++it1)
                {
                    auto it2 = it1;
                    for (++it2; it2 != objs.end(); ++it2)
                    {
                        if ((*it1)->Collision(**it2))
                        {
                            if ((*it1)->GetType() == (*it2)->GetType())
                                (*it1)->SetPosition(**it2);
                            else
                                (*it1)->SetPosition(**it2);
                            do
                            {
                                (*it1)->Update(&rectView);
                                (*it2)->Update(&rectView);
                            } while ((*it1)->Collision(**it2));
                        }
                    }

                    (*it1)->Update(&rectView);
                }
            }
            break;
            }
            
            InvalidateRgn(hWnd, NULL, TRUE);
        }
        break;
    case WM_LBUTTONDOWN:
    {
        ptMousePos.x = LOWORD(lParam);
        ptMousePos.y = HIWORD(lParam);

        int type = rand() % 3 + 1;
        switch (type)
        {
        case CIRCLE:
            objs.push_back(new CCircle(ptMousePos, CIRCLE));
            break;
        case RECTANGLE:
            objs.push_back(new CRectangle(ptMousePos, RECTANGLE));
            break;
        case STAR:
            objs.push_back(new CStar(ptMousePos, STAR));
            break;
        default:
            break;
        }

        InvalidateRgn(hWnd, NULL, TRUE);
    }
    break;
    case WM_KEYDOWN:
    {
        if (wParam == '1')
        {
            seletedMode = REFLECTION;
        }
        else if (wParam == '2')
        {
            seletedMode = COMBINATION;
        }
        else if (wParam == '3')
        {
            seletedMode = DECOMPOSITION;
        }
    }
    break;
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...

        for (CObject* obj : objs)
        {
            obj->Draw(hdc);
        }

        EndPaint(hWnd, &ps);
    }
    break;


    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // �޴� ������ ���� �м��մϴ�:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        KillTimer(hWnd, timer_ID_1);
        PostQuitMessage(0);

        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
