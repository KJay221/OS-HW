storage: storage.o
	g++ -o storage.out storage.o
storage.o: storage.cpp
	g++ -c storage.cpp
generate:
	g++ -o generate.out generate.cpp

#need to delete in final
testmap:
	g++ -o testmap.out testmap.cpp
clean:
	rm *.o *out
delete_file:
	rm -r storage *.output
