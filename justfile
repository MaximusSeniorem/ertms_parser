all: configure test run

coninstall:
    conan install . --output-folder=build --build=missing

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