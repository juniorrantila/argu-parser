#pragma once
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <stdint.h>

namespace argu {

struct parser {
    parser(int argc, char const* argv[])
        : argc(argc)
        , argv(argv)
    {
        add_flag("--help", "-h", "Show this message.", [this] {
            print_usage_and_exit();
        });
    }

    void add_flag(std::string_view long_name, std::string_view short_name,
        std::string_view explanation, std::function<void()>&& callback)
    {
        auto id = flag_callbacks.size();
        flags.push_back({ long_name, short_name, explanation });
        flag_callbacks.push_back(callback);
        long_flag_ids.insert(std::pair{long_name, id});
        short_flag_ids.insert(std::pair{short_name, id});
    }

    void add_option(std::string_view long_name,
        std::string_view short_name, std::string_view placeholder,
        std::string_view explanation,
        std::function<void(char const*)>&& callback)
    {
        auto id = option_callbacks.size();
        options.push_back({
            long_name,
            short_name,
            explanation,
            placeholder,
        });
        option_callbacks.push_back(callback);

        long_option_ids.insert(std::pair{long_name, id});
        short_option_ids.insert(std::pair{short_name, id});
    }

    void add_positional_argument(std::string_view placeholder,
        std::string_view explaination,
        std::function<void(char const*)>&& callback)
    {
        positionals.push_back({
            placeholder,
            explaination,
        });
        positional_callbacks.push_back(callback);
    }

    [[nodiscard]] std::optional<std::string> run() const;

    void print_usage_and_exit(int exit_code = 0) const;

private:
    char const* program_name() const { return argv[0]; }

    struct Flag {
        std::string_view long_name;
        std::string_view short_name;
        std::string_view explanation;
    };
    std::vector<Flag> flags {};

    struct Option {
        std::string_view long_name;
        std::string_view short_name;
        std::string_view explanation;
        std::string_view placeholder;
    };
    std::vector<Option> options {};

    std::unordered_map<std::string_view, uint32_t> short_flag_ids {};
    std::unordered_map<std::string_view, uint32_t> long_flag_ids {};

    std::unordered_map<std::string_view, uint32_t> short_option_ids {};
    std::unordered_map<std::string_view, uint32_t> long_option_ids {};

    std::vector<std::function<void()>> flag_callbacks {};
    std::vector<std::function<void(char const*)>> option_callbacks {};

    struct Positional {
        std::string_view placeholder;
        std::string_view explaination;
    };
    std::vector<Positional> positionals {};
    std::vector<std::function<void(char const*)>> positional_callbacks {};

    int argc;
    char const** argv;
};
}
