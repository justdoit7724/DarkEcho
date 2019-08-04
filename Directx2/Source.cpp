#include "Window.h"
#include "Graphic.h"
#include "Scene.h"
#include "Game_info.h"

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	Window window(hInstance, "Dark Echo");
	Graphic graphic(window.Hwnd());
	Scene scene(graphic.Device(), graphic.DContext());

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		scene.Update();
		scene.Render();
		graphic.Present();
	}

	DestroyWindow(window.Hwnd());

	return 0;
}