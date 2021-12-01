#if 1


#pragma comment(lib, "bdd.lib")
#include "bdd.h"
#include <fstream>
#include <math.h>
#include <iomanip>


// **********************************************************************

#define N_VAR		144			// число булевых переменных 9*4*4=144
#define N				9				// число объектов
#define SqrtN		3
#define M				4				// число свойств
#define LOG_N		4
#define ADD_CONDITIONS		1

// **********************************************************************

#define AND			&
#define OR			|
#define XOR			^
#define NOT			!

// **********************************************************************
#define NAME_A						0
#define NAME_B						1
#define NAME_C						2
#define NAME_D						3
#define NAME_E						4
#define NAME_F						5
#define NAME_G						6
#define NAME_H						7
#define NAME_I						8
// ----------------------------------
#define TEL_NUM_0					0
#define TEL_NUM_1					1
#define TEL_NUM_2					2
#define TEL_NUM_3					3
#define TEL_NUM_4					4
#define TEL_NUM_5					5
#define TEL_NUM_6					6
#define TEL_NUM_7					7
#define TEL_NUM_8					8
// ----------------------------------
#define PHONE_APPLE				0
#define PHONE_XIAOMI			1
#define PHONE_HUAWEI			2
#define PHONE_SAMSUNG		3
#define PHONE_REALME			4
#define PHONE_HTC					5
#define PHONE_VIVO				6
#define PHONE_GOOGLE			7
#define PHONE_OPPO				8
// ----------------------------------
#define FOOD_CUKE					0
#define FOOD_WATERML			1
#define FOOD_PUMPKIN			2
#define FOOD_CHINKEN			3
#define FOOD_PIG					4
#define FOOD_BEEF					5
#define FOOD_CABBAGE			6
#define FOOD_BANANA			7
#define FOOD_PHOTO				8
// **********************************************************************

using namespace std;

ofstream out;
void fun(char* varset, int size); // функция, используемая для вывода решений

// **********************************************************************

void main()
{
	// 初始化 инициализация
	bdd_init(10000000, 1000000);
	bdd_setvarnum(N_VAR);

	// ->--- вводим функцию p(k, i, j) следующим образом ( pk[i][j] ):
	bdd p_name[N][N];		// Инициалы фамилии
	bdd p_tel_num[N][N];	// Последняя цифра телефона
	bdd p_phone[N][N];		// Марка телефона
	bdd p_food[N][N];			// Что любит есть

	int grid[SqrtN][SqrtN];
	int x = 0;
	for (unsigned iv = 0; iv < SqrtN; iv++)
	{
		for (unsigned ig = 0; ig < SqrtN; ig++)
		{
			grid[iv][ig] = x;
			x++;
		}
	}

	unsigned I = 0;
	for (unsigned i = 0; i < N; i++)
	{
		for (unsigned j = 0; j < N; j++)
		{
			p_name[i][j] = bddtrue;
			p_tel_num[i][j] = bddtrue;
			p_phone[i][j] = bddtrue;
			p_food[i][j] = bddtrue;
			for (unsigned k = 0; k < LOG_N; k++) {
				p_name[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + k) : bdd_nithvar(I + k);
				p_tel_num[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N + k) : bdd_nithvar(I + LOG_N + k);
				p_phone[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * 2 + k) : bdd_nithvar(I + LOG_N * 2 + k);
				p_food[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * 3 + k) : bdd_nithvar(I + LOG_N * 3 + k);
			}
		}
		I += LOG_N * M;
	}

	bdd t_bdd = bddtrue;


	// ОГРАНИЧЕНИЯ
	//  >>>>>>>>>>>>>>>>>> Ограничения I типа. В задании: 6, дополнительные: ? <<<<<<<<<<<<<<<<<<<<

#if 1
	// 1.	`F` живет в  доме 1
	t_bdd &= p_name[1][NAME_F];

	// 2.	Кто пользуется телефона Samsung, живет в доме 5
	t_bdd &= p_phone[5][PHONE_SAMSUNG];

	// 3.	`E` живет в доме 3
	t_bdd &= p_name[3][NAME_E];

	// 4.	Кто любит есть говядину, живет в доме 0
	t_bdd &= p_food[0][FOOD_BEEF];

	// 5.	Кто пользуется телефоном Google, живет в доме 7
	t_bdd &= p_phone[7][PHONE_GOOGLE];

	// 6.	Кто любит арбуз, живет в доме 6
	t_bdd &= p_food[6][FOOD_WATERML];

	//----- дополнительные условия -------
#if ADD_CONDITIONS
	t_bdd &= p_name[2][NAME_A];
	t_bdd &= p_name[4][NAME_D];
	t_bdd &= p_name[6][NAME_H];
	t_bdd &= p_name[8][NAME_I];
	t_bdd &= p_tel_num[0][TEL_NUM_3];
	t_bdd &= p_tel_num[2][TEL_NUM_2];
	t_bdd &= p_tel_num[3][TEL_NUM_0];
	t_bdd &= p_tel_num[7][TEL_NUM_8];
	t_bdd &= p_phone[2][PHONE_VIVO];
	t_bdd &= p_phone[4][PHONE_APPLE];
	t_bdd &= p_food[1][FOOD_PUMPKIN];
	t_bdd &= p_food[2][FOOD_CUKE];
	t_bdd &= p_food[5][FOOD_CABBAGE];
	t_bdd &= p_food[8][FOOD_PIG];

#endif

	cout << " > Constraints type 1 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";







	//  >>>>>>>> Ограничения II типа. В задании: 3, дополнительные: ? <<<<<<<

#if 1
	for (unsigned t = 0; t < N; t++)
	{
		//	1.	Обладатели телефонов Google любят есть курицу
		t_bdd &= NOT(	p_phone[t][PHONE_GOOGLE]	XOR		p_food[t][FOOD_CHINKEN]	);

		//	2.	Те, чья последняя цифра номера телефона равна 5, любят есть бананы
		t_bdd &= NOT(	p_tel_num[t][TEL_NUM_5]			XOR		p_food[t][FOOD_BANANA]		);

		//	3.	Обладатели телефонов Huawei, последняя цифра номера телефона равна 1
		t_bdd &= NOT(	p_phone[t][PHONE_HUAWEI]	XOR		p_tel_num[t][TEL_NUM_1]		);

		// 【补充】用苹果的人喜欢吃香蕉
		t_bdd &= NOT(	p_phone[t][PHONE_APPLE]	XOR		p_food[t][FOOD_BANANA]		);

		// 【补充】用小米的人喜欢吃喜欢吃牛肉
		t_bdd &= NOT(p_phone[t][PHONE_XIAOMI]	XOR		p_food[t][FOOD_BEEF]);

		// 【补充】尾号是6的人喜欢吃猪肉
		t_bdd &= NOT(p_tel_num[t][TEL_NUM_6]	XOR		p_food[t][FOOD_PIG]);

		// 【补充】尾号是4的人用三星手机
		t_bdd &= NOT(p_tel_num[t][TEL_NUM_4]	XOR		p_phone[t][PHONE_SAMSUNG]);

		// 【补充】尾号是0的人用realme手机
		t_bdd &= NOT(p_tel_num[t][TEL_NUM_0]	XOR		p_phone[t][PHONE_REALME]);

		// 【补充】用 OPPO 手机的人喜欢吃西瓜
		t_bdd &= NOT(p_phone[t][PHONE_OPPO]	XOR		p_food[t][FOOD_WATERML]);

		// 【补充】尾号是6的人用HTC手机
		t_bdd &= NOT(p_tel_num[t][TEL_NUM_6]	XOR		p_phone[t][PHONE_HTC]);

		//----- дополнительные условия -------

	}
#endif
	cout << " > Constraints type 2 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";








	//  >>>>>>>>>>>>>>>>>>>>>> Ограничения III типа. В задании: 5. <<<<<<<<<<<<<<<<<<<<<<<<

#if 1
	// 1.	Человек, живущие под `C`, любят картофель
	t_bdd		&=		NOT p_name[		grid[2][0]		][NAME_C];
	t_bdd		&=		NOT p_name[		grid[2][2]		][NAME_C];
	t_bdd		&=		NOT p_food[			grid[0][0]		][FOOD_PHOTO];
	t_bdd		&=		NOT p_food[			grid[0][2]		][FOOD_PHOTO];

	bdd tmp_bdd1 = bddtrue;
	for (unsigned i = 0; i < SqrtN - 1; i++)
	{
		for (unsigned j = 0; j < SqrtN; j++)
		{
			tmp_bdd1 |= (				p_name[		grid[i][j]				][NAME_C]
									AND		p_food[			grid[i + 1][j]		][FOOD_PHOTO]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}
	// 胶合
	tmp_bdd1		|=		(p_name[		grid[2][1]		][NAME_C]		AND		p_food[		grid[0][1]		][FOOD_PHOTO]);
	t_bdd			&=	tmp_bdd1;


	// 2.	Человек, живущий под человеком, который пользуется телефоном Apple, имеет последняя цифра номера телефона равна 8
	t_bdd		&=		NOT p_phone[		grid[2][0]		][PHONE_APPLE];
	t_bdd		&=		NOT p_phone[		grid[2][2]		][PHONE_APPLE];
	t_bdd		&=		NOT p_tel_num[	grid[0][0]		][TEL_NUM_8];
	t_bdd		&=		NOT p_tel_num[	grid[0][2]		][TEL_NUM_8];

	bdd tmp_bdd2 = bddtrue;
	for (unsigned i = 0; i < SqrtN - 1; i++)
	{
		for (unsigned j = 0; j < SqrtN; j++)
		{
			tmp_bdd2 |= (			p_phone[		grid[i][j]				][PHONE_APPLE]
									AND p_tel_num[	grid[i + 1][j]		][TEL_NUM_8]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}
	// 胶合
	tmp_bdd2		|=		(p_phone[		grid[2][1]		][PHONE_APPLE]		AND		p_tel_num[		grid[0][1]		][TEL_NUM_8]);
	t_bdd			&=	tmp_bdd2;


	// 7.【补充】	`E` живет под `C`
	t_bdd		&=		NOT p_name[		grid[2][0]		][NAME_C];
	t_bdd		&=		NOT p_name[		grid[2][2]		][NAME_C];
	t_bdd		&=		NOT p_name[		grid[0][0]		][NAME_E];
	t_bdd		&=		NOT p_name[		grid[0][2]		][NAME_E];

	bdd tmp_bdd7 = bddtrue;
	for (unsigned i = 0; i < SqrtN - 1; i++)
	{
		for (unsigned j = 0; j < SqrtN; j++)
		{
			tmp_bdd7 |= (			p_name[		grid[i][j]			][NAME_C]
									AND p_name[		grid[i + 1][j]	][NAME_E]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}
	// 向下胶合
	tmp_bdd7		|=		(p_name[		grid[2][1]		][NAME_C]		AND		p_name[		grid[0][1]		][NAME_E]);
	t_bdd			&=	tmp_bdd7;

	// 6.	【胶合-补充】用谷歌手机的人，下面的喜欢吃南瓜
	t_bdd		&=		NOT p_phone[		grid[2][0]		][PHONE_GOOGLE];
	t_bdd		&=		NOT p_phone[		grid[2][2]		][PHONE_GOOGLE];
	t_bdd		&=		NOT p_food[			grid[0][0]		][FOOD_PUMPKIN];
	t_bdd		&=		NOT p_food[			grid[0][2]		][FOOD_PUMPKIN];

	bdd tmp_bdd6 = bddtrue;
	for (unsigned i = 0; i < SqrtN - 1; i++)
	{
		for (unsigned j = 0; j < SqrtN; j++)
		{
			tmp_bdd6 |= (			p_phone[		grid[i][j]				][PHONE_GOOGLE]
									AND p_food[			grid[i + 1][j]		][FOOD_PUMPKIN]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}
	// 向下胶合
	tmp_bdd6		|=		(p_phone[		grid[2][1]		][PHONE_GOOGLE]		AND		p_food[		grid[0][1]		][FOOD_PUMPKIN]);
	t_bdd			&=	tmp_bdd6;


	// 3.	喜欢吃土豆的人，下面的用OPPO手机
	t_bdd		&=		NOT p_food[		grid[2][0]		][FOOD_PHOTO];
	t_bdd		&=		NOT p_food[		grid[2][2]		][FOOD_PHOTO];
	t_bdd		&=		NOT p_phone[			grid[0][0]		][PHONE_OPPO];
	t_bdd		&=		NOT p_phone[			grid[0][2]		][PHONE_OPPO];

	bdd tmp_bdd3 = bddtrue;
	for (unsigned i = 0; i < SqrtN - 1; i++)
	{
		for (unsigned j = 0; j < SqrtN; j++)
		{
			tmp_bdd3 |= (			p_food[		grid[i][j]				][FOOD_PHOTO]
									AND p_phone[			grid[i + 1][j]		][PHONE_OPPO]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}
	// 向下胶合
	tmp_bdd3		|=		(p_food[		grid[2][1]		][FOOD_PHOTO]		AND		p_phone[		grid[0][1]		][PHONE_OPPO]);
	t_bdd			&=	tmp_bdd3;

	//	4.	Человек, который живет под `B`, пользуется телефоном HTC
	t_bdd		&=		NOT p_name[		grid[2][0]		][NAME_B];
	t_bdd		&=		NOT p_name[		grid[2][2]		][NAME_B];
	t_bdd		&=		NOT p_phone[		grid[0][0]		][PHONE_HTC];
	t_bdd		&=		NOT p_phone[		grid[0][2]		][PHONE_HTC];

	bdd tmp_bdd4 = bddtrue;
	for (unsigned i = 0; i < SqrtN - 1; i++)
	{
		for (unsigned j = 0; j < SqrtN; j++)
		{
			tmp_bdd4 |= (			p_name[		grid[i][j]			][NAME_B]
									AND p_phone[		grid[i + 1][j]	][PHONE_HTC]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}
	// 向下胶合
	tmp_bdd4		|=		(p_name[		grid[2][1]		][NAME_B]		AND		p_phone[		grid[0][1]		][PHONE_HTC]);
	t_bdd			&=	tmp_bdd4;


	//	5.	Человек, который живет под телефоном Vivo, любит есть капусту
	t_bdd		&=		NOT p_phone[		grid[2][0]		][PHONE_VIVO];
	t_bdd		&=		NOT p_phone[		grid[2][2]		][PHONE_VIVO];
	t_bdd		&=		NOT p_food[			grid[0][0]		][FOOD_CABBAGE];
	t_bdd		&=		NOT p_food[			grid[0][2]		][FOOD_CABBAGE];

	bdd tmp_bdd5 = bddtrue;
	for (unsigned i = 0; i < SqrtN - 1; i++)
	{
		for (unsigned j = 0; j < SqrtN; j++)
		{
			tmp_bdd5 |= (			p_phone[		grid[i][j]			][PHONE_VIVO]
									AND p_food[			grid[i + 1][j]	][FOOD_CABBAGE]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}
	// 向下胶合
	tmp_bdd5		|=		(p_phone[		grid[2][1]		][PHONE_VIVO]		AND		p_food[		grid[0][1]		][FOOD_CABBAGE]);
	t_bdd			&=	tmp_bdd5;



	// 【补充】H的右上角住着D?
	t_bdd		&=		NOT p_name[		grid[0][0]		][NAME_H];
	t_bdd		&=		NOT p_name[		grid[0][2]		][NAME_H];
	t_bdd		&=		NOT p_name[		grid[1][2]		][NAME_H];
	t_bdd		&=		NOT p_name[		grid[2][2]		][NAME_H];
	t_bdd		&=		NOT p_name[		grid[0][0]		][NAME_D];
	t_bdd		&=		NOT p_name[		grid[1][0]		][NAME_D];
	t_bdd		&=		NOT p_name[		grid[2][0]		][NAME_D];
	t_bdd		&=		NOT p_name[		grid[2][1]		][NAME_D];


	// 【向右上指的情况】未考虑胶合的情况
	bdd tmp_bdd_3_6 = bddtrue;
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			tmp_bdd_3_6 |= (			p_name[		grid[i][j]					][NAME_H]
										AND p_name[		grid[i - 1][j + 1]		][NAME_D]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i - 1 << ", " << j + 1<< ">  " << endl;
		}
	}
	// 向右上胶合
	tmp_bdd_3_6 |=		(p_name[		grid[0][1]		][NAME_H]		AND		p_name[		grid[2][2]		][NAME_D]);
	t_bdd		&= tmp_bdd_3_6;


	// 【补充】E的右上角住着F?
	t_bdd		&=		NOT p_name[		grid[0][0]		][NAME_E];
	t_bdd		&=		NOT p_name[		grid[0][2]		][NAME_E];
	t_bdd		&=		NOT p_name[		grid[1][2]		][NAME_E];
	t_bdd		&=		NOT p_name[		grid[2][2]		][NAME_E];
	t_bdd		&=		NOT p_name[		grid[0][0]		][NAME_F];
	t_bdd		&=		NOT p_name[		grid[1][0]		][NAME_F];
	t_bdd		&=		NOT p_name[		grid[2][0]		][NAME_F];
	t_bdd		&=		NOT p_name[		grid[2][1]		][NAME_F];


	// 【向右上指的情况】未考虑胶合的情况
	bdd tmp_bdd_3_7 = bddtrue;
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			tmp_bdd_3_7 |= (			p_name[		grid[i][j]					][NAME_E]
										AND p_name[		grid[i - 1][j + 1]		][NAME_F]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i - 1 << ", " << j + 1<< ">  " << endl;
		}
	}
	// 向右上胶合
	tmp_bdd_3_7 |=		(p_name[		grid[0][1]		][NAME_E]		AND		p_name[		grid[2][2]		][NAME_F]);
	t_bdd		&= tmp_bdd_3_7;



		// 【补充】喜欢吃香蕉的右上角住着尾号是2的?
	t_bdd		&=		NOT p_food[				grid[0][0]		][FOOD_BANANA];
	t_bdd		&=		NOT p_food[				grid[0][2]		][FOOD_BANANA];
	t_bdd		&=		NOT p_food[				grid[1][2]		][FOOD_BANANA];
	t_bdd		&=		NOT p_food[				grid[2][2]		][FOOD_BANANA];
	t_bdd		&=		NOT p_tel_num[		grid[0][0]		][TEL_NUM_2];
	t_bdd		&=		NOT p_tel_num[		grid[1][0]		][TEL_NUM_2];
	t_bdd		&=		NOT p_tel_num[		grid[2][0]		][TEL_NUM_2];
	t_bdd		&=		NOT p_tel_num[		grid[2][1]		][TEL_NUM_2];


	// 【向右上指的情况】未考虑胶合的情况
	bdd tmp_bdd_3_8 = bddtrue;
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			tmp_bdd_3_8 |= (			p_food[				grid[i][j]					][FOOD_BANANA]
										AND p_tel_num[		grid[i - 1][j + 1]		][TEL_NUM_2]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i - 1 << ", " << j + 1<< ">  " << endl;
		}
	}
	// 向右上胶合
	tmp_bdd_3_8 |=		(p_food[		grid[0][1]		][FOOD_BANANA]		AND		p_tel_num[		grid[2][2]		][TEL_NUM_2]);
	t_bdd		&= tmp_bdd_3_8;



		// 【补充】D的右上角住着A?
	t_bdd		&=		NOT p_name[		grid[0][0]		][NAME_D];
	t_bdd		&=		NOT p_name[		grid[0][2]		][NAME_D];
	t_bdd		&=		NOT p_name[		grid[1][2]		][NAME_D];
	t_bdd		&=		NOT p_name[		grid[2][2]		][NAME_D];
	t_bdd		&=		NOT p_name[		grid[0][0]		][NAME_A];
	t_bdd		&=		NOT p_name[		grid[1][0]		][NAME_A];
	t_bdd		&=		NOT p_name[		grid[2][0]		][NAME_A];
	t_bdd		&=		NOT p_name[		grid[2][1]		][NAME_A];


	// 【向右上指的情况】未考虑胶合的情况
	bdd tmp_bdd_3_9 = bddtrue;
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			tmp_bdd_3_9 |= (			p_name[		grid[i][j]					][NAME_D]
										AND p_name[		grid[i - 1][j + 1]		][NAME_A]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i - 1 << ", " << j + 1<< ">  " << endl;
		}
	}
	// 向右上胶合
	tmp_bdd_3_9 |=		(p_name[		grid[0][1]		][NAME_D]		AND		p_name[		grid[2][2]		][NAME_A]);
	t_bdd		&= tmp_bdd_3_9;



		// 【补充】G的右上角住着B?
	t_bdd		&=		NOT p_name[		grid[0][0]		][NAME_G];
	t_bdd		&=		NOT p_name[		grid[0][2]		][NAME_G];
	t_bdd		&=		NOT p_name[		grid[1][2]		][NAME_G];
	t_bdd		&=		NOT p_name[		grid[2][2]		][NAME_G];
	t_bdd		&=		NOT p_name[		grid[0][0]		][NAME_B];
	t_bdd		&=		NOT p_name[		grid[1][0]		][NAME_B];
	t_bdd		&=		NOT p_name[		grid[2][0]		][NAME_B];
	t_bdd		&=		NOT p_name[		grid[2][1]		][NAME_B];


	// 【向右上指的情况】未考虑胶合的情况
	bdd tmp_bdd_3_10 = bddtrue;
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			tmp_bdd_3_10 |= (			p_name[		grid[i][j]					][NAME_G]
										AND p_name[		grid[i - 1][j + 1]		][NAME_B]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i - 1 << ", " << j + 1<< ">  " << endl;
		}
	}
	// 向右上胶合
	tmp_bdd_3_10 |=		(p_name[		grid[0][1]		][NAME_G]		AND		p_name[		grid[2][2]		][NAME_B]);
	t_bdd		&= tmp_bdd_3_10;
#endif

	cout << " > Constraints type 3 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";








	//  >>>>>>>>>>>>>>>>>>>>>> Ограничения IV типа. В задании: 4. <<<<<<<<<<<<<<<<<<<<<<<<

#if 1
	// 1.	`F`, указывающий на `I`【胶合】
	// F 不能在哪
	t_bdd &= !p_name[		grid[2][2]		][NAME_F];
	// I 不能在哪
	t_bdd &= !p_name[		grid[0][0]		][NAME_I];
	t_bdd &= !p_name[		grid[1][0]		][NAME_I];

	// 【向下指的情况】未考虑胶合的情况 4.1.1
	bdd tmp_bdd_4_1_1 = bddtrue;
	bdd tmp_bdd_4_1_2 = bddtrue;
	for (unsigned i = 0; i < SqrtN - 1; i++)
	{
		for (unsigned j = 0; j < SqrtN; j++)
		{
			tmp_bdd_4_1_1		 |= (				p_name[		grid[i][j]				][NAME_F]
													AND p_name[		grid[i + 1][j]		][NAME_I]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}

	// 【向右上指的情况】未考虑胶合的情况 4.1.2
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			tmp_bdd_4_1_2		|= (				p_name[		grid[i][j]					][NAME_F]
													AND p_name[		grid[i - 1][j + 1]		][NAME_I]);
			cout << "F: <" << i << ", " << j << ">  " << "I: <" << i - 1 << ", " << j + 1<< ">  " << endl;
		}
	}
	// 向下胶合
	tmp_bdd_4_1_1		|=		(p_name[		grid[2][1]		][NAME_F]		AND		p_name[		grid[0][1]		][NAME_I]);
	// 向右上胶合
	tmp_bdd_4_1_2		|=		(p_name[		grid[0][1]		][NAME_F]		AND		p_name[		grid[2][2]		][NAME_I]);
	t_bdd		&=		tmp_bdd_4_1_1;
	t_bdd		&=		tmp_bdd_4_1_2;


	// 2.	`A`, указывающий на `B`
	// A 不能在哪
	t_bdd &= !p_name[		grid[2][2]		][NAME_A];
	// B 不能在哪
	t_bdd &= !p_name[		grid[0][0]		][NAME_B];
	t_bdd &= !p_name[		grid[1][0]		][NAME_B];

	// 【向下指的情况】未考虑胶合的情况 4.2.1
	bdd tmp_bdd_4_2_1 = bddtrue;
	bdd tmp_bdd_4_2_2 = bddtrue;
	for (unsigned i = 0; i < SqrtN - 1; i++)
	{
		for (unsigned j = 0; j < SqrtN; j++)
		{
			tmp_bdd_4_2_1		 |= (				p_name[		grid[i][j]				][NAME_A]
													AND p_name[		grid[i + 1][j]		][NAME_B]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}

	// 【向右上指的情况】未考虑胶合的情况 4.2.2
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			tmp_bdd_4_2_2		|= (				p_name[		grid[i][j]					][NAME_A]
													AND p_name[		grid[i - 1][j + 1]		][NAME_B]);
			cout << "F: <" << i << ", " << j << ">  " << "I: <" << i - 1 << ", " << j + 1<< ">  " << endl;
		}
	}
	// 向下胶合
	tmp_bdd_4_2_1		|=		(p_name[		grid[2][1]		][NAME_A]		AND		p_name[		grid[0][1]		][NAME_B]);
	// 向右上胶合
	tmp_bdd_4_2_2		|=		(p_name[		grid[0][1]		][NAME_A]		AND		p_name[		grid[2][2]		][NAME_B]);
	t_bdd		&=		tmp_bdd_4_2_1;
	t_bdd		&=		tmp_bdd_4_2_2;


	// 3.	`D`, указывающий на `G`
	// D 不能在哪
	t_bdd &= !p_name[		grid[2][2]		][NAME_D];
	// G 不能在哪
	t_bdd &= !p_name[		grid[0][0]		][NAME_G];
	t_bdd &= !p_name[		grid[1][0]		][NAME_G];

	// 【向下指的情况】未考虑胶合的情况 4.3.1
	bdd tmp_bdd_4_3_1 = bddtrue;
	bdd tmp_bdd_4_3_2 = bddtrue;
	for (unsigned i = 0; i < SqrtN - 1; i++)
	{
		for (unsigned j = 0; j < SqrtN; j++)
		{
			tmp_bdd_4_3_1		 |= (				p_name[		grid[i][j]				][NAME_D]
													AND p_name[		grid[i + 1][j]		][NAME_G]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}

	// 【向右上指的情况】未考虑胶合的情况 4.3.2
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			tmp_bdd_4_3_2		|= (				p_name[		grid[i][j]					][NAME_D]
													AND p_name[		grid[i - 1][j + 1]		][NAME_G]);
			cout << "F: <" << i << ", " << j << ">  " << "I: <" << i - 1 << ", " << j + 1<< ">  " << endl;
		}
	}
	// 向下胶合
	tmp_bdd_4_3_1		|=		(p_name[		grid[2][1]		][NAME_D]		AND		p_name[		grid[0][1]		][NAME_G]);
	// 向右上胶合
	tmp_bdd_4_3_2		|=		(p_name[		grid[0][1]		][NAME_D]		AND		p_name[		grid[2][2]		][NAME_G]);
	t_bdd		&=		tmp_bdd_4_3_1;
	t_bdd		&=		tmp_bdd_4_3_2;



	// 4.	`E`, указывающий на `H`
	// E 不能在哪
	t_bdd &= !p_name[		grid[2][2]		][NAME_E];
	// H 不能在哪
	t_bdd &= !p_name[		grid[0][0]		][NAME_H];
	t_bdd &= !p_name[		grid[1][0]		][NAME_H];

	// 【向下指的情况】未考虑胶合的情况 4.4.1
	bdd tmp_bdd_4_4_1 = bddtrue;
	bdd tmp_bdd_4_4_2 = bddtrue;
	for (unsigned i = 0; i < SqrtN - 1; i++)
	{
		for (unsigned j = 0; j < SqrtN; j++)
		{
			tmp_bdd_4_4_1		 |= (				p_name[		grid[i][j]				][NAME_E]
													AND p_name[		grid[i + 1][j]		][NAME_H]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}

	// 【向右上指的情况】未考虑胶合的情况 4.4.2
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			tmp_bdd_4_4_2		|= (				p_name[		grid[i][j]					][NAME_E]
													AND p_name[		grid[i - 1][j + 1]		][NAME_H]);
			cout << "F: <" << i << ", " << j << ">  " << "I: <" << i - 1 << ", " << j + 1<< ">  " << endl;
		}
	}
	// 向下胶合
	tmp_bdd_4_4_1		|=		(p_name[		grid[2][1]		][NAME_E]		AND		p_name[		grid[0][1]		][NAME_H]);
	// 向右上胶合
	tmp_bdd_4_4_2		|=		(p_name[		grid[0][1]		][NAME_E]		AND		p_name[		grid[2][2]		][NAME_H]);
	t_bdd		&=		tmp_bdd_4_4_1;
	t_bdd		&=		tmp_bdd_4_4_2;


#endif


	cout << " > Constraints type 4 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#if 1
	// >>>>>>>>>>>>>>>>>>>>>> Ограничение VI типа.
	for (unsigned i = 0; i < N; i++)
	{
		bdd temp1 = bddfalse;
		bdd temp2 = bddfalse;
		bdd temp3 = bddfalse;
		bdd temp4 = bddfalse;

		for (unsigned j = 0; j < N; j++)
		{
			temp1 |= p_name[i][j];
			temp2 |= p_tel_num[i][j];
			temp3 |= p_phone[i][j];
			temp4 |= p_food[i][j];
		}
		t_bdd &= temp1 & temp2 & temp3 & temp4;
	}
	cout << " > Constraints type 6 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";


	//Ограничение V типа.
	for (unsigned j = 0; j < N; j++) {
		for (unsigned i = 0; i < N - 1; i++) {
			for (unsigned k = i + 1; k < N; k++) {
				t_bdd &= p_name[i][j] >> !p_name[k][j];
				t_bdd &= p_tel_num[i][j] >> !p_tel_num[k][j];
				t_bdd &= p_phone[i][j] >> !p_phone[k][j];
				t_bdd &= p_food[i][j] >> !p_food[k][j];
			}
		}
	}
	cout << " > Constraints type 5 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";


	// вывод результатов
	out.open("out.txt");
	unsigned satcount = (unsigned)bdd_satcount(t_bdd);
	out << satcount << " solution(s):\n" << endl;
	if (satcount) bdd_allsat(t_bdd, fun);
	out.close();

	//------------------------------
	out.open("dot_all.txt");
	out << bdddot << t_bdd;
	out.close();
	//------------------------------

	bdd_done(); // завершение работы библиотеки
#endif
}

// Ниже реализация функций, управляющих выводом результатов.
// Рекомендуется самостоятельно с ними ознакомиться.
// В собственных заданиях следует использовать эти функции или придумать собственные.

char var[N_VAR];

void print(void)
{
	for (unsigned i = 0; i < N; i++)
	{
		out << i << ": ";
		for (unsigned j = 0; j < M; j++)
		{
			unsigned J = i * M * LOG_N + j * LOG_N;
			unsigned num = 0;
			for (unsigned k = 0; k < LOG_N; k++) num += (unsigned)(var[J + k] << k);
			out << num << ' ';
		}
		out << endl;
	}
	out << endl;
}

void build(char* varset, unsigned n, unsigned I)
{
	if (I == n - 1)
	{
		if (varset[I] >= 0)
		{
			var[I] = varset[I];
			print();
			return;
		}
		var[I] = 0;
		print();
		var[I] = 1;
		print();
		return;
	}
	if (varset[I] >= 0)
	{
		var[I] = varset[I];
		build(varset, n, I + 1);
		return;
	}
	var[I] = 0;
	build(varset, n, I + 1);
	var[I] = 1;
	build(varset, n, I + 1);
}

void fun(char* varset, int size)
{
	build(varset, size, 0);
}

#endif