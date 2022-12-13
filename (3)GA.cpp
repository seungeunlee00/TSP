#include "(3)GA.h"

// Graph ������
Graph::Graph(int N, int initial_node) { 
	if (N < 1) { // node�� ������ 1������ ������ Ȯ��
		cout << "Error: number of node\n";
		exit(1);
	}

	this->N = N; // node�� ���� �Ҵ�
	this->initial_node = initial_node; // ���� node �Ҵ�
	this->total_edges = 0; // total_edge�� 0���� �ʱ�ȭ
}

// edge �߰�
void Graph::addEdge(int src, int dest, int length) { 
	map_edges[make_pair(src, dest)] = length; // map�� edge �߰�
}

// edge�� �����ϴ��� Ȯ��
int Graph::existsEdge(int src, int dest) {
	map<pair<int, int>, int>::iterator it = map_edges.find(make_pair(src, dest));

	if (it != map_edges.end())
		return it->second; // edge�� �Ÿ� return 
	return -1;
}

// Genetic ������
Genetic::Genetic(Graph* graph, int generations, int size_population, int crossover_rate, double mutation_rate) {
	if (generations < 1) { // generation�� 1���� ������ error
		cout << "Error: generations\n";
		exit(1);
	}
	if (size_population < 1) { // population size�� 1���� ������ error
		cout << "Error: population size\n";
		exit(1);
	}
	else if (crossover_rate < 0 || crossover_rate > 100) { // crossover rate�� 0���� �۰ų� 100���� ũ�� error
		cout << "Error: crossover rate\n";
		exit(1);
	}
	else if (mutation_rate < 0 || mutation_rate > 100) { // mutation rate�� 0���� �۰ų� 100���� ũ�� error
		cout << "Error: mutation rate\n";
		exit(1);
	}
	this->graph = graph; // �׷��� �Ҵ�
	this->population_size = size_population; // popluatioin�� size �Ҵ�
	this->population_real_size = 0; // population�� real size�� 0���� �ʱ�ȭ
	this->generations = generations; // generation �Ҵ�
	this->crossover_rate = crossover_rate; // crossover rate �Ҵ�
	this->mutation_rate = mutation_rate; // mutation rate �Ҵ�
}

// ��ȿ���� Ȯ��
int Genetic::isValid(vector<int>& solution) { 
	int total_length = 0;
	set<int> set_solution;

	for (int i = 0; i < graph->N; i++) // �ݺ��Ǵ� ��Ұ� ������ ������ Ȯ��
		set_solution.insert(solution[i]);

	if (set_solution.size() != (unsigned)graph->N)
		return -1;

	for (int i = 0; i < graph->N; i++) { // connection�� ��ȿ���� Ȯ��
		if (i + 1 < graph->N) {
			int length = graph->existsEdge(solution[i], solution[i + 1]);

			// connection�� �ִ��� Ȯ��
			if (length == -1) return -1;
			else total_length += length;
		} else {
			int length = graph->existsEdge(solution[i], solution[0]);

			// connection�� �ִ��� Ȯ��
			if (length == -1) return -1;
			else total_length += length;
			break;
		}
	}
	return total_length;
}

// population�� �����ϴ� ����ü���� Ȯ��
bool Genetic::existsChromosome(const vector<int>& v) {
	for (vector<tuple<vector<int>, int, int> >::iterator it = population.begin(); it != population.end(); ++it) {
		const vector<int>& vec = get<0>(*it);
		if (equal(v.begin(), v.end(), vec.begin())) // vector ��
			return true;
	}
	return false;
}

// ���յ� ���
void Genetic::getFitness() {
	vector<int> fitness;
	
	// ���յ��� ���� �� �湮��ο��� �� �Գ��� ��θ� �� ��
	for (int i = 0; i < population_real_size; ++i) {
		fitness.push_back(get<1>(population[i]));
	}

	int max = *max_element(fitness.begin(), fitness.end());

	for (int i = 0; i < population_real_size; ++i) {
		get<2>(population[i]) = max - get<1>(population[i]);
	}

	// Rank Scaling�� ���� ���յ� �簡��
	// ���ο� ���յ��� ���յ��� ū ������� 0,1,2...N-1
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

// tournament selection ����
int Genetic::tournamentSelect() {
	// ������ ��ü 2�� ���� 
	int rand1 = rand() % population_real_size;
	int rand2 = rand() % population_real_size;

	// rand1�� ���յ��� �� ũ�ٸ�
	if (get<2>(population[rand1]) >= get<2>(population[rand2])) {
		return rand1;
	} else { // rand2�� ���յ��� �� ũ�ٸ�
		return rand2;
	}
}

// initial population ����
void Genetic::initialPopulation() {
	vector<int> parent;
	parent.push_back(graph->initial_node); // initial node�� parent�� ����

	for (int i = 0; i < graph->N; i++) {
		if (i != graph->initial_node)
			parent.push_back(i); // parent ����
	}

	if (isValid(parent) != -1) { // parent�� ��ȿ�ϴٸ�
		population.push_back(make_tuple(parent, isValid(parent), 0)); // population�� �����ϰ�
		population_real_size++; // population_real_size 1 ����
	}

	for (int i = 0; i < generations; i++) {
		// random permutation(����) ����
		random_shuffle(parent.begin() + 1, parent.begin() + (rand() % (graph->N - 1) + 1));

		// permutation�� ��ȿ����, population�� �������� �ʴ��� Ȯ��
		if (isValid(parent) != -1 && !existsChromosome(parent)) {
			population.push_back(make_tuple(parent, isValid(parent), 0)); // population�� �����ϰ�
			population_real_size++; // population_real_size 1 ����
		}
		if (population_real_size == population_size) // population�� size Ȯ��
			break;
	}

	if (population_real_size == 0) // population_real_size�� 0�̶��
		cout << "\nError: initial population";
	else // population_real_size�� 0�� �ƴ϶��
		sort(population.begin(), population.end(), sort_tuple()); // population ����

	getFitness(); // ���յ� ���
}

// ���� Ž���� ����Ͽ� population�� ����
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
	crossover(����) ����: multi point crossover
	2���� random point�� ����, point�� parent���� substring�� ����
	parent1�� �Ųٷ� ������ substring�� parent2�� �ְ� parent2�� ������ substring�� parent1�� �ִ´�.

	Example:
		parent1: 1 2 3 4 5
		parent2: 1 2 4 5 3

		substring in parent1: 2 3 4
		substring in parent2: 2 4 5

		substring inverted in parent1: 4 3 2
		substring inverted in parent2: 5 4 2

		child1: 1 5 4 2 5
		child2: 1 4 3 2 3

		invalid children: parent1������ 5�� 2�� parent2������ 3�� 2�� �߰ߵ�
		Solution: genes�� ���Ǿ����� ������ �ʾҴ��� Ȯ���ϴ� map ���
*/
void Genetic::crossOver(vector<int>& parent1, vector<int>& parent2) {
	vector<int> child1, child2;
	map<int, int> genes1, genes2; // ���Ǿ����� Ȯ���ϱ� ���� genes�� map

	for (int i = 0; i < graph->N; i++) {
		genes1[parent1[i]] = 0; // gene�� ������ ���� ����(0)�� �ʱ�ȭ
		genes2[parent2[i]] = 0;
	}

	int crossover = rand() % 100 + 1; // 1~100���� random number 
	if (crossover <= crossover_rate) { // random number�� crossover rate���� �۰ų� ������
		int point1 = rand() % (graph->N - 1) + 1; // ���� ����Ʈ ����
		int point2 = rand() % (graph->N - point1) + point1;

		if (point1 == point2) { // ����Ʈ�� ���ٸ� ����Ʈ ����
			if (point1 - 1 > 1)
				point1--;
			else if (point2 + 1 < graph->N)
				point2++;
			else { 
				int point = rand() % 10 + 1; // 1~10���� random number
				if (point <= 5)
					point1--;
				else
					point2++;
			}
		}

		// child ����: child1�� parent1�� genes�� �ް� child2�� parent2�� genes�� ����
		for (int i = 0; i < point1; i++) {
			child1.push_back(parent1[i]); // genes �߰�
			child2.push_back(parent2[i]);
			genes1[parent1[i]] = 1; // genes�� 1���� ǥ��
			genes2[parent2[i]] = 1;
		}

		// ������ genes�� 1�� ǥ��
		for (int i = point2 + 1; i < graph->N; i++) {
			genes1[parent1[i]] = 1;
			genes2[parent2[i]] = 1;
		}

		// substring�� �Ųٷ� ��ȯ�Ǵ� �κ�
		for (int i = point2; i >= point1; i--) {
			if (genes1[parent2[i]] == 0) { // gene�� ������ �ʾ�����(0)
				child1.push_back(parent2[i]);
				genes1[parent2[i]] = 1; // gene�� 1�� ǥ��
			} else { // gene�� ���Ǿ��� ����(1), ������ ���� gene�� ���� 
				for (map<int, int>::iterator it = genes1.begin(); it != genes1.end(); ++it) {
					if (it->second == 0) { // gene�� ������ �ʾ�����(0)
						child1.push_back(it->first);
						genes1[it->first] = 1; // 1�� ǥ��
						break;
					}
				}
			}

			if (genes2[parent1[i]] == 0) { // gene�� ������ �ʾ�����(0)
				child2.push_back(parent1[i]);
				genes2[parent1[i]] = 1; // gene�� 1�� ǥ��
			} else { // gene�� ���Ǿ��� ����(1), ������ ���� gene�� ���� 
				for (map<int, int>::iterator it = genes2.begin(); it != genes2.end(); ++it) {
					if (it->second == 0) { // gene�� ������ �ʾ�����(0)
						child2.push_back(it->first);
						genes2[it->first] = 1; // 1�� ǥ��
						break;
					}
				}
			}
		}
		// �����ִ� genes
		// child1�� parent1�� genes�� �ް� child2�� parent2�� genes�� ����
		for (int i = point2 + 1; i < graph->N; i++) {
			child1.push_back(parent1[i]);
			child2.push_back(parent2[i]);
		}

	} else { // random number�� crossover rate���� ũ��
		// child ����: child1�� parent1�� genes�� �ް� child2�� parent2�� genes�� ����
		for (int i = 0; i < graph->N; i++) {
			child1.push_back(parent1[i]); // genes �߰�
			child2.push_back(parent2[i]);
		}
	}

	// mutation(��������): Swap Mutation 
	// �������� �� ��ġ�� �����ϰ� ���� ��ȯ, ���� ��� ���ڵ����� �Ϲ������� ���
	int mutation = rand() % 100 + 1; // 1~100���� random number 
	if (mutation <= mutation_rate) { // random number�� mutation rate���� �۰ų� ������
		// �������� ����: �� ���� genes ��ȯ
		int index_gene1, index_gene2;
		index_gene1 = rand() % (graph->N - 1) + 1;
		index_gene2 = rand() % (graph->N - 1) + 1;

		// child1�� ��������
		int mut = child1[index_gene1];
		child1[index_gene1] = child1[index_gene2];
		child1[index_gene2] = mut;

		// child2�� ��������
		mut = child2[index_gene1];
		child2[index_gene1] = child2[index_gene2];
		child2[index_gene2] = mut;
	}
	
	int total_length_child1 = isValid(child1);
	int total_length_child2 = isValid(child2);

	// child1�� ��ȿ�ϰ� population�� �������� ������ population�� �߰�
	if (total_length_child1 != -1 && !existsChromosome(child1)) {
		insertBinarySearch(child1, total_length_child1); // ����Ž���ϰ� ����
		population_real_size++; // population_real_size 1 ����
	}
	 
	// child2�� ��ȿ�ϰ� population�� �������� ������ population�� �߰�
	if (total_length_child2 != -1 && !existsChromosome(child2)) {
		insertBinarySearch(child2, total_length_child2); // ����Ž���ϰ� ����
		population_real_size++; // population_real_size 1 ����
	}

	sort(population.begin(), population.end(), sort_tuple()); // population �ٽ� ����
	getFitness(); // ���յ� �ٽ� ���
}

// genetic algorithm ����
void Genetic::run() { 
	initialPopulation(); // initial population

	if (population_real_size == 0)
		return;

	for (int i = 0; i < generations; i++) {
		int population_old_size = population_real_size;

		if (population_real_size >= 2) {
			if (population_real_size == 2) {
				crossOver(get<0>(population[0]), get<0>(population[1])); // parent�� crossover ����
			} else { // population�� ���� ũ�Ⱑ 2���� ũ��
				int parent1, parent2;

				do { // tournament selection�� ���� parent ����
					parent1 = tournamentSelect();
					parent2 = tournamentSelect();
				} while (parent1 == parent2);

				crossOver(get<0>(population[parent1]), get<0>(population[parent2])); // �� parent�� crossover ����
			}

			// population�� �����ߴ��� Ȯ���ϱ� ���� population�� ���� ���
			int diff_population = population_real_size - population_old_size;

			if (diff_population == 2) {
				if (population_real_size > population_size) {
					population.pop_back(); // population���� worst parent 2�� ����
					population.pop_back();
					population_real_size -= 2; // ���� popupation�� ũ�� 2 ����
				}
			} else if (diff_population == 1) {
				if (population_real_size > population_size) {
					population.pop_back(); // population���� worst parent 1�� ����
					population_real_size--; // ���� popupation�� ũ�� 1 ����
				}
			}
		} else { // population�� 1���� parent�� ������ ������
			int parent = tournamentSelect(); // tournament selection�� ���� parent ����
			crossOver(get<0>(population[parent]), get<0>(population[parent])); // parent�� crossover ����
			if (population_real_size > population_size) {
				population.pop_back(); // population���� worst parent 1�� ����
				population_real_size--; // ���� popupation�� ũ�� 1 ����
			}
		}
	}

	// population ���
	cout << "\nShowing solutions...\n\n";
	for (vector<tuple<vector<int>, int, int> >::iterator it = population.begin(); it != population.end(); ++it) {
		const vector<int>& vec = get<0>(*it);

		for (int i = 0; i < graph->N; i++)
			cout << vec[i] << " ";
		cout << graph->initial_node;
		cout << " | ����� ����: " << get<1>(*it);
		cout << " | ���յ�: " << get<2>(*it) << endl;
	}
	cout << "\nPopulation size: " << population_real_size << endl;

	// �ִ� ��� ���
	cout << "\n�ִ� ��� : ";
	const vector<int>& vec = get<0>(population[0]);
	for (int i = 0; i < graph->N; i++)
		cout << vec[i] << " ";
	cout << graph->initial_node;
	cout << " | ����� ����: " << get<1>(population[0]);
}