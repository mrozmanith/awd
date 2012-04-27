#include "maxawdopts.h"
#include <custcont.h>

MaxAWDExporterOpts::MaxAWDExporterOpts(void)
{
	// Default values
	compression = (int)DEFLATE;
	exportAttributes = true;

	attributeNamespace = (char*)malloc(25);
	strncpy(attributeNamespace, "http://example.com/awdns\0", 25);

	exportScene = true;
	exportGeometry = true;
	exportUVs = true;
	exportNormals = true;
	exportSkin = true;
	jointsPerVertex = 2;

	exportMaterials = true;
	forceBasenameTextures = true;
	copyTextures = true;
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
					SaveOptions();
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
		SceneOptsDialogProc, "Scene & geometry", 0, APPENDROLL_CLOSED);
	sceneOpts = rollup->GetPanelDlg(index);
	
	index = rollup->AppendRollup(hInstance, MAKEINTRESOURCE(IDD_AWD_MTL_OPTS), 
		MtlOptsDialogProc, "Materials", 0, APPENDROLL_CLOSED);
	mtlOpts = rollup->GetPanelDlg(index);
	
	index = rollup->AppendRollup(hInstance, MAKEINTRESOURCE(IDD_AWD_ANIM_OPTS), 
		AnimOptsDialogProc, "Animation", 0, APPENDROLL_CLOSED);
	animOpts = rollup->GetPanelDlg(index);

	index = rollup->AppendRollup(hInstance, MAKEINTRESOURCE(IDD_AWD_VIEWER_OPTS), 
		ViewerOptsDialogProc, "Flash viewer", 0, APPENDROLL_CLOSED);
	viewerOpts = rollup->GetPanelDlg(index);

	// Set defaults
	ComboBox_SetCurSel(GetDlgItem(generalOpts, IDC_COMP_COMBO), imp->compression);
	SetCheckBox(generalOpts, IDC_INC_ATTR, imp->exportAttributes);
	Edit_SetText(GetDlgItem(generalOpts, IDC_ATTRNS_TEXT), imp->attributeNamespace);
	SetCheckBox(sceneOpts, IDC_INC_SCENE, imp->exportScene);
	SetCheckBox(sceneOpts, IDC_INC_GEOM, imp->exportGeometry);
	SetCheckBox(sceneOpts, IDC_INC_UVS, imp->exportUVs);
	SetCheckBox(sceneOpts, IDC_INC_NORMALS, imp->exportNormals);
	SetCheckBox(sceneOpts, IDC_INC_SKIN, imp->exportSkin);
	SetCheckBox(mtlOpts, IDC_INC_MTL, imp->exportMaterials);
	SetCheckBox(mtlOpts, IDC_TEX_BASENAME, imp->forceBasenameTextures);
	SetCheckBox(mtlOpts, IDC_TEX_FULLPATH, !imp->forceBasenameTextures);
	SetCheckBox(mtlOpts, IDC_TEX_COPY, imp->copyTextures);
	SetCheckBox(mtlOpts, IDC_EMBED_TEX, imp->embedTextures);
	SetCheckBox(animOpts, IDC_INC_SKEL, imp->exportSkeletons);
	SetCheckBox(animOpts, IDC_INC_SKELANIM, imp->exportSkelAnim);
	SetCheckBox(viewerOpts, IDC_SWF_ENABLE, imp->createPreview);
	SetCheckBox(viewerOpts, IDC_SWF_LAUNCH, imp->launchPreview);
	SetCheckBox(viewerOpts, IDC_SWFSB_NETWORK, imp->networkPreview);
	SetCheckBox(viewerOpts, IDC_SWFSB_LOCAL, !imp->networkPreview);

	rollup->Show();
}


void MaxAWDExporterOpts::SaveOptions(void)
{
	// General options
	imp->compression = ComboBox_GetCurSel(GetDlgItem(generalOpts, IDC_COMP_COMBO));
	imp->exportAttributes = (IsDlgButtonChecked(generalOpts, IDC_INC_ATTR) == BST_CHECKED);
	
	int len = Edit_GetTextLength(GetDlgItem(generalOpts, IDC_ATTRNS_TEXT));
	imp->attributeNamespace = (char*)realloc(imp->attributeNamespace, len + 1);
	Edit_GetText(GetDlgItem(generalOpts, IDC_ATTRNS_TEXT), imp->attributeNamespace, len);

	// Scene & geometry options
	imp->exportScene = (IsDlgButtonChecked(sceneOpts, IDC_INC_SCENE) == BST_CHECKED);
	imp->exportGeometry = (IsDlgButtonChecked(sceneOpts, IDC_INC_GEOM) == BST_CHECKED);
	imp->exportUVs = (IsDlgButtonChecked(sceneOpts, IDC_INC_UVS) == BST_CHECKED);
	imp->exportNormals = (IsDlgButtonChecked(sceneOpts, IDC_INC_NORMALS) == BST_CHECKED);
	imp->exportSkin = (IsDlgButtonChecked(sceneOpts, IDC_INC_SKIN) == BST_CHECKED);
	imp->jointsPerVertex = GetISpinner(GetDlgItem(sceneOpts,IDC_JPV_SPINNER))->GetIVal();
					
	// Material options
	imp->exportMaterials = (IsDlgButtonChecked(mtlOpts, IDC_INC_MTL) == BST_CHECKED);
	imp->forceBasenameTextures = (IsDlgButtonChecked(mtlOpts, IDC_TEX_BASENAME) == BST_CHECKED);
	imp->copyTextures = (IsDlgButtonChecked(mtlOpts, IDC_TEX_COPY) == BST_CHECKED);
	imp->embedTextures = (IsDlgButtonChecked(mtlOpts, IDC_TEX_EMBED) == BST_CHECKED);

	// Animation options
	imp->exportSkeletons = (IsDlgButtonChecked(animOpts, IDC_INC_SKEL) == BST_CHECKED);
	imp->exportSkelAnim = (IsDlgButtonChecked(animOpts, IDC_INC_SKELANIM) == BST_CHECKED);

	// Preview options
	imp->createPreview = (IsDlgButtonChecked(viewerOpts, IDC_SWF_ENABLE) == BST_CHECKED);
	imp->launchPreview = (IsDlgButtonChecked(viewerOpts, IDC_SWF_LAUNCH) == BST_CHECKED);
	imp->networkPreview = (IsDlgButtonChecked(viewerOpts, IDC_SWFSB_NETWORK) == BST_CHECKED);
}



INT_PTR CALLBACK MaxAWDExporterOpts::GeneralOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	bool enabled;

	switch (message) {
		case WM_INITDIALOG:
			ComboBox_AddItemData(GetDlgItem(hWnd, IDC_COMP_COMBO), "None");
			ComboBox_AddItemData(GetDlgItem(hWnd, IDC_COMP_COMBO), "GZIP");
			// TODO: Re-enable once Away3D can actually read LZMA compressed files.
			//ComboBox_AddItemData(GetDlgItem(hWnd, IDC_COMP_COMBO), "LZMA");
			return TRUE;

		case WM_COMMAND:
			if ((HWND)lParam == GetDlgItem(hWnd,IDC_INC_ATTR)) {
				enabled = (IsDlgButtonChecked(hWnd,IDC_INC_ATTR) == BST_CHECKED);
				Static_Enable(GetDlgItem(hWnd,IDC_ATTRNS_STATIC), enabled);
				Edit_Enable(GetDlgItem(hWnd,IDC_ATTRNS_TEXT), enabled);
				return TRUE;
			}
			break;
	}

	return FALSE;
}


INT_PTR CALLBACK MaxAWDExporterOpts::SceneOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	ISpinnerControl *spinner;
	bool enabled;

	switch (message) {
		case WM_INITDIALOG:
			spinner = GetISpinner(GetDlgItem(hWnd,IDC_JPV_SPINNER));
			spinner->SetLimits(1, 5, FALSE);
			spinner->SetValue(imp->jointsPerVertex, TRUE);
			spinner->LinkToEdit(GetDlgItem(hWnd,IDC_JPV_EDIT),EDITTYPE_INT);
			return TRUE;

		case WM_COMMAND:
			if ((HWND)lParam == GetDlgItem(hWnd, IDC_INC_GEOM)) {
				enabled = (IsDlgButtonChecked(hWnd, IDC_INC_GEOM) == BST_CHECKED);
				Button_Enable(GetDlgItem(hWnd,IDC_INC_NORMALS), enabled);
				Button_Enable(GetDlgItem(hWnd,IDC_INC_UVS), enabled);
				Button_Enable(GetDlgItem(hWnd,IDC_INC_SKIN), enabled);
				GetISpinner(GetDlgItem(hWnd,IDC_JPV_SPINNER))->Enable(enabled);
				GetICustEdit(GetDlgItem(hWnd,IDC_JPV_EDIT))->Enable(enabled);
				Static_Enable(GetDlgItem(hWnd,IDC_JPV_STATIC), enabled);
				return TRUE;
			}
			else if ((HWND)lParam == GetDlgItem(hWnd,IDC_INC_SKIN)) {
				enabled = (IsDlgButtonChecked(hWnd, IDC_INC_SKIN) == BST_CHECKED);
				GetISpinner(GetDlgItem(hWnd,IDC_JPV_SPINNER))->Enable(enabled);
				GetICustEdit(GetDlgItem(hWnd, IDC_JPV_EDIT))->Enable(enabled);
				Static_Enable(GetDlgItem(hWnd,IDC_JPV_STATIC), enabled);
				return TRUE;
			}
			break;
	}

	return FALSE;
}


INT_PTR CALLBACK MaxAWDExporterOpts::MtlOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	bool enabled;

	switch (message) {
		case WM_COMMAND:
			if ((HWND)lParam == GetDlgItem(hWnd, IDC_INC_MTL)) {
				enabled = (IsDlgButtonChecked(hWnd, IDC_INC_MTL) == BST_CHECKED);
				Button_Enable(GetDlgItem(hWnd,IDC_TEX_FULLPATH), enabled);
				Button_Enable(GetDlgItem(hWnd,IDC_TEX_BASENAME), enabled);
				Button_Enable(GetDlgItem(hWnd,IDC_TEX_EMBED), enabled);
			}
			
			// IDC_TEX_COPY should only be enabled if both the IDC_INC_MTL checkbox
			// and the IDC_TEX_BASENAME radio button are checked.
			enabled = ((IsDlgButtonChecked(hWnd, IDC_INC_MTL)==BST_CHECKED) 
				&& (IsDlgButtonChecked(hWnd, IDC_TEX_BASENAME)==BST_CHECKED));
			Button_Enable(GetDlgItem(hWnd,IDC_TEX_COPY), enabled);

			return TRUE;
	}

	return FALSE;
}


INT_PTR CALLBACK MaxAWDExporterOpts::AnimOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	bool enabled;

	switch (message) {
		case WM_COMMAND:
			if ((HWND)lParam == GetDlgItem(hWnd,IDC_INC_SKELANIM)) {
				enabled = (IsDlgButtonChecked(hWnd, IDC_INC_SKELANIM) == BST_CHECKED);
				Edit_Enable(GetDlgItem(hWnd,IDC_SEQ_TXT), enabled);
				Static_Enable(GetDlgItem(hWnd,IDC_SEQ_STATIC1), enabled);
				Static_Enable(GetDlgItem(hWnd,IDC_SEQ_STATIC2), enabled);
				return TRUE;
			}
			break;
	}

	return FALSE;
}


INT_PTR CALLBACK MaxAWDExporterOpts::ViewerOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	bool enabled;

	switch (message) {
		case WM_COMMAND:
			if ((HWND)lParam == GetDlgItem(hWnd,IDC_SWF_ENABLE)) {
				enabled = (IsDlgButtonChecked(hWnd,IDC_SWF_ENABLE) == BST_CHECKED);
				Button_Enable(GetDlgItem(hWnd,IDC_SWF_LAUNCH), enabled);
				Button_Enable(GetDlgItem(hWnd,IDC_SWFSB_LOCAL), enabled);
				Button_Enable(GetDlgItem(hWnd,IDC_SWFSB_NETWORK), enabled);
				Static_Enable(GetDlgItem(hWnd,IDC_SWFSB_STATIC1), enabled);
				Static_Enable(GetDlgItem(hWnd,IDC_SWFSB_STATIC2), enabled);
				return TRUE;
			}
			break;
	}

	return FALSE;
}



int MaxAWDExporterOpts::Compression(void)
{
	return compression;
}

bool MaxAWDExporterOpts::ExportAttributes(void)
{
	return exportAttributes;
}

char *MaxAWDExporterOpts::AttributeNamespace(void)
{
	return attributeNamespace;
}

bool MaxAWDExporterOpts::ExportScene(void)
{
	return exportScene;
}

bool MaxAWDExporterOpts::ExportGeometry(void)
{
	return exportGeometry;
}

bool MaxAWDExporterOpts::ExportUVs(void)
{
	return exportUVs;
}

bool MaxAWDExporterOpts::ExportNormals(void)
{
	return exportNormals;
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

bool MaxAWDExporterOpts::ForceBasenameTextures(void)
{
	return forceBasenameTextures;
}

bool MaxAWDExporterOpts::CopyTextures(void)
{
	return copyTextures;
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
