#include "maxawdopts.h"
#include <custcont.h>

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
					imp->exportGeometry = (IsDlgButtonChecked(miscOpts, IDC_INC_GEOM) == BST_CHECKED);
					imp->exportScene = (IsDlgButtonChecked(miscOpts, IDC_INC_SCENE) == BST_CHECKED);
					imp->exportMaterials = (IsDlgButtonChecked(mtlOpts, IDC_INC_MTL) == BST_CHECKED);
					imp->embedTextures = (IsDlgButtonChecked(mtlOpts, IDC_EMBED_TEX) == BST_CHECKED);
					imp->exportSkeletons = (IsDlgButtonChecked(animOpts, IDC_INC_SKEL) == BST_CHECKED);
					imp->exportSkelAnim = (IsDlgButtonChecked(animOpts, IDC_INC_SKELANIM) == BST_CHECKED);
					imp->createPreview = (IsDlgButtonChecked(viewerOpts, IDC_SWF_ENABLE) == BST_CHECKED);
					imp->launchPreview = (IsDlgButtonChecked(viewerOpts, IDC_SWF_LAUNCH) == BST_CHECKED);
					imp->networkPreview = (IsDlgButtonChecked(viewerOpts, IDC_SWFSB_NETWORK) == BST_CHECKED);
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
	int index;

	HWND rh = GetDlgItem(hWnd, IDC_ROLLUP);
	IRollupWindow *rollup = GetIRollup(rh);

	index = rollup->AppendRollup(hInstance, (const char *)MAKEINTRESOURCE(IDD_AWD_MISC_OPTS), 
		MiscOptsDialogProc, "Scene & misc");
	miscOpts = rollup->GetPanelDlg(index);

	
	index = rollup->AppendRollup(hInstance, MAKEINTRESOURCE(IDD_AWD_MTL_OPTS), 
		MtlOptsDialogProc, "Materials");
	mtlOpts = rollup->GetPanelDlg(index);
	
	index = rollup->AppendRollup(hInstance, MAKEINTRESOURCE(IDD_AWD_ANIM_OPTS), 
		AnimOptsDialogProc, "Animation");
	animOpts = rollup->GetPanelDlg(index);

	index = rollup->AppendRollup(hInstance, MAKEINTRESOURCE(IDD_AWD_VIEWER_OPTS), 
		ViewerOptsDialogProc, "Flash viewer");
	viewerOpts = rollup->GetPanelDlg(index);

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

	rollup->Show();
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
	// TODO: Implement proc
	return FALSE;
}


INT_PTR CALLBACK MaxAWDExporterOpts::ViewerOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	// TODO: Implement proc
	return FALSE;
}


bool MaxAWDExporterOpts::ExportGeometry(void)
{
	return exportGeometry;
}

bool MaxAWDExporterOpts::ExportScene(void)
{
	return exportScene;
}

bool MaxAWDExporterOpts::ExportMaterials(void)
{
	return exportMaterials;
}

bool MaxAWDExporterOpts::EmbedTextures(void)
{
	return embedTextures;
}

bool MaxAWDExporterOpts::ExportSkeletons(void)
{
	return exportSkeletons;
}

bool MaxAWDExporterOpts::ExportSkelAnim(void)
{
	return exportSkelAnim;
}

int MaxAWDExporterOpts::JointsPerVertex(void)
{
	return jointsPerVertex;
}

bool MaxAWDExporterOpts::CreatePreview(void)
{
	return createPreview;
}

bool MaxAWDExporterOpts::LaunchPreview(void)
{
	return launchPreview;
}

bool MaxAWDExporterOpts::PreviewForDeployment(void)
{
	return networkPreview;
}
