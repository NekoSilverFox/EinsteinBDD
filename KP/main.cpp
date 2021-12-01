#if 1


#pragma comment(lib, "bdd.lib")
#include "bdd.h"
#include <fstream>
#include <math.h>
#include <iomanip>


// **********************************************************************

#define N_VAR							144			// ��ڧ�ݧ� �ҧ�ݧ֧ӧ�� ��֧�֧ާ֧ߧߧ�� 9*4*4=144
#define N									9				// ��ڧ�ݧ� ��ҧ�֧ܧ���
#define SqrtN								3
#define M									4				// ��ڧ�ݧ� ��ӧ�ۧ���
#define LOG_N							4
#define ADD_CONDITIONS		0

// **********************************************************************

#define AND			&
#define OR				|
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
// ----------------------------------------------
#define TEL_NUM_0					0
#define TEL_NUM_1					1
#define TEL_NUM_2					2
#define TEL_NUM_3					3
#define TEL_NUM_4					4
#define TEL_NUM_5					5
#define TEL_NUM_6					6
#define TEL_NUM_7					7
#define TEL_NUM_8					8
// ----------------------------------------------
#define PHONE_APPLE				0
#define PHONE_XIAOMI				1
#define PHONE_HUAWEI			2
#define PHONE_SAMSUNG		3
#define PHONE_REALME			4
#define PHONE_HTC					5
#define PHONE_VIVO					6
#define PHONE_GOOGLE			7
#define PHONE_OPPO				8
// ----------------------------------------------
#define FOOD_CUKE					0
#define FOOD_WATERML			1
#define FOOD_PUMPKIN			2
#define FOOD_CHINKEN			3
#define FOOD_PIG						4
#define FOOD_BEEF					5
#define FOOD_CABBAGE			6
#define FOOD_BANANA				7
#define FOOD_PHOTO				8
// **********************************************************************

using namespace std;

ofstream out;
void fun(char* varset, int size);		// ���ߧܧ�ڧ�, �ڧ���ݧ�٧�֧ާѧ� �էݧ� �ӧ�ӧ�է� ��֧�֧ߧڧ�

// **********************************************************************

void main()
{
	// ��ʼ�� �ڧߧڧ�ڧѧݧڧ٧ѧ�ڧ�
	bdd_init(10000000, 1000000);
	bdd_setvarnum(N_VAR);


	// ->--- �ӧӧ�էڧ� ���ߧܧ�ڧ� p(k, i, j) ��ݧ֧է���ڧ� ��ҧ�ѧ٧�� ( pk[i][j] ):
	bdd p_name[N][N];			// ���ߧڧ�ڧѧݧ� ��ѧާڧݧڧ�
	bdd p_tel_num[N][N];		// �����ݧ֧էߧ�� ��ڧ��� ��֧ݧ֧��ߧ�
	bdd p_phone[N][N];		// ���ѧ�ܧ� ��֧ݧ֧��ߧ�
	bdd p_food[N][N];			// ����� �ݧ�ҧڧ� �֧���


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
			for (unsigned k = 0; k < LOG_N; k++)
			{
				p_name[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + k) : bdd_nithvar(I + k);
				p_tel_num[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N + k) : bdd_nithvar(I + LOG_N + k);
				p_phone[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * 2 + k) : bdd_nithvar(I + LOG_N * 2 + k);
				p_food[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * 3 + k) : bdd_nithvar(I + LOG_N * 3 + k);
			}
		}
		I += LOG_N * M;
	}

	bdd t_bdd = bddtrue;


	// ����������������������
	/////////////////////////////////////////////////////////// ���ԧ�ѧߧڧ�֧ߧڧ� I ��ڧ��. �� �٧ѧէѧߧڧ�: 6, �է���ݧߧڧ�֧ݧ�ߧ��: 7 ///////////////////////////////////////////////////////////

#if 1
	// 1.	`F` �اڧӧ֧� ��  �է�ާ� 1
	t_bdd &= p_name[7][NAME_G];

	// 2.	����� ���ݧ�٧�֧��� ��֧ݧ֧��ߧ� Samsung, �اڧӧ֧� �� �է�ާ� 5
	t_bdd &= p_phone[5][PHONE_SAMSUNG];

	// 3.	`E` �اڧӧ֧� �� �է�ާ� 3
	t_bdd &= p_name[3][NAME_E];

	// 4.	����� �ݧ�ҧڧ� �֧��� �ԧ�ӧ�էڧߧ�, �اڧӧ֧� �� �է�ާ� 0
	t_bdd &= p_food[0][FOOD_BEEF];

	// 5.	����� ���ݧ�٧�֧��� ��֧ݧ֧��ߧ�� Google, �اڧӧ֧� �� �է�ާ� 7
	t_bdd &= p_phone[7][PHONE_GOOGLE];

	// 6.	����� �ݧ�ҧڧ� �ѧ�ҧ��, �اڧӧ֧� �� �է�ާ� 6
	t_bdd &= p_food[6][FOOD_WATERML];

#if ADD_CONDITIONS // ################ �է���ݧߧڧ�֧ݧ�ߧ�� ���ݧ�ӧڧ� ################
	// `A` �اڧӧ֧� ��  �է�ާ� 2
	t_bdd &= p_name[2][NAME_A];

	// `D` �اڧӧ֧� ��  �է�ާ� 1
	t_bdd &= NOT p_name[1][NAME_D];

	// `H` �اڧӧ֧� ��  �է�ާ� 6
	t_bdd &= p_name[6][NAME_H];

	// ����� ����ݧ֧էߧ�� ��ڧ��� �ߧ�ާ֧�� ��֧ݧ֧��ߧ� ��ѧӧߧ� 2, �اڧӧ֧� �� �է�ާ� 2
	t_bdd &= p_tel_num[2][TEL_NUM_2];

	// ����� ����ݧ֧էߧ�� ��ڧ��� �ߧ�ާ֧�� ��֧ݧ֧��ߧ� ��ѧӧߧ� 0, �اڧӧ֧� �� �է�ާ� 3
	t_bdd &= p_tel_num[3][TEL_NUM_0];

	// ����� ����ݧ֧էߧ�� ��ڧ��� �ߧ�ާ֧�� ��֧ݧ֧��ߧ� ��ѧӧߧ� 8, �اڧӧ֧� �� �է�ާ� 7
	t_bdd &= p_tel_num[7][TEL_NUM_8];

	// ����� �ݧ�ҧڧ� �֧��� ���ܧӧ�, �اڧӧ֧� �� �է�ާ� 1
	t_bdd &= p_food[1][FOOD_PUMPKIN];

#endif // ADD_CONDITIONS n1
#endif
	cout << " > Constraints type 1 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";


	/////////////////////////////////////////////////////////// ���ԧ�ѧߧڧ�֧ߧڧ� II ��ڧ��. �� �٧ѧէѧߧڧ�: 3, �է���ݧߧڧ�֧ݧ�ߧ��: 8 ///////////////////////////////////////////////////////////

#if 1
	for (unsigned t = 0; t < N; t++)
	{
		//	1.	���ҧݧѧէѧ�֧ݧ� ��֧ݧ֧��ߧ�� Google �ݧ�ҧ�� �֧��� �ܧ��ڧ��
		t_bdd &= NOT(	p_phone[t][PHONE_GOOGLE]	XOR		p_food[t][FOOD_CHINKEN]	);

		//	2.	����, ���� ����ݧ֧էߧ�� ��ڧ��� �ߧ�ާ֧�� ��֧ݧ֧��ߧ� ��ѧӧߧ� 5, �ݧ�ҧ�� �֧��� �ҧѧߧѧߧ�
		t_bdd &= NOT(	p_tel_num[t][TEL_NUM_5]			XOR		p_food[t][FOOD_BANANA]		);

		//	3.	���ҧݧѧէѧ�֧ݧ� ��֧ݧ֧��ߧ�� Huawei, ����ݧ֧էߧ�� ��ڧ��� �ߧ�ާ֧�� ��֧ݧ֧��ߧ� ��ѧӧߧ� 1
		t_bdd &= NOT(	p_phone[t][PHONE_HUAWEI]	XOR		p_tel_num[t][TEL_NUM_1]		);


#if ADD_CONDITIONS // ################ �է���ݧߧڧ�֧ݧ�ߧ�� ���ݧ�ӧڧ� ################
		// �����䡿����� �� ��֧ݧ֧��ߧѧާ� Apple �ݧ�ҧ�� �֧��� �ҧѧߧѧߧ�
		t_bdd &= NOT(	p_phone[t][PHONE_APPLE]	XOR		p_food[t][FOOD_BANANA]		);

		// �����䡿`I` ���ݧ�٧�֧��� ��֧ݧ֧��ߧ�� HTC
		t_bdd &= NOT(	p_phone[t][PHONE_HTC]	XOR		p_name[t][NAME_I]		);

		// �����䡿����� �� ��֧ݧ֧��ߧѧާ� Xiaomi �ݧ�ҧ�� �֧��� �ԧ�ӧ�էڧߧ�
		t_bdd &= NOT(p_phone[t][PHONE_XIAOMI]	XOR		p_food[t][FOOD_BEEF]);

		// �����䡿����� ����ݧ֧էߧ�� ��ڧ��� �ߧ�ާ֧�� ��֧ݧ֧��ߧ� ��ѧӧߧ� 6, �ݧ�ҧ�� �֧��� c�ӧڧߧڧߧ�
		t_bdd &= NOT(p_tel_num[t][TEL_NUM_6]	XOR		p_food[t][FOOD_PIG]);

		// �����䡿����� ����ݧ֧էߧ�� ��ڧ��� �ߧ�ާ֧�� ��֧ݧ֧��ߧ� ��ѧӧߧ� 4, ���ݧ�٧�֧��� ��֧ݧ֧��ߧ�� Samsung
		t_bdd &= NOT(p_tel_num[t][TEL_NUM_4]	XOR		p_phone[t][PHONE_SAMSUNG]);

		// �����䡿`C` ����ݧ֧էߧ�� ��ڧ��� �ߧ�ާ֧�� ��֧ݧ֧��ߧ� ��ѧӧߧ� 3
		t_bdd &= NOT(p_tel_num[t][TEL_NUM_3]	XOR		p_name[t][NAME_C]);

		// �����䡿����� ����ݧ֧էߧ�� ��ڧ��� �ߧ�ާ֧�� ��֧ݧ֧��ߧ� ��ѧӧߧ� 0, ���ݧ�٧�֧��� ��֧ݧ֧��ߧ�� Realme
		t_bdd &= NOT(p_tel_num[t][TEL_NUM_0]	XOR		p_phone[t][PHONE_REALME]);

		// �����䡿����� ���ݧ�٧�֧��� ��֧ݧ֧��ߧ�� OPPO, �ݧ�ҧ�� �֧��� c�ӧڧߧڧߧ� �ѧ�ҧ��
		t_bdd &= NOT(p_phone[t][PHONE_OPPO]	XOR		p_food[t][FOOD_WATERML]);
#endif  // ADD_CONDITIONS n2
	}
#endif
	cout << " > Constraints type 2 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";



	/////////////////////////////////////////////////////////// ���ԧ�ѧߧڧ�֧ߧڧ� III ��ڧ��. �� �٧ѧէѧߧڧ�: 5. ///////////////////////////////////////////////////////////

#if 1
	// 1.	���֧ݧ�ӧ֧�, �اڧӧ��ڧ� ���� `C`, �ݧ�ҧ�� �ܧѧ����֧ݧ�
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
	// �����½��� �� �����ܧݧ֧ۧܧ� �� �ߧڧ�
	tmp_bdd1		|=		(p_name[		grid[2][1]		][NAME_C]		AND		p_food[		grid[0][1]		][FOOD_PHOTO]);
	t_bdd			&=	tmp_bdd1;
	// -----------------------------------------------------------------------------------------------------



	// 2.	���֧ݧ�ӧ֧�, �اڧӧ��ڧ� ���� ��֧ݧ�ӧ֧ܧ��, �ܧ������ ���ݧ�٧�֧��� ��֧ݧ֧��ߧ�� Apple, �ڧާ֧֧� ����ݧ֧էߧ�� ��ڧ��� �ߧ�ާ֧�� ��֧ݧ֧��ߧ� ��ѧӧߧ� 8
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
	// �����½��� �� �����ܧݧ֧ۧܧ� �� �ߧڧ�
	tmp_bdd2		|=		(p_phone[		grid[2][1]		][PHONE_APPLE]		AND		p_tel_num[		grid[0][1]		][TEL_NUM_8]);
	t_bdd			&=	tmp_bdd2;
	// -----------------------------------------------------------------------------------------------------



	// 3.	���֧ݧ�ӧ֧� �� ��֧ݧ֧��ߧѧާ� OPPO �اڧӧ�� ���� �ܧ�� �ݧ�ҧڧ�֧ݧ�ާ� �ܧѧ����֧ݧ�
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
	// �����½��� �� �����ܧݧ֧ۧܧ� �� �ߧڧ�
	tmp_bdd3		|=		(p_food[		grid[2][1]		][FOOD_PHOTO]		AND		p_phone[		grid[0][1]		][PHONE_OPPO]);
	t_bdd			&=	tmp_bdd3;
	// -----------------------------------------------------------------------------------------------------


	// 4. �� ���ѧӧ�� �ӧ֧��ߧ֧� F �اڧӧ֧� I ������-���ܧݧ֧ۧܧѡ������ˣ�����
	t_bdd		&=		NOT p_name[		grid[0][0]		][NAME_F];
	t_bdd		&=		NOT p_name[		grid[0][2]		][NAME_F];
	t_bdd		&=		NOT p_name[		grid[1][2]		][NAME_F];
	t_bdd		&=		NOT p_name[		grid[2][2]		][NAME_F];
	t_bdd		&=		NOT p_name[		grid[0][0]		][NAME_I];
	t_bdd		&=		NOT p_name[		grid[1][0]		][NAME_I];
	t_bdd		&=		NOT p_name[		grid[2][0]		][NAME_I];
	t_bdd		&=		NOT p_name[		grid[2][1]		][NAME_I];

	// ��������ָ�������δ���ǽ��ϵ����
	bdd tmp_bdd_3_13 = bddtrue;
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			tmp_bdd_3_13 |= (			p_name[		grid[i][j]					][NAME_F]
										AND p_name[		grid[i - 1][j + 1]		][NAME_I]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i - 1 << ", " << j + 1<< ">  " << endl;
		}
	}
	// �������Ͻ��� �J �����ܧݧ֧ۧܧ� �ߧ� ����ѧӧ�-�ӧӧ֧���
	tmp_bdd_3_13 |=		(p_name[		grid[0][1]		][NAME_F]		AND		p_name[		grid[2][2]		][NAME_I]);
	t_bdd		&= tmp_bdd_3_13;
	// -----------------------------------------------------------------------------------------------------



	//	5.	���֧ݧ�ӧ֧�, �ܧ������ �اڧӧ֧� ���� ��֧ݧ֧��ߧ�� Vivo, �ݧ�ҧڧ� �֧��� �ܧѧ�����
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
	// �����½��� �� �����ܧݧ֧ۧܧ� �� �ߧڧ�
	tmp_bdd5		|=		(p_phone[		grid[2][1]		][PHONE_VIVO]		AND		p_food[		grid[0][1]		][FOOD_CABBAGE]);
	t_bdd			&=	tmp_bdd5;
	// -----------------------------------------------------------------------------------------------------



#if ADD_CONDITIONS // ################ �է���ݧߧڧ�֧ݧ�ߧ�� ���ݧ�ӧڧ� ################

	// �����䡿�� ���ѧӧ�� �ӧ֧��ߧ֧� D �اڧӧ֧� A
	t_bdd		&=		NOT p_name[		grid[0][0]		][NAME_D];
	t_bdd		&=		NOT p_name[		grid[0][2]		][NAME_D];
	t_bdd		&=		NOT p_name[		grid[1][2]		][NAME_D];
	t_bdd		&=		NOT p_name[		grid[2][2]		][NAME_D];
	t_bdd		&=		NOT p_name[		grid[0][0]		][NAME_A];
	t_bdd		&=		NOT p_name[		grid[1][0]		][NAME_A];
	t_bdd		&=		NOT p_name[		grid[2][0]		][NAME_A];
	t_bdd		&=		NOT p_name[		grid[2][1]		][NAME_A];

	// ��������ָ�������δ���ǽ��ϵ����
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
	// �������Ͻ��� �J �����ܧݧ֧ۧܧ� �ߧ� ����ѧӧ�-�ӧӧ֧���
	tmp_bdd_3_9 |=		(p_name[		grid[0][1]		][NAME_D]		AND		p_name[		grid[2][2]		][NAME_A]);
	t_bdd		&= tmp_bdd_3_9;
	// -----------------------------------------------------------------------------------------------------

#endif // ADD_CONDITIONS

#endif

	cout << " > Constraints type 3 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";



	//  /////////////////////////////////////////////////////////// ���ԧ�ѧߧڧ�֧ߧڧ� IV ��ڧ��. �� �٧ѧէѧߧڧ�: 4. ///////////////////////////////////////////////////////////

#if 1
	// 1.	huawei, ��ܧѧ٧�ӧѧ��ڧ� �ߧ� I������-���ܧݧ֧ۧܧѡ�paoBUliao
	// F ��������
	t_bdd &= !p_phone[		grid[2][2]		][PHONE_HUAWEI];
	// I ��������
	t_bdd &= !p_name[		grid[0][0]		][NAME_I];
	t_bdd &= !p_name[		grid[1][0]		][NAME_I];

	// ������ָ�������δ���ǽ��ϵ���� 4.1.1
	bdd tmp_bdd_4_5_1 = bddtrue;
	bdd tmp_bdd_4_5_2 = bddtrue;
	for (unsigned i = 0; i < SqrtN - 1; i++)
	{
		for (unsigned j = 0; j < SqrtN; j++)
		{
			tmp_bdd_4_5_1 |= (				p_phone[		grid[i][j]				][PHONE_HUAWEI]
													AND p_name[		grid[i + 1][j]		][NAME_I]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}

	// ��������ָ�������δ���ǽ��ϵ���� 4.1.2
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			tmp_bdd_4_5_2		|= (				p_phone[		grid[i][j]					][FOOD_PUMPKIN]
													AND p_name[		grid[i - 1][j + 1]			][NAME_I]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i - 1 << ", " << j + 1<< ">  " << endl;
		}
	}
	// �����½��� �� �����ܧݧ֧ۧܧ� �� �ߧڧ�
	tmp_bdd_4_5_1		|=		(p_phone[		grid[2][1]		][PHONE_HUAWEI]		AND		p_name[		grid[0][1]		][NAME_I]);
	// �������Ͻ��� �J �����ܧݧ֧ۧܧ� �ߧ� ����ѧӧ�-�ӧӧ֧���
	tmp_bdd_4_5_2		|=		(p_phone[		grid[0][1]		][PHONE_HUAWEI]		AND		p_name[		grid[2][2]		][NAME_I]);
	t_bdd		&=		tmp_bdd_4_5_1;
	t_bdd		&=		tmp_bdd_4_5_2;
	 //-----------------------------------------------------------------------------------------------------



	// 2.	����� ���ݧ�٧�֧��� ��֧ݧ֧��ߧ�� VIVO, ��ܧѧ٧�ӧѧ��ڧ� �ߧ� �ܧ�� �ݧ�ҧ�� �֧��� �ԧ�ӧ�էڧߧ�
	// A ��������
	t_bdd &= !p_phone[		grid[2][2]		][PHONE_VIVO];
	// B ��������
	t_bdd &= !p_food[		grid[0][0]		][FOOD_CABBAGE];
	t_bdd &= !p_food[		grid[1][0]		][FOOD_CABBAGE];

	// ������ָ�������δ���ǽ��ϵ���� 4.2.1
	bdd tmp_bdd_4_2_1 = bddtrue;
	bdd tmp_bdd_4_2_2 = bddtrue;
	for (unsigned i = 0; i < SqrtN - 1; i++)
	{
		for (unsigned j = 0; j < SqrtN; j++)
		{
			tmp_bdd_4_2_1		 |= (p_phone[		grid[i][j]				][PHONE_VIVO]
													AND p_food[		grid[i + 1][j]		][FOOD_CABBAGE]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}

	// ��������ָ�������δ���ǽ��ϵ���� 4.2.2
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			tmp_bdd_4_2_2		|= (				p_phone[		grid[i][j]					][PHONE_VIVO]
													AND p_food[		grid[i - 1][j + 1]		][FOOD_CABBAGE]);
			cout << "F: <" << i << ", " << j << ">  " << "I: <" << i - 1 << ", " << j + 1<< ">  " << endl;
		}
	}
	// �����½��� �� �����ܧݧ֧ۧܧ� �� �ߧڧ�
	tmp_bdd_4_2_1		|=		(p_phone[		grid[2][1]		][PHONE_VIVO]		AND		p_food[		grid[0][1]		][FOOD_CABBAGE]);
	// �������Ͻ��� �J �����ܧݧ֧ۧܧ� �ߧ� ����ѧӧ�-�ӧӧ֧���
	tmp_bdd_4_2_2		|=		(p_phone[		grid[0][1]		][PHONE_VIVO]		AND		p_food[		grid[2][2]		][FOOD_CABBAGE]);
	t_bdd		&=		tmp_bdd_4_2_1;
	t_bdd		&=		tmp_bdd_4_2_2;
	// -----------------------------------------------------------------------------------------------------



	// 3.	`D`, ��ܧѧ٧�ӧѧ��ڧ� �ߧ� `G`
	// D ��������
	t_bdd &= !p_name[		grid[2][2]		][NAME_D];
	// G ��������
	t_bdd &= !p_name[		grid[0][0]		][NAME_G];
	t_bdd &= !p_name[		grid[1][0]		][NAME_G];

	// ������ָ�������δ���ǽ��ϵ���� 4.3.1
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
	// ��������ָ�������δ���ǽ��ϵ���� 4.3.2
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			tmp_bdd_4_3_2		|= (				p_name[		grid[i][j]					][NAME_D]
													AND p_name[		grid[i - 1][j + 1]		][NAME_G]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i - 1 << ", " << j + 1<< ">  " << endl;
		}
	}
	// �����½��� �� �����ܧݧ֧ۧܧ� �� �ߧڧ�
	tmp_bdd_4_3_1		|=		(p_name[		grid[2][1]		][NAME_D]		AND		p_name[		grid[0][1]		][NAME_G]);
	// �������Ͻ��� �J �����ܧݧ֧ۧܧ� �ߧ� ����ѧӧ�-�ӧӧ֧���
	tmp_bdd_4_3_2		|=		(p_name[		grid[0][1]		][NAME_D]		AND		p_name[		grid[2][2]		][NAME_G]);
	t_bdd		&=		tmp_bdd_4_3_1;
	t_bdd		&=		tmp_bdd_4_3_2;
	// -----------------------------------------------------------------------------------------------------



	// 4.	`E`, ��ܧѧ٧�ӧѧ��ڧ� �ߧ� `H`
	// E ��������
	t_bdd &= !p_name[		grid[2][2]		][NAME_E];
	// H ��������
	t_bdd &= !p_name[		grid[0][0]		][NAME_H];
	t_bdd &= !p_name[		grid[1][0]		][NAME_H];

	// ������ָ�������δ���ǽ��ϵ���� 4.4.1
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

	// ��������ָ�������δ���ǽ��ϵ���� 4.4.2
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			tmp_bdd_4_4_2		|= (				p_name[		grid[i][j]					][NAME_E]
													AND p_name[		grid[i - 1][j + 1]		][NAME_H]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i - 1 << ", " << j + 1<< ">  " << endl;
		}
	}
	// �����½��� �� �����ܧݧ֧ۧܧ� �� �ߧڧ�
	tmp_bdd_4_4_1		|=		(p_name[		grid[2][1]		][NAME_E]		AND		p_name[		grid[0][1]		][NAME_H]);
	//�������Ͻ��� �J �����ܧݧ֧ۧܧ� �ߧ� ����ѧӧ�-�ӧӧ֧���
	tmp_bdd_4_4_2		|=		(p_name[		grid[0][1]		][NAME_E]		AND		p_name[		grid[2][2]		][NAME_H]);
	t_bdd		&=		tmp_bdd_4_4_1;
	t_bdd		&=		tmp_bdd_4_4_2;
	// -----------------------------------------------------------------------------------------------------

#endif

	cout << " > Constraints type 4 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";



	///////////////////////////////////////////////////////////  ���ԧ�ѧߧڧ�֧ߧڧ� VI ��ڧ��.  ///////////////////////////////////////////////////////////
#if 1

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


	//���ԧ�ѧߧڧ�֧ߧڧ� V ��ڧ��.
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


	// �ӧ�ӧ�� ��֧٧�ݧ��ѧ���
	out.open("out.txt");
	unsigned satcount = (unsigned)bdd_satcount(t_bdd);
	out << satcount << " solution(s):\n" << endl;
	cout << "----------------------------------" << endl;
	cout << satcount << " solution(s):\n" << endl;
	if (satcount) bdd_allsat(t_bdd, fun);
	out.close();

	//------------------------------
	out.open("dot_all.txt");
	out << bdddot << t_bdd;
	out.close();
	//------------------------------

	bdd_done(); // �٧ѧӧ֧��֧ߧڧ� ��ѧҧ��� �ҧڧҧݧڧ��֧ܧ�
#endif
}


// ���ڧا� ��֧ѧݧڧ٧ѧ�ڧ� ���ߧܧ�ڧ�, ����ѧӧݧ���ڧ� �ӧ�ӧ�է�� ��֧٧�ݧ��ѧ���.
// ���֧ܧ�ާ֧ߧէ�֧��� ��ѧާ������֧ݧ�ߧ� �� �ߧڧާ� ��٧ߧѧܧ�ާڧ����.
// �� ���ҧ��ӧ֧ߧߧ�� �٧ѧէѧߧڧ�� ��ݧ֧է�֧� �ڧ���ݧ�٧�ӧѧ�� ���� ���ߧܧ�ڧ� �ڧݧ� ���ڧէ�ާѧ�� ���ҧ��ӧ֧ߧߧ��.
#if 1

char var[N_VAR];

void print(void)
{
	for (unsigned i = 0; i < N; i++)
	{
		out << i << ": ";
		cout << i << ": ";
		for (unsigned j = 0; j < M; j++)
		{
			unsigned J = i * M * LOG_N + j * LOG_N;
			unsigned num = 0;
			for (unsigned k = 0; k < LOG_N; k++) num += (unsigned)(var[J + k] << k);
			out << num << ' ';
			cout << num << ' ';
		}
		out << endl;
		cout << endl;
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

#endif
