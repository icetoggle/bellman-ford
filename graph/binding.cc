
#include "directed_graph.h"

using namespace v8;

void Graph::Init(Local<Object> exports) {
  // Prepare constructor template
  Isolate* isolate = exports->GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  Local<ObjectTemplate> addon_data_tpl = ObjectTemplate::New(isolate);
  addon_data_tpl->SetInternalFieldCount(1);
  Local<Object> addon_data = addon_data_tpl->NewInstance(context).ToLocalChecked();

  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New, addon_data); // this calls the New member function
  tpl->SetClassName(String::NewFromUtf8(isolate, "Graph").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "add_node", add_node);
  // tpl->PrototypeTemplate()->Set(String::NewSymbol("add_node"),
  //     FunctionTemplate::New(add_node)->GetFunction());
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "add_edge", add_edge);
  // tpl->PrototypeTemplate()->Set(String::NewSymbol("add_edge"),
  // 	  FunctionTemplate::New(add_edge)->GetFunction());

  NODE_SET_PROTOTYPE_METHOD(tpl, "update_edge", update_edge);
  // tpl->PrototypeTemplate()->Set(String::NewSymbol("update_edge"),
  //     FunctionTemplate::New(update_edge)->GetFunction());

  NODE_SET_PROTOTYPE_METHOD(tpl, "print", print);
  // tpl->PrototypeTemplate()->Set(String::NewSymbol("print"),
  //     FunctionTemplate::New(print)->GetFunction());

  NODE_SET_PROTOTYPE_METHOD(tpl, "trim", trim);
  // tpl->PrototypeTemplate()->Set(String::NewSymbol("trim"),
  //     FunctionTemplate::New(trim)->GetFunction());

  NODE_SET_PROTOTYPE_METHOD(tpl, "bellmanford", bellmanford);
  // tpl->PrototypeTemplate()->Set(String::NewSymbol("bellmanford"),
  //     FunctionTemplate::New(bellmanford)->GetFunction());

  Local<Function> constructor = tpl->GetFunction(context).ToLocalChecked();
  addon_data->SetInternalField(0, constructor);
  exports->Set(context, String::NewFromUtf8(isolate, "Graph").ToLocalChecked(), constructor).FromJust();
  // Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
  // target->Set(String::NewSymbol("Graph"), constructor);
}
// this is the "New member function"
void Graph::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  if(args.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    Graph* obj = new Graph();
    obj->Wrap(args.Holder());
    args.GetReturnValue().Set(args.Holder());
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 0;
    Local<Value> argv[0] = { };
    Local<Function> cons =
        args.Data().As<Object>()->GetInternalField(0).As<Function>();
    Local<Object> result =
        cons->NewInstance(context, argc, argv).ToLocalChecked();
    args.GetReturnValue().Set(result);
  }
}

void Graph::add_node(const v8::FunctionCallbackInfo<v8::Value>& args)
{
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 1 ) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "add_node: wrong number of arguments").ToLocalChecked()));
    return;
  }

  Graph* graph = ObjectWrap::Unwrap<Graph>(args.Holder());
  // Local<Context> context = isolate->GetCurrentContext();
  v8::String::Utf8Value param1(isolate, args[0]);
  std::string name(*param1);

  graph->add_node( name );
  args.GetReturnValue().Set(Boolean::New(isolate, true));
}

void Graph::add_edge(const v8::FunctionCallbackInfo<v8::Value>& args)
{
  Isolate* isolate = args.GetIsolate();

  Graph* graph = ObjectWrap::Unwrap<Graph>(args.Holder());
  Local<Context> context = isolate->GetCurrentContext();

  if( args.Length() == 3 ){
    v8::String::Utf8Value param1(isolate, args[0]);
    v8::String::Utf8Value param2(isolate, args[1]);

    std::string from = std::string(*param1);
    std::string to = std::string(*param2);
    double weight = args[2]->NumberValue(context).FromJust();
    graph->add_edge( from, to, weight );  
    args.GetReturnValue().Set(Boolean::New(isolate, true));

  } else {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "add_edge: wrong number of arguments").ToLocalChecked()));
  }
}

void Graph::update_edge(const v8::FunctionCallbackInfo<v8::Value>& args)
{
  Isolate* isolate = args.GetIsolate();

  Graph* graph = ObjectWrap::Unwrap<Graph>(args.Holder());
  Local<Context> context = isolate->GetCurrentContext();
  if( args.Length() == 3 ){
    v8::String::Utf8Value param1(isolate, args[0]);
    v8::String::Utf8Value param2(isolate, args[1]);

    std::string from = std::string(*param1);
    std::string to = std::string(*param2);
    double weight = args[2]->NumberValue(context).FromJust();

    graph->update_edge( from, to, weight );
    args.GetReturnValue().Set(Boolean::New(isolate, true));

  } else {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "update_edge: wrong number of arguments").ToLocalChecked()));
  }
}

void Graph::print(const v8::FunctionCallbackInfo<v8::Value>& args)
{
  Isolate* isolate = args.GetIsolate();

  Graph* graph = ObjectWrap::Unwrap<Graph>(args.Holder());
  // Local<Context> context = isolate->GetCurrentContext();
  graph->print();
  args.GetReturnValue().Set(Boolean::New(isolate, true));
}

void Graph::trim(const v8::FunctionCallbackInfo<v8::Value>& args)
{
  Isolate* isolate = args.GetIsolate();

  Graph* graph = ObjectWrap::Unwrap<Graph>(args.Holder());
  // Local<Context> context = isolate->GetCurrentContext();
  graph->trim();
  args.GetReturnValue().Set(Boolean::New(isolate, true));
}

void Graph::bellmanford(const v8::FunctionCallbackInfo<v8::Value>& args)
{
  Isolate* isolate = args.GetIsolate();

  Graph* graph = ObjectWrap::Unwrap<Graph>(args.Holder());
  Local<Context> context = isolate->GetCurrentContext();

  if( args.Length() != 1 ) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "bellmanford(source_node): wrong number of arguments").ToLocalChecked()));
    return;
  }
  v8::String::Utf8Value param1(isolate, args[0]);
  std::string source = std::string(*param1);
  std::vector<std::vector<std::string> > path;
  graph->bellman_ford(source, path);

  Local<Array> result = Array::New(isolate, path.size());
  
  for(unsigned long int i = 0; i < path.size(); ++i){
    Local<Array> keyvalue = Array::New(isolate, path[i].size());
    for(unsigned long int j = 0; j < path[i].size(); ++j){
      keyvalue->Set(context, j, String::NewFromUtf8(isolate, path[i][j].c_str()).ToLocalChecked());
    }
    result->Set(context, i, keyvalue);
  }
  args.GetReturnValue().Set(result);
}

// #endif
