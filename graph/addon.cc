#ifndef GRAPH_ADDON_H
#define GRAPH_ADDON_H

#include <node.h>
#include "directed_graph.h"

using namespace v8;

void InitAll(Local<Object> exports) {
  Graph::Init(exports);
}

NODE_MODULE(graph, InitAll)

#endif