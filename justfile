all: configure test run


configure:
    #!/usr/bin/bash
    conan install . --output-folder=build --build=missing
    (cd build; cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release)


test:
    cmake --build build

run:
    ./build/test

clean:
    rm -rf build