default: test

config:
    rm -rf build
    conan install . --output-folder=build --build=missing
    #!/usr/bin/bash
    cmake -B build -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release #--debug-find


build:
    cmake --build build

test: build
    ./build/parser_tests


sandbox: build
    ./build/sandbox

clean:
    rm -rf build