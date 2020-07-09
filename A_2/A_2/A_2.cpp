#include <iostream>
#include <fstream>
#include <locale>
#include <stdlib.h>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

int N; // кол-во вершин
int M; // кол-во ребер
const int INF = 999; // бесконечность

/*ввод графа из файла, заданного при помощи матрицы с весом рёбер*/
void input_matrix(ifstream &in, int **graph)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			in >> graph[i][j];
}

/* печать графа c веом рёбер(вывод в graphic.png) */
void print_graph(int **graph) {
	ofstream out;
	out.open("graphic.dot");
	out << "graph G{" << "\n";
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (graph[i][j] != 0 && i < j)
				out << i << " -- " << j  << " [label=\" " << graph[i][j] << "\"];\n";
	out << "}";
	out.close();
	system("dot -Tpng graphic.dot -o graphic.png ");
	system("graphic.png");
}

/*Посчет количества рёбер*/
int count_rib(int **graph) {
	int count = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (graph[i][j] != 0 && i < j)
				count++;
	return count;
}

/*перевод из массива в список ребер с весом*/
void matrix_to_vector(int **graph, vector< pair<int, pair<int, int> > >& vector_rib) {
	int k = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (graph[i][j] != 0 && i < j) {
				vector_rib[k] = { graph[i][j] ,{ i, j } };
				k++;
			}
}

/*------------------------------------------------------------------------------------------------------*/

/* Основная функция
ifs - входной поток;
ofs - выходной поток;
*/

int main() {
	ifstream ifs;
	ofstream ofs;
	setlocale(LC_ALL, "rus");

	try {
		int ans;
		do {
			cout << "Редактировать файл ввода? (1-да, 0 -нет)." << endl;
			cin >> ans;
			if (ans == 1) system("input.txt");
		} while (ans != 1 && ans != 0);


		ifs.open("input.txt");
		ifs >> N;

		int **graph_cost_m = new int*[N]; //зануляем матрицу
		for (int i = 0; i < N; i++)
		{
			graph_cost_m[i] = new int[N];
			for (int j = 0; j < N; j++)
				graph_cost_m[i][j] = 0;
		}


		input_matrix(ifs, graph_cost_m);
		ifs.close();

		do {
			cout << "Вывести граф? (1-да, 0 -нет)." << endl;
			cin >> ans;
			if (ans == 1) print_graph(graph_cost_m);
		} while (ans != 1 && ans != 0);

		/*матрица смежности в список рёбер с весом*/
		M = count_rib(graph_cost_m);
		vector< pair<int, pair<int, int> > > graph_cost_v (M); // вес - вершина 1 - вершина 2
		matrix_to_vector(graph_cost_m, graph_cost_v);


		/*Алгоритм Крускала*/
		int cost = 0;  // общий вес
		vector < pair<int, int> > result;

		sort(graph_cost_v.begin(), graph_cost_v.end());

		vector<int> tree_id(N);
		for (int i = 0; i < N; ++i) tree_id[i] = i;

		for (int i = 0; i < M; ++i)
		{
			int a = graph_cost_v[i].second.first,  // первая вершина(первый конец)
				b = graph_cost_v[i].second.second, // вторая вершина(второй конец)
				l = graph_cost_v[i].first;         // вес ребра

			if (tree_id[a] != tree_id[b])
			{
				cost += l;                            //подсчёт общего веса
				result.push_back(make_pair(a, b));
				int old_id = tree_id[b],
					new_id = tree_id[a];
				for (int j = 0; j < N; ++j)
					if (tree_id[j] == old_id)
						tree_id[j] = new_id;
			}
		}


		/*вывод результата в файл*/
		ofs.open("output.txt");
		for (int i = 0; i < N - 1; i++) ofs << result[i].first << " -- " << result[i].second << endl;
		ofs.close();

		ofs.open("output.dot");
		ofs << "graph G{" << "\n";
		for (int i = 0; i < N - 1; i++)
			ofs << result[i].first << " -- " << result[i].second << endl;
		ofs << "}";
		ofs.close();

		/*открытие результата*/
		do {
			cout << "Вывести результат? (1-в тестовом документе, 2 - в графике, 0 -не выводить)." << endl;
			cin >> ans;
			if (ans == 1) system("output.txt"); 
			if (ans == 2) {
				system("dot -Tpng output.dot -o output.png ");
				system("output.png");
			}
		} while (ans != 1 && ans != 2 && ans != 0);
	}

	catch (...) {
		cout << "Ошибка файла!";
	}

	cout << "Программа выполнена." << endl;
	return 0;
}

