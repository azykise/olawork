#ifndef _MINI_INCLUDE_SHARED_POINTER_H_
#define _MINI_INCLUDE_SHARED_POINTER_H_

#include <stdlib.h>
#include <string>

//�������ü�����ְ��ָ��
//ʹ��ͬһ��ԭ��ָ������ʼ��2������ָ���ǲ���ȷ��
template<typename T>
class SharedPointer
{
public:

	//ʹ��һ��ԭ��ָ�빹��һ��sp
	//ʹ��explicit��ֹ��SharedPointer<A> sp = new A();����ʽת����
	//ʹ��member template ��֤ SharedPointer<Base> sp( new Deriver() );�����̬��
	template<typename Y>
	explicit SharedPointer(Y* p):
		m_pRP(p),
		m_pCounter((unsigned int*)malloc(sizeof(unsigned int*)))
	{
		*m_pCounter = 1;
	}

	//SharedPointer<Base> spb( (SharedPointer<Deriver>)spd );
	template<typename Y>
	SharedPointer(const SharedPointer<Y>& sp):
		m_pRP(static_cast<T*>(sp.getPointer())),
		m_pCounter(sp.getCounter())
	{
		if (m_pCounter)
		{
			++(*m_pCounter);
		}
	}

	//SharedPointer<Base> spb = (SharedPointer<Deriver>)spd;
	template<typename Y>
	SharedPointer& operator = (const SharedPointer<Y>& sp)
	{
		if(m_pRP == sp.getPointer())
			return *this;

		SharedPointer<T> tmp(sp);
		swap(tmp);
		return *this;
	}

	//������ʱ��release
	~SharedPointer()
	{
		release();
	}

	//��дĬ�ϵĹ��캯��:��ָ��
	SharedPointer():
		m_pRP(0),
		m_pCounter(0)
	{

	}

	//��дĬ�ϸ��ƹ��캯��:���ü�����1
	SharedPointer(const SharedPointer& sp):
		m_pRP(sp.m_pRP),
		m_pCounter(sp.m_pCounter)
	{
		if(m_pCounter)
			++(*m_pCounter);
	}

	//��дĬ�ϸ�ֵ�����:tmp�Ĺ��콫��ptr������1 tmp��������ԭptr������һ
	inline SharedPointer& operator = (const SharedPointer& sp)
	{
		if(this->m_pRP == sp.m_pRP)
			return *this;

		SharedPointer<T> tmp(sp);
		swap(tmp);

		return *this;
	}

	template<typename Y>
	Y* convertTo(){return dynamic_cast<Y*>(m_pRP);}

	inline T& operator * () const {return *m_pRP;}
	inline T* operator -> () const {return m_pRP;} 

	inline bool isNull(void) const {return m_pRP == 0;}
	inline void setNull(void){if(m_pRP){release(); m_pRP = 0; m_pCounter = 0;}}

	inline T* getPointer() const {return m_pRP;}
	inline unsigned int* getCounter() const {return m_pCounter;}
	inline unsigned int usedCount()const {return *m_pCounter;}

protected:

	//������һ �������0���ͷ�ptr
	inline void release()
	{
		if(m_pCounter)
		{
			if(--(*m_pCounter) == 0)
				destory();
		}
	}

	//�ͷ�ptr
	inline void destory()
	{
		delete m_pRP;
		m_pRP = 0;
		free(m_pCounter);
		m_pCounter = 0;
	}

	template<typename Y>
	inline void swap(SharedPointer<Y>& sp)
	{
		std::swap(m_pRP,sp.m_pRP);
		std::swap(m_pCounter,sp.m_pCounter);
	}

	unsigned int* m_pCounter;
	T* m_pRP;
};

#ifndef pointer
#define pointer SharedPointer 
#endif

#endif
