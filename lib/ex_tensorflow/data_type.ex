defmodule ExTensorflow.DataType do
  def float, do: 1
  def double, do: 2
  def int32, do: 3  # Int32 tensors are always in 'host' memory.
  def utin8, do: 4
  def int16, do: 5
  def int8, do: 6
  def string, do: 7
  def complex64, do: 8  # Single-precision complex
  def complex, do: 8    # Old identifier kept for API backwards compatibility
  def int64, do: 9
  def bool, do: 10
  def qint8, do: 11     # Quantized int8
  def quint8, do: 12    # Quantized uint8
  def qint32, do: 13    # Quantized int32
  def bfloat16, do: 14  # Float32 truncated to 16 bits.  Only for cast ops.
  def qint16, do: 15    # Quantized int16
  def quint16, do: 16   # Quantized uint16
  def uint16, do: 17
  def complex128, do: 18  # Double-precision complex
  def half, do: 19
  def resource, do: 20
end