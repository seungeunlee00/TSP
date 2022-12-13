#include "(3)GA.h"

// Graph 생성자
Graph::Graph(int N, int initial_node) { 
	if (N < 1) { // node의 개수가 1개보다 적은지 확인
		cout << "Error: number of node\n";
		exit(1);
	}

	this->N = N; // node의 개수 할당
	this->initial_node = initial_node; // 시작 node 할당
	this->total_edges = 0; // total_edge를 0으로 초기화
}

// edge 추가
void Graph::addEdge(int src, int dest, int length) { 
	map_edges[make_pair(src, dest)] = length; // map에 edge 추가
}

// edge가 존재하는지 확인
int Graph::existsEdge(int src, int dest) {
	map<pair<int, int>, int>::iterator it = map_edges.find(make_pair(src, dest));

	if (it != map_edges.end())
		return it->second; // edge의 거리 return 
	return -1;
}

// Genetic 생성자
Genetic::Genetic(Graph* graph, int generations, int size_population, int crossover_rate, double mutation_rate) {
	if (generations < 1) { // generation가 1보다 작으면 error
		cout << "Error: generations\n";
		exit(1);
	}
	if (size_population < 1) { // population size가 1보다 작으면 error
		cout << "Error: population size\n";
		exit(1);
	}
	else if (crossover_rate < 0 || crossover_rate > 100) { // crossover rate가 0보다 작거나 100보다 크면 error
		cout << "Error: crossover rate\n";
		exit(1);
	}
	else if (mutation_rate < 0 || mutation_rate > 100) { // mutation rate가 0보다 작거나 100보다 크면 error
		cout << "Error: mutation rate\n";
		exit(1);
	}
	this->graph = graph; // 그래프 할당
	this->population_size = size_population; // popluatioin의 size 할당
	this->population_real_size = 0; // population의 real size를 0으로 초기화
	this->generations = generations; // generation 할당
	this->crossover_rate = crossover_rate; // crossover rate 할당
	this->mutation_rate = mutation_rate; // mutation rate 할당
}

// 유효한지 확인
int Genetic::isValid(vector<int>& solution) { 
	int total_length = 0;
	set<int> set_solution;

	for (int i = 0; i < graph->N; i++) // 반복되는 요소가 있지는 않은지 확인
		set_solution.insert(solution[i]);

	if (set_solution.size() != (unsigned)graph->N)
		return -1;

	for (int i = 0; i < graph->N; i++) { // connection이 유효한지 확인
		if (i + 1 < graph->N) {
			int length = graph->existsEdge(solution[i], solution[i + 1]);

			// connection이 있는지 확인
			if (length == -1) return -1;
			else total_length += length;
		} else {
			int length = graph->existsEdge(solution[i], solution[0]);

			// connection이 있는지 확인
			if (length == -1) return -1;
			else total_length += length;
			break;
		}
	}
	return total_length;
}

// population에 존재하는 염색체인지 확인
bool Genetic::existsChromosome(const vector<int>& v) {
	for (vector<tuple<vector<int>, int, int> >::iterator it = population.begin(); it != population.end(); ++it) {
		const vector<int>& vec = get<0>(*it);
		if (equal(v.begin(), v.end(), vec.begin())) // vector 비교
			return true;
	}
	return false;
}

// 적합도 계산
void Genetic::getFitness() {
	vector<int> fitness;
	
	// 적합도는 가장 긴 방문경로에서 각 게놈의 경로를 뺀 값
	for (int i = 0; i < population_real_size; ++i) {
		fitness.push_back(get<1>(population[i]));
	}

	int max = *max_element(fitness.begin(), fitness.end());

	for (int i = 0; i < population_real_size; ++i) {
		get<2>(population[i]) = max - get<1>(population[i]);
	}

	// Rank Scaling을 통해 적합도 재가공
	// 새로운 적합도는 적합도가 큰 순위대로 0,1,2...N-1
	for (int i = 0; i < population_real_size; i++) {
		int cnt = 0;
		for (int j = 0; j < population_real_size; j++) {
			if (fitness[i] < fitness[j]) {
				cnt++;
			}
		}
		get<2>(population[i]) = cnt;
	}
}

// tournament selection 진행
int Genetic::tournamentSelect() {
	// 임의의 개체 2개 선택 
	int rand1 = rand() % population_real_size;
	int rand2 = rand() % population_real_size;

	// rand1의 적합도가 더 크다면
	if (get<2>(population[rand1]) >= get<2>(population[rand2])) {
		return rand1;
	} else { // rand2의 적합도가 더 크다면
		return rand2;
	}
}

// initial population 생성
void Genetic::initialPopulation() {
	vector<int> parent;
	parent.push_back(graph->initial_node); // initial node를 parent에 삽입

	for (int i = 0; i < graph->N; i++) {
		if (i != graph->initial_node)
			parent.push_back(i); // parent 생성
	}

	if (isValid(parent) != -1) { // parent가 유효하다면
		population.push_back(make_tuple(parent, isValid(parent), 0)); // population에 삽입하고
		population_real_size++; // population_real_size 1 증가
	}

	for (int i = 0; i < generations; i++) {
		// random permutation(순열) 생성
		random_shuffle(parent.begin() + 1, parent.begin() + (rand() % (graph->N - 1) + 1));

		// permutation이 유효한지, population에 존재하지 않는지 확인
		if (isValid(parent) != -1 && !existsChromosome(parent)) {
			population.push_back(make_tuple(parent, isValid(parent), 0)); // population에 삽입하고
			population_real_size++; // population_real_size 1 증가
		}
		if (population_real_size == population_size) // population의 size 확인
			break;
	}

	if (population_real_size == 0) // population_real_size이 0이라면
		cout << "\nError: initial population";
	else // population_real_size이 0이 아니라면
		sort(population.begin(), population.end(), sort_tuple()); // population 정렬

	getFitness(); // 적합도 계산
}

// 이진 탐색을 사용하여 population에 삽입
void Genetic::insertBinarySearch(vector<int>& child, int total_length) {
	int min = 0;
	int max = population_real_size - 1;
	 
	while (max >= min) {
		int mid = min + (max - min) / 2;

		if (total_length == get<1>(population[mid])) {
			population.insert(population.begin() + mid, make_tuple(child, total_length, 0));
			return;
		}
		else if (total_length > get<1>(population[mid])) min = mid + 1;
		else max = mid - 1;
	}
	population.insert(population.begin() + min, make_tuple(child, total_length, 0));
}

/*
	crossover(교차) 생성: multi point crossover
	2개의 random point를 선택, point는 parent에서 substring을 생성
	parent1의 거꾸로 반전된 substring을 parent2에 넣고 parent2의 반전된 substring을 parent1에 넣는다.

	Example:
		parent1: 1 2 3 4 5
		parent2: 1 2 4 5 3

		substring in parent1: 2 3 4
		substring in parent2: 2 4 5

		substring inverted in parent1: 4 3 2
		substring inverted in parent2: 5 4 2

		child1: 1 5 4 2 5
		child2: 1 4 3 2 3

		invalid children: parent1에서는 5가 2번 parent2에서는 3이 2번 발견됨
		Solution: genes가 사용되었는지 사용되지 않았는지 확인하는 map 사용
*/
void Genetic::crossOver(vector<int>& parent1, vector<int>& parent2) {
	vector<int> child1, child2;
	map<int, int> genes1, genes2; // 사용되었는지 확인하기 위한 genes의 map

	for (int i = 0; i < graph->N; i++) {
		genes1[parent1[i]] = 0; // gene은 사용되지 않은 상태(0)로 초기화
		genes2[parent2[i]] = 0;
	}

	int crossover = rand() % 100 + 1; // 1~100에서 random number 
	if (crossover <= crossover_rate) { // random number가 crossover rate보다 작거나 같으면
		int point1 = rand() % (graph->N - 1) + 1; // 랜덤 포인트 생성
		int point2 = rand() % (graph->N - point1) + point1;

		if (point1 == point2) { // 포인트가 같다면 포인트 수정
			if (point1 - 1 > 1)
				point1--;
			else if (point2 + 1 < graph->N)
				point2++;
			else { 
				int point = rand() % 10 + 1; // 1~10에서 random number
				if (point <= 5)
					point1--;
				else
					point2++;
			}
		}

		// child 생성: child1은 parent1의 genes를 받고 child2는 parent2의 genes를 받음
		for (int i = 0; i < point1; i++) {
			child1.push_back(parent1[i]); // genes 추가
			child2.push_back(parent2[i]);
			genes1[parent1[i]] = 1; // genes를 1으로 표시
			genes2[parent2[i]] = 1;
		}

		// 나머지 genes도 1로 표시
		for (int i = point2 + 1; i < graph->N; i++) {
			genes1[parent1[i]] = 1;
			genes2[parent2[i]] = 1;
		}

		// substring이 거꾸로 교환되는 부분
		for (int i = point2; i >= point1; i--) {
			if (genes1[parent2[i]] == 0) { // gene이 사용되지 않았을때(0)
				child1.push_back(parent2[i]);
				genes1[parent2[i]] = 1; // gene을 1로 표시
			} else { // gene이 사용되었을 때는(1), 사용되지 않은 gene을 선택 
				for (map<int, int>::iterator it = genes1.begin(); it != genes1.end(); ++it) {
					if (it->second == 0) { // gene이 사용되지 않았을때(0)
						child1.push_back(it->first);
						genes1[it->first] = 1; // 1로 표시
						break;
					}
				}
			}

			if (genes2[parent1[i]] == 0) { // gene이 사용되지 않았을때(0)
				child2.push_back(parent1[i]);
				genes2[parent1[i]] = 1; // gene을 1로 표시
			} else { // gene이 사용되었을 때는(1), 사용되지 않은 gene을 선택 
				for (map<int, int>::iterator it = genes2.begin(); it != genes2.end(); ++it) {
					if (it->second == 0) { // gene이 사용되지 않았을때(0)
						child2.push_back(it->first);
						genes2[it->first] = 1; // 1로 표시
						break;
					}
				}
			}
		}
		// 남아있는 genes
		// child1은 parent1의 genes를 받고 child2는 parent2의 genes를 받음
		for (int i = point2 + 1; i < graph->N; i++) {
			child1.push_back(parent1[i]);
			child2.push_back(parent2[i]);
		}

	} else { // random number가 crossover rate보다 크면
		// child 생성: child1은 parent1의 genes를 받고 child2는 parent2의 genes를 받음
		for (int i = 0; i < graph->N; i++) {
			child1.push_back(parent1[i]); // genes 추가
			child2.push_back(parent2[i]);
		}
	}

	// mutation(돌연변이): Swap Mutation 
	// 무작위로 두 위치를 선택하고 값을 교환, 순열 기반 인코딩에서 일반적으로 사용
	int mutation = rand() % 100 + 1; // 1~100에서 random number 
	if (mutation <= mutation_rate) { // random number가 mutation rate보다 작거나 같으면
		// 돌연변이 생성: 두 개의 genes 교환
		int index_gene1, index_gene2;
		index_gene1 = rand() % (graph->N - 1) + 1;
		index_gene2 = rand() % (graph->N - 1) + 1;

		// child1의 돌연변이
		int mut = child1[index_gene1];
		child1[index_gene1] = child1[index_gene2];
		child1[index_gene2] = mut;

		// child2의 돌연변이
		mut = child2[index_gene1];
		child2[index_gene1] = child2[index_gene2];
		child2[index_gene2] = mut;
	}
	
	int total_length_child1 = isValid(child1);
	int total_length_child2 = isValid(child2);

	// child1가 유효하고 population에 존재하지 않으면 population에 추가
	if (total_length_child1 != -1 && !existsChromosome(child1)) {
		insertBinarySearch(child1, total_length_child1); // 이진탐색하고 삽입
		population_real_size++; // population_real_size 1 증가
	}
	 
	// child2가 유효하고 population에 존재하지 않으면 population에 추가
	if (total_length_child2 != -1 && !existsChromosome(child2)) {
		insertBinarySearch(child2, total_length_child2); // 이진탐색하고 삽입
		population_real_size++; // population_real_size 1 증가
	}

	sort(population.begin(), population.end(), sort_tuple()); // population 다시 정렬
	getFitness(); // 적합도 다시 계산
}

// genetic algorithm 실행
void Genetic::run() { 
	initialPopulation(); // initial population

	if (population_real_size == 0)
		return;

	for (int i = 0; i < generations; i++) {
		int population_old_size = population_real_size;

		if (population_real_size >= 2) {
			if (population_real_size == 2) {
				crossOver(get<0>(population[0]), get<0>(population[1])); // parent에 crossover 적용
			} else { // population의 실제 크기가 2보다 크면
				int parent1, parent2;

				do { // tournament selection을 통해 parent 선택
					parent1 = tournamentSelect();
					parent2 = tournamentSelect();
				} while (parent1 == parent2);

				crossOver(get<0>(population[parent1]), get<0>(population[parent2])); // 두 parent에 crossover 적용
			}

			// population이 증가했는지 확인하기 위해 population의 차이 계산
			int diff_population = population_real_size - population_old_size;

			if (diff_population == 2) {
				if (population_real_size > population_size) {
					population.pop_back(); // population에서 worst parent 2개 삭제
					population.pop_back();
					population_real_size -= 2; // 실제 popupation의 크기 2 감소
				}
			} else if (diff_population == 1) {
				if (population_real_size > population_size) {
					population.pop_back(); // population에서 worst parent 1개 삭제
					population_real_size--; // 실제 popupation의 크기 1 감소
				}
			}
		} else { // population이 1개의 parent를 가지고 있을때
			int parent = tournamentSelect(); // tournament selection을 통해 parent 선택
			crossOver(get<0>(population[parent]), get<0>(population[parent])); // parent에 crossover 적용
			if (population_real_size > population_size) {
				population.pop_back(); // population에서 worst parent 1개 삭제
				population_real_size--; // 실제 popupation의 크기 1 감소
			}
		}
	}

	// population 출력
	cout << "\nShowing solutions...\n\n";
	for (vector<tuple<vector<int>, int, int> >::iterator it = population.begin(); it != population.end(); ++it) {
		const vector<int>& vec = get<0>(*it);

		for (int i = 0; i < graph->N; i++)
			cout << vec[i] << " ";
		cout << graph->initial_node;
		cout << " | 경로의 길이: " << get<1>(*it);
		cout << " | 적합도: " << get<2>(*it) << endl;
	}
	cout << "\nPopulation size: " << population_real_size << endl;

	// 최단 경로 출력
	cout << "\n최단 경로 : ";
	const vector<int>& vec = get<0>(population[0]);
	for (int i = 0; i < graph->N; i++)
		cout << vec[i] << " ";
	cout << graph->initial_node;
	cout << " | 경로의 길이: " << get<1>(population[0]);
}