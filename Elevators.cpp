//DEVELOPED BY : ALIMEHDIPOUR 
#include <iostream>
#include <queue>
#include <string>
#include <climits>
using namespace std;

int minDistance(int[], bool[]);
void Dijkstra(int graph[150][150], int src,int dest);

queue<string> Result_queue;

int main()
{
	int I, J, N;
	int elevators_count = 0;
	const int Uninitialized = 0;
	std::cin >> N >> I >> J;
	//EACH INPUT
	while (!(N == 0 && I == 0 && J == 0))
	{
		int arr[10][150][150] = { 0 };
		elevators_count = N;
		//ELEVATORS
		for (int k = 0; k < N; k++) {
			int lp;
			std::cin >> lp;
			//LEVELS
			int TEMP_arr[150] = {0};
			for (int kk = 0; kk < lp; kk++) {
				int tt;
				std::cin  >>tt;
				tt++;
				TEMP_arr[kk] = tt;
			}
			for (int pp = 0; pp < lp; pp++) {
				int current = TEMP_arr[pp];
				int next = TEMP_arr[pp + 1];
				if (current == Uninitialized && next == Uninitialized) {/*DO NOTHING*/ }
				if (current == Uninitialized && next != Uninitialized) {/*DO NOTHING*/ }
				if (current != Uninitialized && next == Uninitialized) { arr[k][current][current] = 0; }
				if (current != Uninitialized && next != Uninitialized) {
					arr[k][current][next] = abs(next - current);
					arr[k][next][current] = abs(next - current);

				}
			}
		}
		int totall_adjancy_matrix[150][150] = {0};
		//UNION EACH ELEVATOR ADJANCY MATRIX ==> E1 U E2 U E3 U ... E10 = TOTAL ADJANCY MATRIX
		for (int i = 0; i < elevators_count; i++) {
			for (int j = 0; j < 150; j++) {
				for (int k = 0; k < 150; k++) {
					if (totall_adjancy_matrix[j][k] <= arr[i][j][k])
						totall_adjancy_matrix[j][k] = arr[i][j][k];
				}
			}
		}
		//NOW WE CAN FIND SHORTEST PATH FROM  I TO J WITH DIJKESTRA ALGORITHM
		Dijkstra(totall_adjancy_matrix, I+1, J+1);
		std::cin >> N >> I >> J;
	}
	while (!Result_queue.empty()){ 
		std::cout << Result_queue.front() << endl;
		Result_queue.pop();
	}
}
	
int minDistance(int dist[], bool sptSet[])
{
	int min = INT_MAX, min_index;
	for (int v = 0; v < 150; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;
	return min_index;
}
void Dijkstra(int graph[150][150], int start,int end)
{
	int dist[150]; 
	bool sptSet[150];
	for (int i = 0; i < 150; i++)
		dist[i] = INT_MAX, sptSet[i] = false;
	dist[start] = 0;
	for (int count = 0; count < 150 - 1; count++) {
		int u = minDistance(dist, sptSet);
		sptSet[u] = true;
		for (int v = 0; v < 150; v++)
			if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
				&& dist[u] + graph[u][v] < dist[v])
				dist[v] = dist[u] + graph[u][v];
	}
	if (dist[end] == 2147483647)
		//std::cout << "infinity";
		Result_queue.push("infinity");
	else
		//std::cout << dist[dest] << std::endl;
		Result_queue.push(to_string(dist[end]));
}
