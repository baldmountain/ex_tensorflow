defmodule ExTensorflow.Code do
  def ok, do: 0
  def cancelled, do: 1
  def unknown, do: 2
  def invalid_argument, do: 3
  def deadline_exceeded, do: 4
  def not_found, do: 5
  def already_exists, do: 6
  def permission_denied, do: 7
  def unauthenticated, do: 16
  def resource_exhausted, do: 8
  def failed_precondition, do: 9
  def aborted, do: 10
  def out_of_range, do: 11
  def unimplemented, do: 12
  def internal, do: 13
  def unavailable, do: 14
  def data_loss, do: 15
end