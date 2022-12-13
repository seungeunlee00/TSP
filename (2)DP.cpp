#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;

int N; // 노드의 개수
int W[20][20]; // 각 노드 간의 거리 저장
int dp[20][1 << 20]; // dp[현재 도시][방문 여부]를 저장하는 배열
                     // 1<<20 = 2^20개의 노드 방문 여보를 비트로 표시

// 방문한 노드를 제외하고 현재 노드에서 나머지를 순회하는 비용을 구하는 함수
// node : 현재 노드, visitedBit : 노드 방문 여부를 비트로 저장
int tsp(int node, int visitedBit) {
    if (visitedBit == ((1 << N) - 1)) { // 전부 방문한 경우 (모든 visitbit가 1임)
        if (W[node][1] != 0) // 출발한 노드로 돌아 갈 수 있는 길이 있으면
            return W[node][1]; // 출발한 노드로 돌아가는 거리 리턴
        else return 10000000; 
    }

    // 0이 아니라면 최소값 바로 리턴
    if (dp[node][visitedBit] != 0) return dp[node][visitedBit];

    // 0이면
    dp[node][visitedBit] = 10000000; 
    for (int i = 1; i <= N; i++) {
        // i 노드에 방문한 적 없고 연결되어 있어 갈 수 있는 길이 있으면
        if (!(visitedBit & (1 << (i - 1))) && W[node][i] != 0) { 
            dp[node][visitedBit] = min(dp[node][visitedBit] , W[node][i] + tsp(i, visitedBit | (1 << (i - 1))));
            // i 노드를 방문 체크하고 i를 시작점으로 tsp함수 재귀 진행
        }
    }

    return dp[node][visitedBit];
}


int main() {
    cin >> N;  // 노드의 개수 N 입력받기
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> W[i][j]; // 반복문을 통해 W 배열의 값 입력받기
        }
    }

    clock_t start, end;
    start = clock(); // 실행 시간 측정 시작
    cout << "\n최단 경로의 길이 : " << tsp(1, 1) << endl; // 노드 1에서 출발했다고 가정
    end = clock(); // 실행 시간 측정 종료

    cout << "\n(2) 실행 시간 : " << (float)(end - start) << "ms\n";
    return 0;
}