.PHONY: all directories testComputeShaders

all: testComputeShaders testDrawShaders

directories:
	mkdir -p shaders/testCompute
	mkdir -p shaders/testDraw

shaders/testCompute/%.comp.spv: src/testCompute/glsl/%.comp
	glslc -c $< -o $@

shaders/testDraw/%.frag.spv: src/testDraw/glsl/%.frag
	glslc -c $< -o $@

shaders/testDraw/%.vert.spv: src/testDraw/glsl/%.vert
	glslc -c $< -o $@

testComputeShaders: directories shaders/testCompute/square.comp.spv

testDrawShaders: directories shaders/testDraw/tris2D_colored.frag.spv shaders/testDraw/tris2D_colored.vert.spv 

clean:
	rm -rf shaders
