// Dijkstra_implementation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include "MinHeap.h"
using namespace std;
int minDistance(vector<int>& dist, vector<bool>& weight_found, int number_of_cities);
void printSolution(vector<int>& dist, int number_of_cities);
void inserting_city(vector<int>& parent, vector<string>& cities);
void dijkstra(vector<vector<int> >& graph, int src, int num_of_cities, vector<string>& cities);
int main()
{
	
	int test_cases;
	int num_of_cities;
	
	cin >> test_cases;
	while (test_cases != 0) {
		cin >> num_of_cities;
		vector<string> name_of_city(num_of_cities);
		vector<vector<int> > graph(num_of_cities, vector<int>(num_of_cities));
		for (int k = 0; k < num_of_cities; k++) {
			cin >> name_of_city[k];
		}
		for (int i = 0; i < num_of_cities; i++) {
			for (int j = 0; j < num_of_cities; j++) {
				cin >> graph[i][j];
			}
		}

		/*
		cout << test_cases << endl;
		cout << num_of_cities << endl;
		for (int k = 0; k < num_of_cities; k++) {
			cout << name_of_city[k] << endl;
		}
		for (int i = 0; i < num_of_cities; i++) {
			for (int j = 0; j < num_of_cities; j++) {
				cout << graph[i][j];
			}
			cout << endl;
		}
		*/
		dijkstra(graph, 0, num_of_cities, name_of_city);
		test_cases--;
	}
	/*
	city c;
	MinHeap h(4);
	h.insertKey(city(0,90));
	h.insertKey(city(1,10));
	//h.deleteKey(1);
	h.insertKey(city(2,300));
	h.insertKey(city(3,67));
	//h.insertKey(4);
	//h.insertKey(45);
	c = h.extractMin();
	cout << c.i << " ";
	cout << c.dist << " ";
	//h.decreaseKey(2, 1);
	cout << h.getMin();
	*/
    return 0;
}
/*
int minDistance(vector<int>& dist, vector<bool>& weight_found, int number_of_cities, vector<string>& city)
{
	// Initialize min value 
	int min = INT_MAX, min_index;

	for (int v = 0; v < number_of_cities; v++) {
		if (weight_found[v] == false && dist[v] <= min) {
			min = dist[v], min_index = v;
			//cout << city[v] << endl;
		}
	}
	for (int i = 0; i < dist.size(); i++)
		cout << dist[i] << endl;
	cout << "new vector is:" << endl;
	for (int i = 0; i < weight_found.size(); i++)
		cout << weight_found[i] << endl;
	cout << "The min_index is: " << min_index << endl;
	return min_index;
}
*/

void inserting_city(vector<int>& parent, vector<string>& cities) {
	stack<int> numbers;
	int y = parent.size()-1;
	while (y != 0) {
		numbers.push(y);
		y = parent[y];
	}
	numbers.push(0);
	while (numbers.size() != 0) {
		cout << cities[numbers.top()] << " ";
		numbers.pop();
	}
}

int minDistance(vector<int>& dist, vector<bool>& weight_found, int number_of_cities)
{
	// Initialize min value 
	int min = INT_MAX, min_index;
	//int min_index = INT_MAX;
	MinHeap cities_heaps(number_of_cities);
	for (int v = 0; v < number_of_cities; v++) {
		if (weight_found[v] == false && dist[v] <= min) {
			//min = dist[v], min_index = v;
			cities_heaps.insertKey(city(v,dist[v]));
		}
	}
	min_index=cities_heaps.deleteKey().i;

	
	return min_index;
}

void dijkstra(vector<vector<int> >& graph, int src, int num_of_cities, vector<string>& cities)
{
	vector<int> dist(num_of_cities);     
	vector<int> parent(num_of_cities);
	vector<bool> weight_found(num_of_cities); 
	for (int i = 0; i < num_of_cities; i++) {
		dist[i] = INT_MAX;
		weight_found[i] = false;
	}
 
	dist[src] = 0;

	for (int count = 0; count < num_of_cities; count++)
	{
		int u = minDistance(dist, weight_found,num_of_cities);
		 
		weight_found[u] = true;
 
		for (int v = 0; v < num_of_cities; v++) {

			if (!weight_found[v] && graph[u][v] && dist[u] != INT_MAX
				&& dist[u] + graph[u][v] < dist[v]) {
				dist[v] = dist[u] + graph[u][v];
				parent[v] = u;
				//cout << cities[v] << v << " " << dist[v] << graph[u][v] << dist[u] << " " << u << endl;
			}
		}
		
	} 
	inserting_city(parent, cities);
	cout << dist[num_of_cities - 1] << endl;
}

