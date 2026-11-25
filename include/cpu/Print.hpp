#pragma once

#include "cpu/Register.hpp"
#include "cpu/Types.hpp"
#include "cpu/RegisterFile.hpp"
#include <iostream>
#include <string_view>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/color.h>

namespace cpu
{
    void printError(
        errors::RegisterError error,
        std::string_view object = "[annoymous]")
    {
        switch (error)
        {
            case errors::RegisterError::NotFound:
            {
                fmt::println(
                    "{} {} {} {}",
                    fmt::styled("[ERROR]",                     fg(fmt::rgb(208, 19, 19))),
                    fmt::styled("<Register>",                  fg(fmt::rgb(64, 12, 48))),
                    fmt::styled(fmt::format("\"{}\"", object), fg(fmt::rgb(20, 112, 20))),
                    fmt::styled("was not found.",              fg(fmt::rgb(255, 255, 255))));
                break;
            }

            case errors::RegisterError::Invalid:
            {
                fmt::println(
                    "{} {} {} {}",
                    fmt::styled("[ERROR]",                     fg(fmt::rgb(208, 19, 19))),
                    fmt::styled("<Register>",                  fg(fmt::rgb(64, 12, 48))),
                    fmt::styled(fmt::format("\"{}\"", object), fg(fmt::rgb(0, 255, 0))),
                    fmt::styled("is invalid.",                 fg(fmt::rgb(255, 255, 255))));
                break;
            }

            default:
                fmt::println(fg(fmt::color::red), "Unknown error");
                break;
        }
    }



}