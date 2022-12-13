#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;

int N; // ����� ����
int W[20][20]; // �� ��� ���� �Ÿ� ����
int dp[20][1 << 20]; // dp[���� ����][�湮 ����]�� �����ϴ� �迭
                     // 1<<20 = 2^20���� ��� �湮 ������ ��Ʈ�� ǥ��

// �湮�� ��带 �����ϰ� ���� ��忡�� �������� ��ȸ�ϴ� ����� ���ϴ� �Լ�
// node : ���� ���, visitedBit : ��� �湮 ���θ� ��Ʈ�� ����
int tsp(int node, int visitedBit) {
    if (visitedBit == ((1 << N) - 1)) { // ���� �湮�� ��� (��� visitbit�� 1��)
        if (W[node][1] != 0) // ����� ���� ���� �� �� �ִ� ���� ������
            return W[node][1]; // ����� ���� ���ư��� �Ÿ� ����
        else return 10000000; 
    }

    // 0�� �ƴ϶�� �ּҰ� �ٷ� ����
    if (dp[node][visitedBit] != 0) return dp[node][visitedBit];

    // 0�̸�
    dp[node][visitedBit] = 10000000; 
    for (int i = 1; i <= N; i++) {
        // i ��忡 �湮�� �� ���� ����Ǿ� �־� �� �� �ִ� ���� ������
        if (!(visitedBit & (1 << (i - 1))) && W[node][i] != 0) { 
            dp[node][visitedBit] = min(dp[node][visitedBit] , W[node][i] + tsp(i, visitedBit | (1 << (i - 1))));
            // i ��带 �湮 üũ�ϰ� i�� ���������� tsp�Լ� ��� ����
        }
    }

    return dp[node][visitedBit];
}


int main() {
    cin >> N;  // ����� ���� N �Է¹ޱ�
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> W[i][j]; // �ݺ����� ���� W �迭�� �� �Է¹ޱ�
        }
    }

    clock_t start, end;
    start = clock(); // ���� �ð� ���� ����
    cout << "\n�ִ� ����� ���� : " << tsp(1, 1) << endl; // ��� 1���� ����ߴٰ� ����
    end = clock(); // ���� �ð� ���� ����

    cout << "\n(2) ���� �ð� : " << (float)(end - start) << "ms\n";
    return 0;
}