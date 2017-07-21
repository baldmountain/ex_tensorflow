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

  def load_nifs do
    :erlang.load_nif('./ex_tensorflow', 0)
  end

  def version() do
    raise "NIF version/0 not implemented"
  end

  @spec data_type_size(non_neg_integer) :: non_neg_integer
  def data_type_size(_type) do
    raise "NIF data_type_size/1 not implemented"
  end

  def new_status() do
    raise "NIF new_status/0 not implemented"
  end

  def set_status(_status, _code, _msg) do
    raise "NIF set_status/3 not implemented"
  end

  def get_code(_status) do
    raise "NIF get_code/1 not implemented"
  end
end
