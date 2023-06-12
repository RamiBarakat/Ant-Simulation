G = gcc -g
O = -o
names = parent sender senderChild receiver helper master_spy spy opengl

files:
	$(G) parent.c $(O) parent
	$(G) sender.c $(O) sender
	$(G) senderChild.c $(O) senderChild
	$(G) receiver.c $(O) receiver
	$(G) helper.c $(O) helper
	$(G) spy.c $(O) spy
	$(G) master_spy.c $(O) master_spy
	gcc -o opengl opengl.c -lGL -lGLU -lglut -lm

clean:
	rm -f $(names)
