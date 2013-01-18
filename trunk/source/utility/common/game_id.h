#pragma once

typedef unsigned long long game_id;
typedef unsigned int	   uint_id;
//------------------------------
//name: ID������
//desc: ����һ��ID ������ʱ��(��ȷ����) id�������� ������� ���
//by:	lwf 2010.8.15
//------------------------------
namespace GameID
{
	//��������ʼ��
	void init();

	//��ȡһ���µ�ID id �������Ϳ�����һ��16λ�����ֵ
	game_id new_id(unsigned short type_id);
	//��64λidת����32λ ������ id�������ͺ�id�������
	uint_id short_id(game_id id);
};