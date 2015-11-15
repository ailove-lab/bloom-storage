blume_test: test-bloom.c blume.h blume.c murmur.h murmur.c
	clang -Wall -g -O3 test-bloom.c murmur.c blume.c -o blume_test -lm

clean:
	rm blume_test
