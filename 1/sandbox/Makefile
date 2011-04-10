all: forktest pipetest infromfile


forktest:
	gcc forktest.c -o fork_test
	./fork_test

pipetest:
	gcc pipetest.c -o pipe_test
	./pipe_test

infromfile:
	gcc infromfile.c -o in_from_file
	./in_from_file

clean:
	rm fork_test pipe_test in_from_file