#pragma once

#include "3dsmaxsdk_preinclude.h"
#include "max.h"
#include "resource.h"

#include "awd/awd.h"

class MaxAWDExporterOpts
{
private:
	int compression;

	bool exportScene;
	bool exportGeometry;
	bool exportSkin;
	int jointsPerVertex;
	
	bool exportMaterials;
	bool embedTextures;

	bool exportSkeletons;
	bool exportSkelAnim;

	bool createPreview;
	bool launchPreview;
	bool networkPreview;

	static MaxAWDExporterOpts *imp;

	static HWND generalOpts;
	static HWND sceneOpts;
	static HWND mtlOpts;
	static HWND animOpts;
	static HWND viewerOpts;

	// Main proc function and helpers
	static INT_PTR CALLBACK DialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	static void InitDialog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Tab panel proc functions
	static INT_PTR CALLBACK GeneralOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	static INT_PTR CALLBACK SceneOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	static INT_PTR CALLBACK MtlOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	static INT_PTR CALLBACK AnimOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	static INT_PTR CALLBACK ViewerOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

public:
	MaxAWDExporterOpts(void);
	~MaxAWDExporterOpts(void);

	bool ShowDialog(void);

	// Getters for options
	int Compression(void);

	bool ExportScene(void);
	bool ExportGeometry(void);
	bool ExportSkin(void);
	int JointsPerVertex();

	bool ExportMaterials(void);
	bool EmbedTextures(void);

	bool ExportSkeletons(void);
	bool ExportSkelAnim(void);
	
	bool CreatePreview(void);
	bool LaunchPreview(void);
	bool PreviewForDeployment(void);
};

