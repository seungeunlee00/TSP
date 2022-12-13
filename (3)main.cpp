#include "(3)GA.h"

int main() {
	srand(time(NULL));

	int N; 
	cin >> N; // 노드의 개수 N 입력받기

	Graph* graph = new Graph(N, 0); // 그래프 생성

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			int e;
			cin >> e;
			graph->addEdge(i, j, e); // 반복문을 통해 edge 생성
		}
	}

	// parameter: generations rate, population size(20), crossover rate(95%), mutation rate(0.5%)
	Genetic genetic(graph, 1000, 20, 95, 0.5);

	clock_t start, end;
	start = clock(); // 실행 시간 측정 시작

	genetic.run();

	end = clock(); // 실행 시간 측정 종료
	cout << "\n(3) 실행 시간 : " << (float)(end - start) << "ms\n";

	return 0;
}