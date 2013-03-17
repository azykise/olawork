#include <stdlib.h>

#include "pthread.h"
#include "ola_thread.h"

class OlaPTSingle : public OlaTSingle
{
public:
	OlaPTSingle():mSingle(0),mMutex(0)
	{
		pthread_cond_init(&mSingle,0);
		pthread_mutex_init(&mMutex,0);
	};

	virtual ~OlaPTSingle()
	{
		if(mSingle)
		{
			pthread_cond_destroy(&mSingle);
			mSingle = 0;
		}

		if(mMutex)
		{
			pthread_mutex_destroy(&mMutex);
			mMutex = 0;
		}
	};	

	pthread_cond_t mSingle;
	pthread_mutex_t mMutex;
};

class OlaPTMutex : public OlaTMutex
{
public:
	OlaPTMutex():mMutex(0)
	{
		pthread_mutexattr_t mattr;
		pthread_mutexattr_init(&mattr);
		pthread_mutexattr_settype(&mattr,PTHREAD_MUTEX_RECURSIVE_NP);

		pthread_mutex_init(&mMutex,&mattr);

		pthread_mutexattr_destroy(&mattr);
	}

	virtual ~OlaPTMutex()
	{
		if(mMutex)
		{
			pthread_mutex_destroy(&mMutex);
			mMutex = 0;
		}
	}

	virtual void request() 
	{
		if(mMutex)
		{
			pthread_mutex_lock(&mMutex);
		}
	}

	virtual void release() 
	{
		if (mMutex)
		{
			pthread_mutex_unlock(&mMutex);
		}
	}

	pthread_mutex_t mMutex;
};

struct tThreadProcParams
{
	OlaTThreadProc* proc;
	OlaTThreadObj* thd_obj;
};

void* ThreadProcFunBridge(void* params)
{
	tThreadProcParams* tparams = static_cast<tThreadProcParams*>(params);

	OlaTThreadProc* proc = tparams->proc;
	OlaTThreadObj* thd_obj = tparams->thd_obj;

	free(tparams);

	proc->threadProcFun(thd_obj);

	return 0;
}

class OlaPTThreadObj : public OlaTThreadObj
{
public:
	OlaPTThreadObj(OlaTThreadProc* proc):OlaTThreadObj(proc)
	{		
		tThreadProcParams* tparams = (tThreadProcParams*)malloc(sizeof(tThreadProcParams));
		tparams->proc = proc;
		tparams->thd_obj = this;
		pthread_create(&mThread,0,ThreadProcFunBridge,(void*)(tparams));
	}

	virtual ~OlaPTThreadObj()
	{
		//PTHREAD_MUTEX_INITIALIZER
	}

	pthread_t mThread;
};


OlaTSingle* OlaThread::CreateTSingle()
{
	return new OlaPTSingle();
}

OlaTMutex* OlaThread::CreateTMutex()
{
	return new OlaPTMutex();
}

OlaTThreadObj* OlaThread::CreateTThreadObj(OlaTThreadProc* proc)
{
	return new OlaPTThreadObj(proc);
}

void OlaThread::DeleteTObj( OlaTObj* tobj )
{
	delete tobj;
}

void OlaThread::SendThreadSingle( OlaTSingle* sng )
{
	OlaPTSingle* pt_sng = static_cast<OlaPTSingle*>(sng);

	pthread_mutex_lock(&(pt_sng->mMutex));
	pthread_cond_signal(&(pt_sng->mSingle));
	pthread_mutex_unlock(&(pt_sng->mMutex));
}

void OlaThread::WaitForThreadSingle( OlaTSingle* sng )
{
	OlaPTSingle* pt_sng = static_cast<OlaPTSingle*>(sng);
	pthread_mutex_lock(&(pt_sng->mMutex));
	pthread_cond_wait(&(pt_sng->mSingle),&(pt_sng->mMutex));
	pthread_mutex_unlock(&(pt_sng->mMutex));
}

void* OlaThread::WaitForThreadEnd( OlaTThreadObj* thd )
{
	OlaPTThreadObj* pt_thd = static_cast<OlaPTThreadObj*>(thd);

	void* p = 0;
	pthread_join(pt_thd->mThread,&p);
	return p;
}