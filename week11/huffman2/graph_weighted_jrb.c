#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../lib/include/dllist.h"
#include "graph_weighted_jrb.h"

Graph_W createGraph()
{
	Graph_W newGraph;
	newGraph.edges = make_jrb();
	newGraph.vertices = make_jrb();

	return newGraph;
}


void addVertext(Graph_W graph, int id, char *name)
{
	JRB node = jrb_find_int(graph.vertices, id);

	if (node == NULL) {
		jrb_insert_int(graph.vertices, id, new_jval_s(name));
	} else {
		printf("Vertex already existed!\n");
	}
}


char *getVertexName(Graph_W graph, int id)
{
	JRB node = jrb_find_int(graph.vertices, id);

	if (node != NULL)
		return jval_s(node->val);
	return NULL;
}


int isAdjacent(Graph_W graph, int v1, int v2)
{
	JRB node1, tree, node2;

	node1 = jrb_find_int(graph.edges, v1);
	if (node1 != NULL) {
		tree = (JRB) jval_v(node1->val);
		node2 = jrb_find_int(tree, v2);

		if (node2 != NULL) return 1;
	}

	return 0;
}


void addEdge(Graph_W graph, int v1, int v2, double weight)
{
	if (isAdjacent(graph, v1, v2)) return;

	JRB node1, tree;

	node1 = jrb_find_int(graph.edges, v1);
	if (node1 == NULL) {
		tree = make_jrb();
		jrb_insert_int(graph.edges, v1, new_jval_v(tree));
		jrb_insert_int(tree, v2, new_jval_d(weight));
	} else {
		tree = (JRB) jval_v(node1->val);
		jrb_insert_int(tree, v2, new_jval_d(weight));
	}
}


double getEdgeValue(Graph_W graph, int v1, int v2)
{
	JRB node1, tree, node2;

	node1 = jrb_find_int(graph.edges, v1);
	if (node1 != NULL) {
		tree = (JRB) jval_v(node1->val);
		node2 = jrb_find_int(tree, v2);
		if (node2 != NULL) return jval_d(node2->val);
	}

	return INFINITIVE_VALUE;
}


void dropGraph(Graph_W graph)
{
	JRB node;
	
	jrb_traverse(node, graph.edges) {
		jrb_free_tree(jval_v(node->val));
	}

	jrb_free_tree(graph.vertices);
	jrb_free_tree(graph.edges);
}


int outDegree(Graph_W graph, int v, int *outDegreeList)
{
	int count = 0;
	JRB node, tree;

	node = jrb_find_int(graph.edges, v);
	if (node != NULL) {
		tree = (JRB) jval_v(node->val);
		JRB ptr;
		jrb_traverse(ptr, tree) {
			outDegreeList[count++] = jval_i(ptr->key);
		}
	}

	return count;
}

int inDegree(Graph_W graph, int v, int* inDegreeList)
{
	int count = 0;

	JRB node;

	node = jrb_find_int(graph.vertices, v);
	if (node != NULL) {
		JRB ptr;
		jrb_traverse(ptr, graph.vertices) {
			int inVertex = jval_i(ptr->key);
			if (isAdjacent(graph, inVertex, v))
				inDegreeList[count++] = inVertex;
		}
	}

	return count;
}

double shortestPath(Graph_W graph, int start, int stop, int *outputPath, int *length)
{
	// Return if the first or last node is not valid
	JRB check1 = jrb_find_int(graph.vertices, start);
	JRB check2 = jrb_find_int(graph.vertices, stop);

	if (check1 == NULL) {
		printf("Graph does not have vertex %d\n", start);
		return INFINITIVE_VALUE;
	}
	if (check1 == NULL) {
		printf("Graph does not have vertex %d\n", stop);
		return INFINITIVE_VALUE;
	}

	// main variable
	JRB parent = make_jrb();
	JRB distance = make_jrb();
	Dllist queue = new_dllist();

	int V = 0;
	JRB node;
	// We maintain two informations about each node:
    // the parent node (the node it come from) and the distance from the starting node
    // by traversing through the parent node
	jrb_traverse(node, graph.vertices) {
		int key = jval_i(node->key);
		double dist = INFINITIVE_VALUE;
		if (key == start) dist = 0;

		// At first, all node does not have a parent, and its distance will be set to INFINITIVE_VALUE,
        // except for the starting node
		jrb_insert_int(parent, key, new_jval_i(start));
		jrb_insert_int(distance, key, new_jval_d(dist));
		V++;
	}


	// Create a priority queue, it which the order is determined by the distance to that node from the starting node
    // At first, the starting node will be the first element, since its distance would be 0, and the rest is INFINITIVE_VALUE
	jrb_traverse(node, graph.vertices) {
		int key = jval_i(node->key);
		if (key == start) {
			dll_prepend(queue, new_jval_i(start));
		} else {
			dll_append(queue, new_jval_i(key));
		}
	}

	while(dll_empty(queue) != 1) {
		Dllist Dlnode = dll_first(queue);
		int keyID = jval_i(node->val);
		int total;
		int adjcentList[V];
		// If the first element in the queue is the node we are looking for,
        // that mean the path we took to it is shortest
		if (keyID == stop) {
			total = 0;
			// Look up all the node in JRB parent
			while (keyID != start) {
				// Use adjacents[] as a middle array,
                // since the way we traverse will return the node in reverse order
                adjcentList[total++] = keyID;
                keyID = jval_i(jrb_find_int(parent, keyID)->val);
			}
			// Return the correct path order
			for (int i = 0; i < total; i++) {
				outputPath[i] = adjcentList[total - 1 - i];
			}
			// Return the total number of node in the route
			*length = total;

			// Return the total weight;  
			return jval_d(jrb_find_int(distance, stop)->val);
		}

		// Dequeue the first element
		dll_delete_node(Dlnode);

		// find current distance from distance list;
		JRB keyNode = jrb_find_int(distance, keyID);
		double keyDistance = jval_d(keyNode->val);

		
		int count = outDegree(graph, keyID, adjcentList);
		for (int i = 0; i < count; i++) {
			int adjID = adjcentList[i];
			double weight = getEdgeValue(graph, keyID, adjID);

			double adjDistance = jval_d(jrb_find_int(distance, adjID)->val);
			// If the path to adjacent node can be shorten
			// by going through the current node
			// we set its parent to the current node
			if (keyDistance + weight < adjDistance) {
				// set new distance
				adjDistance = keyDistance + weight;
				node = jrb_find_int(distance, adjID);
				jrb_delete_node(node);
				jrb_insert_int(distance, adjID, new_jval_d(adjDistance));
				// set new parent
				node = jrb_find_int(parent, adjID);
				jrb_delete_node(node);
				jrb_insert_int(parent, adjID, new_jval_i(keyID));
			
				Dllist temp;
				// Dequeue the adjacent node and re-add it to the queue to maintain the priority
				dll_traverse(temp, queue) {
					if (jval_i(temp->val) == adjID) {
						dll_delete_node(temp);
						break;
					}
				}

				dll_traverse(temp, queue) {
					int dlKey = jval_i(temp->val);
					int dist = jval_d(jrb_find_int(distance, dlKey)->val);
					if (dist > adjDistance) {
						dll_insert_b(temp, new_jval_i(adjID));
						break;
					}
				}
			}
		}
	}
	free_dllist(queue);
	jrb_free_tree(parent);
	jrb_free_tree(distance);
	return jval_d(jrb_find_int(distance, stop)->val);
}



int getHighestID(Graph_W graph)
{	
	int max = -1000;
	JRB ptr;
	jrb_traverse(ptr, graph.vertices) {
		int key = jval_i(ptr->key);
		if(key >= max)
			max = key;
	}
	return max;
}

// CHUA TEST

//chua test 
void BFS(Graph_W graph,int start,int stop,void(*func)(int))
{
	JRB check1 = jrb_find_int(graph.vertices,start);
	JRB check2 = jrb_find_int(graph.vertices,stop);
	if (check1 == NULL ) {
		printf("Graph does not have vertex %d\n", start);
		return;
	}
	if (stop != -1 && check2 == NULL) {
		printf("Graph does not have vertex %d\n", stop);
		return;
	}
	// Create a Graph name 'parent' in which each node's key is
    // a vertex, and it's key indicate whether the vertex has been
    // parent (1) or not (0)
	JRB parent = make_jrb();
	JRB ptr;
	int V = 0;
	jrb_traverse(ptr,graph.vertices) {
		V++;
		jrb_insert_int(parent, jval_i(ptr->key), new_jval_i(0));
	}
	int output[V];
	
	// Create a queue and enqueue the first element
	Dllist queue = new_dllist();
	dll_append(queue,new_jval_i(start));
	// Traversing
	while(dll_empty(queue) != 1) {
		// Take first element in the queue
		Dllist node = dll_first(queue);
		int key = jval_i(node->val);
		// Dequeue this element
		dll_delete_node(node);
		
		JRB tmp = jrb_find_int(parent, key);
		if(jval_i(tmp->val) == 0) {
			// Pass the vertex to external function
			func(key);
			// Mark this element as 'parent'
			jrb_delete_node(tmp);
			jrb_insert_int(parent, key, new_jval_i(1));		
		}

		// If this is the required vertex, return
		if (key == stop) {
			free_dllist(queue);
			jrb_free_tree(parent);
			return;
		}
		
		int count = outDegree(graph, key, output);
		for (int i = 0; i < count; i++)
		{
			JRB ptr = jrb_find_int(parent, output[i]);
			if(jval_i(ptr->val) == 0)
				dll_append(queue,new_jval_i(output[i])); 
		}

	}
	free_dllist(queue);
	jrb_free_tree(parent);
}

// DFS ..........................................................

void DFS(Graph_W graph, int start, int stop, void (*func)(int))
{

	JRB check1 = jrb_find_int(graph.vertices,start);
	JRB check2 = jrb_find_int(graph.vertices,stop);
	if(check1 == NULL ) {
		printf("Graph_W does not have vertex %d\n", start);
		return;
	}
	if(stop != -1 && check2 == NULL) {
		printf("Graph_W does not have vertex %d\n", stop);
		return;
	}
	
	// Create a Graph_W name 'parent' in which each node's key is
    // a vertex, and it's key indicate whether the vertex has been
    // parent (1) or not (0)
	JRB parent = make_jrb();
	JRB ptr;
	int V = 0;
	jrb_traverse(ptr, graph.vertices) {
		V++;
		jrb_insert_int(parent, jval_i(ptr->key), new_jval_i(0));
	}
	int output[V];

    // Create a stack and push the first element
	Dllist stack = new_dllist();
	dll_append(stack, new_jval_i(start));

    // Traversing
	while(dll_empty(stack) != 1) {
        // Take the top element in the stack
		Dllist dll_tmp = dll_last(stack);
		int key = jval_i(dll_tmp->val);

		JRB tmp = jrb_find_int(parent, key);
		if(jval_i(tmp->val) == 0) {
        	// Pass the vertex to external function
			func(key);
	        // Mark this element as 'parent'
			jrb_delete_node(tmp);
			jrb_insert_int(parent, key, new_jval_i(1));
		} else {
			// Pop this element
			dll_delete_node(dll_tmp);
		}

		// If this is the required vertex, return
		if (key == stop) {
			free_dllist(stack);
			jrb_free_tree(parent);
			return;
		}

		int count = outDegree(graph, key, output);
		for (int i = count -1 ; i >= 0; i--)
		{
	            // If the vertice has not been parent, push it
			tmp = jrb_find_int(parent, output[i]);
			if (jval_i(tmp->val) == 0) {
				key = jval_i(tmp->key);
				dll_append(stack, new_jval_i(key));
			}
		}
	}
	free_dllist(stack);
	jrb_free_tree(parent);
}


int isCycleVertex(Graph_W graph, int start)
{
	JRB parent = make_jrb();
	JRB ptr;
	int V = 0;
	jrb_traverse(ptr, graph.vertices) {
		V++;
		jrb_insert_int(parent, jval_i(ptr->key), new_jval_i(0));
	}
	int output[V];

    // Create a stack and push the first element
	Dllist stack = new_dllist();
	dll_append(stack, new_jval_i(start));
    // Traversing
    // printf("\n");
	while(dll_empty(stack) != 1) {
        // Take the top element in the stack
		Dllist dll_tmp = dll_last(stack);
		int key = jval_i(dll_tmp->val);

		JRB tmp = jrb_find_int(parent, key);
		if(jval_i(tmp->val) == 0) {
        	// Pass the vertex to external function
			//printf("%4d ", key);
	        // Mark this element as 'parent'
			jrb_delete_node(tmp);
			jrb_insert_int(parent, key, new_jval_i(1));
		} else {
			// Pop this element
			dll_delete_node(dll_tmp);
		}
		int count = outDegree(graph, key, output);
		for (int i = count -1 ; i >= 0; i--) {
			if (start == output[i])
			{
				jrb_free_tree(parent);
				free_dllist(stack);
				return 1;
			}
			// If the vertice has not been parent, push it
			tmp = jrb_find_int(parent, output[i]);
			if (jval_i(tmp->val) == 0) {
				key = jval_i(tmp->key);
				dll_append(stack, new_jval_i(key));
			}
		}
	}
	
	free_dllist(stack);
	jrb_free_tree(parent);
	return 0;
}

int isCycle(Graph_W graph)
{	
	JRB ptr;
	jrb_traverse(ptr,graph.edges) {
		if(isCycleVertex(graph,jval_i(ptr->key)) == 1)
			return 1;
	}
	return 0;
}

void showVertices(Graph_W graph)
{
	JRB ptr;
	jrb_traverse(ptr, graph.vertices) {
		printf("%d - \"%s\"\n", jval_i(ptr->key), jval_s(ptr->val));
	}
}

void printGraph(Graph_W graph)
{
	JRB ptr;	
	jrb_traverse(ptr, graph.edges) {
		printf("%s: ", getVertex(graph, jval_i(ptr->key)));
		JRB tree = (JRB) jval_v(ptr->val);
		JRB ptr2;
		jrb_traverse(ptr2, tree) {
			printf("%8s",getVertex(graph, jval_i(ptr2->key)));
		}
		printf("\n");
	}
}

void topologicalSort(Graph_W graph, void (*visit)(int))
{
	JRB ptr;
	int V = 0;
	jrb_traverse(ptr, graph.vertices) {
		V++;
	}
	int indegree_node[V];
	int indegree_arr[V];

	Dllist queue = new_dllist();
	jrb_traverse(ptr, graph.vertices) {
		int key = jval_i(ptr->key);
		indegree_arr[key] = inDegree(graph, key, indegree_node);
		if(indegree_arr[key] == 0)
			dll_append(queue, new_jval_i(key));
	}
	// Traversing
	while(dll_empty(queue) != 1) {
		// Take first element in the queue
		Dllist node = dll_first(queue);
		int key = jval_i(node->val);
			// Dequeue this element
		dll_delete_node(node);

		visit(key);
		int outdegree_node[V];

		int count = outDegree(graph, key, outdegree_node);
		for (int i = 0; i < count; ++i)	{
			int tail = outdegree_node[i];
			indegree_arr[tail]--;
			if (indegree_arr[tail] == 0) {
				dll_append(queue, new_jval_i(tail));
			}
		}	
	}
	free_dllist(queue);
}

void topologicalSort_T(Graph_W g, int * output, int * n) {
	JRB node;
	int indegreeList[100];
	int adjacents[100];
	int count = 0;
	int i, total;
	int key;
	int tail;
	Dllist queue = new_dllist();
	Dllist temp;

	jrb_traverse(node, g.vertices) {
		indegreeList[count] = inDegree(g, jval_i(node->key), adjacents);
		if (indegreeList[count] == 0) {
			dll_append(queue, node->key);
		}
		count++;
	}

	*n = count;
	count = 0;
    // indegreeList
    // 0, 1, 1, 1, 2, 0
    // -1, 0, 0, 1, 1, 0

	while(dll_empty(queue) != 1) {
		temp = dll_first(queue);
		key = jval_i(temp->val);
		dll_delete_node(temp);

		output[count] = key;
		indegreeList[key] = -1;

		total = outDegree(g, key, adjacents);
		for (i = 0; i < total; i++) {
			tail = adjacents[i];
			indegreeList[tail]--;
			if (indegreeList[tail] == 0) {
				dll_append(queue, new_jval_i(tail));
			}
		}

		count++;
	}
}