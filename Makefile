all :
	cmake -S . -B build -DCMAKE_POLICY_VERSION_MINIMUM=3.5
	cmake --build build --verbose

.PHONY : all clean

clean:
	rm -rf build