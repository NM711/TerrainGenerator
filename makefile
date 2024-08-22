COMP="g++"


generate_dir:
	mkdir build
	mkdir ./build/depen
	mkdir ./build/bin

test:
	$(COMP) -o showcase ./main.cpp ./build/depen/terraingenerator.o -L./build/depen -l:ecautomata.a
	mv ./showcase ./build/bin

build:
	make generate_dir
	$(COMP) -c ./lib/ecautomata/ecautomata.cpp ./lib/ecautomata/util/seeder.cpp ./lib/ecautomata/util/grid.cpp
	ar rvs ecautomata.a ./ecautomata.o ./seeder.o ./grid.o  
	$(COMP) -c  -l:ecautomata.a ./generator.cpp -o terraingenerator.o
	mv *.o *.a ./build/depen

clean:
	rm -rf ./build
      
