#include "erl_nif.h"
#include <tensorflow/c/c_api.h>
#include <string.h>
#include <stdio.h>

ErlNifResourceType *STATUS_RES_TYPE;
ErlNifResourceType *BUFFER_RES_TYPE;
ErlNifResourceType *GRAPH_RES_TYPE;
ErlNifResourceType *TENSOR_RES_TYPE;
ErlNifResourceType *OP_DESC_RES_TYPE;
ErlNifResourceType *OPERATION_RES_TYPE;

// This is called everytime a resource is deallocated (which happens when
// enif_release_resource is called and Erlang garbage collects the memory)
void
status_res_destructor(ErlNifEnv *env, void *res) {
  TF_DeleteStatus(*(TF_Status**)res);
}

void
buffer_res_destructor(ErlNifEnv *env, void *res) {
  TF_DeleteBuffer(*(TF_Buffer **)res);
}

void
graph_res_destructor(ErlNifEnv *env, void *res) {
  TF_DeleteGraph(*(TF_Graph **)res);
}

void
tensor_res_destructor(ErlNifEnv *env, void *res) {
  TF_DeleteTensor(*(TF_Tensor **)res);
}

void tensor_deallocator(void* data, size_t len, void* arg) {
  enif_free(data);
}

void
op_desc_res_destructor(ErlNifEnv *env, void *res) {
  // do nothing since TF_FinishOperation will delete the op_desc
}

void
operation_res_destructor(ErlNifEnv *env, void *res) {
  // do nothing since TF_DeleteGraph will delete the operation
}

int
load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info) {
  int flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;
  STATUS_RES_TYPE = enif_open_resource_type(env, NULL, "status", status_res_destructor, flags, NULL);
  BUFFER_RES_TYPE = enif_open_resource_type(env, NULL, "buffer", buffer_res_destructor, flags, NULL);
  GRAPH_RES_TYPE = enif_open_resource_type(env, NULL, "graph", graph_res_destructor, flags, NULL);
  TENSOR_RES_TYPE = enif_open_resource_type(env, NULL, "tensor", tensor_res_destructor, flags, NULL);
  OP_DESC_RES_TYPE = enif_open_resource_type(env, NULL, "op_desc", op_desc_res_destructor, flags, NULL);
  OPERATION_RES_TYPE = enif_open_resource_type(env, NULL, "operation", operation_res_destructor, flags, NULL);
  return 0;
}

ERL_NIF_TERM
make_binary_from_string(ErlNifEnv *env, const char *s) {
  ErlNifBinary output;
  size_t len = strlen(s);

  enif_alloc_binary(len, &output);
  memcpy(output.data, s, len);

  return enif_make_binary(env, &output);
}

static ERL_NIF_TERM
version(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  return make_binary_from_string(env, TF_Version());
}

static ERL_NIF_TERM
data_type_size(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  int type;
  enif_get_int(env, argv[0], &type);
  size_t size = TF_DataTypeSize(type);
  return enif_make_int(env, size);
}

static ERL_NIF_TERM
new_status(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  // Let's allocate the memory for a TF_Status * pointer
  TF_Status **status_res = enif_alloc_resource(STATUS_RES_TYPE, sizeof(TF_Status *));

  // Let's create status and copy the memory where the pointer is stored
  TF_Status *status = TF_NewStatus();
  memcpy((void *) status_res, (void *) &status, sizeof(TF_Status *));

  // We can now make the Erlang term that holds the resource...
  ERL_NIF_TERM term = enif_make_resource(env, status_res);

  // ...and release the resource so that it will be freed when Erlang garbage collects
  enif_release_resource(status_res);

  return term;
}

static ERL_NIF_TERM
set_status(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  int code;
  char *msg;
  TF_Status **status;
  ErlNifBinary s;

  enif_get_resource(env, argv[0], STATUS_RES_TYPE, (void *) &status);

  enif_get_int(env, argv[1], &code);

  enif_inspect_binary(env, argv[2], &s);
  msg = malloc(s.size + 1);
  msg[s.size] = 0;
  memcpy(msg, s.data, s.size);

  TF_SetStatus(*status, code, msg);
  return argv[0];
}

static ERL_NIF_TERM
get_code(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  TF_Status **status;
  enif_get_resource(env, argv[0], STATUS_RES_TYPE, (void *) &status);

  TF_Code code = TF_GetCode(*status);

  return enif_make_int(env, code);
}

static ERL_NIF_TERM
message(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  TF_Status **status;
  enif_get_resource(env, argv[0], STATUS_RES_TYPE, (void *) &status);

  const char *message = TF_Message(*status);

  return make_binary_from_string(env, message);
}

static ERL_NIF_TERM
new_buffer(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  // Let's allocate the memory for a TF_Buffer * pointer
  TF_Buffer **buffer_res = enif_alloc_resource(BUFFER_RES_TYPE, sizeof(TF_Buffer *));

  // Let's create buffer and copy the memory where the pointer is stored
  TF_Buffer *buffer = TF_NewBuffer();
  memcpy((void *) buffer_res, (void *) &buffer, sizeof(TF_Buffer *));

  // We can now make the Erlang term that holds the resource...
  ERL_NIF_TERM term = enif_make_resource(env, buffer_res);

  // ...and release the resource so that it will be freed when Erlang garbage collects
  enif_release_resource(buffer_res);

  return term;
}

static ERL_NIF_TERM
new_buffer_from_string(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  ErlNifBinary s;
  // Let's allocate the memory for a TF_Buffer * pointer
  TF_Buffer **buffer_res = enif_alloc_resource(BUFFER_RES_TYPE, sizeof(TF_Buffer *));
  // get string parameter
  enif_inspect_binary(env, argv[0], &s);

  // Let's create buffer and copy the memory where the pointer is stored
  TF_Buffer *buffer = TF_NewBufferFromString(s.data, s.size);
  memcpy((void *) buffer_res, (void *) &buffer, sizeof(TF_Buffer *));

  // We can now make the Erlang term that holds the resource...
  ERL_NIF_TERM term = enif_make_resource(env, buffer_res);

  // ...and release the resource so that it will be freed when Erlang garbage collects
  enif_release_resource(buffer_res);

  return term;
}

static ERL_NIF_TERM
string_from_buffer(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  TF_Buffer **buffer;
  enif_get_resource(env, argv[0], BUFFER_RES_TYPE, (void *) &buffer);

  ErlNifBinary output;

  enif_alloc_binary((*buffer)->length, &output);
  memcpy(output.data, (*buffer)->data, (*buffer)->length);

  return enif_make_binary(env, &output);
}

static ERL_NIF_TERM
new_graph(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  // Let's allocate the memory for a TF_Graph * pointer
  TF_Graph **graph_res = enif_alloc_resource(GRAPH_RES_TYPE, sizeof(TF_Graph *));

  // Let's create graph and copy the memory where the pointer is stored
  TF_Graph *graph = TF_NewGraph();
  memcpy((void *) graph_res, (void *) &graph, sizeof(TF_Graph *));

  // We can now make the Erlang term that holds the resource...
  ERL_NIF_TERM term = enif_make_resource(env, graph_res);

  // ...and release the resource so that it will be freed when Erlang garbage collects
  enif_release_resource(graph_res);

  return term;
}

/*
 * There is a big assumption here. We will need a lot of different kinds of tensor create functions.
 * This one takes a binary and assumes it is all uint8 with the size of the binary.
 */
static ERL_NIF_TERM
create_tensor(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  ErlNifBinary s;
  int64_t dims;

  // Let's allocate the memory for a TF_Tensor * pointer
  TF_Tensor **tensor_res = enif_alloc_resource(TENSOR_RES_TYPE, sizeof(TF_Tensor *));
  // get string parameter
  enif_inspect_binary(env, argv[0], &s);

  dims = s.size;

  // allocate since the input binary will be collected.
  void *ptr = enif_alloc(s.size+1);
  memset(ptr, 0, s.size+1);
  memcpy(ptr, (void *) s.data, s.size);

  // Let's create tensor and copy the memory where the pointer is stored
  TF_Tensor *tensor = TF_NewTensor(TF_UINT8, &dims, 1, ptr, s.size, tensor_deallocator, 0);
  memcpy((void *) tensor_res, (void *) &tensor, sizeof(TF_Tensor *));

  // We can now make the Erlang term that holds the resource...
  ERL_NIF_TERM term = enif_make_resource(env, tensor_res);

  // ...and release the resource so that it will be freed when Erlang garbage collects
  enif_release_resource(tensor_res);

  return term;
}

static ERL_NIF_TERM
tensor_type(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  TF_Tensor **tensor;
  enif_get_resource(env, argv[0], TENSOR_RES_TYPE, (void *) &tensor);
  TF_DataType type =  TF_TensorType(*tensor);
  return enif_make_int(env, type);
}

static ERL_NIF_TERM
new_operation(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  ErlNifBinary s;
  char* op_type;
  char* op_name;
  TF_Graph **graph;
  enif_get_resource(env, argv[0], GRAPH_RES_TYPE, (void *) &graph);

  // get string parameter
  enif_inspect_binary(env, argv[1], &s);
  op_type = enif_alloc(s.size+1);
  memset(op_type, 0, s.size+1);
  memcpy(op_type, (void *) s.data, s.size);

  enif_inspect_binary(env, argv[2], &s);
  op_name = enif_alloc(s.size+1);
  memset(op_name, 0, s.size+1);
  memcpy(op_name, (void *) s.data, s.size);

  // Let's allocate the memory for a TF_OperationDescription * pointer
  TF_OperationDescription **op_desc_res = enif_alloc_resource(OP_DESC_RES_TYPE, sizeof(TF_OperationDescription *));

  // Let's create op_desc and copy the memory where the pointer is stored
  TF_OperationDescription *op_desc = TF_NewOperation(*graph, op_type, op_name);
  memcpy((void *)op_desc_res, (void *) &op_desc, sizeof(TF_OperationDescription *));
  fprintf(stderr, "graph1> %p\n", op_desc);

  // We can now make the Erlang term that holds the resource...
  ERL_NIF_TERM term = enif_make_resource(env, op_desc_res);

  // ...and release the resource so that it will be freed when Erlang garbage collects
  enif_release_resource(op_desc_res);

  // enif_free(op_type);
  // enif_free(op_name);

  return term;
}

static ERL_NIF_TERM
set_attr_int(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  ErlNifBinary s;
  TF_OperationDescription **op_desc;
  enif_get_resource(env, argv[0], OP_DESC_RES_TYPE, (void *) &op_desc);
  fprintf(stderr, "graph2> %p\n", *op_desc);

  // get string parameter
  enif_inspect_binary(env, argv[1], &s);
  char* name = enif_alloc(s.size+1);
  memset(name, 0, s.size+1);
  memcpy(name, (void *) s.data, s.size);

  int value;
  enif_get_int(env, argv[2], &value);

  TF_SetAttrInt(*op_desc, name, value);
  enif_free(name);
  return argv[0];
}

static ERL_NIF_TERM
set_attr_type(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  ErlNifBinary s;
  TF_OperationDescription **op_desc;
  enif_get_resource(env, argv[0], OP_DESC_RES_TYPE, (void *) &op_desc);

  // get string parameter
  enif_inspect_binary(env, argv[1], &s);
  char* name = enif_alloc(s.size+1);
  memset(name, 0, s.size+1);
  memcpy(name, (void *) s.data, s.size);

  int value;
  enif_get_int(env, argv[2], &value);

  TF_SetAttrType(*op_desc, name, value);
  enif_free(name);
  return argv[0];
}

static ERL_NIF_TERM
set_attr_tensor(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  TF_OperationDescription **op_desc;
  enif_get_resource(env, argv[0], OP_DESC_RES_TYPE, (void *) &op_desc);

  // get string parameter
  ErlNifBinary s;
  enif_inspect_binary(env, argv[1], &s);
  char* name = enif_alloc(s.size);
  memset(name, 0, s.size+1);
  memcpy(name, (void *) s.data, s.size);

  TF_Tensor **tensor;
  enif_get_resource(env, argv[2], TENSOR_RES_TYPE, (void *) &tensor);

  TF_Status *status = TF_NewStatus();

  TF_SetAttrTensor(*op_desc, name, *tensor, status);

  enif_free(name);

  int code = TF_GetCode(status);
  if (code == TF_OK) {
    TF_DeleteStatus(status);
    return enif_make_tuple(env, 2, enif_make_atom(env, "ok"), argv[0]);
  }
  const char *msg = TF_Message(status);
  TF_DeleteStatus(status);
  return enif_make_tuple(env, 2, enif_make_atom(env, "error"), make_binary_from_string(env, msg));
}

static ERL_NIF_TERM
add_input(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  TF_OperationDescription **op_desc = 0;
  enif_get_resource(env, argv[0], OP_DESC_RES_TYPE, (void *) &op_desc);

  TF_Operation **operation = 0;
  enif_get_resource(env, argv[1], OPERATION_RES_TYPE, (void *) &operation);
  TF_Input input = {
    *operation,
    0
  };
  TF_AddInput(*op_desc, TF_OperationInput(input));
  return argv[0];
}

static ERL_NIF_TERM
finish_operation(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  TF_OperationDescription **op_desc;
  enif_get_resource(env, argv[0], OP_DESC_RES_TYPE, (void *) &op_desc);
  fprintf(stderr, "graph3> %p\n", *op_desc);

  TF_Status *status = TF_NewStatus();

  TF_Operation *operation = TF_FinishOperation(*op_desc, status);
  fprintf(stderr, "operation> %p\n", operation);

  int code = TF_GetCode(status);
  if (code == TF_OK) {
    TF_DeleteStatus(status);
    TF_Operation **operation_res = enif_alloc_resource(OPERATION_RES_TYPE, sizeof(TF_Operation *));

    // Let's create operation and copy the memory where the pointer is stored
    memcpy((void *) operation_res, (void *) &operation, sizeof(TF_Operation *));

    // We can now make the Erlang term that holds the resource...
    ERL_NIF_TERM term = enif_make_resource(env, operation_res);

    // ...and release the resource so that it will be freed when Erlang garbage collects
    enif_release_resource(operation_res);

    return enif_make_tuple(env, 2, enif_make_atom(env, "ok"), term);
  }
  const char *msg = TF_Message(status);
  ERL_NIF_TERM error_message = make_binary_from_string(env, msg);
  TF_DeleteStatus(status);
  return enif_make_tuple(env, 2, enif_make_atom(env, "error"), error_message);
}

// Let's define the array of ErlNifFunc beforehand:
static ErlNifFunc nif_funcs[] = {
  // {erl_function_name, erl_function_arity, c_function}
  {"version", 0, version},
  {"data_type_size", 1, data_type_size},
  {"new_status", 0, new_status},
  {"set_status", 3, set_status},
  {"get_code", 1, get_code},
  {"message", 1, message},
  {"new_buffer", 0, new_buffer},
  {"new_buffer_from_string", 1, new_buffer_from_string},
  {"string_from_buffer", 1, string_from_buffer},
  {"new_graph", 0, new_graph},
  {"create_tensor", 1, create_tensor},
  {"tensor_type", 1, tensor_type},
  {"new_operation", 3, new_operation},
  {"set_attr_int", 3, set_attr_int},
  {"set_attr_type", 3, set_attr_type},
  {"set_attr_tensor", 3, set_attr_tensor},
  {"add_input", 2, add_input},
  {"finish_operation", 1, finish_operation},
};

ERL_NIF_INIT(Elixir.ExTensorflow, nif_funcs, load, NULL, NULL, NULL)