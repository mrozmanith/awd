#include "maxawdopts.h"
#include <custcont.h>

MaxAWDExporterOpts::MaxAWDExporterOpts(void)
{
	// Default values
	compression = (int)DEFLATE;

	exportScene = true;
	exportGeometry = true;
	exportSkin = true;
	jointsPerVertex = 2;

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
HWND MaxAWDExporterOpts::generalOpts = NULL;
HWND MaxAWDExporterOpts::sceneOpts = NULL;
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
					// General options
					imp->compression = ComboBox_GetCurSel(GetDlgItem(generalOpts, IDC_COMP_COMBO));

					// Scene & geometry options
					imp->exportScene = (IsDlgButtonChecked(sceneOpts, IDC_INC_SCENE) == BST_CHECKED);
					imp->exportGeometry = (IsDlgButtonChecked(sceneOpts, IDC_INC_GEOM) == BST_CHECKED);
					imp->exportSkin = (IsDlgButtonChecked(sceneOpts, IDC_INC_SKIN) == BST_CHECKED);
					imp->jointsPerVertex = GetISpinner(GetDlgItem(sceneOpts,IDC_JPV_SPINNER))->GetIVal();
					
					// Material options
					imp->exportMaterials = (IsDlgButtonChecked(mtlOpts, IDC_INC_MTL) == BST_CHECKED);
					imp->embedTextures = (IsDlgButtonChecked(mtlOpts, IDC_EMBED_TEX) == BST_CHECKED);

					// Animation options
					imp->exportSkeletons = (IsDlgButtonChecked(animOpts, IDC_INC_SKEL) == BST_CHECKED);
					imp->exportSkelAnim = (IsDlgButtonChecked(animOpts, IDC_INC_SKELANIM) == BST_CHECKED);

					// Preview options
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

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;
	}

	return FALSE;
}



void MaxAWDExporterOpts::InitDialog(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	int index;

	HWND rh = GetDlgItem(hWnd, IDC_ROLLUP);
	IRollupWindow *rollup = GetIRollup(rh);

	index = rollup->AppendRollup(hInstance, MAKEINTRESOURCE(IDD_AWD_GENERAL_OPTS),
		GeneralOptsDialogProc, "General");
	generalOpts = rollup->GetPanelDlg(index);

	index = rollup->AppendRollup(hInstance, MAKEINTRESOURCE(IDD_AWD_SCENE_OPTS), 
		SceneOptsDialogProc, "Scene & geometry");
	sceneOpts = rollup->GetPanelDlg(index);
	
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
	ComboBox_SetCurSel(GetDlgItem(generalOpts, IDC_COMP_COMBO), imp->compression);
	SetCheckBox(sceneOpts, IDC_INC_SCENE, imp->exportScene);
	SetCheckBox(sceneOpts, IDC_INC_GEOM, imp->exportGeometry);
	SetCheckBox(sceneOpts, IDC_INC_SKIN, imp->exportSkin);
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


INT_PTR CALLBACK MaxAWDExporterOpts::GeneralOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message) {
		case WM_INITDIALOG:
			ComboBox_AddItemData(GetDlgItem(hWnd, IDC_COMP_COMBO), "None");
			ComboBox_AddItemData(GetDlgItem(hWnd, IDC_COMP_COMBO), "GZIP");
			// TODO: Re-enable once Away3D can actually read LZMA compressed files.
			//ComboBox_AddItemData(GetDlgItem(hWnd, IDC_COMP_COMBO), "LZMA");
			return TRUE;
	}

	return FALSE;
}


INT_PTR CALLBACK MaxAWDExporterOpts::SceneOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message) {
		case WM_INITDIALOG:
			ISpinnerControl *spinner = GetISpinner(GetDlgItem(hWnd,IDC_JPV_SPINNER));
			spinner->SetLimits(1, 5, FALSE);
			spinner->SetValue(imp->jointsPerVertex, TRUE);
			spinner->LinkToEdit(GetDlgItem(hWnd,IDC_JPV_EDIT),EDITTYPE_INT);
			return TRUE;
	}

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



int MaxAWDExporterOpts::Compression(void)
{
	return compression;
}

bool MaxAWDExporterOpts::ExportScene(void)
{
	return exportScene;
}

bool MaxAWDExporterOpts::ExportGeometry(void)
{
	return exportGeometry;
}

bool MaxAWDExporterOpts::ExportSkin(void)
{
	return exportSkin;
}

int MaxAWDExporterOpts::JointsPerVertex(void)
{
	return jointsPerVertex;
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
