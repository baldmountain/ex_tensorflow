MIX = mix
CFLAGS = -g -O3 -Wall

ERLANG_PATH = $(shell erl -eval 'io:format("~s", [lists:concat([code:root_dir(), "/erts-", erlang:system_info(version), "/include"])])' -s init stop -noshell)
CFLAGS += -I$(ERLANG_PATH)

ifneq ($(OS),Windows_NT)
	CFLAGS += -fPIC
	LDFLAGS += -ltensorflow

	ifeq ($(shell uname),Darwin)
		LDFLAGS += -dynamiclib -undefined dynamic_lookup
	endif
endif

.PHONY: all tensorflow clean

all: tensorflow

tensorflow:
	$(MIX) compile

ex_tensorflow.so: src/ex_tensorflow.c
	$(CC) $(CFLAGS) -shared $(LDFLAGS) -o $@ src/ex_tensorflow.c

clean:
	$(MIX) clean
	rm -f ex_tensorflow.so
