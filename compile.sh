if ! [ -e ./code.bf ]; then
    touch code.bf
fi
gcc -Wall -c -pthread ./queue.c
gcc -Wall -c -pthread ./stack.c
gcc -Wall -c -pthread ./main.c
gcc -pthread -o bfi ./queue.o ./stack.o ./main.o
rm ./*.o
chmod +x bfi
./bfi
