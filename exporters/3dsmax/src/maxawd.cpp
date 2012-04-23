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
#include "utils.h"

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
	// Open the dialog (provided that prompts are not suppressed) and
	// if it returns false, return to cancel the export.
	if (!suppressPrompts && !opts.ShowDialog()) {
		return true;
	}

	PrepareExport();

	// Traverse MAX nodes and add to AWD structure.
	INode *root = i->GetRootNode();
	ExportSkeletons(root);
	ExportNode(root, NULL);

	// Export animation if enabled and if a sequences.txt file was found
	if (opts.ExportSkelAnim()) {
		SequenceMetaData *sequences = LoadSequenceFile(name);
		if (sequences != NULL)
			ExportAnimation(sequences);
	}

	// Flush serialized AWD structure to file
	int fd = open(name, _O_TRUNC | _O_CREAT | _O_BINARY | _O_RDWR, _S_IWRITE);
	awd->flush(fd);
	close(fd);

	// Copy viewer HTML and SWF template to output directory
	if (opts.CreatePreview()) {
		bool launch = (!suppressPrompts && opts.LaunchPreview());
		CopyViewer(name, launch);
	}

	// Free used memory
	CleanUp();

	// Export worked
	return TRUE;
}


void MaxAWDExporter::PrepareExport()
{
	cache = new BlockCache();
	colMtlCache = new ColorMaterialCache();
	skeletonCache = new SkeletonCache();
	awd = new AWD(UNCOMPRESSED, 0);
}


void MaxAWDExporter::CleanUp()
{
	delete awd;
	delete cache;
	delete colMtlCache;
	delete skeletonCache;
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


void MaxAWDExporter::CopyViewer(const TCHAR *awdFullPath, bool launch)
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

	if (launch) {
		ShellExecute(NULL, "open", outHtmlPath, NULL, NULL, SW_SHOWNORMAL);
	}
}


void MaxAWDExporter::ExportNode(INode *node, AWDSceneBlock *parent)
{
	Object *obj;
	bool goDeeper;

	AWDSceneBlock *awdParent = NULL;

	// By default, also parse children of this node
	goDeeper = true;

	obj = node->GetObjectRef();
	if (obj && obj->ClassID()==BONE_OBJ_CLASSID) {
		// This will have already been exported by the initial sweep
		// for bones/skeletons, so there is no need to recurse deeper
		goDeeper = false;
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

				// Add generated mesh instance to AWD scene graph.
				// This can be null, if exporter was configured not
				// to export scene graph objects.
				if (awdMesh) {
					if (parent) {
						parent->add_child(awdMesh);
					}
					else {
						awd->add_scene_block(awdMesh);
					}
				}

				// Store the new block (if any) as parent to be used for
				// blocks that represent children of this Max node.
				awdParent = awdMesh;
			}
		}
	}

	if (goDeeper) {
		int i;
		int numChildren = node->NumberOfChildren();
		for (i=0; i<numChildren; i++) {
			ExportNode(node->GetChildNode(i), awdParent);
		}
	}
}


AWDMeshInst * MaxAWDExporter::ExportTriObject(Object *obj, INode *node, ISkin *skin)
{
	AWDMaterial *awdMtl = NULL;
	AWDTriGeom *awdGeom = NULL;

	if (opts.ExportGeometry()) {
		awdGeom = ExportTriGeom(obj, node, skin);
	}

	// Export material
	if (opts.ExportMaterials()) {
		awdMtl = ExportNodeMaterial(node);
	}

	// Export instance
	if (opts.ExportScene()) {
		Matrix3 mtx = node->GetNodeTM(0) * Inverse(node->GetParentTM(0));
		double *mtxData = (double *)malloc(12*sizeof(double));
		SerializeMatrix3(mtx, mtxData);

		char *name = node->GetName();
		AWDMeshInst *inst = new AWDMeshInst(name, strlen(name), awdGeom, mtxData);

		if (awdMtl)
			inst->add_material(awdMtl);
	
		return inst;
	}

	return NULL;
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
	if (skin->GetNumBones()) {
		int iVtx;
		awd_float64 *weights;
		awd_uint32 *indices;

		// TODO: Replace with option
		const int jointsPerVertex = 2;

		// Get skeleton information from cache and geometry information
		// through an ISkinContextData interface.
		SkeletonCacheItem *skel = skeletonCache->GetFromBone(skin->GetBone(0));
		ISkinContextData *context = skin->GetContextInterface(node);

		// If the skeleton used for this skin could not be found,
		// break now or the code below will crash
		if (skel == NULL)
			return;

		// Configure skeleton (i.e. update bind matrices) for the 
		// binding defined by this particular skin.
		skel->ConfigureForSkin(skin);

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
				INode *bone = skin->GetBone(iBone);

				// Calculate index in stream
				int strIdx = iVtx*jointsPerVertex + iBone;

				if (iBone < numBones) {
					weights[strIdx] = context->GetBoneWeight(iVtx, iBone);
					indices[strIdx] = skel->IndexOfBone(bone);

					weightSum += weights[strIdx];
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
}


void MaxAWDExporter::ExportSkeletons(INode *node)
{
	Object *obj = node->GetObjectRef();
	if (obj && obj->ClassID() == BONE_OBJ_CLASSID) {
		ExportSkeleton(node);
	}
	else {
		// This wasn't a bone, but there might be bones
		// further down the hierarchy from this one
		int i;
		for (i=0; i<node->NumberOfChildren(); i++) {
			ExportSkeletons(node->GetChildNode(i));
		}
	}
}


void MaxAWDExporter::ExportSkeleton(INode *rootBone)
{
	// Add to skeleton cache so that animation export can find
	// this skeleton and sample it's animation. This will also
	// construct an intermediate structure that can be used to
	// look-up joint indices and more.
	AWDSkeleton *awdSkel = skeletonCache->Add(rootBone);
	awd->add_skeleton(awdSkel);
}


void MaxAWDExporter::ExportAnimation(SequenceMetaData *sequences)
{
	if (skeletonCache->HasItems() && sequences != NULL) {
		int ticksPerFrame;
		int frameDur;
		SkeletonCacheItem *curSkel;

		ticksPerFrame = GetTicksPerFrame();
		frameDur = floor(TicksToSec(ticksPerFrame) * 1000.0 + 0.5); // ms

		skeletonCache->IterReset();
		while ((curSkel = skeletonCache->IterNext()) != NULL) {
			SequenceMetaData *curSeq = sequences;

			while (curSeq) {
				int f;
				AWDSkeletonAnimation *awdAnim;

				// TODO: Consider concatenating names if >1 skeleton
				awdAnim = new AWDSkeletonAnimation(curSeq->name, strlen(curSeq->name));
				awd->add_skeleton_anim(awdAnim);

				// Loop through frames for this sequence and create poses
				for (f=curSeq->start; f<curSeq->stop; f++) {
					SkeletonCacheJoint *curJoint;
					AWDSkeletonPose *pose;

					TimeValue t = f * ticksPerFrame;

					// TODO: Consider coming  up with a proper name
					pose = new AWDSkeletonPose("", 0);

					// TODO: Sample all bones for this pose
					curSkel->IterReset();
					while ((curJoint = curSkel->IterNext()) != NULL) {
						INode *bone = curJoint->maxBone;
						Matrix3 tm = bone->GetNodeTM(t) * Inverse(bone->GetParentTM(t));

						awd_float64 *mtx = (awd_float64*)malloc(sizeof(awd_float64)*12);
						SerializeMatrix3(tm, mtx);

						pose->set_next_transform(mtx);
					}

					// Store pose in AWD document
					awdAnim->set_next_frame_pose(pose, frameDur);
					awd->add_skeleton_pose(pose);
				}

				// Proceed to next sequence
				curSeq = curSeq->next;
			}
		}
	}
}