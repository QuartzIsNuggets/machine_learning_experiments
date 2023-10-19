cmake_minimum_required(VERSION 3.24)
project(linear_algebra)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PREBUILT_MODULE_PATH ${CMAKE_BINARY_DIR}/modules)

function(add_module name file)
	file(MAKE_DIRECTORY ${PREBUILT_MODULE_PATH})
	add_custom_target(
			${name}.pcm
			COMMAND
			${CMAKE_CXX_COMPILER}
			-std=c++2b
			-stdlib=libc++
			-fprebuilt-module-path=${PREBUILT_MODULE_PATH}
			-c ${CMAKE_CURRENT_SOURCE_DIR}/${file}
			--precompile
			-o ${PREBUILT_MODULE_PATH}/${name}.pcm
			)
endfunction()

add_module(la_concepts src/concepts.cppm)
add_module(la_concepts-scalars src/concepts/scalars.cppm)
add_module(la_concepts-matrices src/concepts/matrices.cppm)
add_module(la_concepts-vectors src/concepts/vectors.cppm)
add_module(la_concepts-operations src/concepts/operations.cppm)
add_module(combination src/combination.cppm)
add_module(matrix src/matrix.cppm)
add_module(transforms src/transforms.cppm)
add_module(printing src/printing.cppm)
add_module(assembly src/assembly.cppm)
add_module(assembly-shapes src/assembly/shapes.cppm)

add_executable(linear_algebra
		src/main.cpp
		src/matrix.cppm
		src/transforms.cppm
		src/printing.cppm
		src/combination.cppm
		src/concepts.cppm
		src/concepts/operations.cppm
		src/concepts/vectors.cppm
		src/concepts/matrices.cppm
		src/concepts/scalars.cppm
		src/assembly.cppm
		src/assembly/shapes.cppm)
add_dependencies(linear_algebra
		la_concepts-scalars.pcm
		la_concepts-matrices.pcm
		la_concepts-vectors.pcm
		la_concepts-operations.pcm
		la_concepts.pcm
		matrix.pcm
		combination.pcm
		transforms.pcm
		printing.pcm
		assembly-shapes.pcm
		assembly.pcm)
target_compile_options(linear_algebra PRIVATE -fprebuilt-module-path=${PREBUILT_MODULE_PATH})
target_compile_options(linear_algebra PRIVATE -stdlib=libc++)
target_link_options(linear_algebra PRIVATE -stdlib=libc++)
