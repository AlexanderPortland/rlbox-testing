RLBOX_ROOT=../rlbox_wasm2c_sandbox

#RLBOX headers
RLBOX_INCLUDE=$(RLBOX_ROOT)/build/_deps/rlbox-src/code/include

#Our Wasi-SDK
WASI_SDK_ROOT=$(RLBOX_ROOT)/build/_deps/wasiclang-src/

#location of our wasi/wasm runtime
WASM2C_RUNTIME_PATH=$(RLBOX_ROOT)/build/_deps/mod_wasm2c-src/wasm2c
WASI_RUNTIME_FILES=$(addprefix $(WASM2C_RUNTIME_PATH), /wasm-rt-impl.c /wasm-rt-exceptions-impl.c)
WASI_RUNTIME_FILES2=$(addprefix $(RLBOX_ROOT)/src, /wasm2c_rt_mem.c /wasm2c_rt_minwasi.c)

WASI_CLANG=$(WASI_SDK_ROOT)/bin/clang
WASI_SYSROOT=$(WASI_SDK_ROOT)/share/wasi-sysroot
WASM2C=$(RLBOX_ROOT)/build/_deps/mod_wasm2c-src/bin/wasm2c

#CFLAGS for compiling files to place nice with wasm2c
# lOOK INTO PASSING THESE FLAGS AT CARGO BUILD
WASM_CFLAGS=-Wl,--export-all -Wl,--no-entry -Wl,--growable-table -Wl,--stack-first -Wl,-z,stack-size=1048576 -Wl,--import-memory -Wl,--import-table

# all: rust_from_c/target/wasm32-rlbox/debug/rust_from_c.wasm mylib.wasm.c mylib.wasm.o myapp
all: mylib.wasm mylib.wasm.c myapp

clean:
	rm -rf mylib.wasm.c mylib.wasm.h myapp *.o
	cd rust_from_c && cargo clean && cd -

#Step 1: build our library into wasm, using clang from the wasi-sdk
mylib.wasm: mylib.c
	$(WASI_CLANG) --sysroot $(WASI_SYSROOT) -O3 $(WASM_CFLAGS) dummy_main.c ./rust_from_c/src/mylib.c -o mylib.wasm

#Step 2: use wasm2c to convert our wasm to a C implementation of wasm we can link with our app.
mylib.wasm.c: mylib.wasm
	$(WASM2C) mylib.wasm -o mylib.wasm.c

#Step 3: compiling and linking our application with our library
mylib.wasm.o: mylib.wasm.c
	$(CC) -c $(WASI_RUNTIME_FILES) -c $(WASI_RUNTIME_FILES2) -I$(RLBOX_INCLUDE) -I$(RLBOX_ROOT)/include -I$(WASM2C_RUNTIME_PATH) mylib.wasm.c

myapp: mylib.wasm.o
	$(CXX) -std=c++17 main.cpp -o myapp -I$(RLBOX_INCLUDE) -I$(RLBOX_ROOT)/include -I$(WASM2C_RUNTIME_PATH) *.o -lpthread



# USING RUST COMPILED TO WASI

# rust_from_c/target/wasm32-rlbox/debug/rust_from_c.wasm: rust_from_c/src/lib.rs
# # cd rust_from_c && cargo build -v --target wasm32-wasi && cd -
# 	cd rust_from_c && cargo build -Z build-std=std,panic_abort --target wasm32-rlbox.json && cd -
	

# #Step 2: use wasm2c to convert our wasm to a C implementation of wasm we can link with our app.
# mylib.wasm.c: rust_from_c/target/wasm32-rlbox/debug/rust_from_c.wasm
# 	$(WASM2C) rust_from_c/target/wasm32-rlbox/debug/rust_from_c.wasm -o mylib.wasm.c

# #Step 3: compiling and linking our application with our library
# mylib.wasm.o: mylib.wasm.c mylib.wasm.aux.c
# 	$(CC) -c $(WASI_RUNTIME_FILES) -c $(WASI_RUNTIME_FILES2) -I$(RLBOX_INCLUDE) -I$(RLBOX_ROOT)/include -I$(WASM2C_RUNTIME_PATH) mylib.wasm.c mylib.wasm.aux.c

# myapp: mylib.wasm.o main.cpp rust_from_c/src/rust_from_c.h
# 	$(CXX) -std=c++17 main.cpp -o myapp -I$(RLBOX_INCLUDE) -I$(RLBOX_ROOT)/include -I$(WASM2C_RUNTIME_PATH) *.o -lpthread