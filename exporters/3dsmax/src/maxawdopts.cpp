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

	createPreview = true;
	launchPreview = true;
	networkPreview = false;
}


// Static members
MaxAWDExporterOpts *MaxAWDExporterOpts::imp = NULL;
HWND MaxAWDExporterOpts::miscOpts = NULL;
HWND MaxAWDExporterOpts::mtlOpts = NULL;
HWND MaxAWDExporterOpts::animOpts = NULL;
HWND MaxAWDExporterOpts::viewerOpts = NULL;


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
						ShowWindow(animOpts, FALSE);
						ShowWindow(viewerOpts, FALSE);
						break;
					case 1:
						ShowWindow(miscOpts, FALSE);
						ShowWindow(mtlOpts, TRUE);
						ShowWindow(animOpts, FALSE);
						ShowWindow(viewerOpts, FALSE);
						break;
					case 2:
						ShowWindow(miscOpts, FALSE);
						ShowWindow(mtlOpts, FALSE);
						ShowWindow(animOpts, TRUE);
						ShowWindow(viewerOpts, FALSE);
						break;
					case 3:
						ShowWindow(miscOpts, FALSE);
						ShowWindow(mtlOpts, FALSE);
						ShowWindow(animOpts, FALSE);
						ShowWindow(viewerOpts, TRUE);
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

	tci.pszText = _T("Scene && misc");
	TabCtrl_InsertItem(tabPanel, 0, &tci);
	miscOpts = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_AWD_MISC_OPTS), 
		tabPanel, MiscOptsDialogProc);
	
	tci.pszText = _T("Materials");
	TabCtrl_InsertItem(tabPanel, 1, &tci);
	mtlOpts = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_AWD_MTL_OPTS), 
		tabPanel, MtlOptsDialogProc);
	
	tci.pszText = _T("Animation");
	TabCtrl_InsertItem(tabPanel, 2, &tci);
	animOpts = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_AWD_ANIM_OPTS), 
		tabPanel, AnimOptsDialogProc);

	tci.pszText = _T("Previewer");
	TabCtrl_InsertItem(tabPanel, 3, &tci);
	viewerOpts = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_AWD_VIEWER_OPTS), 
		tabPanel, ViewerOptsDialogProc);

	// Set defaults
	SetCheckBox(miscOpts, IDC_INC_GEOM, imp->exportGeometry);
	SetCheckBox(miscOpts, IDC_INC_SCENE, imp->exportScene);
	SetCheckBox(mtlOpts, IDC_INC_MTL, imp->exportMaterials);
	SetCheckBox(mtlOpts, IDC_EMBED_TEX, imp->embedTextures);
	SetCheckBox(animOpts, IDC_INC_SKEL, imp->exportSkeletons);
	SetCheckBox(animOpts, IDC_INC_SKELANIM, imp->exportSkelAnim);
	SetCheckBox(viewerOpts, IDC_SWF_ENABLE, imp->createPreview);
	SetCheckBox(viewerOpts, IDC_SWF_LAUNCH, imp->launchPreview);
	SetCheckBox(viewerOpts, IDC_SWFSB_NETWORK, imp->networkPreview);
	SetCheckBox(viewerOpts, IDC_SWFSB_LOCAL, !imp->networkPreview);

	// Select first tab
	TabCtrl_SetCurSel(tabPanel, 0);
	ShowWindow(miscOpts, TRUE);
	ShowWindow(mtlOpts, FALSE);
	ShowWindow(animOpts, FALSE);
	ShowWindow(viewerOpts, FALSE);
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


INT_PTR CALLBACK MaxAWDExporterOpts::AnimOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	// TODO: implement proc
	return FALSE;
}


INT_PTR CALLBACK MaxAWDExporterOpts::ViewerOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	// TODO: Implement proc
	return FALSE;
}


