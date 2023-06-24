# C++ Toolchain Configuration
#  https://bazel.build/docs/cc-toolchain-config-reference
#  https://github.com/bazelbuild/bazel/blob/master/tools/build_defs/cc/action_names.bzl
load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load(
    "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "feature",
    "flag_group",
    "flag_set",
    "tool_path",
)

all_compile_actions = [
    ACTION_NAMES.c_compile,
    ACTION_NAMES.cpp_compile,
    ACTION_NAMES.linkstamp_compile,
    ACTION_NAMES.assemble,
    ACTION_NAMES.preprocess_assemble,
    ACTION_NAMES.cpp_header_parsing,
    ACTION_NAMES.cpp_module_compile,
    ACTION_NAMES.cpp_module_codegen,
]

all_link_actions = [
    ACTION_NAMES.cpp_link_executable,
    ACTION_NAMES.cpp_link_dynamic_library,
    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
]

def _impl(ctx):
    llvm_version = "14.0.0"
    llvm_prefix = "/usr"
    llvm_bindir = llvm_prefix + "/bin"

    tool_paths = [
        tool_path(
            name = "gcc",
            path = llvm_bindir + "/clang-14",
        ),
        tool_path(
            name = "ld",
            path = llvm_bindir + "/ld.lld-14",
        ),
        tool_path(
            name = "ar",
            path = llvm_bindir + "/llvm-ar-14",
        ),
        tool_path(
            name = "cpp",
            path = llvm_bindir + "/clang-cpp-14",
        ),
        tool_path(
            name = "gcov",
            path = llvm_bindir + "/gcov",
        ),
        tool_path(
            name = "nm",
            path = llvm_bindir + "/llvm-nm-14",
        ),
        tool_path(
            name = "objdump",
            path = llvm_bindir + "/llvm-objdump-14",
        ),
        tool_path(
            name = "strip",
            path = llvm_bindir + "/llvm-strip-14",
        ),
    ]

    features = [
        feature(
            name = "default_compiler_flags",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = all_compile_actions,
                    flag_groups = ([
                        flag_group(
                            flags = [
                                "-O2", "-DNDEBUG",
                                "-Wall", "-Wextra", "-Wpedantic", "-fPIC",
                                "-std=c++20", "-stdlib=libc++"
                            ],
                        ),
                    ]),
                ),
            ],
        ),
        feature(
            name = "default_linker_flags",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = all_link_actions,
                    flag_groups = ([
                        flag_group(
                            flags = [
                                "-lc++", "-lc++abi",
                                "-lm", "-ldl", "-lpthread"
                            ],
                        ),
                    ]),
                ),
            ],
        ),
    ]

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        features = features,
        cxx_builtin_include_directories = [
            llvm_prefix + "/lib/llvm-14/include/c++/v1",
            llvm_prefix + "/lib/llvm-14/lib/clang/{}/include".format(llvm_version),
            "/usr/local/include",
            "/usr/include/x86_64-linux-gnu",
            "/usr/include",
        ],
        toolchain_identifier = "local",
        host_system_name = "local",
        target_system_name = "local",
        target_cpu = "linux_x86_64",
        target_libc = "unknown",
        compiler = "clang",
        abi_version = "unknown",
        abi_libc_version = "unknown",
        tool_paths = tool_paths,
    )

cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {},
    provides = [CcToolchainConfigInfo],
)
