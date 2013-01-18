

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Thu Oct 25 17:54:23 2007
 */
/* Compiler settings for .\maxcomponents.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __maxcomponents_h__
#define __maxcomponents_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISkinEngine_FWD_DEFINED__
#define __ISkinEngine_FWD_DEFINED__
typedef interface ISkinEngine ISkinEngine;
#endif 	/* __ISkinEngine_FWD_DEFINED__ */


#ifndef ___ISkinEngineEvents_FWD_DEFINED__
#define ___ISkinEngineEvents_FWD_DEFINED__
typedef interface _ISkinEngineEvents _ISkinEngineEvents;
#endif 	/* ___ISkinEngineEvents_FWD_DEFINED__ */


#ifndef __SkinEngine_FWD_DEFINED__
#define __SkinEngine_FWD_DEFINED__

#ifdef __cplusplus
typedef class SkinEngine SkinEngine;
#else
typedef struct SkinEngine SkinEngine;
#endif /* __cplusplus */

#endif 	/* __SkinEngine_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __MAXCOMPONENTSLib_LIBRARY_DEFINED__
#define __MAXCOMPONENTSLib_LIBRARY_DEFINED__

/* library MAXCOMPONENTSLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_MAXCOMPONENTSLib;

#ifndef __ISkinEngine_INTERFACE_DEFINED__
#define __ISkinEngine_INTERFACE_DEFINED__

/* interface ISkinEngine */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ISkinEngine;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("68CD6A91-34C2-11D3-9F71-00C04FB150E3")
    ISkinEngine : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetNumPoints( 
            /* [in] */ int numPoints) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetNumBones( 
            /* [in] */ int numBones) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetPointData( 
            /* [in] */ int pointIdx,
            /* [in] */ int numData,
            /* [in] */ DWORD b_stride,
            /* [in] */ int *BoneIndexArray,
            /* [in] */ DWORD w_stride,
            /* [in] */ float *WeightArray,
            /* [in] */ DWORD sci_stride,
            /* [in] */ int *SubCurveIdxArray,
            /* [in] */ DWORD ssi_stride,
            /* [in] */ int *SubSegIdxArray,
            /* [in] */ DWORD ssd_stride,
            /* [in] */ float *SubSegDistArray,
            /* [in] */ DWORD t_stride,
            /* [size_is][in] */ float *TangentsArray,
            /* [in] */ DWORD op_stride,
            /* [size_is][in] */ float *OPointsArray) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetInitBoneTM( 
            /* [in] */ int boneIdx,
            /* [size_is][in] */ float *InitTM) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetBoneTM( 
            /* [in] */ int boneIdx,
            /* [size_is][in] */ float *currentTM) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE MapPoint( 
            /* [in] */ int idx,
            /* [in] */ float *pin,
            /* [out] */ float *pout) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetInitTM( 
            /* [size_is][in] */ float *InitTM) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetBoneFlags( 
            /* [in] */ int boneIdx,
            /* [in] */ DWORD flags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISkinEngineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISkinEngine * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISkinEngine * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISkinEngine * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetNumPoints )( 
            ISkinEngine * This,
            /* [in] */ int numPoints);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetNumBones )( 
            ISkinEngine * This,
            /* [in] */ int numBones);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetPointData )( 
            ISkinEngine * This,
            /* [in] */ int pointIdx,
            /* [in] */ int numData,
            /* [in] */ DWORD b_stride,
            /* [in] */ int *BoneIndexArray,
            /* [in] */ DWORD w_stride,
            /* [in] */ float *WeightArray,
            /* [in] */ DWORD sci_stride,
            /* [in] */ int *SubCurveIdxArray,
            /* [in] */ DWORD ssi_stride,
            /* [in] */ int *SubSegIdxArray,
            /* [in] */ DWORD ssd_stride,
            /* [in] */ float *SubSegDistArray,
            /* [in] */ DWORD t_stride,
            /* [size_is][in] */ float *TangentsArray,
            /* [in] */ DWORD op_stride,
            /* [size_is][in] */ float *OPointsArray);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetInitBoneTM )( 
            ISkinEngine * This,
            /* [in] */ int boneIdx,
            /* [size_is][in] */ float *InitTM);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetBoneTM )( 
            ISkinEngine * This,
            /* [in] */ int boneIdx,
            /* [size_is][in] */ float *currentTM);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *MapPoint )( 
            ISkinEngine * This,
            /* [in] */ int idx,
            /* [in] */ float *pin,
            /* [out] */ float *pout);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetInitTM )( 
            ISkinEngine * This,
            /* [size_is][in] */ float *InitTM);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetBoneFlags )( 
            ISkinEngine * This,
            /* [in] */ int boneIdx,
            /* [in] */ DWORD flags);
        
        END_INTERFACE
    } ISkinEngineVtbl;

    interface ISkinEngine
    {
        CONST_VTBL struct ISkinEngineVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISkinEngine_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISkinEngine_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISkinEngine_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISkinEngine_SetNumPoints(This,numPoints)	\
    (This)->lpVtbl -> SetNumPoints(This,numPoints)

#define ISkinEngine_SetNumBones(This,numBones)	\
    (This)->lpVtbl -> SetNumBones(This,numBones)

#define ISkinEngine_SetPointData(This,pointIdx,numData,b_stride,BoneIndexArray,w_stride,WeightArray,sci_stride,SubCurveIdxArray,ssi_stride,SubSegIdxArray,ssd_stride,SubSegDistArray,t_stride,TangentsArray,op_stride,OPointsArray)	\
    (This)->lpVtbl -> SetPointData(This,pointIdx,numData,b_stride,BoneIndexArray,w_stride,WeightArray,sci_stride,SubCurveIdxArray,ssi_stride,SubSegIdxArray,ssd_stride,SubSegDistArray,t_stride,TangentsArray,op_stride,OPointsArray)

#define ISkinEngine_SetInitBoneTM(This,boneIdx,InitTM)	\
    (This)->lpVtbl -> SetInitBoneTM(This,boneIdx,InitTM)

#define ISkinEngine_SetBoneTM(This,boneIdx,currentTM)	\
    (This)->lpVtbl -> SetBoneTM(This,boneIdx,currentTM)

#define ISkinEngine_MapPoint(This,idx,pin,pout)	\
    (This)->lpVtbl -> MapPoint(This,idx,pin,pout)

#define ISkinEngine_SetInitTM(This,InitTM)	\
    (This)->lpVtbl -> SetInitTM(This,InitTM)

#define ISkinEngine_SetBoneFlags(This,boneIdx,flags)	\
    (This)->lpVtbl -> SetBoneFlags(This,boneIdx,flags)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISkinEngine_SetNumPoints_Proxy( 
    ISkinEngine * This,
    /* [in] */ int numPoints);


void __RPC_STUB ISkinEngine_SetNumPoints_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISkinEngine_SetNumBones_Proxy( 
    ISkinEngine * This,
    /* [in] */ int numBones);


void __RPC_STUB ISkinEngine_SetNumBones_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISkinEngine_SetPointData_Proxy( 
    ISkinEngine * This,
    /* [in] */ int pointIdx,
    /* [in] */ int numData,
    /* [in] */ DWORD b_stride,
    /* [in] */ int *BoneIndexArray,
    /* [in] */ DWORD w_stride,
    /* [in] */ float *WeightArray,
    /* [in] */ DWORD sci_stride,
    /* [in] */ int *SubCurveIdxArray,
    /* [in] */ DWORD ssi_stride,
    /* [in] */ int *SubSegIdxArray,
    /* [in] */ DWORD ssd_stride,
    /* [in] */ float *SubSegDistArray,
    /* [in] */ DWORD t_stride,
    /* [size_is][in] */ float *TangentsArray,
    /* [in] */ DWORD op_stride,
    /* [size_is][in] */ float *OPointsArray);


void __RPC_STUB ISkinEngine_SetPointData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISkinEngine_SetInitBoneTM_Proxy( 
    ISkinEngine * This,
    /* [in] */ int boneIdx,
    /* [size_is][in] */ float *InitTM);


void __RPC_STUB ISkinEngine_SetInitBoneTM_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISkinEngine_SetBoneTM_Proxy( 
    ISkinEngine * This,
    /* [in] */ int boneIdx,
    /* [size_is][in] */ float *currentTM);


void __RPC_STUB ISkinEngine_SetBoneTM_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISkinEngine_MapPoint_Proxy( 
    ISkinEngine * This,
    /* [in] */ int idx,
    /* [in] */ float *pin,
    /* [out] */ float *pout);


void __RPC_STUB ISkinEngine_MapPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISkinEngine_SetInitTM_Proxy( 
    ISkinEngine * This,
    /* [size_is][in] */ float *InitTM);


void __RPC_STUB ISkinEngine_SetInitTM_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISkinEngine_SetBoneFlags_Proxy( 
    ISkinEngine * This,
    /* [in] */ int boneIdx,
    /* [in] */ DWORD flags);


void __RPC_STUB ISkinEngine_SetBoneFlags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISkinEngine_INTERFACE_DEFINED__ */


#ifndef ___ISkinEngineEvents_INTERFACE_DEFINED__
#define ___ISkinEngineEvents_INTERFACE_DEFINED__

/* interface _ISkinEngineEvents */
/* [object][helpstring][uuid] */ 


EXTERN_C const IID IID__ISkinEngineEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("68CD6A93-34C2-11D3-9F71-00C04FB150E3")
    _ISkinEngineEvents : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetInterpCurvePiece3D( 
            /* [in] */ int BoneId,
            /* [in] */ int CurveId,
            /* [in] */ int SegId,
            /* [in] */ float distance,
            /* [size_is][out] */ float *pPoint) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTangentPiece3D( 
            /* [in] */ int BoneId,
            /* [in] */ int CurveId,
            /* [in] */ int SegId,
            /* [in] */ float distance,
            /* [size_is][out] */ float *pPoint) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct _ISkinEngineEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ISkinEngineEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ISkinEngineEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ISkinEngineEvents * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetInterpCurvePiece3D )( 
            _ISkinEngineEvents * This,
            /* [in] */ int BoneId,
            /* [in] */ int CurveId,
            /* [in] */ int SegId,
            /* [in] */ float distance,
            /* [size_is][out] */ float *pPoint);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTangentPiece3D )( 
            _ISkinEngineEvents * This,
            /* [in] */ int BoneId,
            /* [in] */ int CurveId,
            /* [in] */ int SegId,
            /* [in] */ float distance,
            /* [size_is][out] */ float *pPoint);
        
        END_INTERFACE
    } _ISkinEngineEventsVtbl;

    interface _ISkinEngineEvents
    {
        CONST_VTBL struct _ISkinEngineEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ISkinEngineEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _ISkinEngineEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _ISkinEngineEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _ISkinEngineEvents_GetInterpCurvePiece3D(This,BoneId,CurveId,SegId,distance,pPoint)	\
    (This)->lpVtbl -> GetInterpCurvePiece3D(This,BoneId,CurveId,SegId,distance,pPoint)

#define _ISkinEngineEvents_GetTangentPiece3D(This,BoneId,CurveId,SegId,distance,pPoint)	\
    (This)->lpVtbl -> GetTangentPiece3D(This,BoneId,CurveId,SegId,distance,pPoint)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE _ISkinEngineEvents_GetInterpCurvePiece3D_Proxy( 
    _ISkinEngineEvents * This,
    /* [in] */ int BoneId,
    /* [in] */ int CurveId,
    /* [in] */ int SegId,
    /* [in] */ float distance,
    /* [size_is][out] */ float *pPoint);


void __RPC_STUB _ISkinEngineEvents_GetInterpCurvePiece3D_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE _ISkinEngineEvents_GetTangentPiece3D_Proxy( 
    _ISkinEngineEvents * This,
    /* [in] */ int BoneId,
    /* [in] */ int CurveId,
    /* [in] */ int SegId,
    /* [in] */ float distance,
    /* [size_is][out] */ float *pPoint);


void __RPC_STUB _ISkinEngineEvents_GetTangentPiece3D_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* ___ISkinEngineEvents_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SkinEngine;

#ifdef __cplusplus

class DECLSPEC_UUID("68CD6A92-34C2-11D3-9F71-00C04FB150E3")
SkinEngine;
#endif
#endif /* __MAXCOMPONENTSLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


