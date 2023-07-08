# matrix

# build an executable file

```bazel build //main:main```

# run the executable program

```./bazel-bin/main/main {params}```

# test

```bazel test test:concurrent_queue_test```

To make C++20 modules available in Bazel, involved [cc module](https://github.com/rnburn/rules_cc_module) in this project.