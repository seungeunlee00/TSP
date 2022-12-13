#include "(3)GA.h"

int main() {
	srand(time(NULL));

	int N; 
	cin >> N; // ����� ���� N �Է¹ޱ�

	Graph* graph = new Graph(N, 0); // �׷��� ����

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			int e;
			cin >> e;
			graph->addEdge(i, j, e); // �ݺ����� ���� edge ����
		}
	}

	// parameter: generations rate, population size(20), crossover rate(95%), mutation rate(0.5%)
	Genetic genetic(graph, 1000, 20, 95, 0.5);

	clock_t start, end;
	start = clock(); // ���� �ð� ���� ����

	genetic.run();

	end = clock(); // ���� �ð� ���� ����
	cout << "\n(3) ���� �ð� : " << (float)(end - start) << "ms\n";

	return 0;
}