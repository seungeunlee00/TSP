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
	int N; // node의 개수
	int total_edges; // edges의 총 개수
	int initial_node; // 시작 node
	map<pair<int, int>, int> map_edges; // edges의 map
public:
	Graph(int N, int initial_node); // 생성자
	void addEdge(int n1, int n2, int weight); // edge 추가
	int existsEdge(int src, int dest); // edge가 존재하는지 확인
	friend class Genetic; // private membres에 접근
};

typedef tuple<vector<int>, int, int> my_tuple;

struct sort_tuple { // tuple을 경로의 길이에 따라 오름차순 정렬
	bool operator()(const my_tuple& first, const my_tuple& second) {
		return get<1>(first) < get<1>(second);
	}
};

class Genetic {
private:
	Graph* graph; 
	vector< my_tuple > population; // vector, total length, fitness
	int population_size; // population의 size
	int population_real_size; // population의 real size
	int generations; // amount of generations
	int crossover_rate; // crossover(교차) rate
	double mutation_rate; // mutation(돌연변이) rate
private:
	void initialPopulation(); // initial population 생성
public:
	Genetic(Graph* graph, int generations, int population_size, int crossover_rate, double mutation_rate); // 생성자
	int isValid(vector<int>& solution); // 유효한지 확인
	void getFitness(); // 적합도 계산
	void crossOver(vector<int>& parent1, vector<int>& parent2); // crossover(교차) 생성
	void insertBinarySearch(vector<int>& child, int total_length); // 이진 탐색을 사용하여 population에 삽입
	bool existsChromosome(const vector<int>& v); // population에 존재하는 염색체인지 확인
	int tournamentSelect(); // tournament selection 진행
	void run(); // genetic algorithm 실행
};

#endif