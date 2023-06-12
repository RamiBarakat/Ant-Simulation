G = gcc -g
O = -o
names = main timer
thread_args = -lpthread -lrt -lglut -lGLU -lGL -lm

files:
	$(G) main.c $(O) main $(thread_args)

clean:
	rm -f $(names)