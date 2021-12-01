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
	// 1.	`F` живет в  доме 1
	t_bdd &= p_name[1][NAME_F];

	// 2.	Кто пользуется телефона Samsung, живет в доме 5
	t_bdd &= p_phone[5][PHONE_SAMSUNG];

	// 3.	`E` живет в доме 4
	t_bdd &= p_name[4][NAME_E];

	// 4.	Кто любит есть говядину, живет в доме 0
	t_bdd &= p_food[0][FOOD_BEEF];

	// 5.	Кто пользуется телефоном Google, живет в доме 7
	t_bdd &= p_phone[7][PHONE_GOOGLE];

	// 6.	Кто любит арбуз, живет в доме 6
	t_bdd &= p_food[6][FOOD_WATERML];

	//----- дополнительные условия -------


	cout << " > Constraints type 1 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";







	//  >>>>>>>> Ограничения II типа. В задании: 3, дополнительные: ? <<<<<<<
	for (unsigned t = 0; t < N; t++)
	{
		//	1.	Обладатели телефонов Google любят есть курицу
		t_bdd &= NOT(	p_phone[t][PHONE_GOOGLE]	XOR		p_food[t][FOOD_CHINKEN]	);

		//	2.	Те, чья последняя цифра номера телефона равна 5, любят есть бананы
		t_bdd &= NOT(	p_tel_num[t][TEL_NUM_5]			XOR		p_food[t][FOOD_BANANA]		);

		//	3.	Обладатели телефонов Huawei, последняя цифра номера телефона равна 1
		t_bdd &= NOT(	p_phone[t][PHONE_HUAWEI]	XOR		p_tel_num[t][TEL_NUM_1]		);

		//----- дополнительные условия -------
		//task &= !(p_name[t][6] ^ p_tel_num[t][3]); //22.	Мышь зовут Марк. (n2)
		//task &= !(p_name[t][3] ^ p_tel_num[t][5]); //23.	Ёжа зовут Фёдор. (n2)
		//task &= !(p_name[t][4] ^ p_food[t][2]); //24.	Лев пьёт кока-кола. (n2)
		//task &= !(p_name[t][2] ^ p_phone[t][6]); //25.	Олень играет на арфе. (n2)
		//task &= !(p_tel_num[t][8] ^ p_food[t][3]); //26.	Полина пьёт воду. (n2)
		//task &= !(p_name[t][1] ^ p_tel_num[t][7]); //27.	Волка зовут Вера. (n2)
		//task &= !(p_name[t][3] ^ p_phone[t][4]); //28.	Ёж играет на саксофоне. (n2)
		//task &= !(p_food[t][2] ^ p_phone[t][5]); //29.	Животное, которое пьёт кока-кола, играет на баяне. (n2)
		//task &= !(p_name[t][7] ^ p_food[t][8]); //30.	Кот пьёт молоко. (n2)
		//task &= !(p_name[t][1] ^ p_food[t][6]); //31.	Волк пьёт водку. (n2)
	}
	cout << " > Constraints type 2 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(t_bdd) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(t_bdd) << "\n\n";









	//  >>>>>>>>>>>>>>>>>>>>>> Ограничения III типа. В задании: 5. <<<<<<<<<<<<<<<<<<<<<<<<
	// 1.	Человек, живущие под `C`, любят картофель
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


	// 2.	Человек, живущий под человеком, который пользуется телефоном Apple, имеет последняя цифра номера телефона равна 8
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

	//	3.	`C` живет ниже `E`
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

	//	4.	Человек, который живет под `B`, пользуется телефоном HTC
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

	//	5.	Человек, который живет под телефоном Vivo, любит есть капусту
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

	//  >>>>>>>>>>>>>>>>>>>>>> Ограничения IV типа. В задании: 4. <<<<<<<<<<<<<<<<<<<<<<<<
	// 1.	`F`, указывающий на `I`
	// F 不能在哪
	t_bdd &= !p_name[		grid[2][2]		][NAME_F];
	// I 不能在哪
	t_bdd &= !p_name[		grid[0][0]		][NAME_I];

	// 【向下指的情况】未考虑胶合的情况 4.1.1
	for (unsigned i = 0; i < SqrtN - 1; i++)
	{
		for (unsigned j = 0; j < SqrtN; j++)
		{
			t_bdd |= (				p_name[		grid[i][j]				][NAME_F]
								AND p_name[		grid[i + 1][j]		][NAME_I]);
			//cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}
	// 【向右上指的情况】未考虑胶合的情况 4.1.2
	for (unsigned i = 1; i < SqrtN; i++)
	{
		for (unsigned j = 0; j < SqrtN -1; j++)
		{
			t_bdd |= !(				p_name[		grid[i][j]				][NAME_F]
								XOR p_name[		grid[i - 1][j + 1]		][NAME_I]);
			cout << "F: <" << i << ", " << j << ">  " << "I: <" << i + 1 << ", " << j << ">  " << endl;
		}
	}

	// 2.	`A`, указывающий на `B`


	// 3.	`D`, указывающий на `G`
	// 4.	`E`, указывающий на `H`


	// 16.	Лев ссылается на слон.
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

	// 17.	Волк ссылается на олень.
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

	// 18. Вера ссылается на Кирилла.
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