default: test

configure:
    conan install . --output-folder=build --build=missing
    #!/usr/bin/bash
    cmake -B build -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release #--debug-find


test:
    cmake --build build 
    ./build/test
    ./build/main

clean:
    rm -rf build