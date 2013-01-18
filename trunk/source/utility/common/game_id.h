#pragma once

typedef unsigned long long game_id;
typedef unsigned int	   uint_id;
//------------------------------
//name: ID生成器
//desc: 生成一个ID 由生成时间(精确到秒) id生成类型 生成序号 组成
//by:	lwf 2010.8.15
//------------------------------
namespace GameID
{
	//生成器初始化
	void init();

	//获取一个新的ID id 生成类型可以是一个16位的随机值
	game_id new_id(unsigned short type_id);
	//将64位id转换成32位 保留了 id生成类型和id生成序号
	uint_id short_id(game_id id);
};