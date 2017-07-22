#include "erl_nif.h"
#include <tensorflow/c/c_api.h>
#include <string.h>
#include <stdio.h>

ErlNifResourceType *STATUS_RES_TYPE;
ErlNifResourceType *BUFFER_RES_TYPE;

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

int
load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info) {
  int flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;
  STATUS_RES_TYPE = enif_open_resource_type(env, NULL, "status", status_res_destructor, flags, NULL);
  BUFFER_RES_TYPE = enif_open_resource_type(env, NULL, "buffer", buffer_res_destructor, flags, NULL);
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

  // Let's create conn and copy the memory where the pointer is stored
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
  unsigned len = 0;
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

  // Let's create conn and copy the memory where the pointer is stored
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

  // Let's create conn and copy the memory where the pointer is stored
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
  {"string_from_buffer", 1, string_from_buffer}
};

ERL_NIF_INIT(Elixir.ExTensorflow, nif_funcs, load, NULL, NULL, NULL)