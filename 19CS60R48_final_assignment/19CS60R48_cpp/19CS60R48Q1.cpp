// Name:Abhishek Shaw
// Roll No.:19CS60R48
// Question 1
#include<iostream>
using namespace std;
/*
Solution is based on Dfs back edge. 
The following algorithm finds if there is a back edge present in the graph
*/
void DFS(int , int *, int **, int **, int);

int main()
{
	int nodes, edges;
	int cycle_flag = 0;
	cin>>nodes>>edges;
	if(nodes<0 or edges<0){
		cout<<"Invalid Input";
		exit(0);
	}
	int **matrix = new int*[nodes];  //matrix is Adjaceny Matrix which stores data for direct edge. for A[i][j] there is a directed edge from i to j
	int **vertex = new int*[nodes];	 //vertex is a matrix used to store discoverd time, finish time, and current status of all the nodes. 0 means not discovered, 1 means discovered and processing, 2 means processed fully
	for(int i = 0 ; i < nodes ; i++)
	{
		matrix[i] = new int[nodes];
		for(int k = 0; k < nodes ; k++)
			matrix[i][k] = 0;
		vertex[i] = new int[3];
		vertex[i][0]= 0;
		vertex[i][1]= 0;
		vertex[i][2]= 0;
	}
	
	int u,v;
	for(int i = 0 ; i < edges ; i++)
	{
		cin>>u>>v;
		if(u<0 or v<0 or u>=nodes or v>=nodes){
			cout<<"Invalid Input\n";
			exit(0);
		}
		matrix[u][v] = 1;
	}
	for(int i = 0 ; i < nodes ; i++)   
	{
		if(vertex[i][0] == 0)
		{
			DFS(i , &cycle_flag, matrix, vertex, nodes);
		}
	}
	if(cycle_flag)
		cout<<"Graph contains cycle";
	else
		cout<<"Graph doesn’t contain cycle";
	delete[] matrix;
	delete[] vertex;
	cout<<"\n";
	return 0;
}


void DFS(int node, int *cycle_flag, int **matrix, int **vertex, int nodes_size)
{
	static int time = 0;
	if(*cycle_flag == 0)
	{
		vertex[node][0] = 1;
		time++;
		vertex[node][1] = time;
		
		    //searching the adjacent nodes for edges
			for(int adjacent_node = 0 ; adjacent_node < nodes_size ; adjacent_node++)
			{
				if(matrix[node][adjacent_node])
				{
					if(vertex[adjacent_node][0] == 0)
					{
						//if the node is not yet visited then it will make a tree edge
						DFS(adjacent_node, cycle_flag, matrix, vertex, nodes_size);
					}
					
					//checking for back edge condition
					if(vertex[adjacent_node][0] == 1 && vertex[adjacent_node][1] <= time)  
					{
						*cycle_flag = 1;
						return;
					}
				}
			}
			vertex[node][0] = 2;
			time++;
			vertex[node][2] = time;
	}
}