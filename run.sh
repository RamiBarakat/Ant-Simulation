#!/bin/sh

# This file only to run the project quickly for testing not for final production
# sed -i -e 's/\r$//' run.sh
#run chmod 777 run.sh to make it executable

rm -f parent
rm -f sender
rm -f senderChild
rm -f receiver
rm -f helper
rm -f spy
rm -f master_spy

# gcc -o opengl opengl.c -lGL -lGLU -lglut -lm
gcc -g spy.c -o spy
gcc -g master_spy.c -o master_spy
gcc -g receiver.c -o receiver
gcc -g helper.c -o helper
gcc -g senderChild.c -o senderChild
gcc -g sender.c -o sender
gcc -g parent.c -o parent

./parent $1 $2
