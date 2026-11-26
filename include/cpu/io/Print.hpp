#pragma once

#include "cpu/storage/Register.hpp"
#include "cpu/Types.hpp"
#include "cpu/storage/RegisterFile.hpp"
#include "cpu/errors/RegisterError.hpp"
#include <iostream>
#include <string_view>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/color.h>

namespace cpu::io
{

    auto error_color = fmt::rgb(208, 19, 19);
    auto str_color = fmt::rgb(126, 194, 99);
    auto object_color = fmt::rgb(73, 118, 191);
    auto text_color = fmt::rgb(214, 214, 214);
    auto type_color = fmt::rgb(100, 178, 215);
    auto number_color = fmt::rgb(211, 156, 101);

    std::string stringWrap(std::string_view str)
    {
        return fmt::format("\"{}\"", str);
    }

    std::string typeWrap(std::string_view str)
    {
        return fmt::format("@{}", str);
    }

    std::string idWrap(std::string_view str)
    {
        return fmt::format("#{}", str);
    }

    void printError(
        errors::RegisterError error,
        std::string_view input = "anonymous")
    {
        switch (error)
        {
            case errors::RegisterError::NAME_NOT_FOUND:
            {
                fmt::println(
                    "{} {} {} {}",
                    fmt::styled("ERROR",           fg(error_color)),
                    fmt::styled("@Register",        fg(type_color)),
                    fmt::styled(stringWrap(input), fg(str_color)),
                    fmt::styled("was not found.",  fg(text_color))
                );
                
                break;
            }

            case errors::RegisterError::ID_NOT_FOUND:
            {
                fmt::println(
                    "{} {} {} {}",
                    fmt::styled("ERROR",                     fg(error_color)),
                    fmt::styled(typeWrap("Register"),                  fg(type_color)),
                    fmt::styled(idWrap(input),             fg(str_color)),
                    fmt::styled("does not exist.",             fg(text_color)));
                break;
            }
            case errors::RegisterError::NULL_REGISTER:
            {
                fmt::println(
                    "[{}] {} {} {}",
                    fmt::styled("ERROR", fg(error_color)),
                    fmt::styled(typeWrap("Register"), fg(type_color)),
                    fmt::styled(idWrap(input), fg(number_color)),
                    fmt::styled("does not exist.", fg(text_color)));
                break;
            }

            default:
                std::cerr << fmt::format("{} {}", fmt::styled("[ERROR]", fg(error_color)), fmt::styled("Unknown error", fg(text_color))) << std::endl;
        }
    }

}
