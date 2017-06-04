TEST = MyQueue.o Testcase.o

test: $(TEST)
	gcc -o $@ $(TEST) -lrt -lpthread

MyQueue.o: MyQueue.c MyQueue.h
	gcc -c $<

Testcase.o: Testcase.c MyQueue.h
	gcc -c $<

clean:
	rm -f $(TEST) test
