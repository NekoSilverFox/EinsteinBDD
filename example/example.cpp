#include <iostream>
#include <fstream>

/** 增加库和头文件
*
*/
#pragma (lib, "bdd.h")
#include "bdd.h"

using namespace std;
ofstream out;

void fun(char* varset, int size);

int main()
{
	/** 初始化：分配内存（静态cache）
	* 1000 个 nodenum；cachesize 为100
	*/
	bdd_init(1000, 100);

	/** 说明有 6 个变量
	*
	*/
	bdd_setvarnum(6);

	/** 为这6个变量分配值
	* `bdd X = bdd_ithvar(value)` 代表为 变量X 分配值，值为 value
	* `bdd X = bdd_nithvar(value)` 代表为 变量**非X** 分配值，值为 value
	*/
	bdd a1 = bdd_ithvar(0);
	bdd a2 = bdd_ithvar(1);
	bdd a3 = bdd_ithvar(2);
	bdd b1 = bdd_ithvar(3);
	bdd b2 = bdd_ithvar(4);
	bdd b3 = bdd_ithvar(5);


	/** 用这些变量来组成公式(BDD树) g
	*		&	合取
	*		|	析取
	*/
	bdd g = a1 & b1 | a2 & b2 | a3 & b3;

	/** bdd_node_count(g)
	*	获取 BDD树 g 的节点数量
	*	число вершин BDD
	*/
	cout << "【bdd_node_count(g) 获取树g 的节点数量】 a1 < a2 < a3 < b1 < b2 <b3："
			<< bdd_nodecount(g) << " 个结点" << endl;  // 14 个结点


	/** out << bdddot << g << endl;
	*	输出为dot格式表述的BDD图
	*	输出流 << bdddot << BDD
	*	在线解析 dot 格式：https://dreampuf.github.io/GraphvizOnline/
	*/
	out.open("bdd1.txt");
	out << bdddot << g << endl;
	out.close();


	/** bdd_swapvar(n, m) 改变节点的顺序
	*					0		 1		  2		  3		  4		  5
	*	将：			a1 < a2 < a3 < b1 < b2 <b3
	*	改变为：	a1 < b1 < a2 < b2 < a3 < b3
	*/
	//											 0		 3		  2		  1		  4		  5
	bdd_swapvar(1, 3);		//		a1 < b1 < a3 < a2 < b2 <b3
	//											 0		 3		  1		  2		  4		  5
	bdd_swapvar(2, 1);		//		a1 < b1 < a2 < a3 < b2 <b3
	//											 0		 3		  1		  4		  2		  5
	bdd_swapvar(2, 4);		//		a1 < b1 < a2 < b2 < a3 <b3
	cout << "【bdd_node_count(g) 获取交换完节点后的树 g 的节点数量】 a1 < a2 < a3 < b1 < b2 <b3："
		<< bdd_nodecount(g) << " 个结点" << endl;  // 6 个结点
	out.open("bdd2.txt");
	out << bdddot << g << endl;
	out.close();


	/** out<<bddset<<z<<endl
	*	输出所有使该BDD成真的赋值

	*	bdd_satcount(BDD树)
	*
	*/
	out.open("set.txt");
	out << bdd_satcount(g) << " sets:\n" << endl;
	out << bddset << g << endl;
}

