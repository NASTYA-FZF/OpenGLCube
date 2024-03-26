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
	bool ortho;
public:
	Win() : glRl(NULL) { Create(NULL, L"Cube"); ortho = true; }
	~Win() { wglDeleteContext(glRl); }
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

BOOL App::InitInstance()
{
	// TODO: добавьте специализированный код или вызов базового класса
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

void VertexTop(double R, double r, double psi, double fi)
{
	glVertex3d((R + r * cos(psi)) * cos(fi), (R + r * cos(psi)) * sin(fi), r * sin(psi));
}

void DrawCube()
{
	glBegin(GL_QUADS);
	glColor3d(1, 0, 0); glVertex3d(-0.5, -0.5, 0.5); //
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
	glColor3d(1, 1, 0); glVertex3d(0.5, -0.5, 0.5); // 
	glEnd();
}

void SetProjection(bool ortho)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (ortho) glOrtho(-1, 1, -1, 1, -1, 1);
	else glFrustum(-0.5, 0.5, -0.5, 0.5, 1., 10.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (!ortho) glTranslated(0., 0., -2.);
}

void Win::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: добавьте свой код обработчика сообщений
	// Не вызывать CFrameWnd::OnPaint() для сообщений рисования
	CRect my_rect;
	GetClientRect(&my_rect);
	int width = my_rect.right - my_rect.left;
	int height = my_rect.bottom - my_rect.top;
	double stepFi = 0.1, stepPsi = 0.1, R = 0.5, r = 0.2;
	wglMakeCurrent(dc, glRl);
	glViewport((width - height) / 2, 0, height, height);
	
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	SetProjection(ortho);
	glRotated(aroundY, 0, 1, 0);
	glRotated(aroundX, 1, 0, 0);

	DrawCube();

	glFlush();
	SwapBuffers(dc);
	wglMakeCurrent(NULL, NULL);
}


void Win::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO: добавьте свой код обработчика сообщений
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
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного

	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	if (nChar == VK_LEFT) aroundY++;
	if (nChar == VK_RIGHT) aroundY--;
	if (nChar == VK_UP) aroundX--;
	if (nChar == VK_DOWN) aroundX++;
	if (nChar == 0x4F) ortho = true;
	if (nChar == 0x50) ortho = false;
	Invalidate(FALSE);
}
