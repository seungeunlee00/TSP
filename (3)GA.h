#pragma once
#ifndef GA_H
#define GA_H

#include<vector>
#include<tuple>
#include<map>
#include<set>
#include<iostream>
#include<algorithm> 
#include<ctime>
using namespace std;

class Graph {
private:
	int N; // node�� ����
	int total_edges; // edges�� �� ����
	int initial_node; // ���� node
	map<pair<int, int>, int> map_edges; // edges�� map
public:
	Graph(int N, int initial_node); // ������
	void addEdge(int n1, int n2, int weight); // edge �߰�
	int existsEdge(int src, int dest); // edge�� �����ϴ��� Ȯ��
	friend class Genetic; // private membres�� ����
};

typedef tuple<vector<int>, int, int> my_tuple;

struct sort_tuple { // tuple�� ����� ���̿� ���� �������� ����
	bool operator()(const my_tuple& first, const my_tuple& second) {
		return get<1>(first) < get<1>(second);
	}
};

class Genetic {
private:
	Graph* graph; 
	vector< my_tuple > population; // vector, total length, fitness
	int population_size; // population�� size
	int population_real_size; // population�� real size
	int generations; // amount of generations
	int crossover_rate; // crossover(����) rate
	double mutation_rate; // mutation(��������) rate
private:
	void initialPopulation(); // initial population ����
public:
	Genetic(Graph* graph, int generations, int population_size, int crossover_rate, double mutation_rate); // ������
	int isValid(vector<int>& solution); // ��ȿ���� Ȯ��
	void getFitness(); // ���յ� ���
	void crossOver(vector<int>& parent1, vector<int>& parent2); // crossover(����) ����
	void insertBinarySearch(vector<int>& child, int total_length); // ���� Ž���� ����Ͽ� population�� ����
	bool existsChromosome(const vector<int>& v); // population�� �����ϴ� ����ü���� Ȯ��
	int tournamentSelect(); // tournament selection ����
	void run(); // genetic algorithm ����
};

#endif