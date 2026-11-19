workspace "Cpu"
    architecture "x86_64"
    configurations {
        "Debug",
        "Release"
    }

    location "build"

    language "C++"
    cppdialect "C++20"

    toolset "gcc"

    targetdir ("build/bin/%{cfg.buildcfg}")
    objdir ("build/obj/%{cfg.buildcfg}")

    filter "configurations:Debug"
        symbols "On"
        optimize "Off"
        defines { "DEBUG" }

    filter "configurations:Release"
        symbols "On"
        optimize "Speed"
        defines { "NDEBUG" }

    filter {}

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

    filter "files:**.cpp"
        buildoptions { "-Wall", "-Wextra", "-Wpedantic" }

    filter {}