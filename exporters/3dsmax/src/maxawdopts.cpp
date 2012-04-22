#include "maxawdopts.h"


MaxAWDExporterOpts::MaxAWDExporterOpts(void)
{
	// Default values
	exportGeometry = true;
	exportScene = true;

	exportMaterials = true;
	embedTextures = false;

	exportSkeletons = true;
	exportSkelAnim = true;
}


MaxAWDExporterOpts::~MaxAWDExporterOpts(void)
{
}


bool MaxAWDExporterOpts::ShowDialog(void)
{
	int ret = DialogBoxParam(hInstance, 
				MAKEINTRESOURCE(IDD_PANEL), 
				GetActiveWindow(), 
				DialogProc, (LPARAM)this);

	return (ret != 0);
}




INT_PTR CALLBACK MaxAWDExporterOpts::DialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static MaxAWDExporterOpts *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (MaxAWDExporterOpts *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}
