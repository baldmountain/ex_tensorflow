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

  # def constant(graph, name, data) do
  #   t = ExTensorflow.create_tensor value
  #   g
  #   |> ExTensorflow.opBuilder("Const", name)
  #   |> ExTensorflow.setAttr("dtype", ExTensorflow.dataType(t))
  #   |> ExTensorflow.setAttr("value", t)
  #   |> ExTensorflow.build()
  #   |> ExTensorflow.output(0)
  # end

  # def normalize_image(image) do
  #   g = ExTensorflow.new_graph
  #   input (constant g "input" image-bytes)
  #   output (div g
  #             (sub g
  #               (resizeBilinear g
  #                 (expandDims g
  #                   (tf-cast g (decodeJpeg g input, 3) DataType/FLOAT)
  #                   (constant g "make_batch" (int 0)))
  #                 (constant g "size" (int-array 2 (seq [H W]))))
  #               (constant g "mean" mean))
  #             (constant g "scale" scale))
  #   s (Session. g)]
  #   (-> s (.runner) (.fetch (.name (.op output))) (.run) (.get 0))))

  # end


  def classify(file_name) do
    {:ok, graph_def} = File.read("./model/tensorflow_inception_graph.pb")
    {:ok, labels} = File.read("./model/imagenet_comp_graph_label_strings.txt")
    {:ok, image_bytes} = File.read(file_name)
    # {:ok, image} = normalize_image(image_bytes)
  end
end