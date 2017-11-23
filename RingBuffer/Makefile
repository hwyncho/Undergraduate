TEST = MyQueue.o main.o

test: $(TEST)
	gcc -o $@ $(TEST) -lrt -lpthread

MyQueue.o: MyQueue.c MyQueue.h
	gcc -c $<

main.o: main.c MyQueue.h
	gcc -c $<

clean:
	rm -f $(TEST) test
