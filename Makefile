all: ex_tensorflow.so

ex_tensorflow.so: src/ex_tensorflow.c
	clang -O3 -fPIC -I$(ERL_INCLUDE_PATH) -dynamiclib -undefined dynamic_lookup -o ex_tensorflow.so -ltensorflow src/ex_tensorflow.c

clean:
	rm ex_tensorflow.so
