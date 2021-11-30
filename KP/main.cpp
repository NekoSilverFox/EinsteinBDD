#pragma comment(lib, "bdd.lib")
#include "bdd.h"
#include <fstream>
#include <math.h>
#include <iomanip>

using namespace std;
#define N_VAR 144	// ��ڧ�ݧ� �ҧ�ݧ֧ӧ�� ��֧�֧ާ֧ߧߧ�� 9*4*4=144
#define N 9				// ��ڧ�ݧ� ��ҧ�֧ܧ���
#define SqrtN 3
#define M 4				// ��ڧ�ݧ� ��ӧ�ۧ���
#define LOG_N 4


ofstream out;
void fun(char* varset, int size); // ���ߧܧ�ڧ�, �ڧ���ݧ�٧�֧ާѧ� �էݧ� �ӧ�ӧ�է� ��֧�֧ߧڧ�

void main(void) {

	// ��ʼ�� �ڧߧڧ�ڧѧݧڧ٧ѧ�ڧ�
	bdd_init(10000000, 1000000);
	bdd_setvarnum(N_VAR);

	// ->--- �ӧӧ�էڧ� ���ߧܧ�ڧ� p(k, i, j) ��ݧ֧է���ڧ� ��ҧ�ѧ٧�� ( pk[i][j] ):
	bdd p_name[N][N];	// ��������
	bdd p_last_num[N][N];	// ����
	bdd p_phone[N][N];	// ����
	bdd p_food[N][N];	// ����

	int iii[SqrtN][SqrtN];
	int x = 0;
	for (unsigned iv = 0; iv < SqrtN; iv++){
		for (unsigned ig = 0; ig < SqrtN; ig++){
			iii[iv][ig] = x;
			x++;
		}
	}
	unsigned I = 0;
	for (unsigned i = 0; i < N; i++) {
		for (unsigned j = 0; j < N; j++) {
			p_name[i][j] = bddtrue;
			p_last_num[i][j] = bddtrue;
			p_phone[i][j] = bddtrue;
			p_food[i][j] = bddtrue;
			for (unsigned k = 0; k < LOG_N; k++) {
				p_name[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + k) : bdd_nithvar(I + k);
				p_last_num[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N + k) : bdd_nithvar(I + LOG_N + k);
				p_phone[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * 2 + k) : bdd_nithvar(I + LOG_N * 2 + k);
				p_food[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * 3 + k) : bdd_nithvar(I + LOG_N * 3 + k);
			}
		}
		I += LOG_N * M;
	}

	bdd task = bddtrue;
#if 0
	//  >>>>>>>>>>>>>>>>>> ���ԧ�ѧߧڧ�֧ߧڧ� I ��ڧ��. �� �٧ѧէѧߧڧ�: 8, �է���ݧߧڧ�֧ݧ�ߧ��: 5 <<<<<<<<<<<<<<<<<<<<
	task &= p1[1][7]; //1.	���� �է�ާ� 1 �ߧѧ��էڧ��� �ܧ��. (n1)
	task &= p1[0][8]; //2.	���� �է�ާ� 0 �ߧѧ��էڧ��� �ҧ��. (n1)
	task &= p3[3][3]; //3.	���ڧӧ��ߧ��, �ߧѧ��է��֧֧�� �ߧ� �է�ާ� 3, �ڧԧ�ѧ֧� �ߧ� ��ܧ�ڧ�ܧ�. (n1)
	task &= p4[2][7]; //4.	���ڧӧ��ߧ��, �ߧѧ��է��֧֧�� �ߧ� �է�ާ� 2, ���ק� ����. (n1)
	task &= p2[4][4]; //5.	���ڧӧ��ߧ�ԧ�, �اڧӧ��֧ԧ� �� �է�ާ� 4, �٧�ӧ�� ����ާѧ�. (n1)
	task &= p1[5][3]; //6.	���� �اڧӧ֧� �� �է�ާ� 5. (n1)
	task &= p2[6][6]; //7.	���ڧӧ��ߧ�ԧ�, �اڧӧ��֧ԧ� �� �է�ާ� 6, �٧�ӧ�� ���֧�ԧ֧�. (n1)
	task &= p3[7][8]; //8.	���ڧӧ��ߧ��, �ߧѧ��է��֧֧�� �ߧ� �է�ާ� 7, �ڧԧ�ѧ֧� �ߧ� ����ҧ�. (n1)
	//----- �է���ݧߧڧ�֧ݧ�ߧ�� ���ݧ�ӧڧ� -------
	task &= p4[8][3]; //19.	 ���ڧӧ��ߧ��, �ܧ������ �اڧӧק� �� �է�ާ� 8, ���ק� �ӧ�է�.(n1)
	task &= p1[3][5]; //20.	 ���ݧ�� �اڧӧק� �� �է�ާ� 3.(n1)
	task &= p4[0][0]; //21.	 ���ڧӧ��ߧ��, �ܧ������ �اڧӧק� �� �է�ާ� 0, ���ק� ��ѧ�.(n1)
	task &= p3[8][7]; //32.	���ڧӧ��ߧ��, �ܧ������ �اڧӧק� �� �է�ާ� 8, �ڧԧ�ѧ֧� �ߧ� �ѧܧܧ��է֧�ߧ�.(n1)
	task &= p3[0][1]; //33.	���ڧӧ��ߧ��, �ܧ������ �اڧӧק� �� �է�ާ� 0, �ڧԧ�ѧ֧� �ߧ� ��ڧѧߧڧߧ�.(n1)
	task &= p3[1][0]; //34.	���ڧӧ��ߧ��, �ܧ������ �اڧӧק� �� �է�ާ� 1, �ڧԧ�ѧ֧� �ߧ� �ԧڧ�ѧ��.(n1)
	task &= p3[2][2]; //35.	���ڧӧ��ߧ��, �ܧ������ �اڧӧק� �� �է�ާ� 2, �ڧԧ�ѧ֧� �ߧ� �ӧڧ�ݧ�ߧ�֧ݧ�.(n1)
#endif

	//����������������������
	//  >>>>>>>>>>>>>>>>>> ���ԧ�ѧߧڧ�֧ߧڧ� I ��ڧ��. �� �٧ѧէѧߧڧ�: 8, �է���ݧߧڧ�֧ݧ�ߧ��: 5 <<<<<<<<<<<<<<<<<<<<
	task &= p_name[1][5];		// 1.	`F` �اڧӧ֧� ��  �է�ާ� 1
	task &= p_name[0][8];		// 2.	`B` ���ݧ�٧�֧��� ��֧ݧ֧��ߧ� Samsung
	task &= p_phone[3][3];		// 3.	`E` �اڧӧ֧� �� �է�ާ� 4
	task &= p_food[2][7];		// 4.	`C` �ݧ�ҧڧ� �֧��� �ԧ�ӧ�էڧߧ�
	task &= p_last_num[4][4];		// 5.	`G` ���ݧ�٧�֧��� ��֧ݧ֧��ߧ�� Google
	task &= p_name[5][3];		// 6.	`H` �ݧ�ҧڧ� �ѧ�ҧ��

	//----- �է���ݧߧڧ�֧ݧ�ߧ�� ���ݧ�ӧڧ� -------
	//task &= p4[8][3]; //19.	 ���ڧӧ��ߧ��, �ܧ������ �اڧӧק� �� �է�ާ� 8, ���ק� �ӧ�է�.(n1)
	//task &= p1[3][5]; //20.	 ���ݧ�� �اڧӧק� �� �է�ާ� 3.(n1)
	//task &= p4[0][0]; //21.	 ���ڧӧ��ߧ��, �ܧ������ �اڧӧק� �� �է�ާ� 0, ���ק� ��ѧ�.(n1)
	//task &= p3[8][7]; //32.	���ڧӧ��ߧ��, �ܧ������ �اڧӧק� �� �է�ާ� 8, �ڧԧ�ѧ֧� �ߧ� �ѧܧܧ��է֧�ߧ�.(n1)
	//task &= p3[0][1]; //33.	���ڧӧ��ߧ��, �ܧ������ �اڧӧק� �� �է�ާ� 0, �ڧԧ�ѧ֧� �ߧ� ��ڧѧߧڧߧ�.(n1)
	//task &= p3[1][0]; //34.	���ڧӧ��ߧ��, �ܧ������ �اڧӧק� �� �է�ާ� 1, �ڧԧ�ѧ֧� �ߧ� �ԧڧ�ѧ��.(n1)
	//task &= p3[2][2]; //35.	���ڧӧ��ߧ��, �ܧ������ �اڧӧק� �� �է�ާ� 2, �ڧԧ�ѧ֧� �ߧ� �ӧڧ�ݧ�ߧ�֧ݧ�.(n1)











	cout << " > Constraints type 1 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(task) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(task) << "\n\n";

	//  >>>>>>>> ���ԧ�ѧߧڧ�֧ߧڧ� II ��ڧ��. �� �٧ѧէѧߧڧ�: 5, �է���ݧߧڧ�֧ݧ�ߧ��: 10  <<<<<<<
	for (unsigned t = 0; t < N; t++) {
		task &= !(p_phone[t][7] ^ p_name[t][0]); //9.	���ѧߧէ� �ڧԧ�ѧ֧� �ߧ� �ѧܧܧ��է֧�ߧ�. (n2)
		task &= !(p_name[t][3] ^ p_food[t][5]); //10.	���� ���ק� ��ڧӧ�. (n2)
		task &= !(p_phone[t][3] ^ p_food[t][1]); //11.	���ڧӧ��ߧ��, �ڧԧ�ѧ��ڧ� �ߧ� ��ܧ�ڧ�ܧ�, ���ק� �ܧ���. (n2)
		task &= !(p_name[t][2] ^ p_last_num[t][6]); //12.	���ݧ֧ߧ� �٧�ӧ�� ���֧�ԧ֧�. (n2)
		task &= !(p_last_num[t][3] ^ p_food[t][7]); //13.	���ѧ�� ���ק� ����. (n2)
		//----- �է���ݧߧڧ�֧ݧ�ߧ�� ���ݧ�ӧڧ� -------
		task &= !(p_name[t][6] ^ p_last_num[t][3]); //22.	������ �٧�ӧ�� ���ѧ��. (n2)
		task &= !(p_name[t][3] ^ p_last_num[t][5]); //23.	���ا� �٧�ӧ�� ���קէ��. (n2)
		task &= !(p_name[t][4] ^ p_food[t][2]); //24.	���֧� ���ק� �ܧ�ܧ�-�ܧ�ݧ�. (n2)
		task &= !(p_name[t][2] ^ p_phone[t][6]); //25.	���ݧ֧ߧ� �ڧԧ�ѧ֧� �ߧ� �ѧ���. (n2)
		task &= !(p_last_num[t][8] ^ p_food[t][3]); //26.	����ݧڧߧ� ���ק� �ӧ�է�. (n2)
		task &= !(p_name[t][1] ^ p_last_num[t][7]); //27.	����ݧܧ� �٧�ӧ�� ���֧��. (n2)
		task &= !(p_name[t][3] ^ p_phone[t][4]); //28.	���� �ڧԧ�ѧ֧� �ߧ� ��ѧܧ����ߧ�. (n2)
		task &= !(p_food[t][2] ^ p_phone[t][5]); //29.	���ڧӧ��ߧ��, �ܧ������ ���ק� �ܧ�ܧ�-�ܧ�ݧ�, �ڧԧ�ѧ֧� �ߧ� �ҧѧ�ߧ�. (n2)
		task &= !(p_name[t][7] ^ p_food[t][8]); //30.	����� ���ק� �ާ�ݧ�ܧ�. (n2)
		task &= !(p_name[t][1] ^ p_food[t][6]); //31.	����ݧ� ���ק� �ӧ�էܧ�. (n2)
	}
	cout << " > Constraints type 2 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(task) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(task) << "\n\n";



	//  >>>>>>>> ���ԧ�ѧߧڧ�֧ߧڧ� III ��ڧ��. �� �٧ѧէѧߧڧ�: 2. <<<<<<
	// ��ݧ֧ӧ�-��ӧ֧���
	// 14. ���ڧӧ��ߧ��, �ܧ������ ���ק� �ܧ���, ����ݧѧ֧��� �ӧߧڧ� �ߧ� �اڧӧ��ߧ�ԧ�, �ܧ������ ���ק� �ӧ�էܧ�.
	for (unsigned iv = 0; iv < SqrtN; iv++) task &= !p_food[iii[iv][0]][6] & !p_food[iii[iv][SqrtN - 1]][1];
	for (unsigned ig = 0; ig < SqrtN; ig++) task &= !p_food[iii[0][ig]][6] & !p_food[iii[SqrtN - 1][ig]][1];
	for (unsigned iv = 1; iv < SqrtN; iv++) {
		for (unsigned ig = 1; ig < SqrtN; ig++)
			task &= !(p_food[iii[iv][ig]][6] ^ p_food[iii[iv - 1][ig - 1]][1]);
	}

	// 15. ���ѧ�ѧ� ����ݧѧ֧��� �ӧߧڧ� �ߧ� ���קէ��.
	for (unsigned iv = 0; iv < SqrtN; iv++) task &= !p_last_num[iii[iv][0]][5] & !p_last_num[iii[iv][SqrtN - 1]][2];
	for (unsigned ig = 0; ig < SqrtN; ig++) task &= !p_last_num[iii[0][ig]][5] & !p_last_num[iii[SqrtN - 1][ig]][2];
	for (unsigned iv = 1; iv < SqrtN; iv++) {
		for (unsigned ig = 1; ig < SqrtN; ig++)
			task &= !(p_last_num[iii[iv][ig]][5] ^ p_last_num[iii[iv - 1][ig - 1]][2]);
	}
	cout << " > Constraints type 3 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(task) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(task) << "\n\n";


	//���ԧ�ѧߧڧ�֧ߧڧ� IV ��ڧ��. �� �٧ѧէѧߧڧ�: 3.
	// 16.	���֧� ����ݧѧ֧��� �ߧ� ��ݧ��.
	bdd temp1 = bddtrue;
	bdd temp2 = bddtrue;

	for (unsigned iv = 0; iv < SqrtN; iv++) temp1 &= !p_name[iii[iv][0]][4] & !p_name[iii[iv][SqrtN - 1]][5];
	for (unsigned ig = 0; ig < SqrtN; ig++) temp1 &= !p_name[iii[0][ig]][4] & !p_name[iii[SqrtN - 1][ig]][5];
	for (unsigned iv = 1; iv < SqrtN; iv++) {
		for (unsigned ig = 1; ig < SqrtN; ig++) temp1 &= !(p_name[iii[iv][ig]][4] ^ p_name[iii[iv - 1][ig - 1]][5]); //
	}

	for (unsigned iv = 0; iv < SqrtN; iv++) temp2 &= !p_name[iii[iv][0]][4] & !p_name[iii[iv][SqrtN - 1]][5];
	for (unsigned iv = 0; iv < SqrtN; iv++) { /*0 1 2*/
		for (unsigned ig = 1; ig < SqrtN; ig++) /* 1 2 */
			temp2 &= !(p_name[iii[iv][ig]][4] ^ p_name[iii[iv][ig-1]][5]);
	}
	task &= temp1 | temp2;

	// 17.	����ݧ� ����ݧѧ֧��� �ߧ� ��ݧ֧ߧ�.
	temp1 = bddtrue;
	temp2 = bddtrue;

	for (unsigned iv = 0; iv < SqrtN; iv++) temp1 &= !p_food[iii[iv][0]][6] & !p_last_num[iii[iv][SqrtN - 1]][0];
	for (unsigned ig = 0; ig < SqrtN; ig++) temp1 &= !p_food[iii[0][ig]][6] & !p_last_num[iii[SqrtN - 1][ig]][0];
	for (unsigned iv = 1; iv < SqrtN; iv++) {
		for (unsigned ig = 1; ig < SqrtN; ig++) temp1 &= !(p_food[iii[iv][ig]][6] ^ p_last_num[iii[iv - 1][ig - 1]][0]);
	}

	for (unsigned iv = 0; iv < SqrtN; iv++) temp2 &= !p_food[iii[iv][0]][6] & !p_last_num[iii[iv][SqrtN - 1]][0];
	for (unsigned iv = 0; iv < SqrtN; iv++) { /*0 1 2*/
		for (unsigned ig = 1; ig < SqrtN; ig++) /* 1 2 */
			temp2 &= !(p_food[iii[iv][ig]][6] ^ p_last_num[iii[iv][ig - 1]][0]);
	}
	task &= temp1 | temp2;

	// 18. ���֧�� ����ݧѧ֧��� �ߧ� ���ڧ�ڧݧݧ�.
	temp1 = bddtrue;
	temp2 = bddtrue;

	for (unsigned iv = 0; iv < SqrtN; iv++) temp1 &= !p_last_num[iii[iv][0]][4] & !p_last_num[iii[iv][SqrtN - 1]][1];
	for (unsigned ig = 0; ig < SqrtN; ig++) temp1 &= !p_last_num[iii[0][ig]][4] & !p_last_num[iii[SqrtN - 1][ig]][1];
	for (unsigned iv = 1; iv < SqrtN; iv++) {
		for (unsigned ig = 1; ig < SqrtN; ig++) temp1 &= !(p_last_num[iii[iv][ig]][4] ^ p_last_num[iii[iv - 1][ig - 1]][1]);
	}

	for (unsigned iv = 0; iv < SqrtN; iv++) temp2 &= !p_last_num[iii[iv][0]][4] & !p_last_num[iii[iv][SqrtN - 1]][1];
	for (unsigned iv = 0; iv < SqrtN; iv++) { /*0 1 2*/
		for (unsigned ig = 1; ig < SqrtN; ig++) /* 1 2 */
			temp2 &= !(p_last_num[iii[iv][ig]][4] ^ p_last_num[iii[iv][ig - 1]][1]);
	}
	task &= temp1 | temp2;
	cout << " > Constraints type 4 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(task) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(task) << "\n\n";

	//���ԧ�ѧߧڧ�֧ߧڧ� VI ��ڧ��.
	for (unsigned i = 0; i < N; i++)
	{
		bdd temp1 = bddfalse;
		bdd temp2 = bddfalse;
		bdd temp3 = bddfalse;
		bdd temp4 = bddfalse;

		for (unsigned j = 0; j < N; j++)
		{
			temp1 |= p_name[i][j];
			temp2 |= p_last_num[i][j];
			temp3 |= p_phone[i][j];
			temp4 |= p_food[i][j];
		}
		task &= temp1 & temp2 & temp3 & temp4;
	}
	cout << " > Constraints type 6 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(task) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(task) << "\n\n";


	//���ԧ�ѧߧڧ�֧ߧڧ� V ��ڧ��.
	for (unsigned j = 0; j < N; j++) {
		for (unsigned i = 0; i < N - 1; i++) {
			for (unsigned k = i + 1; k < N; k++) {
				task &= p_name[i][j] >> !p_name[k][j];
				task &= p_last_num[i][j] >> !p_last_num[k][j];
				task &= p_phone[i][j] >> !p_phone[k][j];
				task &= p_food[i][j] >> !p_food[k][j];
			}
		}
	}
	cout << " > Constraints type 5 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(task) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(task) << "\n\n";

	// �ӧ�ӧ�� ��֧٧�ݧ��ѧ���
	out.open("out.txt");
	unsigned satcount = (unsigned)bdd_satcount(task);
	out << satcount << " solution(s):\n" << endl;
	if (satcount) bdd_allsat(task, fun);
	out.close();

	//------------------------------
	out.open("dot_all.txt");
	out << bdddot << task;
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

