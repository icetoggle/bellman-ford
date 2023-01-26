/*
	Description: Directed Graph container designed to facilitate bellman-ford algorithm.

	Author: Gabriel Ferrin
	Date: May 7th 2013
*/
#ifndef DIRECTED_GRAPH_H
#define DIRECTED_GRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include <node.h>
#include <node_object_wrap.h>

class Graph : public node::ObjectWrap {
public:

	Graph();
	Graph( std::string & );
	~Graph();

	static void Init(v8::Local<v8::Object> target);

	void print();
	void trim();
	void add_node( std::string & name );
	void add_edge( std::string &, std::string &, double );
	void update_edge( std::string &, std::string &, double );
	bool search( std::string & name );
	bool generate_path( std::vector<std::vector<std::string> >& path, std::vector<int> & parents);
	bool bellman_ford( std::string & name, std::vector<std::vector<std::string> > & );

private:
	/* Structs */
	struct GEdge;

	struct GNode
	{
		std::string name;
		std::vector<GEdge> edges;
		int key;

		GNode( std::string name, int k ) : name( name ), key( k ) {}
	};

	struct GEdge
	{
		double weight;
		GNode * node; // node that the edge is pointed towards

		GEdge( double weight, GNode *& n ) : weight( weight ), node( n ) {}
	};

	/* Variables */ 
	std::vector<GNode> nodes;
	
	/* V8 binding functions */
	static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void add_node(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void add_edge(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void update_edge(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void print(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void trim(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void bellmanford(const v8::FunctionCallbackInfo<v8::Value>& args);

	static v8::Global<v8::Function> constructor;
	
	/* Functions */ 
	void add_edge( GNode *& origin, GNode *& destination, double weight );
	bool bellman_ford( GNode *& origin, std::vector<std::vector<std::string> > & );
	GNode * get_node( std::string & name )
	{
		for( unsigned long int i = 0; i < nodes.size(); ++i ){
			if( name == nodes.at(i).name ){
				return &nodes.at(i);
			}
		}
		// this should be fixed to an empty node
		return &nodes.at(0);
	}	 
};

#endif
