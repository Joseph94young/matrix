# bazel/build_system.bzl
load("//bazel/cc_module:defs.bzl", "cc_module", "cc_module_binary", "cc_module_library")

def matrix_module(name, deps = [], *args, **kwargs):
  cc_module(
      name = name,
      deps = deps + 
        [
            "//toolchain:_Builtin_stddef_max_align_t",
            "//toolchain:std"
        ],
      *args,
      **kwargs)

def matrix_module_binary(name, deps = [], *args, **kwargs):
  cc_module_binary(
      name = name,
      deps = deps + 
        [
            "//toolchain:_Builtin_stddef_max_align_t",
            "//toolchain:std"
        ],
      *args,
      **kwargs)

def matrix_module_library(name, deps = [], *args, **kwargs):
  cc_module_library(
      name = name,
      deps = deps + 
        [
            "//toolchain:_Builtin_stddef_max_align_t",
            "//toolchain:std"
        ],
      *args,
      **kwargs)