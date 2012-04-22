#pragma once

#include "3dsmaxsdk_preinclude.h"
#include "max.h"
#include "resource.h"

class MaxAWDExporterOpts
{
private:
	bool exportGeometry;
	bool exportScene;

	bool exportMaterials;
	bool embedTextures;

	bool exportSkeletons;
	bool exportSkelAnim;

	static MaxAWDExporterOpts *imp;

	static HWND miscOpts;
	static HWND mtlOpts;
	static HWND animOpts;
	static HWND viewerOpts;

	// Main proc function and helpers
	static INT_PTR CALLBACK DialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	static void InitDialog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Tab panel proc functions
	static INT_PTR CALLBACK MiscOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	static INT_PTR CALLBACK MtlOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	static INT_PTR CALLBACK AnimOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	static INT_PTR CALLBACK ViewerOptsDialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

public:
	MaxAWDExporterOpts(void);
	~MaxAWDExporterOpts(void);

	bool ShowDialog(void);
};

