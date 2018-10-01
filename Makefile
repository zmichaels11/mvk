.PHONY: all directories testComputeShaders

all: testComputeShaders

directories:
	mkdir -p shaders/testCompute

shaders/testCompute/%.comp.spv: src/testCompute/glsl/%.comp
	glslc -c $< -o $@

testComputeShaders: directories shaders/testCompute/square.comp.spv

clean:
	rm -rf shaders
