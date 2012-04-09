//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "awd/awd.h"
#include "awd/platform.h"
#include "maxawd.h"

#define MaxAWDExporter_CLASS_ID	Class_ID(0xa8e047f2, 0x81e112c0)





class MaxAWDExporterClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 	{ return new MaxAWDExporter(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return MaxAWDExporter_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("MaxAWDExporter"); }		// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetMaxAWDExporterDesc() { 
	static MaxAWDExporterClassDesc MaxAWDExporterDesc;
	return &MaxAWDExporterDesc; 
}





INT_PTR CALLBACK MaxAWDExporterOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static MaxAWDExporter *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (MaxAWDExporter *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}


//--- MaxAWDExporter -------------------------------------------------------
MaxAWDExporter::MaxAWDExporter()
{

}

MaxAWDExporter::~MaxAWDExporter() 
{

}

int MaxAWDExporter::ExtCount()
{
	return 1;
}

const TCHAR *MaxAWDExporter::Ext(int n)
{		
	return _T("AWD");
}

const TCHAR *MaxAWDExporter::LongDesc()
{
	return _T("Away3D AWD File");
}
	
const TCHAR *MaxAWDExporter::ShortDesc() 
{
	return _T("Away3D");
}

const TCHAR *MaxAWDExporter::AuthorName()
{			
	return _T("Away3D");
}

const TCHAR *MaxAWDExporter::CopyrightMessage() 
{	
	return _T("Copyright 2012 The Away3D Team");
}

const TCHAR *MaxAWDExporter::OtherMessage1() 
{		
	return _T("");
}

const TCHAR *MaxAWDExporter::OtherMessage2() 
{		
	return _T("");
}

unsigned int MaxAWDExporter::Version()
{				
	return 100;
}

void MaxAWDExporter::ShowAbout(HWND hWnd)
{			
	// Optional
}

BOOL MaxAWDExporter::SupportsOptions(int ext, DWORD options)
{
	#pragma message(TODO("Decide which options to support.  Simply return true for each option supported by each Extension the exporter supports."))
	return TRUE;
}


int	MaxAWDExporter::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	/*
	if(!suppressPrompts)
		DialogBoxParam(hInstance, 
				MAKEINTRESOURCE(IDD_PANEL), 
				GetActiveWindow(), 
				MaxAWDExporterOptionsDlgProc, (LPARAM)this);
	*/

	int fd = open(name, _O_TRUNC | _O_CREAT | _O_BINARY | _O_RDWR);

	awd = new AWD(UNCOMPRESSED, 0);

	INode *root = i->GetRootNode();
	ExportNode(root);

	awd->flush(fd);

	close(fd);

	// Export worked
	return TRUE;
}


void MaxAWDExporter::ExportNode(INode *node)
{
	int i;
	int numChildren;
	Object *obj;

	obj = node->GetObjectRef();
	if (obj) {
		SClass_ID scid = obj->SuperClassID();
		Class_ID cid = obj->ClassID();

		if (obj->CanConvertToType(triObjectClassID)) {
			TriObject *triObject = (TriObject*)obj->ConvertToType(0, triObjectClassID);
			if (triObject != NULL) {
				ExportTriObject(triObject);

				// If conversion created a new object, dispose it
				if (triObject != obj) 
					triObject->DeleteMe();
			}
		}
	}

	numChildren = node->NumberOfChildren();
	for (i=0; i<numChildren; i++) {
		ExportNode(node->GetChildNode(i));
	}
}


void MaxAWDExporter::ExportTriObject(TriObject *obj)
{
	// jidder
}
