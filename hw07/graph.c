#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <time.h>  // enables srand(time(NULL)) to make pseudorandom generator better
#include "graph.h"

// HELPER FUNCTIONS
int _get_random_weight();
void _shuffle_idxs(size_t* idxs, size_t num_idxs);
size_t* _get_shuffled_idxs(size_t num_idxs);

int get_in_degree(Graph graph, int idx_of_vertex_to) {				//Function counts the number of edges coming into a node
	int edges_in = 0;
	for(int i = 0; i < graph.num_vertices; i++) {					//For each vertex, if the weight of the edge is not zero, count the edge
		if(graph.weights_from_to[i][idx_of_vertex_to] != 0) {
			edges_in++;
		}
	}
	return edges_in;
}

int get_out_degree(Graph graph, int idx_of_vertex_from) {			//Function counts the number of edges going out into a node
	int edges_out = 0;
	for(int i = 0; i < graph.num_vertices; i++) {					//For each vertex, if the weight of the edge is not zero, count the edge
		if(graph.weights_from_to[idx_of_vertex_from][i] != 0) {
			edges_out++;
		}
	}
	return edges_out;
}

Graph create_random_complete_graph(size_t num_vertices) {								//Function creates a complete graph
	Graph random_complete_graph = create_graph_with_no_edges(num_vertices);
	for(int i = 0; i < num_vertices; i++) {												//For each vertex
		for(int j = 0; j < num_vertices; j++) {
			if(i != j) {																//If the vertices do not equal each other, adda weight to the edge
				random_complete_graph.weights_from_to[i][j] = _get_random_weight();
			}
			else {																		//Otherwise, make the edge a weight of zero
				random_complete_graph.weights_from_to[i][j] = 0;
			}
		}
	}
	return random_complete_graph;
}

Graph create_random_cycle_graph(size_t num_vertices) {									//Function creates a cycle graph
	assert(num_vertices >= 2);
	Graph random_cycle_graph = create_graph_with_no_edges(num_vertices);
	size_t* indexes_shuffled = _get_shuffled_idxs(num_vertices);
	for(int i = 0; i < num_vertices - 1; i++) {											//For each vertex, create an edge with the vertex in front of it
		random_cycle_graph.weights_from_to[indexes_shuffled[i]][indexes_shuffled[i + 1]] = _get_random_weight();
	}
	random_cycle_graph.weights_from_to[indexes_shuffled[num_vertices - 1]][indexes_shuffled[0]] = _get_random_weight();			//Create an edge between the last and first vertex
	free(indexes_shuffled);
	return random_cycle_graph;
}

Graph create_random_wheel_graph(size_t num_vertices) {									//Function creates a wheel graph
	assert(num_vertices >= 3);
	Graph random_wheel_graph = create_graph_with_no_edges(num_vertices);
	create_random_cycle_graph(num_vertices - 1);										//Call create a cycle function to create the cycle vertices surrounding the hub vertex
	size_t* indexes_shuffled = _get_shuffled_idxs(num_vertices);
	int vertices_num = random_wheel_graph.num_vertices;
	for(int i = 0; i < vertices_num; i++) {												//For each vertex in the cycle, connect it to the hub vertex
		if(i != indexes_shuffled[num_vertices - 1]) {
			random_wheel_graph.weights_from_to[indexes_shuffled[vertices_num - 1]][i] = _get_random_weight();
			random_wheel_graph.weights_from_to[i][indexes_shuffled[vertices_num - 1]] = _get_random_weight();
		}
	}
	return random_wheel_graph;
}

Graph create_random_graph(size_t num_vertices, size_t num_edges) {						//Function creates a random graph
	Graph random_graph = create_graph_with_no_edges(num_vertices);
	int edges_formed = 0;
	size_t* indexes_shuffled_1 = _get_shuffled_idxs(num_vertices);
	size_t* indexes_shuffled_2 = _get_shuffled_idxs(num_vertices);
	while(edges_formed < num_edges) {													//While there are still edges to be formed, create an edge between two random vertices
		if(random_graph.weights_from_to[indexes_shuffled_1[0]][indexes_shuffled_2[0]] == 0) {
			random_graph.weights_from_to[indexes_shuffled_1[0]][indexes_shuffled_2[0]] = _get_random_weight();
			edges_formed++; 
		}
		_shuffle_idxs(indexes_shuffled_1, num_vertices);								//Randomize the vertices before the next edge connection
		_shuffle_idxs(indexes_shuffled_2, num_vertices);
	}
	free(indexes_shuffled_1);
	free(indexes_shuffled_2);
	return random_graph; // TODO: delete this line and complete function.
}

bool is_strongly_connected(Graph graph) {												//Function uses breath first search to determine strength of graph
	bool backward = false;
	bool forward = false;
	bool queue_status = false;
	int vertices_num = graph.num_vertices;
	int* vertex_queue = malloc(vertices_num * sizeof *vertex_queue);
	int index_queue = 0;
	for(int i = 0; i < vertices_num; i++) {												//For each pair of vertices, check if the edge is strong in one direction
		for(int j = 0; j < vertices_num; j++) {
			if(graph.weights_from_to[j][vertex_queue[i]] != 0) {
				for(int num_queue = 0; num_queue < vertices_num; num_queue++) {
					if(vertex_queue[num_queue] == j) {
						queue_status = true;
					}
				}
				if(!queue_status) {
					vertex_queue[index_queue++] = i;
				}
				queue_status = false;
			}
			if(index_queue == vertices_num - 1) {
				backward = true;
			}
		}
	}
	for(int i = 0; i < vertices_num; i++) {												//Reset queue for vertices
		vertex_queue[i] = 0;
	}
	index_queue = 0;
	for(int i = 0; i < vertices_num; i++) {												//For each pair of verticies, check if the edge is strong in the other direction
		for(int j = 0; j < vertices_num; j++) {
			if(graph.weights_from_to[vertex_queue[i]][j] != 0) {
				for(int num_queue = 0; num_queue < vertices_num; num_queue++) {
					if(vertex_queue[num_queue] == j) {
						queue_status = true;
					}
				}
				if(!queue_status) {
					vertex_queue[index_queue++] = i;
				}
				queue_status = false;
			}
			if(index_queue == vertices_num - 1) {
				forward = true;
			}
		}
	}
	return (backward && forward);														//If the edge is strong in both directions for all vertex pairs, the graph is strong
}

Graph create_graph_with_no_edges(size_t num_vertices) {
	// Create a graph containing vertices but no edges.  Note:  weight of 0 indicates no edge.
	Graph graph = {
		.num_vertices = num_vertices,
		.weights_from_to = malloc((sizeof *graph.weights_from_to) * num_vertices
								+ (sizeof **graph.weights_from_to) * num_vertices * num_vertices) };
	int* a_weight_0_to_0 = (int*)(&graph.weights_from_to[num_vertices]);
	for(size_t idx_of_vertex_from = 0; idx_of_vertex_from < num_vertices; idx_of_vertex_from++) {
		graph.weights_from_to[idx_of_vertex_from] = a_weight_0_to_0 + idx_of_vertex_from * num_vertices;
		for(int vertex_idx_to = 0; vertex_idx_to < num_vertices; vertex_idx_to++) {
			graph.weights_from_to[idx_of_vertex_from][vertex_idx_to] = 0;
		}
	}
	/* This "matrix" is stored as an array of arrays.  First, there is an array of addresses to
	 * each array (row).  Then, there are 'num_vertices' arrays (one for each row of the matrix).
	 * To reduce calls to malloc(..) and simplify freeing, we combine all of that into one block.
	 *
	 * You can simply refer to it the same as you would a 2D array.
	 * Ex: graph.weights_from_to[2][3] = 5;
	 *     // creates an edge from vertex 2 to vertex 3 with weight 5.
	 */
	return graph;
}

void print_graph(char const* label, Graph graph) {
	// Print the graph as text. including the in-degree and out-degree.
	// ∙ Okay to modify this to your liking, if you wish.
	char* labels[] = { "A","B","C","D","E","F","G","H","I","J","K","L","M",
		                "N","O","P","Q","R","S","T","U","V","W","X","Y","Z" };
	size_t num_labels = sizeof labels / sizeof labels[0];
	assert(graph.num_vertices < num_labels);  // add more labels above, if you want to do larger graphs

	printf("%s\n", label);
	for(size_t idx_vertex = 0; idx_vertex < graph.num_vertices; idx_vertex++) {
		printf("∙ %s ", labels[idx_vertex]);
		int in_degree = get_in_degree(graph, idx_vertex);
		int out_degree = get_out_degree(graph, idx_vertex);
		printf("(in=%d/out=%d): ", in_degree, out_degree);
		bool found_edge = false;
		for(size_t vertex_idx_to = 0; vertex_idx_to < graph.num_vertices; vertex_idx_to++) {
			if(idx_vertex != vertex_idx_to) {
				int weight = graph.weights_from_to[idx_vertex][vertex_idx_to];
				if(weight >= 1) {
					if(found_edge) {
						printf(", ");
					}
					else {
						found_edge = true;
					}
					printf("→%s (%d)", labels[vertex_idx_to], weight);
				}
			}
		}
		if(!found_edge) {
			printf("(no edges)");
		}
		printf("\n");
	}
	printf("\n");
}

void free_graph(Graph* a_graph) {
	// Free the matrix (array of arrays) the 'Graph' object refers to.
	free(a_graph -> weights_from_to);
	a_graph -> weights_from_to = NULL;
	a_graph -> num_vertices = 0;
}

int _get_random_weight() {
	// Return a random int from 1 to MAX_WEIGHT, inclusive.
	int MAX_WEIGHT = 5;
	int random_weight = rand() % MAX_WEIGHT + 1;
	assert(random_weight >= 0 && random_weight <= MAX_WEIGHT);
	return random_weight;
}

size_t* _get_shuffled_idxs(size_t num_idxs) {
	// Return an array of indexes (numbers) from 0 to (num_idxs - 1), in random order.
	// ∙ Caller is responsible for freeing memory.
	// ∙ You can call _shuffle_idxs(…) to shuffle the array again in-place without recreating it.
	size_t* idxs = malloc(num_idxs * sizeof *idxs);
	for(size_t i = 0; i < num_idxs; i++) {
		idxs[i] = i;
	}
	_shuffle_idxs(idxs, num_idxs);
	for(int i = 0; i < num_idxs; i++) {
		printf("%zd%s", idxs[i], i + 1 < num_idxs ? ", " : "\n");
	}
	return idxs;
}

void _shuffle_idxs(size_t* idxs, size_t num_idxs) {
	// Re-shuffle an array of indexes created with _get_shuffled_idxs(…).
	for(size_t src_pos = num_idxs - 1; src_pos >= 1; src_pos--) {
		size_t dst_pos = rand() % src_pos;
		size_t copy_of_dst = idxs[dst_pos];
		idxs[dst_pos] = idxs[src_pos];
		idxs[src_pos] = copy_of_dst;
	}
}

#define VERSION_GRAPH_C 1
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
