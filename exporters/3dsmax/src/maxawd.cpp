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

#include <Windows.h>

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


//--- Utilities ------------------------------------------------------------
static void SerializeMatrix3(Matrix3 &mtx, double *output)
{
	Point3 row;
	
	row = mtx.GetRow(0);
	output[0] = row.x;
	output[1] = -row.z;
	output[2] = -row.y;

	row = mtx.GetRow(2);
	output[3] = -row.x;
	output[4] = row.z;
	output[5] = row.y;

	row = mtx.GetRow(1);
	output[6] = -row.x;
	output[7] = row.z;
	output[8] = row.y;

	row = mtx.GetRow(3);
	output[9] = -row.x;
	output[10] = row.z;
	output[11] = row.y;
}


static int IndexOfSkinMod(Object *obj, IDerivedObject **derivedObject)
{
	if (obj != NULL && obj->SuperClassID() == GEN_DERIVOB_CLASS_ID) {
		int i;

		IDerivedObject *derived = (IDerivedObject *)obj;

		for (i=0; i < derived->NumModifiers(); i++) {
			Modifier *mod = derived->GetModifier(i);

			void *skin = mod->GetInterface(I_SKIN);
			if (skin != NULL) {
				*derivedObject = derived;
				return i;
			}
		}
	}
	
	return -1;
}

int ReplaceString(char *buf, int *size, char *find, char *rep)
{
    char *p;
    char *tmp;
    int findLen;
    int repLen;
    int endLen;

    p = strstr(buf, find);
    if (p == NULL)
        return 0;

    // Take trailing part and store temporarily
    findLen = strlen(find);
    endLen = strlen(p) - findLen;
    tmp = (char *)malloc(endLen);
    memcpy(tmp, p+findLen, endLen);

    // Replace string in buffer and move to end
    // of replaced string
    repLen = strlen(rep);
    memcpy(p, rep, repLen);
    p += repLen;

    // Append trailing string
    memcpy(p, tmp, endLen);

    free(tmp);

	// Save new size of buffer
    *size = (p-buf) + endLen;

	return 1;
}

//--- MaxAWDExporter -------------------------------------------------------
MaxAWDExporter::MaxAWDExporter()
{
	cache = new BlockCache();
	colMtlCache = new ColorMaterialCache();
}

MaxAWDExporter::~MaxAWDExporter() 
{
	// TODO: Free caches
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

	// Copy viewer HTML and SWF template to output directory
	// TODO: Make this optional and configurable
	CopyViewer(name);

 	int fd = open(name, _O_TRUNC | _O_CREAT | _O_BINARY | _O_RDWR, _S_IWRITE);

	awd = new AWD(UNCOMPRESSED, 0);

	INode *root = i->GetRootNode();
	ExportNode(root, NULL);

	awd->flush(fd);

	close(fd);

	// Export worked
	return TRUE;
}


void MaxAWDExporter::CopyViewerHTML(char *templatePath, char *outPath, char *name)
{
	char *buf;
	int bufLen;

	bufLen = 0xffff;
	buf = (char *)malloc(bufLen);

    FILE *in = fopen(templatePath, "r");
    bufLen = fread((void *)buf, sizeof(char), bufLen, in);
	memset((void *)(buf + bufLen), 0, 1);
    fclose(in);

    ReplaceString(buf, &bufLen, "%NAME%", name);

    FILE *out = fopen(outPath, "w");
    fwrite(buf, sizeof(char), bufLen, out);
    fclose(out);

	free(buf);
}


void MaxAWDExporter::CopyViewer(const TCHAR *awdFullPath)
{
	char awdDrive[4];
	char awdPath[1024];
	char awdName[256];
	char maxExe[1024];
	char maxDrive[4];
	char maxPath[1024];
	char tplHtmlPath[1024];
	char tplSwfPath[1024];
	char tplJsPath[1024];
	char outHtmlPath[1024];
	char outSwfPath[1024];
	char outJsPath[1024];

	HMODULE mod = GetModuleHandle(NULL);
	GetModuleFileName(mod, maxExe, 1024);

	_splitpath_s(maxExe, maxDrive, 4, maxPath, 1024, NULL, 0, NULL, 0);
	_splitpath_s(awdFullPath, awdDrive, 4, awdPath, 1024, awdName, 256, NULL, 0);

	// Assemble paths for inputs (templates)
	_makepath_s(tplHtmlPath, 1024, maxDrive, maxPath, "plugins\\maxawd\\template", "html");
	_makepath_s(tplSwfPath, 1024, maxDrive, maxPath, "plugins\\maxawd\\viewer", "swf");
	_makepath_s(tplJsPath, 1024, maxDrive, maxPath, "plugins\\maxawd\\swfobject", "js");

	// Assemble paths for outputs
	_makepath_s(outHtmlPath, 1024, awdDrive, awdPath, awdName, "html");
	_makepath_s(outSwfPath, 1024, awdDrive, awdPath, "viewer", "swf");
	_makepath_s(outJsPath, 1024, awdDrive, awdPath, "swfobject", "js");

	// Copy HTML, and evaluate any variables in the template
	CopyViewerHTML(tplHtmlPath, outHtmlPath, awdName);

	// Copy SWF and JS files as-is
	CopyFile(tplSwfPath, outSwfPath, true);
	CopyFile(tplJsPath, outJsPath, true);

	//TODO: Move to end of export operation and make optional
	//ShellExecute(NULL, "open", outHtmlPath, NULL, NULL, SW_SHOWNORMAL);
}


void MaxAWDExporter::ExportNode(INode *node, AWDSceneBlock *parent)
{
	int i;
	int numChildren;
	Object *obj;

	AWDSceneBlock *awdParent = NULL;

	obj = node->GetObjectRef();
	if (obj && obj->ClassID()==BONE_OBJ_CLASSID) {
		// This is a bone that isn't necessarily bound to a skin. Ignore this
		// for now. Bones that are bound will be exported as part of the skin.
		// TODO: Add option to GUI allowing export of un-bound bones.
	}
	else {
		int skinIdx;
		ObjectState os;

		IDerivedObject *derivedObject = NULL;
		skinIdx = IndexOfSkinMod(node->GetObjectRef(), &derivedObject);
		if (skinIdx >= 0) {
			// Flatten all modifiers up to but not including
			// the skin modifier.
			os = derivedObject->Eval(0, skinIdx + 1);
		}
		else {
			// Flatten entire modifier stack
			os = node->EvalWorldState(0);
		}
	
		obj = os.obj;
		if (obj) {
			if (obj->CanConvertToType(triObjectClassID)) {
				AWDMeshInst *awdMesh;
			
				// Check if there is a skin, that can be
				// exported as part of the geometry.
				ISkin *skin = NULL;
				if (derivedObject != NULL && skinIdx >= 0) {
					Modifier *mod = derivedObject->GetModifier(skinIdx);
					skin = (ISkin *)mod->GetInterface(I_SKIN);
				}

				awdMesh = ExportTriObject(obj, node, skin);

				if (parent) {
					parent->add_child(awdMesh);
				}
				else {
					awd->add_scene_block(awdMesh);
				}

				// Store the new block as parent to be used for
				// blocks that represent children of this Max node.
				awdParent = awdMesh;
			}
		}
	}

	numChildren = node->NumberOfChildren();
	for (i=0; i<numChildren; i++) {
		ExportNode(node->GetChildNode(i), awdParent);
	}
}


AWDMeshInst * MaxAWDExporter::ExportTriObject(Object *obj, INode *node, ISkin *skin)
{
	AWDTriGeom *awdGeom = ExportTriGeom(obj, node, skin);

	Matrix3 mtx = node->GetNodeTM(0) * Inverse(node->GetParentTM(0));
	double *mtxData = (double *)malloc(12*sizeof(double));
	SerializeMatrix3(mtx, mtxData);

	// Export material
	AWDMaterial *awdMtl = ExportNodeMaterial(node);

	// Export instance
	char *name = node->GetName();
	AWDMeshInst *inst = new AWDMeshInst(name, strlen(name), awdGeom, mtxData);
	inst->add_material(awdMtl);
	
	return inst;
}


AWDTriGeom *MaxAWDExporter::ExportTriGeom(Object *obj, INode *node, ISkin *skin)
{
	AWDTriGeom *awdGeom;

	awdGeom = (AWDTriGeom *)cache->Get(obj);
	if (awdGeom == NULL) {
		int i;
		int numVerts, numTris;
		AWD_str_ptr vertData;
		AWD_str_ptr indexData;

		TriObject *triObject = (TriObject*)obj->ConvertToType(0, triObjectClassID);	

		Mesh& mesh = triObject->GetMesh();

		// Calculate offset matrix from the object TM (which includes geometry
		// offset) and the node TM (which doesn't.) This will be used to transform
		// all vertices into node space.
		Matrix3 offsMtx = node->GetObjectTM(0) * Inverse(node->GetNodeTM(0));

		numVerts = mesh.getNumVerts();
		vertData.v = malloc(3 * numVerts * sizeof(double));

		for (i=0; i<numVerts; i++) {
			// Transform vertex into node space
			Point3& vtx = offsMtx * mesh.getVert(i);
			vertData.f64[i*3+0] = -vtx.x;
			vertData.f64[i*3+1] = vtx.z;
			vertData.f64[i*3+2] = vtx.y;
		}

		numTris = mesh.getNumFaces();
		indexData.v = malloc(3 * numTris * sizeof(int));

		for (i=0; i<numTris; i++) {
			Face& face = mesh.faces[i];
			DWORD *inds = face.getAllVerts();

			indexData.ui32[i*3+0] = inds[0];
			indexData.ui32[i*3+1] = inds[2];
			indexData.ui32[i*3+2] = inds[1];
		}

		AWDSubGeom *sub = new AWDSubGeom();
		sub->add_stream(VERTICES, AWD_FIELD_FLOAT32, vertData, numVerts*3);
		sub->add_stream(TRIANGLES, AWD_FIELD_UINT16, indexData, numTris*3);

		if (skin) {
			ExportSkin(node, skin, sub);
		}

		char *name = node->GetName();

		// TODO: Use another name for the geometry
		awdGeom = new AWDTriGeom(name, strlen(name));
		awdGeom->add_sub_mesh(sub);
		awd->add_mesh_data(awdGeom);

		cache->Set(obj, awdGeom);

		// If conversion created a new object, dispose it
		if (triObject != obj) 
			triObject->DeleteMe();
	}

	return awdGeom;
}


AWDMaterial *MaxAWDExporter::ExportNodeMaterial(INode *node) 
{
	AWDMaterial *awdMtl;
	Mtl *mtl = node->GetMtl();

	if (mtl == NULL) {
		awd_color color = node->GetWireColor();

		// Look in the cache for an existing "default" color material
		// that matches the color of this object. If none exists,
		// create a new one and store it in the cache.
		awdMtl = colMtlCache->Get(color);
		if (awdMtl == NULL) {
			awdMtl = new AWDMaterial(AWD_MATTYPE_COLOR, "", 0);
			awdMtl->color = color;
			awd->add_material(awdMtl);

			colMtlCache->Set(color, awdMtl);
		}
	}
	else {
		awdMtl = (AWDMaterial *)cache->Get(mtl);
		if (awdMtl == NULL) {
			int i;
			const MSTR &name = mtl->GetName();

			if (mtl->IsSubClassOf(Class_ID(DMTL_CLASS_ID, 0))) {
				StdMat *stdMtl = (StdMat *)mtl;
			}

			for (i=0; i<mtl->NumSubTexmaps(); i++) {
				Texmap *tex = mtl->GetSubTexmap(i);

				// If there is a texture, AND that texture is a plain bitmap
				if (tex != NULL && tex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0)) {
					MSTR slotName = mtl->GetSubTexmapSlotName(i);
					const MSTR diff = _M("Diffuse Color");

					if (slotName == diff) {
						AWDBitmapTexture *awdDiffTex;
					
						awdDiffTex = ExportBitmapTexture((BitmapTex *)tex);

						awdMtl = new AWDMaterial(AWD_MATTYPE_TEXTURE, name.data(), name.length());
						awdMtl->set_texture(awdDiffTex);
					}
				}
			}

			// If no material was created during the texture search loop, this
			// is a plain color material.
			if (awdMtl == NULL) 
				awdMtl = new AWDMaterial(AWD_MATTYPE_COLOR, name.data(), name.Length());

			awd->add_material(awdMtl);
			cache->Set(mtl, awdMtl);
		}
	}

	return awdMtl;
}


AWDBitmapTexture * MaxAWDExporter::ExportBitmapTexture(BitmapTex *tex)
{
	AWDBitmapTexture *awdTex;
	MSTR name;
	char *path;

	name = tex->GetName();
	path = tex->GetMapName();

	// TODO: Deal differently with embedded textures
	awdTex = new AWDBitmapTexture(EXTERNAL, name.data(), name.length());
	awdTex->set_url(path, strlen(path));

	awd->add_texture(awdTex);

	return awdTex;
}


void MaxAWDExporter::ExportSkin(INode *node, ISkin *skin, AWDSubGeom *sub)
{
	int iVtx;
	awd_float64 *weights;
	awd_uint32 *indices;

	// First export skeleton block
	ExportSkeleton(skin);

	// TODO: Replace with option
	const int jointsPerVertex = 2;

	ISkinContextData *context = skin->GetContextInterface(node);

	int numVerts = context->GetNumPoints();
	weights = (awd_float64*)malloc(jointsPerVertex * numVerts * sizeof(awd_float64));
	indices = (awd_uint32*)malloc(jointsPerVertex * numVerts * sizeof(awd_uint32));

	for (iVtx=0; iVtx<numVerts; iVtx++) {
		int iBone;
		int numBones;
		double weightSum = 0;

		numBones = context->GetNumAssignedBones(iVtx);

		// For each weight/index slot, retrieve weight/index values
		// from skin, or after having run out of assigned bones for
		// a vertex, set weight to zero.
		for (iBone=0; iBone<jointsPerVertex; iBone++) {

			// Calculate index in stream
			int strIdx = iVtx*jointsPerVertex + iBone;

			if (iBone < numBones) {
				weights[strIdx] = context->GetBoneWeight(iVtx, iBone);
				indices[strIdx] = context->GetAssignedBone(iVtx, iBone);

				weightSum += weights[iBone];
			}
			else {
				weights[strIdx] = 0.0;
				indices[strIdx] = 0;
			}
		}

		// Normalize weights (sum must be 1.0)
		double scale = 1/weightSum;
		for (iBone=0; iBone<jointsPerVertex; iBone++) {
			weights[iVtx*jointsPerVertex + iBone] *= scale;
		}
	}

	AWD_str_ptr weightPtr;
	AWD_str_ptr indexPtr;
	weightPtr.f64 = weights;
	indexPtr.ui32 = indices;
	sub->add_stream(VERTEX_WEIGHTS, AWD_FIELD_FLOAT32, weightPtr, numVerts*jointsPerVertex);
	sub->add_stream(JOINT_INDICES, AWD_FIELD_UINT16, indexPtr, numVerts*jointsPerVertex);
}


void MaxAWDExporter::ExportSkeleton(ISkin *skin)
{
	int i;
	int numBones = skin->GetNumBones();
	BlockCache skelCache;

	AWDSkeleton *awdSkel;

	// TODO: Find proper name
	awdSkel = new AWDSkeleton("skel", 4);

	for (i=0; i<numBones; i++) {
		INode *bone;
		char *name;
		AWDSkeletonJoint *awdJoint;
		Matrix3 invBindTM;
		awd_float64 *invBindMtx;

		bone = skin->GetBone(i);

		skin->GetBoneInitTM(bone, invBindTM);
		invBindTM = Inverse(invBindTM);

		invBindMtx = (awd_float64*)malloc(sizeof(awd_float64) * 12);
		SerializeMatrix3(invBindTM, invBindMtx);

		name = bone->GetName();
		awdJoint = new AWDSkeletonJoint(name, strlen(name), invBindMtx);

		AWDSkeletonJoint *awdParent = (AWDSkeletonJoint *)skelCache.Get(bone->GetParentNode());
		if (awdParent != NULL) {
			awdParent->add_child_joint(awdJoint);
		}
		else {
			awdSkel->set_root_joint(awdJoint);
		}

		skelCache.Set(bone, awdJoint);
	}

	awd->add_skeleton(awdSkel);
}
