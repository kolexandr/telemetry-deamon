all :
	cmake -S . -B build
	cmake --build build --verbose

.PHONY : all clean

clean:
	rm -rf build