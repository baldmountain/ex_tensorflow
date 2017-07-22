all: ex_tensorflow.so

ex_tensorflow.so: src/ex_tensorflow.c
	gcc -O3 -fPIC -I$(ERL_INCLUDE_PATH) -shared -Wl,-soname,ex_tensorflow.s0.1 -o ex_tensorflow.so -ltensorflow src/ex_tensorflow.c

.PHONY: clean
clean:
	rm ex_tensorflow.so
