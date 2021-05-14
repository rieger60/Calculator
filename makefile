Calculator: main.c
	gcc -o Calculator main.c resource.o -mwindows

Resource: resource.rc
	windres resource.rc resource.o
