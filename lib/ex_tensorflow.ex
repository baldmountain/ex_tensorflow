defmodule ExTensorflow do
  @moduledoc """
  Documentation for ExTensorflow.
  """

  @doc """
  Hello world.

  ## Examples

      iex> ExTensorflow.version
      "1.2.1"

  """
  @on_load :load_nifs

  @type status :: any
  @type buffer :: any

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


end
