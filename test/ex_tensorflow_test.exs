defmodule ExTensorflowTest do
  use ExUnit.Case
  doctest ExTensorflow

  alias ExTensorflow.DataType
  alias ExTensorflow.Code

  test "version" do
    assert ExTensorflow.version == "1.2.1"
  end

  test "data_type_size" do
    assert ExTensorflow.data_type_size(DataType.float) == 4
    assert ExTensorflow.data_type_size(DataType.resource) == 0
    assert ExTensorflow.data_type_size(DataType.string) == 0
    assert ExTensorflow.data_type_size(DataType.int64) == 8
  end

  test "Status" do
    status = ExTensorflow.new_status
    assert ExTensorflow.get_code(status) == Code.ok
    status = ExTensorflow.set_status(status, Code.invalid_argument, "bad arg")
    assert ExTensorflow.get_code(status) == Code.invalid_argument
  end
end
