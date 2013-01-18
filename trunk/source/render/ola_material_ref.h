#ifndef _OLA_MATERIAL_REFERENCE_H__
#define _OLA_MATERIAL_REFERENCE_H__

#include "ola_stl.h"

#include "ola_material.h"

class OlaMaterialReference;

class OlaMaterialReferencePool
{
public:
	friend OlaMaterialReference;

	typedef OlaQueue<OlaMaterialReference*> MaterialRefQueue;
public:
	OlaMaterialReferencePool();
	virtual ~OlaMaterialReferencePool();

	OlaMaterialReference* requireUnusedReference(OlaMaterial* mat);

protected:

	OlaMaterialReference* mUsedRefHead;

	MaterialRefQueue mUnusedQueue;

};

class OlaMaterialReference : public IRefCounter
{
	friend OlaMaterialReferencePool;
public:
	void setMaterial(OlaMaterial* mat);

	inline OlaMaterial* material(){return mMaterial;}

	virtual void delRef();

protected:
	OlaMaterialReference(OlaMaterial* mat,OlaMaterialReferencePool* pool);

	virtual ~OlaMaterialReference();

	void _setNext(OlaMaterialReference* next);
	void _popOut();

	OlaMaterial* mMaterial;
	OlaMaterialReferencePool* mPool;

	OlaMaterialReference* mPre;
	OlaMaterialReference* mNext;
};

#endif