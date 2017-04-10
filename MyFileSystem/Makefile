TEST = FileSystem.o Mount.o Buffer.o Disk.o Testcase.o

test: $(TEST)
	gcc -o $@ $(TEST) -lrt -lpthread

FileSystem.o: FileSystem.c FileSystem.h Buffer.h
	gcc -c $<

Mount.o: Mount.c FileSystem.h Disk.h
	gcc -c $<

Buffer.o: Buffer.c Buffer.h Disk.h
	gcc -c $< -lrt -lpthread

Disk.o: Disk.c Disk.h
	gcc -c $<

Testcase.o: Testcase.c Buffer.h Disk.h FileSystem.h
	gcc -c $<

clean:
	rm -f $(TEST) test MY_DISK
