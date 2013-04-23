//************************************************************************** 
//* Asciiexp.h	- Ascii File Exporter
//* 
//* By Christer Janson
//* Kinetix Development
//*
//* January 20, 1997 CCJ Initial coding
//*
//* Class definition 
//*
//* Copyright (c) 1997, All Rights Reserved. 
//***************************************************************************

#ifndef __ASCIIEXP__H
#define __ASCIIEXP__H

#include <string>
#include <vector>
#include <map>

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "stdmat.h"
#include "decomp.h"
#include "shape.h"
#include "interpik.h"

#include "asciitok.h"

#include "IGame/IGame.h"
#include "IGame/IGameObject.h"
#include "IGame/IGameProperty.h"
#include "IGame/IGameControl.h"
#include "IGame/IGameModifier.h"
#include "IGame/IConversionManager.h"
#include "IGame/IGameError.h"
#include "IGame/IGameFX.h"

extern ClassDesc* GetAsciiExpDesc();
extern TCHAR *GetString(int id);
extern HINSTANCE hInstance;

#define VERSION			200			// Version number * 100
//#define FLOAT_OUTPUT	_T("%4.4f")	// Float precision for output
#define CFGFILENAME		_T("OLA_ASCIIEXP.CFG")	// Configuration file

class MtlKeeper {
public:
	BOOL	AddMtl(Mtl* mtl);
	int		GetMtlID(Mtl* mtl);
	int		Count();
	Mtl*	GetMtl(int id);

	Tab<Mtl*> mtlTab;
};

//bone info for every vertex
struct VertexBoneInfo
{
	VertexBoneInfo();
	void reset();
	int boneID[4];
	float weights[4];
	int out4;
};

struct SkeletonBone
{
	int id;
	TCHAR name[256];
};

typedef std::map<int,SkeletonBone*> SkeletonBoneTable;

// This is the main class for the exporter.
class BoneModData;
class ISkin;
class AsciiExp : public SceneExport {
public:
	AsciiExp();
	~AsciiExp();

	// SceneExport methods
	int    ExtCount();     // Number of extensions supported 
	const TCHAR * Ext(int n);     // Extension #n (i.e. "ASC")
	const TCHAR * LongDesc();     // Long ASCII description (i.e. "Ascii Export") 
	const TCHAR * ShortDesc();    // Short ASCII description (i.e. "Ascii")
	const TCHAR * AuthorName();    // ASCII Author name
	const TCHAR * CopyrightMessage();   // ASCII Copyright message 
	const TCHAR * OtherMessage1();   // Other message #1
	const TCHAR * OtherMessage2();   // Other message #2
	unsigned int Version();     // Version number * 100 (i.e. v3.01 = 301) 
	void	ShowAbout(HWND hWnd);  // Show DLL's "About..." box
	int		DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0); // Export	file
	BOOL	SupportsOptions(int ext, DWORD options);

	// Other methods

	// Node enumeration
	BOOL	nodeEnum(INode* node, int indentLevel);
	void	PreProcess(INode* node, int& nodeCount);

	// High level export
	void	ExportGlobalInfo();
	void	ExportMaterialList();	
	void	ExportGeomObject(INode* node, int indentLevel); 
	void	ExportLightObject(INode* node, int indentLevel); 
	void	ExportCameraObject(INode* node, int indentLevel); 
	void	ExportShapeObject(INode* node, int indentLevel); 
	void	ExportHelperObject(INode* node, int indentLevel);

	// Mid level export
	void	ExportMesh(INode* node, TimeValue t, int indentLevel); 	
	void	ExportAnimKeys(INode* node, int indentLevel);
	void	ExportMaterial(INode* node, int indentLevel); 
	void	ExportAnimMesh(INode* node, int indentLevel); 
	void	ExportSkeleton(INode* node);
	void	ExportIKJoints(INode* node, int indentLevel);
	void	ExportNodeTM(INode* node, int indentLevel);
	void	ExportNodeHeader(INode* node, TCHAR* type, int indentLevel);
	void	ExportCameraSettings(CameraState* cs, CameraObject* cam, TimeValue t, int indentLevel);
	void	ExportLightSettings(LightState* ls, GenLight* light, TimeValue t, int indentLevel);

	void	ExportDmlFile(INode* node);

	// Low level export
	void	DumpPoly(PolyLine* line, Matrix3 tm, int indentLevel);
	void	DumpMatrix3(Matrix3* m, int indentLevel);
	void	DumpMaterial(Mtl* mtl, int mtlID, int subNo, int indentLevel);
	void	DumpTexture(Texmap* tex, Class_ID cid, int subNo, float amt, int	indentLevel);
	void	DumpJointParams(JointParams* joint, int indentLevel);
	void	DumpUVGen(StdUVGen* uvGen, int indentLevel); 
	void	DumpPosSample(INode* node, int indentLevel); 
	void	DumpRotSample(INode* node, int indentLevel); 
	void	DumpScaleSample(INode* node, int indentLevel); 
	void	DumpPoint3Keys(Control* cont, int indentLevel);
	void	DumpFloatKeys(Control* cont, int indentLevel);
	void	DumpPosKeys(Control* cont, int indentLevel);
	void	DumpRotKeys(Control* cont, int indentLevel);
	void	DumpScaleKeys(Control* cont, int indentLevel);

	// Misc methods
	TCHAR*	GetMapID(Class_ID cid, int subNo);
	Point3	GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv);
	BOOL	CheckForAndExportFaceMap(Mtl* mtl, Mesh* mesh, int indentLevel); 
	void	make_face_uv(Face *f, Point3 *tv);
	BOOL	TMNegParity(Matrix3 &m);
	TSTR	GetIndent(int indentLevel);
	TCHAR*	FixupName(TCHAR* name);
	void	CommaScan(TCHAR* buf);
	BOOL	CheckForAnimation(INode* node, BOOL& pos, BOOL& rot, BOOL& scale);
	TriObject*	GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt);
	BOOL	IsKnownController(Control* cont);
	BoneModData* GetBoneModeData(INode* node);
	ISkin*	GetSelectedSkinModify(INode* node);
	void	GetVertexBoneInfo(VertexBoneInfo* out,int skined_vid,BoneModData* bone_mod,ISkin* skin);
	std::string ToLower(const std::string& str);
	std::string ReplaceAll(const std::string& str,const std::string& from,const std::string& to);
	std::string ReplaceChar(const std::string& str,char from,char to);
	std::string GetFilename(const std::string& fullname);

	// A collection of overloaded value to string converters.
	TSTR	Format(int value);
	TSTR	Format(float value);
	TSTR	Format(Color value);
	TSTR	Format(Point3 value); 
	TSTR	Format(AngAxis value); 
	TSTR	Format(Quat value);
	TSTR	Format(ScaleValue value);
	TSTR	Format(VertexBoneInfo& value);

	// Configuration methods
	TSTR	GetCfgFilename();
	BOOL	ReadConfig();
	void	WriteConfig();
	
	// Interface to member variables
	inline BOOL	GetIncludeMesh()			{ return bIncludeMesh; }
	inline BOOL	GetIncludeAnim()			{ return bIncludeAnim; }
	inline BOOL	GetIncludeMtl()				{ return bIncludeMtl; }
	inline BOOL GetIncludeSkin()			{ return bIncludeSkinInfo;}
	inline BOOL	GetIncludeSkeleton()		{ return bIncludeSkeletonInfo;}
	inline BOOL	GetIncludeMeshAnim()		{ return bIncludeMeshAnim; }
	inline BOOL	GetIncludeCamLightAnim()	{ return bIncludeCamLightAnim; }
	inline BOOL	GetIncludeIKJoints()		{ return bIncludeIKJoints; }
	inline BOOL	GetIncludeNormals()			{ return bIncludeNormals; }
	inline BOOL	GetIncludeTangents()		{ return bIncludeTangents;}
	inline BOOL	GetIncludeTextureCoords()	{ return bIncludeTextureCoords; }
	inline BOOL	GetIncludeVertexColors()	{ return bIncludeVertexColors; }
	inline BOOL	GetIncludeObjGeom()			{ return bIncludeObjGeom; }
	inline BOOL	GetIncludeObjShape()		{ return bIncludeObjShape; }
	inline BOOL	GetIncludeObjCamera()		{ return bIncludeObjCamera; }
	inline BOOL	GetIncludeObjLight()		{ return bIncludeObjLight; }
	inline BOOL	GetIncludeObjHelper()		{ return bIncludeObjHelper; }
	inline BOOL	GetAlwaysSample()			{ return bAlwaysSample; }
	inline int	GetMeshFrameStep()			{ return nMeshFrameStep; }
	inline int	GetKeyFrameStep()			{ return nKeyFrameStep; }
	inline int	GetPrecision()				{ return nPrecision; }
	inline TimeValue GetStaticFrame()		{ return nStaticFrame; }
	inline Interface*	GetInterface()		{ return ip; }


	inline void	SetIncludeMesh(BOOL val)			{ bIncludeMesh = val; }
	inline void	SetIncludeAnim(BOOL val)			{ bIncludeAnim = val; }
	inline void	SetIncludeMtl(BOOL val)				{ bIncludeMtl = val; }
	inline void SetIncludeSkin(BOOL val)			{ bIncludeSkinInfo = val;}
	inline void SetIncludeSkeleton(BOOL val)		{ bIncludeSkeletonInfo = val;}
	inline void SetIncludeTangent(BOOL val)			{ bIncludeTangents = val;}
	inline void	SetIncludeMeshAnim(BOOL val)		{ bIncludeMeshAnim = val; }
	inline void	SetIncludeCamLightAnim(BOOL val)	{ bIncludeCamLightAnim = val; }
	inline void	SetIncludeIKJoints(BOOL val)		{ bIncludeIKJoints = val; }
	inline void	SetIncludeNormals(BOOL val)			{ bIncludeNormals = val; }
	inline void	SetIncludeTextureCoords(BOOL val)	{ bIncludeTextureCoords = val; }
	inline void	SetIncludeVertexColors(BOOL val)	{ bIncludeVertexColors = val; }
	inline void	SetIncludeObjGeom(BOOL val)			{ bIncludeObjGeom = val; }
	inline void	SetIncludeObjShape(BOOL val)		{ bIncludeObjShape = val; }
	inline void	SetIncludeObjCamera(BOOL val)		{ bIncludeObjCamera = val; }
	inline void	SetIncludeObjLight(BOOL val)		{ bIncludeObjLight = val; }
	inline void	SetIncludeObjHelper(BOOL val)		{ bIncludeObjHelper = val; }
	inline void	SetAlwaysSample(BOOL val)			{ bAlwaysSample = val; }
	inline void SetMeshFrameStep(int val)			{ nMeshFrameStep = val; }
	inline void SetKeyFrameStep(int val)			{ nKeyFrameStep = val; }
	inline void SetPrecision(int val)				{ nPrecision = val; }
	inline void SetStaticFrame(TimeValue val)		{ nStaticFrame = val; }

private:
	BOOL	bIncludeMesh;
	BOOL	bIncludeAnim;
	BOOL	bIncludeMtl;
	BOOL	bIncludeMeshAnim;
	BOOL	bIncludeCamLightAnim;
	BOOL	bIncludeIKJoints;
	BOOL	bIncludeNormals;
	BOOL	bIncludeTextureCoords;
	BOOL	bIncludeObjGeom;
	BOOL	bIncludeObjShape;
	BOOL	bIncludeObjCamera;
	BOOL	bIncludeObjLight;
	BOOL	bIncludeObjHelper;
	BOOL	bAlwaysSample;
	BOOL	bIncludeVertexColors;
	BOOL	bIncludeSkinInfo;
	BOOL	bIncludeSkeletonInfo;
	BOOL    bIncludeTangents;
	int		nMeshFrameStep;
	int		nKeyFrameStep;
	int		nPrecision;
 	TimeValue	nStaticFrame;

	Interface*	ip;
	FILE*		pStream;
	std::string sFilename;
	int			nTotalNodeCount;
	int			nCurNode;
	TCHAR		szFmtStr[16];

	SkeletonBoneTable mSkeletonBones;

	IGameScene* mIGame;

	MtlKeeper	mtlList;
};

#endif // __ASCIIEXP__H
