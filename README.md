#LLVM on Docker

This repository is set to be a `LLVM` repository that runs with `docker`. Great for isolating `llvm` development.

To run, make sure you have `docker` installed and run:

```
$ docker build .
```

This will create a build for the container, afterwards just run:

```
$ docker-compose run llvm
```

This will get you inside the container and you will see a prompt like this:

```
root@<container_id>:/var/Debug+Asserts#
```

Here you can start building your llvm by:

```
root@<container_id>:/var/Debug+Asserts# cmake $SRC_ROOT
root@<container_id>:/var/Debug+Asserts# cmake --build . --target install 
root@<container_id>:/var/Debug+Asserts# cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DLLVM_ENABLE_ASSERTIONS=On
```

And then every time you modify the source code under `src`, just go to the container and run `make` 
