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

	static INT_PTR CALLBACK DialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

public:
	MaxAWDExporterOpts(void);
	~MaxAWDExporterOpts(void);

	bool ShowDialog(void);
};

