COMP=g++
COMP_WASM=em++

EMFLAGS=-s WASM=1 -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -s MODULARIZE=1 -s EXPORT_NAME='createModule'

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
	$(COMP) -c  -l:ecautomata.a ./core/generator.cpp -o terraingenerator.o
	mv *.o *.a ./build/depen

build_wasm:
	# Create output directory if it doesn't exist
	mkdir -p ./build/wasm_depen
	# Compile the library files with Emscripten
	em++ -c ./lib/ecautomata/ecautomata.cpp ./lib/ecautomata/util/seeder.cpp ./lib/ecautomata/util/grid.cpp -std=c++17 -O3
	# Create static library
	emar rvs ./build/wasm_depen/ecautomata.a ./ecautomata.o ./seeder.o ./grid.o
	# Compile generator and bindings, linking with the library
	em++ ./core/generator.cpp ./wasm/bindings.cpp -L./build/wasm_depen -l:ecautomata.a -o ./build/wasm_depen/terrain.js \
		-std=c++17 \
		-lembind \
		-s WASM=1 \
		-s MODULARIZE=1 \
		-s EXPORT_NAME="createTerrainModule" \
		-s ALLOW_MEMORY_GROWTH=1 \
		-s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
		-O3
	# Clean up intermediate object files
	mv *.o ./build/wasm_depen/
clean:
	rm -rf ./build
      
