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


// Static members
MaxAWDExporterOpts *MaxAWDExporterOpts::imp = NULL;
HWND MaxAWDExporterOpts::miscOpts = NULL;
HWND MaxAWDExporterOpts::mtlOpts = NULL;


MaxAWDExporterOpts::~MaxAWDExporterOpts(void)
{
}


bool MaxAWDExporterOpts::ShowDialog(void)
{
	int ret = DialogBoxParam(hInstance, 
				MAKEINTRESOURCE(IDD_AWD_OPTIONS), 
				GetActiveWindow(), 
				DialogProc, (LPARAM)this);

	return (ret == IDOK);
}



INT_PTR CALLBACK MaxAWDExporterOpts::DialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message) {
		case WM_INITDIALOG:
			imp = (MaxAWDExporterOpts *)lParam;
			InitDialog(hWnd, message, wParam, lParam);
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_COMMAND:
			switch (wParam) {
				case IDC_OK:
					// TODO: Set options
					EndDialog(hWnd, IDOK);
					break;

				case IDC_CANCEL:
					EndDialog(hWnd, IDCANCEL);
					break;
			}
			return TRUE;

		case WM_NOTIFY:
			if (((LPNMHDR)lParam)->code == TCN_SELCHANGE) {
				switch (TabCtrl_GetCurSel(((LPNMHDR)lParam)->hwndFrom)) {
					case 0:
						ShowWindow(miscOpts, TRUE);
						ShowWindow(mtlOpts, FALSE);
						break;
					case 1:
						ShowWindow(miscOpts, FALSE);
						ShowWindow(mtlOpts, TRUE);
						break;
				};
				return TRUE;
			}
			break;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;
	}

	return 0;
}



void MaxAWDExporterOpts::InitDialog(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	HWND tabPanel = GetDlgItem(hWnd, IDC_TABPANEL);
	TCITEM tci = {0};
	tci.mask = TCIF_TEXT|TCIF_STATE;

	tci.pszText = _T("Scene & misc");
	TabCtrl_InsertItem(tabPanel, 0, &tci);
	
	tci.pszText = _T("Materials");
	TabCtrl_InsertItem(tabPanel, 1, &tci);
	
	miscOpts = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_AWD_MISC_OPTS), 
		tabPanel, MiscOptsDialogProc);

	mtlOpts = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_AWD_MTL_OPTS), 
		tabPanel, MtlOptsDialogProc);

	// Select first tab
	TabCtrl_SetCurSel(tabPanel, 0);
	ShowWindow(miscOpts, TRUE);
	ShowWindow(mtlOpts, FALSE);
}


INT_PTR CALLBACK MaxAWDExporterOpts::MiscOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	// TODO: implement proc
	return FALSE;
}


INT_PTR CALLBACK MaxAWDExporterOpts::MtlOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	// TODO: Implement proc
	return FALSE;
}


