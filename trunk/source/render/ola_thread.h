#ifndef _MINI_INCLUDE_OLA_THREAD_H_
#define _MINI_INCLUDE_OLA_THREAD_H_

class OlaTObj
{
public:
	virtual ~OlaTObj(){};
};

class OlaTSingle : public OlaTObj
{
protected:
	OlaTSingle(){};
};

class OlaTMutex : public OlaTObj
{
protected:
	OlaTMutex(){};
public:
	virtual void request() = 0;
	virtual void release() = 0;
};

class OlaTThreadObj;
class OlaTThreadProc
{	
public:
	virtual int threadProcFun(OlaTThreadObj* thd_obj) = 0;
};

class OlaTThreadObj : public OlaTObj
{
protected:
	OlaTThreadObj(OlaTThreadProc* proc){};
};

class OlaThread
{
public:	

	static OlaTSingle* CreateTSingle();

	static OlaTMutex* CreateTMutex();

	static OlaTThreadObj* CreateTThreadObj(OlaTThreadProc* proc);

	static void DeleteTObj(OlaTObj* tobj);

	static void SendThreadSingle(OlaTSingle* sng);

	static void WaitForThreadSingle(OlaTSingle* sng);

	static void* WaitForThreadEnd(OlaTThreadObj* thd);
};

#endif