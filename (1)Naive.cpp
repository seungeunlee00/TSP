﻿#include <iostream>
#include <algorithm>
#include <ctime>
using namespace std;

int N; // 노드의 개수
int W[100][100]; // 각 노드 간의 거리 저장
int minLength = 1000000; // 최단 경로의 길이
bool visited[100]; // 각 노드의 방문 여부를 저장

// 현재 노드에서 나머지 노드에 방문하는 모든 경우를 탐색해서 최단 경로의 길이를 구하는 함수
// node : 현재 노드, Sum : 현재까지 거리의 합, count : 방문한 노드 카운트
void tsp(int node, int Sum, int count) { 
    if (count == N) { // 모든 노드를 방문 했을 경우
        if (W[node][1] != 0) { // 출발한 노드로 돌아 갈 수 있는 길이 있으면
            minLength = min(minLength, Sum + W[node][1]); // 최단 경로의 길이를 갱신
        }
        return;
    }
    visited[node] = true; // node의 visited값을 true로 변경

    for (int i = 1; i <= N; i++) { 
        // i 노드에 방문한 적 없고 연결되어 있어 갈 수 있는 길이 있으면
        if (!visited[i] && W[node][i] != 0) { 
            tsp(i, Sum + W[node][i], count + 1); // i를 시작점으로 tsp함수 재귀 진행
        }
    }
    visited[node] = false; // node의 visited값을 false로 변경
}

int main() {
    cin >> N; // 노드의 개수 N 입력받기
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> W[i][j]; // 반복문을 통해 W 배열의 값 입력받기
        }
    }

    clock_t start, end;
    start = clock(); // 실행 시간 측정 시작

    tsp(1, 0, 1); // 노드 1에서 출발했다고 가정
    end = clock(); // 실행 시간 측정 종료

    cout << "\n최단 경로의 길이 : " << minLength;
    cout << "\n(1) 실행 시간 : " << (float)(end - start) << "ms\n";
    return 0;
}

/*
# 입력 
5
 0 14  4 10 20
14  0  7  8  7
 4  5  0  7 16
11  7  9  0  2
18  7 17  4  0 

# 출력
30

# 입력 
10
 0 12  3 23  1  5 32 65 56  8
12  0  9 18  3 41 45  5 21 70
 3  9  0 89 56 21 12 49  4  8
23 18 89  0  8  7 46 75 17 50
 1  3 56  8  0 55 22 86 10 71
 5 41 21  7 55  0 21  7  6 12
32 45 12 46 22 21  0 11 60 44
65  5 49 75 86  7 11  0  7 20
56 21  4 17 10  6 60  7  0 41
 8 70  8 50 71 12 44 20  41 0

# 출력
30

# 입력 
15
0 29 82 46 68 52 72 42 51 55 29 74 23 72 46
29 0 55 46 42 43 43 23 23 31 41 51 11 52 21
82 55 0 68 46 55 23 43 41 29 79 21 64 31 51
46 46 68 0 82 15 72 31 62 42 21 51 51 43 64
68 42 46 82 0 74 23 52 21 46 82 58 46 65 23
52 43 55 15 74 0 61 23 55 31 33 37 51 29 59
72 43 23 72 23 61 0 42 23 31 77 37 51 46 33
42 23 43 31 52 23 42 0 33 15 37 33 33 31 37
51 23 41 62 21 55 23 33 0 29 62 46 29 51 11
55 31 29 42 46 31 31 15 29 0 51 21 41 23 37
29 41 79 21 82 33 77 37 62 51 0 65 42 59 61
74 51 21 51 58 37 37 33 46 21 65 0 61 11 55
23 11 64 51 46 51 51 33 29 41 42 61 0 62 23
72 52 31 43 65 29 46 31 51 23 59 11 62 0 59
46 21 51 64 23 59 33 37 11 37 61 55 23 59 0  

# 출력
291
*/
