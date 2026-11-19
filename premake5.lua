workspace "Cpu"
    architecture "x86_64"
    
    -- Different build configurations
    configurations {
        "Debug",
        "Release",
        "Native"
    }

    location "build"

    language "C++"
    cppdialect "C++26"
    toolset "gcc"

    -- Executable output and object file directories
    targetdir ("build/bin/%{cfg.buildcfg}")
    objdir ("build/obj/%{prj.name}/%{cfg.buildcfg}")

    filter "configurations:Debug"
        symbols "On"
        optimize "Off"
        defines { "DEBUG" }


    filter "configurations:Release"
        symbols "On"
        optimize "Speed"
        defines { "NDEBUG" }


    filter "configurations:Native"
        symbols "On"
        optimize "Speed"
        defines { "NDEBUG" }
    
    filter {}

-- ======================================
--                Main
-- ======================================

project "Cpu"
    kind "ConsoleApp"
    language "C++"

    files {
        "include/**.hpp",
        "include/**.tpp",
        "src/**.hpp",
        "src/**.cpp",
        "src/**.tpp"
    }

    includedirs {
        "include"
    }

    -- Compiler-specific settings
    warnings "Extra"

    buildoptions {
        "-Wall",
        "-Wextra",
        "-Wpedantic",
        "-Wconversion",
        "-Wcast-align",
        "-Wunused",
        "-Wshadow",
        "-freflection"
    }

    filter "configurations:Debug"
        buildoptions {
            "-O0"
        }

    filter "configurations:Release"
        buildoptions {
            "-O3",
            "-march=x86-64",
            "-flto",
            "-mtune=native",
            "-fomit-frame-pointer"
        }

        linkoptions {
            "-flto"
        }

    filter "configurations:Native"
        buildoptions {
            "-Ofast",
            "-march=native",
            "-mtune=native",
            "-fomit-frame-pointer",
            "-flto"
        }

        linkoptions {
            "-flto"
        }
    filter {}


-- ======================================
--              Benchmarks
-- ======================================

project "Benchmarks"
    kind "ConsoleApp"
    language "C++"

    files {
        "benchmarks/**.cpp",
        "include/**.hpp",
        "include/**.tpp",

    }
    
    removefiles {
        "benchmarks/exclude/**.cpp"
    }


    includedirs {
        "include"
    }

    links {
        "benchmark",
        "benchmark_main"
    }

    -- Compiler-specific settings
    warnings "Extra"

    buildoptions {
        "-Wall",
        "-Wextra",
        "-Wpedantic",
        "-Wconversion",
        "-Wcast-align",
        "-Wunused",
        "-Wshadow",
        "-freflection"
    }

    filter "configurations:Debug"
        buildoptions {
            "-O0",
        }

    filter "configurations:Release"
        buildoptions {
            "-O3",
            "-march=x86-64",
            "-flto",
            "-mtune=native",
            "-fomit-frame-pointer"
        }

            linkoptions {
                "-flto"
            }

    filter "configurations:Native"
        buildoptions {
            "-Ofast",
            "-march=native",
            "-mtune=native",
            "-fomit-frame-pointer",
            "-flto"
        }
    filter {}

-- ======================================
--               Tests
-- ======================================

project "Tests"
    kind "ConsoleApp"
    language "C++"

    files {
        "tests/**.cpp",
        "include/**.hpp",
        "include/**.tpp",

    }

    includedirs {
        "include"
    }

    links {
        "gtest",
        "gtest_main"
    }

    -- Compiler-specific settings
    warnings "Extra"

    buildoptions {
        "-Wall",
        "-Wextra",
        "-Wpedantic",
        "-Wconversion",
        "-Wcast-align",
        "-Wunused",
        "-Wshadow",
        "-freflection"
    }

    filter "configurations:Debug"
        buildoptions {
            "-O0",
        }

    filter "configurations:Release"
        buildoptions {
            "-O3",
            "-march=x86-64",
            "-flto",
            "-mtune=native",
            "-fomit-frame-pointer"
        }

            linkoptions {
                "-flto"
            }

    filter "configurations:Native"
        buildoptions {
            "-Ofast",
            "-march=native",
            "-mtune=native",
            "-fomit-frame-pointer",
            "-flto"
        }
    filter {}
