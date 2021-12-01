#if 0
#pragma comment(lib, "bdd.lib")
#include "bdd.h"
#include <fstream>
#include <math.h>
#include <iomanip>

using namespace std;
#define N_VAR 144	// число булевых переменных 9*4*4=144
#define N 9				// число объектов
#define SqrtN 3
#define M 4				// число свойств
#define LOG_N 4
#define K 30

ofstream out;
void fun(char* varset, int size); // функция, используемая для вывода решений

void main(void) {

	// инициализация
	bdd_init(10000000, 1000000);
	bdd_setvarnum(N_VAR);

	// ->--- вводим функцию p(k, i, j) следующим образом ( pk[i][j] ):
	bdd p_type[N][N];
	bdd p_name[N][N];
	bdd p_music[N][N];
	bdd p_drink[N][N];

	int iii[SqrtN][SqrtN]; // 九宫格
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
			p_type[i][j] = bddtrue;
			p_name[i][j] = bddtrue;
			p_music[i][j] = bddtrue;
			p_drink[i][j] = bddtrue;
			for (unsigned k = 0; k < LOG_N; k++) {
				p_type[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + k) : bdd_nithvar(I + k);
				p_name[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N + k) : bdd_nithvar(I + LOG_N + k);
				p_music[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * 2 + k) : bdd_nithvar(I + LOG_N * 2 + k);
				p_drink[i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * 3 + k) : bdd_nithvar(I + LOG_N * 3 + k);
			}
		}
		I += LOG_N * M;
	}

	bdd task = bddtrue;

	//ОГРАНИЧЕНИЯ
	//  >>>>>>>>>>>>>>>>>> Ограничения I типа. В задании: 8, дополнительные: 5 <<<<<<<<<<<<<<<<<<<<
	task &= p_type[1][7]; //1.	На доме 1 находится кот. (n1)
	task &= p_type[0][8]; //2.	На доме 0 находится бык. (n1)
	task &= p_music[3][3]; //3.	Животное, находящееся на доме 3, играет на скрипке. (n1)
	task &= p_drink[2][7]; //4.	Животное, находящееся на доме 2, пьёт сок. (n1)
	task &= p_name[4][4]; //5.	Животного, живущего в доме 4, зовут Роман. (n1)
	task &= p_type[5][3]; //6.	Ёж живет в доме 5. (n1)
	task &= p_name[6][6]; //7.	Животного, живущего в доме 6, зовут Сергей. (n1)
	task &= p_music[7][8]; //8.	Животное, находящееся на доме 7, играет на трубе. (n1)
	//----- дополнительные условия -------
#if 1
	task &= p_drink[8][3]; //19.	 Животное, которое живёт в доме 8, пьёт воду.(n1)
	task &= p_type[3][5]; //20.	 Слон живёт в доме 3.(n1)
	task &= p_drink[0][0]; //21.	 Животное, которое живёт в доме 0, пьёт чай.(n1)
	task &= p_music[8][7]; //32.	Животное, которое живёт в доме 8, играет на аккордеоне.(n1)
	task &= p_music[0][1]; //33.	Животное, которое живёт в доме 0, играет на пианино.(n1)
	task &= p_music[1][0]; //34.	Животное, которое живёт в доме 1, играет на гитаре.(n1)
	task &= p_music[2][2]; //35.	Животное, которое живёт в доме 2, играет на виолончель.(n1)
#endif
	cout << " > Constraints type 1 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(task) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(task) << "\n\n";

	//  >>>>>>>> Ограничения II типа. В задании: 5, дополнительные: 10  <<<<<<<
	for (unsigned t = 0; t < N; t++) {
		task &= !(p_music[t][7] ^ p_type[t][0]); //9.	Панда играет на аккордеоне. (n2)
		task &= !(p_type[t][3] ^ p_drink[t][5]); //10.	Ёж пьёт пиво. (n2)
		task &= !(p_music[t][3] ^ p_drink[t][1]); //11.	Животное, играющий на скрипке, пьёт кофе. (n2)
		task &= !(p_type[t][2] ^ p_name[t][6]); //12.	Олень зовут Сергей. (n2)
		task &= !(p_name[t][3] ^ p_drink[t][7]); //13.	Марк пьёт сок. (n2)
		//----- дополнительные условия -------
#if 1
		task &= !(p_type[t][6] ^ p_name[t][3]); //22.	Мышь зовут Марк. (n2)
		task &= !(p_type[t][3] ^ p_name[t][5]); //23.	Ёжа зовут Фёдор. (n2)
		task &= !(p_type[t][4] ^ p_drink[t][2]); //24.	Лев пьёт кока-кола. (n2)
		task &= !(p_type[t][2] ^ p_music[t][6]); //25.	Олень играет на арфе. (n2)
		task &= !(p_name[t][8] ^ p_drink[t][3]); //26.	Полина пьёт воду. (n2)
		task &= !(p_type[t][1] ^ p_name[t][7]); //27.	Волка зовут Вера. (n2)
		task &= !(p_type[t][3] ^ p_music[t][4]); //28.	Ёж играет на саксофоне. (n2)
		task &= !(p_drink[t][2] ^ p_music[t][5]); //29.	Животное, которое пьёт кока-кола, играет на баяне. (n2)
		task &= !(p_type[t][7] ^ p_drink[t][8]); //30.	Кот пьёт молоко. (n2)
		task &= !(p_type[t][1] ^ p_drink[t][6]); //31.	Волк пьёт водку. (n2)
#endif
	}
	cout << " > Constraints type 2 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(task) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(task) << "\n\n";



	//  >>>>>>>> Ограничения III типа. В задании: 2. <<<<<<
	// слева-сверху
	// 14. Животное, которое пьёт водку(6), ссылается вниз на животного, которое пьёт кофе(1). 后面的是下面的
	//for (unsigned iv = 0; iv < SqrtN; iv++) task &= !p_drink[		iii[iv][0]		][6]	&	 !p_drink[		iii[iv][SqrtN - 1]		][1];		// 禁止掉了下角的4个（伏特加）
	for (unsigned iv = 0; iv < SqrtN; iv++) task &= !p_drink[		iii[iv][0]		][6];		// 禁止掉了下角的4个（伏特加）
	for (unsigned iv = 0; iv < SqrtN; iv++) task &= !p_drink[		iii[iv][SqrtN - 1]		][1];		// 禁止掉了下角的4个（伏特加）

	for (unsigned ig = 0; ig < SqrtN; ig++) task &= !p_drink[		iii[0][ig]		][6]	&	 !p_drink[		iii[SqrtN - 1][ig]		][1];

	//
	for (unsigned iv = 1; iv < SqrtN; iv++) {
		for (unsigned ig = 1; ig < SqrtN; ig++)
			task &= !(p_drink[iii[iv][ig]][6] ^ p_drink[iii[iv - 1][ig - 1]][1]);
	}

	// 15. Тарас ссылается вниз на Фёдор.
	for (unsigned iv = 0; iv < SqrtN; iv++) task &= !p_name[		iii[iv][0]		][5] & !p_name[		iii[iv][SqrtN - 1]		]	[2];

	for (unsigned ig = 0; ig < SqrtN; ig++) task &= !p_name[		iii[0][ig]		][5] & !p_name[		iii[SqrtN - 1][ig]		][2];
	for (unsigned iv = 1; iv < SqrtN; iv++) {
		for (unsigned ig = 1; ig < SqrtN; ig++)
			task &= !(p_name[iii[iv][ig]][5] ^ p_name[iii[iv - 1][ig - 1]][2]);
	}
	cout << " > Constraints type 3 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(task) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(task) << "\n\n";


	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Ограничения IV типа. В задании: 3.<<<<<<<<<<<<<<<<<<<<<<<<
	// 16.	Лев ссылается на слон.
	bdd temp1 = bddtrue;
	bdd temp2 = bddtrue;

	// 左上
	for (unsigned iv = 0; iv < SqrtN; iv++) temp1 &= !p_type[		iii[iv][0]		][4] & !p_type[		iii[iv][SqrtN - 1	]		][5];

	for (unsigned ig = 0; ig < SqrtN; ig++) temp1 &= !p_type[iii[0][ig]][4] & !p_type[iii[SqrtN - 1][ig]][5];

	for (unsigned iv = 1; iv < SqrtN; iv++) {
		for (unsigned ig = 1; ig < SqrtN; ig++) temp1 &= !(p_type[iii[iv][ig]][4] ^ p_type[iii[iv - 1][ig - 1]][5]); //
	}

	// 左边
	for (unsigned iv = 0; iv < SqrtN; iv++) temp2 &= !p_type[iii[iv][0]][4] & !p_type[iii[iv][SqrtN - 1]][5];

	for (unsigned iv = 0; iv < SqrtN; iv++) { /*0 1 2*/
		for (unsigned ig = 1; ig < SqrtN; ig++) /* 1 2 */
			temp2 &= !(p_type[iii[iv][ig]][4] ^ p_type[iii[iv][ig-1]][5]);
	}
	task &= temp1 | temp2;

	// 17.	Волк ссылается на олень.
	temp1 = bddtrue;
	temp2 = bddtrue;

	for (unsigned iv = 0; iv < SqrtN; iv++) temp1 &= !p_drink[iii[iv][0]][6] & !p_name[iii[iv][SqrtN - 1]][0];

	for (unsigned ig = 0; ig < SqrtN; ig++) temp1 &= !p_drink[iii[0][ig]][6] & !p_name[iii[SqrtN - 1][ig]][0];

	for (unsigned iv = 1; iv < SqrtN; iv++) {
		for (unsigned ig = 1; ig < SqrtN; ig++) {
			temp1 &= !(p_drink[iii[iv][ig]][6] ^ p_name[iii[iv - 1][ig - 1]][0]);
			cout << '<' << iv << ", " << ig << ">  ";
		}
		cout << endl;
	}

	for (unsigned iv = 0; iv < SqrtN; iv++) temp2 &= !p_drink[iii[iv][0]][6] & !p_name[iii[iv][SqrtN - 1]][0];

	for (unsigned iv = 0; iv < SqrtN; iv++) { /*0 1 2*/
		for (unsigned ig = 1; ig < SqrtN; ig++) /* 1 2 */
			temp2 &= !(p_drink[iii[iv][ig]][6] ^ p_name[iii[iv][ig - 1]][0]);
	}

	task &= temp1 | temp2;

	// 18. Вера ссылается на Кирилла.
	temp1 = bddtrue;
	temp2 = bddtrue;

	for (unsigned iv = 0; iv < SqrtN; iv++) temp1 &= !p_name[iii[iv][0]][4] & !p_name[iii[iv][SqrtN - 1]][1];

	for (unsigned ig = 0; ig < SqrtN; ig++) temp1 &= !p_name[iii[0][ig]][4] & !p_name[iii[SqrtN - 1][ig]][1];

	for (unsigned iv = 1; iv < SqrtN; iv++) {
		for (unsigned ig = 1; ig < SqrtN; ig++) temp1 &= !(p_name[iii[iv][ig]][4] ^ p_name[iii[iv - 1][ig - 1]][1]);
	}

	for (unsigned iv = 0; iv < SqrtN; iv++) temp2 &= !p_name[iii[iv][0]][4] & !p_name[iii[iv][SqrtN - 1]][1];

	for (unsigned iv = 0; iv < SqrtN; iv++) { /*0 1 2*/
		for (unsigned ig = 1; ig < SqrtN; ig++) /* 1 2 */
			temp2 &= !(p_name[iii[iv][ig]][4] ^ p_name[iii[iv][ig - 1]][1]);
	}

	task &= temp1 | temp2;
	cout << " > Constraints type 4 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(task) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(task) << "\n\n";



#if 0
	---------------------------------------------------------------------------------------------------------------------------------------
#endif // 0



	//Ограничение VI типа.
	for (unsigned i = 0; i < N; i++)
	{
		bdd temp1 = bddfalse;
		bdd temp2 = bddfalse;
		bdd temp3 = bddfalse;
		bdd temp4 = bddfalse;

		for (unsigned j = 0; j < N; j++)
		{
			temp1 |= p_type[i][j];
			temp2 |= p_name[i][j];
			temp3 |= p_music[i][j];
			temp4 |= p_drink[i][j];
		}
		task &= temp1 & temp2 & temp3 & temp4;
	}
	cout << " > Constraints type 6 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(task) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(task) << "\n\n";


	//Ограничение V типа.
	for (unsigned j = 0; j < N; j++) {
		for (unsigned i = 0; i < N - 1; i++) {
			for (unsigned k = i + 1; k < N; k++) {
				task &= p_type[i][j] >> !p_type[k][j];
				task &= p_name[i][j] >> !p_name[k][j];
				task &= p_music[i][j] >> !p_music[k][j];
				task &= p_drink[i][j] >> !p_drink[k][j];
			}
		}
	}
	cout << " > Constraints type 5 have been applied.\n"
		<< "Number of nodes : " << bdd_nodecount(task) << "\n"
		<< "Number of solution(s): " << std::fixed << std::setprecision(0) << (double)bdd_satcount(task) << "\n\n";

	// вывод результатов
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