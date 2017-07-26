# ExTensorflow

**Tensorflow wrapper for elixir**

There is just enough here to get something running. It's assumed you have Elixir installed. Follow
the instructions here: https://elixir-lang.org/install.html

First install the tensorflow C Api: https://www.tensorflow.org/install/install_c

Most of the work here is based on this blog post:

Excellent nif example code

http://andrealeopardi.com/posts/using-c-from-elixir-with-nifs/

You need to set ERL\_INCLUDE\_PATH MacOS:

```bash
export ERL_INCLUDE_PATH=/usr/local/Cellar/erlang/20.0/lib/erlang/usr/include
```

Linux

```bash
export ERL_INCLUDE_PATH=/usr/lib/erlang/usr/include
```

Run

```bash
make ex_tensorflow.so
```

To make the NIF shared library that elixir will load.

Run tests to make sure things work:

```bash
LD_PRELOAD=/usr/local/lib/libtensorflow.so mix test
```

You need the LD_PRELOAD to make sure the tensorflow shared library is loaded.

Load iex:

```bash
LD_PRELOAD=/usr/local/lib/libtensorflow.so iex -S mix
```

example.ex has an exmple to run a classification using the inception5 model. You need to run

```bash
curl -o inception5h.zip https://storage.googleapis.com/download.tensorflow.org/models/inception5h.zip
mkdir model
cd model
unzip ../inception5h.zip
```

to setup a model. You'll need a test image. I've been using:

```bash
curl -o parrot.jpg http://weknowyourdreams.com/image.php?pic=/images/parrot/parrot-06.jpg
```

but any jpeg image will do. The from the iex prompt you can run:

```bash
Example.classify("parrot.jpg")
```

to run the model. (Doesn't work yet. )
