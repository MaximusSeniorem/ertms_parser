all: configure test run

configure:
    #!/usr/bin/bash
    mkdir -p build
    cmake -B build

test:
    cmake --build build

run:
    ./build/test

clean:
    rm -rf build