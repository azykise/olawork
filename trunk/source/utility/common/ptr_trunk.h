#ifndef _OLA_INCLUDE__TRUNK_KIT__H__
#define _OLA_INCLUDE__TRUNK_KIT__H__

class OlaPtrTrunk
{
public:
	
	OlaPtrTrunk();
	~OlaPtrTrunk();	

	inline int size(){return len;}

	inline int count(){return 0;}

protected:
	unsigned int* data;
	int len;
};

#endif