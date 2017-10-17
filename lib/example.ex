defmodule Example do

  # Some constants specific to the pre-trained model at:
  # https://storage.googleapis.com/download.tensorflow.org/models/inception5h.zip
  #
  # - The model was trained with images scaled to 224x224 pixels.
  # - The colors, represented as R, G, B in 1-byte each were converted to
  #   float using (value - Mean)/Scale.
  @w 224
  @h 224
  @mean 117.0
  @scale 1.0

  def binaryOp(graph, type, in1, in2) do
    {:ok, operation} = graph
      |> ExTensorflow.new_operation(type, type)
      |> ExTensorflow.add_input(in1)
      |> ExTensorflow.add_input(in2)
      |> ExTensorflow.finish_operation()
    operation
  end

  def div(g, x, y) do
    binaryOp(g, "Div", x, y)
  end

  def sub(g, x, y) do
    binaryOp(g, "Sub", x, y)
  end

  def resizeBilinear(g, images, size) do
    binaryOp(g, "ResizeBilinear", images, size)
  end

  def expandDims(g, input, dim) do
    binaryOp(g, "ExpandDims", input, dim)
  end

  def tf_cast(g, value, dtype) do
    {:ok, operation} = g
      |> ExTensorflow.new_operation("Cast", "Cast")
      |> ExTensorflow.add_input(value)
      |> ExTensorflow.set_attr_type("DstT", dtype)
      |> ExTensorflow.finish_operation()
    operation
  end

  def decodeJpeg(g, contents, channels) do
    {:ok, operation} = g
      |> ExTensorflow.new_operation("DecodeJpeg", "DecodeJpeg")
      |> ExTensorflow.add_input(contents)
      |> ExTensorflow.set_attr_int("channels", channels)
      |> ExTensorflow.finish_operation()
    operation
  end

  def constant(graph, name, value) do
    t = ExTensorflow.create_tensor(value)
    {:ok, operation} = graph
      |> ExTensorflow.new_operation("Const", name)
      |> ExTensorflow.set_attr_type("dtype", ExTensorflow.tensor_type(t))
      |> ExTensorflow.set_attr_tensor("value", t)
      |> ExTensorflow.finish_operation()
    operation
  end

  def normalize_image(image_bytes) do
    graph = ExTensorflow.new_graph
    input = constant(graph, "input", image_bytes)
    image = tf_cast(graph, decodeJpeg(graph, input, 3), ExTensorflow.DataType.float)
    dim = constant(graph, "make_batch", 0)
    dims = expandDims(graph, image, dim)
    size = constant(graph, "size", [@h, @w])
    mean = constant(graph, "mean", @mean)
    sub = sub(graph, dims, mean)
    scale = constant(graph, "scale", @scale)
    output = div(graph, sub, scale)
    {:ok, session} = ExTensorflow.new_session(graph)
    {:ok, session} = ExTensorflow.run_session(session, output)
  end


  def classify(file_name) do
    {:ok, graph_def} = File.read("./model/tensorflow_inception_graph.pb")
    {:ok, labels} = File.read("./model/imagenet_comp_graph_label_strings.txt")
    {:ok, image_bytes} = File.read(file_name)
    output = normalize_image(image_bytes)
  end
end