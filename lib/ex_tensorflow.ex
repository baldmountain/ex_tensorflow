defmodule ExTensorflow do
  @moduledoc """
  Documentation for ExTensorflow.
  """

  @doc """
  Hello world.

  ## Examples

      iex> ExTensorflow.version
      "1.3.0"

  """
  @on_load :load_nifs

  @type status :: any
  @type buffer :: any
  @type graph :: any
  @type tensor :: any
  @type op_description :: any
  @type operation :: any
  @type output :: any

  def load_nifs do
    :erlang.load_nif('./ex_tensorflow', 0)
  end

  @spec version() :: binary
  def version() do
    raise "NIF version/0 not implemented"
  end

  @spec data_type_size(non_neg_integer) :: non_neg_integer
  def data_type_size(_type) do
    raise "NIF data_type_size/1 not implemented"
  end

  @spec new_status() :: status
  def new_status() do
    raise "NIF new_status/0 not implemented"
  end

  @spec set_status(status, integer, binary) :: binary
  def set_status(_status, _code, _msg) do
    raise "NIF set_status/3 not implemented"
  end

  @spec get_code(status) :: binary
  def get_code(_status) do
    raise "NIF get_code/1 not implemented"
  end

  @spec message(status) :: binary
  def message(_status) do
    raise "NIF message/1 not implemented"
  end

  @spec new_buffer() :: buffer
  def new_buffer() do
    raise "NIF new_buffer/0 not implemented"
  end

  @spec new_buffer_from_string(binary) :: buffer
  def new_buffer_from_string(_binary) do
    raise "NIF new_buffer_from_string/1 not implemented"
  end

  @spec string_from_buffer(buffer) :: binary
  def string_from_buffer(_buffer) do
    raise "NIF string_from_buffer/1 not implemented"
  end

  @spec new_graph() :: graph
  def new_graph() do
    raise "NIF new_graph/0 not implemented"
  end

  # The simplest tensor. It's a single dimenstion tensor with all values uint8
  @spec create_tensor(binary) :: tensor
  def create_tensor(_binary) do
    raise "NIF create_tensor/1 not implemented"
  end

  @spec tensor_type(binary) :: non_neg_integer
  def tensor_type(tensor) do
    raise "NIF tensor_type/1 not implemented"
  end


  @spec new_operation(graph, binary, binary) :: op_description
  def new_operation(_graph, _type, _name) do
    raise "NIF new_operation/3 not implemented"
  end

  @spec set_attr_int(op_description, binary, integer) :: op_description
  def set_attr_int(_op_description, _name, _value) do
    raise "NIF set_attr_int/3 not implemented"
  end

  @spec set_attr_type(op_description, binary, non_neg_integer) :: op_description
  def set_attr_type(_op_description, _name, _type) do
    raise "NIF set_attr_type/3 not implemented"
  end

  @spec set_attr_tensor(op_description, binary, tensor) :: {:ok, op_description} | {:error, binary}
  def set_attr_tensor(_op_description, _name, _tensor) do
    raise "NIF set_attr_int/3 not implemented"
  end

  @spec add_input(op_description, operation) :: op_description
  def add_input(_op_description, _output) do
    raise "NIF add_input/2 not implemented"
  end

  @spec finish_operation(op_description) :: {:ok, operation} | {:error, binary}
  def finish_operation(_op_description) do
    raise "NIF finish_operation/1 not implemented"
  end
end
