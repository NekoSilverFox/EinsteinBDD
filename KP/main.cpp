#if 1


#pragma comment(lib, "bdd.lib")
#include "bdd.h"
#include <fstream>
#include <math.h>
#include <iomanip>


// **********************************************************************

#define N_VAR		144			// ��ڧ�ݧ� �ҧ�ݧ֧ӧ�� ��֧�֧ާ֧ߧߧ�� 9*4*4=144
#define N				9				// ��ڧ�ݧ� ��ҧ�֧ܧ���
#define SqrtN		3
#define M				4				// ��ڧ�ݧ� ��ӧ�ۧ���
#define LOG_N		4

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
void fun(char* varset, int size); // ���ߧܧ�ڧ�, �ڧ���ݧ�٧�֧ާѧ� �էݧ� �ӧ�ӧ�է� ��֧�֧ߧڧ�

// **********************************************************************

void main()
{
	// ��ʼ�� �ڧߧڧ�ڧѧݧڧ٧ѧ�ڧ�
	bdd_init(10000000, 1000000);
	bdd_setvarnum(N_VAR);

	// ->--- �ӧӧ�էڧ� ���ߧܧ�ڧ� p(k, i, j) ��ݧ֧է���ڧ� ��ҧ�ѧ٧�� ( pk[i][j] ):
	bdd p_name[N][N];		// ���ߧڧ�ڧѧݧ� ��ѧާڧݧڧ�
	bdd p_tel_num[N][N];	// �����ݧ֧էߧ�� ��ڧ��� ��֧ݧ֧��ߧ�
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






	// ����������������������
	//  >>>>>>>>>>>>>>>>>> ���ԧ�ѧߧڧ�֧ߧڧ� I ��ڧ��. �� �٧ѧէѧߧڧ�: 6, �է���ݧߧڧ�֧ݧ�ߧ��: ? <<<<<<<<<<<<<<<<<<<<
	// 1.	`F` �اڧӧ֧� ��  �է�ާ� 1
	t_bdd &= p_name[1][NAME_F];

	// 2.	����� ���ݧ�٧�֧��� ��֧ݧ֧��ߧ� Samsung, �اڧӧ֧� �� �է�ާ� 5
	t_bdd &= p_phone[5][PHONE_SAMSUNG];

	// 3.	`E` �اڧӧ֧� �� �է�ާ� 4
	t_bdd &= p_name[4][NAME_E];

	// 4.	����� �ݧ�ҧڧ� �֧��� �ԧ�ӧ�էڧߧ�, �اڧӧ֧� �� �է�ާ� 0
	t_bdd &= p_food[0][FOOD_BEEF];

	// 5.	����� ���ݧ�٧�֧��� ��֧ݧ֧��ߧ�� Google, �اڧӧ֧� �� �է�ާ� 7
	t_bdd &= p_phone[7][PHONE_GOOGLE];

	// 6.	����� �ݧ�ҧڧ� �ѧ�ҧ��, �اڧӧ֧� �� �է�ާ� 6
	t_bdd &= p_food[6][FOOD_WATERML];

	//----- �է���ݧߧڧ�֧ݧ�ߧ�� ���ݧ�ӧڧ� -------


	cout << " > Constraints type 1 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";







	//  >>>>>>>> ���ԧ�ѧߧڧ�֧ߧڧ� II ��ڧ��. �� �٧ѧէѧߧڧ�: 3, �է���ݧߧڧ�֧ݧ�ߧ��: ? <<<<<<<
	for (unsigned t = 0; t < N; t++)
	{
		//	1.	���ҧݧѧէѧ�֧ݧ� ��֧ݧ֧��ߧ�� Google �ݧ�ҧ�� �֧��� �ܧ��ڧ��
		t_bdd &= NOT(	p_phone[t][PHONE_GOOGLE]	XOR		p_food[t][FOOD_CHINKEN]	);

		//	2.	����, ���� ����ݧ֧էߧ�� ��ڧ��� �ߧ�ާ֧�� ��֧ݧ֧��ߧ� ��ѧӧߧ� 5, �ݧ�ҧ�� �֧��� �ҧѧߧѧߧ�
		t_bdd &= NOT(	p_tel_num[t][TEL_NUM_5]			XOR		p_food[t][FOOD_BANANA]		);

		//	3.	���ҧݧѧէѧ�֧ݧ� ��֧ݧ֧��ߧ�� Huawei, ����ݧ֧էߧ�� ��ڧ��� �ߧ�ާ֧�� ��֧ݧ֧��ߧ� ��ѧӧߧ� 1
		t_bdd &= NOT(	p_phone[t][PHONE_HUAWEI]	XOR		p_tel_num[t][TEL_NUM_1]		);

		//----- �է���ݧߧڧ�֧ݧ�ߧ�� ���ݧ�ӧڧ� -------
		//task &= !(p_name[t][6] ^ p_tel_num[t][3]); //22.	������ �٧�ӧ�� ���ѧ��. (n2)
		//task &= !(p_name[t][3] ^ p_tel_num[t][5]); //23.	���ا� �٧�ӧ�� ���קէ��. (n2)
		//task &= !(p_name[t][4] ^ p_food[t][2]); //24.	���֧� ���ק� �ܧ�ܧ�-�ܧ�ݧ�. (n2)
		//task &= !(p_name[t][2] ^ p_phone[t][6]); //25.	���ݧ֧ߧ� �ڧԧ�ѧ֧� �ߧ� �ѧ���. (n2)
		//task &= !(p_tel_num[t][8] ^ p_food[t][3]); //26.	����ݧڧߧ� ���ק� �ӧ�է�. (n2)
		//task &= !(p_name[t][1] ^ p_tel_num[t][7]); //27.	����ݧܧ� �٧�ӧ�� ���֧��. (n2)
		//task &= !(p_name[t][3] ^ p_phone[t][4]); //28.	���� �ڧԧ�ѧ֧� �ߧ� ��ѧܧ����ߧ�. (n2)
		//task &= !(p_food[t][2] ^ p_phone[t][5]); //29.	���ڧӧ��ߧ��, �ܧ������ ���ק� �ܧ�ܧ�-�ܧ�ݧ�, �ڧԧ�ѧ֧� �ߧ� �ҧѧ�ߧ�. (n2)
		//task &= !(p_name[t][7] ^ p_food[t][8]); //30.	����� ���ק� �ާ�ݧ�ܧ�. (n2)
		//task &= !(p_name[t][1] ^ p_food[t][6]); //31.	����ݧ� ���ק� �ӧ�էܧ�. (n2)
	}
	cout << " > Constraints type 2 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";









	//  >>>>>>>>>>>>>>>>>>>>>> ���ԧ�ѧߧڧ�֧ߧڧ� III ��ڧ��. �� �٧ѧէѧߧڧ�: 5. <<<<<<<<<<<<<<<<<<<<<<<<
	// 1.	���֧ݧ�ӧ֧�, �اڧӧ��ڧ� ���� `C`, �ݧ�ҧ�� �ܧѧ����֧ݧ�
	t_bdd		&=		NOT p_name[		grid[2][0]		][NAME_C];
	t_bdd		&=		NOT p_name[		grid[2][2]		][NAME_C];
	t_bdd		&=		NOT p_food[			grid[0][0]		][FOOD_PHOTO];
	t_bdd		&=		NOT p_food[			grid[0][2]		][FOOD_PHOTO];

#if 0
	for (unsigned iv = 0; iv < SqrtN; iv++)
	{
			t_bdd &=		NOT p_name[		grid[iv][0]					][NAME_C]
							& NOT p_food[			grid[iv][SqrtN - 1]		][FOOD_PHOTO];
	}

	for (unsigned ig = 0; ig < SqrtN; ig++)
	{
		t_bdd &=			NOT p_name[		grid[0][ig]				][NAME_C]
							& NOT p_food[		grid[SqrtN - 1][ig]		][FOOD_PHOTO];
	}

	for (unsigned iv = 1; iv < SqrtN; iv++)
	{
		for (unsigned ig = 1; ig < SqrtN; ig++)
		{
			t_bdd &=		NOT (			p_name[		grid[iv][ig]				][NAME_C]
											XOR p_food[		grid[iv - 1][ig - 1]		][FOOD_PHOTO]
										);
		}
	}


	// 2.	���֧ݧ�ӧ֧�, �اڧӧ��ڧ� ���� ��֧ݧ�ӧ֧ܧ��, �ܧ������ ���ݧ�٧�֧��� ��֧ݧ֧��ߧ�� Apple, �ڧާ֧֧� ����ݧ֧էߧ�� ��ڧ��� �ߧ�ާ֧�� ��֧ݧ֧��ߧ� ��ѧӧߧ� 8
	for (unsigned iv = 0; iv < SqrtN; iv++)
	{
			t_bdd &=		NOT p_phone[		grid[iv][0]					][PHONE_APPLE]
							& NOT p_tel_num[		grid[iv][SqrtN - 1]	][TEL_NUM_8];
	}

	for (unsigned ig = 0; ig < SqrtN; ig++)
	{
		t_bdd &=			NOT p_phone[		grid[0][ig]					][PHONE_APPLE]
							& NOT p_tel_num[		grid[SqrtN - 1][ig]	][TEL_NUM_8];
	}

	for (unsigned iv = 1; iv < SqrtN; iv++)
	{
		for (unsigned ig = 1; ig < SqrtN; ig++)
		{
			t_bdd &=		NOT (			p_phone[		grid[iv][ig]						][PHONE_APPLE]
											XOR p_tel_num[		grid[iv - 1][ig - 1]		][TEL_NUM_8]
										);
		}
	}

	//	3.	`C` �اڧӧ֧� �ߧڧا� `E`
		for (unsigned iv = 0; iv < SqrtN; iv++)
	{
			t_bdd &=		NOT p_name[		grid[iv][0]					][NAME_C]
							& NOT p_name[		grid[iv][SqrtN - 1]		][NAME_E];
	}

	for (unsigned ig = 0; ig < SqrtN; ig++)
	{
		t_bdd &=			NOT p_name[		grid[0][ig]					][NAME_C]
							& NOT p_name[		grid[SqrtN - 1][ig]		][NAME_E];
	}

	for (unsigned iv = 1; iv < SqrtN; iv++)
	{
		for (unsigned ig = 1; ig < SqrtN; ig++)
		{
			t_bdd &=		NOT(			p_name[		grid[iv][ig]					][NAME_C]
										XOR p_name[		grid[iv - 1][ig - 1]		][NAME_E]);
		}
	}

	//	4.	���֧ݧ�ӧ֧�, �ܧ������ �اڧӧ֧� ���� `B`, ���ݧ�٧�֧��� ��֧ݧ֧��ߧ�� HTC
	for (unsigned iv = 0; iv < SqrtN; iv++)
	{
			t_bdd &=		NOT p_name[		grid[iv][0]					][NAME_B]
							& NOT p_name[		grid[iv][SqrtN - 1]		][PHONE_HTC];
	}

	for (unsigned ig = 0; ig < SqrtN; ig++)
	{
		t_bdd &=			NOT p_name[		grid[0][ig]				][NAME_B]
							& NOT p_phone[		grid[SqrtN - 1][ig]	][PHONE_HTC];
	}

	for (unsigned iv = 1; iv < SqrtN; iv++)
	{
		for (unsigned ig = 1; ig < SqrtN; ig++)
		{
			t_bdd &= NOT(		p_name[		grid[iv][ig]					][NAME_B]
								XOR p_phone[		grid[iv - 1][ig - 1]		][PHONE_HTC]
								);
		}
	}

	//	5.	���֧ݧ�ӧ֧�, �ܧ������ �اڧӧ֧� ���� ��֧ݧ֧��ߧ�� Vivo, �ݧ�ҧڧ� �֧��� �ܧѧ�����
	for (unsigned iv = 0; iv < SqrtN; iv++)
	{
			t_bdd &=		NOT p_phone[		grid[iv][0]					][PHONE_VIVO]
							& NOT p_food[			grid[iv][SqrtN - 1]			][FOOD_CABBAGE];
	}

	for (unsigned ig = 0; ig < SqrtN; ig++)
	{
		t_bdd &=			NOT p_phone[		grid[0][ig]				][PHONE_VIVO]
							& NOT p_food[			grid[SqrtN - 1][ig]		][FOOD_CABBAGE];
	}

	for (unsigned iv = 1; iv < SqrtN; iv++)
	{
		for (unsigned ig = 1; ig < SqrtN; ig++)
		{
			t_bdd &=		NOT(			p_phone[		grid[iv][ig]				][PHONE_VIVO]
										XOR p_food[			grid[iv - 1][ig - 1]		][FOOD_CABBAGE]);
		}
	}
#endif
	cout << " > Constraints type 3 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";









#if 0

	//  >>>>>>>>>>>>>>>>>>>>>> ���ԧ�ѧߧڧ�֧ߧڧ� IV ��ڧ��. �� �٧ѧէѧߧڧ�: 4. <<<<<<<<<<<<<<<<<<<<<<<<
	// 1.	`F`, ��ܧѧ٧�ӧѧ��ڧ� �ߧ� `I`
	// F ��������
	t_bdd &= !p_name[		grid[2][2]		][NAME_F];
	// I ��������
	t_bdd &= !p_name[		grid[0][0]		][NAME_I];

	// ������ָ�������δ���ǽ��ϵ���� 4.1.1
	for (unsigned i = 0; i < SqrtN - 1; i++)
	{
		for (unsigned j = 0; j < SqrtN; j++)
		{
			t_bdd |= (				p_name[		grid[i][j]				][NAME_F]
								AND p_name[		grid[i + 1][j]		][NAME_I]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}
	// ��������ָ�������δ���ǽ��ϵ���� 4.1.2
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			t_bdd |= !(				p_name[		grid[i][j]				][NAME_F]
								XOR p_name[		grid[i - 1][j + 1]		][NAME_I]);
			cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}

	// 2.	`A`, ��ܧѧ٧�ӧѧ��ڧ� �ߧ� `B`


	// 3.	`D`, ��ܧѧ٧�ӧѧ��ڧ� �ߧ� `G`
	// 4.	`E`, ��ܧѧ٧�ӧѧ��ڧ� �ߧ� `H`


	// 16.	���֧� ����ݧѧ֧��� �ߧ� ��ݧ��.
	tmp_bdd_1 = bddtrue;
	tmp_bdd_2 = bddtrue;

	for (unsigned iv = 0; iv < SqrtN; iv++) tmp_bdd_1 &= !p_name[	grid[iv][0]	][4] & !p_name[		grid[iv][SqrtN - 1]	][5];
	for (unsigned ig = 0; ig < SqrtN; ig++) tmp_bdd_1 &= !p_name[	grid[0][ig]	][4] & !p_name[		grid[SqrtN - 1][ig]	][5];
	for (unsigned iv = 1; iv < SqrtN; iv++) {
		for (unsigned ig = 1; ig < SqrtN; ig++) tmp_bdd_1 &= !(p_name[grid[iv][ig]][4] ^ p_name[grid[iv - 1][ig - 1]][5]); //
	}

	for (unsigned iv = 0; iv < SqrtN; iv++) tmp_bdd_2 &= !p_name[grid[iv][0]][4] & !p_name[grid[iv][SqrtN - 1]][5];
	for (unsigned iv = 0; iv < SqrtN; iv++) { /*0 1 2*/
		for (unsigned ig = 1; ig < SqrtN; ig++) /* 1 2 */
			tmp_bdd_2 &= !(p_name[grid[iv][ig]][4] ^ p_name[grid[iv][ig-1]][5]);
	}
	t_bdd &= tmp_bdd_1 | tmp_bdd_2;

	// 17.	����ݧ� ����ݧѧ֧��� �ߧ� ��ݧ֧ߧ�.
	tmp_bdd_1 = bddtrue;
	tmp_bdd_2 = bddtrue;

	for (unsigned iv = 0; iv < SqrtN; iv++) tmp_bdd_1 &= !p_food[grid[iv][0]][6] & !p_tel_num[grid[iv][SqrtN - 1]][0];
	for (unsigned ig = 0; ig < SqrtN; ig++) tmp_bdd_1 &= !p_food[grid[0][ig]][6] & !p_tel_num[grid[SqrtN - 1][ig]][0];
	for (unsigned iv = 1; iv < SqrtN; iv++) {
		for (unsigned ig = 1; ig < SqrtN; ig++)
			tmp_bdd_1 &= !(p_food[grid[iv][ig]][6] ^ p_tel_num[grid[iv - 1][ig - 1]][0]);
	}

	for (unsigned iv = 0; iv < SqrtN; iv++) tmp_bdd_2 &= !p_food[grid[iv][0]][6] & !p_tel_num[grid[iv][SqrtN - 1]][0];
	for (unsigned iv = 0; iv < SqrtN; iv++) { /*0 1 2*/
		for (unsigned ig = 1; ig < SqrtN; ig++) /* 1 2 */
			tmp_bdd_2 &= !(p_food[grid[iv][ig]][6] ^ p_tel_num[grid[iv][ig - 1]][0]);
	}
	t_bdd &= tmp_bdd_1 | tmp_bdd_2;

	// 18. ���֧�� ����ݧѧ֧��� �ߧ� ���ڧ�ڧݧݧ�.
	tmp_bdd_1 = bddtrue;
	tmp_bdd_2 = bddtrue;

	for (unsigned iv = 0; iv < SqrtN; iv++) tmp_bdd_1 &= !p_tel_num[grid[iv][0]][4] & !p_tel_num[grid[iv][SqrtN - 1]][1];
	for (unsigned ig = 0; ig < SqrtN; ig++) tmp_bdd_1 &= !p_tel_num[grid[0][ig]][4] & !p_tel_num[grid[SqrtN - 1][ig]][1];
	for (unsigned iv = 1; iv < SqrtN; iv++) {
		for (unsigned ig = 1; ig < SqrtN; ig++) tmp_bdd_1 &= !(p_tel_num[grid[iv][ig]][4] ^ p_tel_num[grid[iv - 1][ig - 1]][1]);
	}

	for (unsigned iv = 0; iv < SqrtN; iv++) tmp_bdd_2 &= !p_tel_num[grid[iv][0]][4] & !p_tel_num[grid[iv][SqrtN - 1]][1];
	for (unsigned iv = 0; iv < SqrtN; iv++) { /*0 1 2*/
		for (unsigned ig = 1; ig < SqrtN; ig++) /* 1 2 */
			tmp_bdd_2 &= !(p_tel_num[grid[iv][ig]][4] ^ p_tel_num[grid[iv][ig - 1]][1]);
	}
	t_bdd &= tmp_bdd_1 | tmp_bdd_2;
#endif

	cout << " > Constraints type 4 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";







	// >>>>>>>>>>>>>>>>>>>>>> ���ԧ�ѧߧڧ�֧ߧڧ� VI ��ڧ��.
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
	if (satcount) bdd_allsat(t_bdd, fun);
	out.close();

	//------------------------------
	out.open("dot_all.txt");
	out << bdddot << t_bdd;
	out.close();
	//------------------------------

	bdd_done(); // �٧ѧӧ֧��֧ߧڧ� ��ѧҧ��� �ҧڧҧݧڧ��֧ܧ�
}

// ���ڧا� ��֧ѧݧڧ٧ѧ�ڧ� ���ߧܧ�ڧ�, ����ѧӧݧ���ڧ� �ӧ�ӧ�է�� ��֧٧�ݧ��ѧ���.
// ���֧ܧ�ާ֧ߧէ�֧��� ��ѧާ������֧ݧ�ߧ� �� �ߧڧާ� ��٧ߧѧܧ�ާڧ����.
// �� ���ҧ��ӧ֧ߧߧ�� �٧ѧէѧߧڧ�� ��ݧ֧է�֧� �ڧ���ݧ�٧�ӧѧ�� ���� ���ߧܧ�ڧ� �ڧݧ� ���ڧէ�ާѧ�� ���ҧ��ӧ֧ߧߧ��.

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