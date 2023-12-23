#define _USE_MATH_DEFINES
#include <afxwin.h>
#include <gl/GL.h>
#include <math.h>
#pragma comment (lib,"opengl32.lib")

class App : public CWinApp
{

public:
	virtual BOOL InitInstance();
};

App my_app;

class Win : public CFrameWnd
{
	HGLRC glRl;
	double aroundY;
	double aroundX;
public:
	Win(): glRl(NULL) { Create(NULL, L"Cube"); }
	~Win() { wglDeleteContext(glRl); }
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

BOOL App::InitInstance()
{
	// TODO: �������� ������������������ ��� ��� ����� �������� ������
	m_pMainWnd = new Win;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return CWinApp::InitInstance();
}
BEGIN_MESSAGE_MAP(Win, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


void Win::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �������� ���� ��� ����������� ���������
	// �� �������� CFrameWnd::OnPaint() ��� ��������� ���������
	CRect my_rect;
	GetClientRect(&my_rect);
	int width = my_rect.right - my_rect.left;
	int height = my_rect.bottom - my_rect.top;
	double stepFi = 0.05, stepPsi = 0.05, R = 0.3, stepR = 0.01, r = 0.1;
	wglMakeCurrent(dc, glRl);
	glViewport((width - height) / 2, 0, height, height);
	
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-1, 1, -1, 1, -1, 1);
	glFrustum(-0.5, 0.5, -0.5, 0.5, 1., 10.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0., 0., -2.);
	glRotated(aroundY, 0, 1, 0);
	glRotated(aroundX, 1, 0, 0);
	glBegin(GL_POINTS);
		/*glColor3d(1, 0, 0); glVertex3d(-0.5, -0.5, 0.5); //
		glColor3d(1, 1, 0); glVertex3d(0.5, -0.5, 0.5); //
		glColor3d(0, 0, 1); glVertex3d(0.5, 0.5, 0.5); //
		glColor3d(1, 0, 1); glVertex3d(-0.5, 0.5, 0.5); //

		glColor3d(0, 1, 1); glVertex3d(-0.5, -0.5, -0.5);
		glColor3d(1, 0, 0); glVertex3d(-0.5, 0.5, -0.5); //
		glColor3d(0, 1, 0); glVertex3d(0.5, 0.5, -0.5); //
		glColor3d(1, 0, 1); glVertex3d(0.5, -0.5, -0.5); //

		glColor3d(1, 1, 0); glVertex3d(0.5, -0.5, 0.5); //
		glColor3d(1, 0, 1); glVertex3d(0.5, -0.5, -0.5); //
		glColor3d(0, 1, 0); glVertex3d(0.5, 0.5, -0.5); //
		glColor3d(0, 0, 1); glVertex3d(0.5, 0.5, 0.5); //

		glColor3d(1, 0, 0); glVertex3d(-0.5, -0.5, 0.5); //
		glColor3d(1, 0, 1); glVertex3d(-0.5, 0.5, 0.5); //
		glColor3d(1, 0, 0); glVertex3d(-0.5, 0.5, -0.5); //
		glColor3d(0, 1, 1); glVertex3d(-0.5, -0.5, -0.5);

		glColor3d(1, 0, 1); glVertex3d(-0.5, 0.5, 0.5); //
		glColor3d(0, 0, 1); glVertex3d(0.5, 0.5, 0.5); //
		glColor3d(0, 1, 0); glVertex3d(0.5, 0.5, -0.5); //
		glColor3d(1, 0, 0); glVertex3d(-0.5, 0.5, -0.5); //

		glColor3d(1, 0, 0); glVertex3d(-0.5, -0.5, 0.5); //
		glColor3d(0, 1, 1); glVertex3d(-0.5, -0.5, -0.5);
		glColor3d(1, 0, 1); glVertex3d(0.5, -0.5, -0.5); //
		glColor3d(1, 1, 0); glVertex3d(0.5, -0.5, 0.5); // */

		for (double psi = -M_PI; psi < M_PI; psi += stepPsi)
		{
			for (double fi = 0.; fi < 2 * M_PI; fi += stepFi)
			{
				glColor3d(sin(psi), sin(fi), cos(psi)); glVertex3d((R + r * cos(psi)) * cos(fi), (R + r * cos(psi)) * sin(fi), r * sin(psi));
			}
		}
	glEnd();

	glFlush();
	SwapBuffers(dc);
	wglMakeCurrent(NULL, NULL);
}


void Win::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO: �������� ���� ��� ����������� ���������
	if (glRl != NULL) wglDeleteContext(glRl);
	CDC* dc = GetDC();
	PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32 };
	pfd.cDepthBits = 24;
	int pixelFormat = ChoosePixelFormat(*dc, &pfd);
	SetPixelFormat(*dc, pixelFormat, &pfd);
	glRl = wglCreateContext(*dc);
	ReleaseDC(dc);
	Invalidate(FALSE);
}


void Win::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �������� ���� ��� ����������� ��������� ��� ����� ������������

	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	if (nChar == VK_LEFT) aroundY++;
	if (nChar == VK_RIGHT) aroundY--;
	if (nChar == VK_UP) aroundX--;
	if (nChar == VK_DOWN) aroundX++;
	Invalidate(FALSE);
}
