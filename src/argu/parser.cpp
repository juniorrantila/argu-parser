#include "parser.h"
#include <sstream>

using namespace std::literals;

namespace argu {

using c_string = char const*;

std::optional<std::string> parser::run() const
{
    auto err_out = std::stringstream();

    auto program_name_view = std::string_view(program_name());
    size_t used_positionals = 0;
    for (int i = 1; i < argc; i++) {
        auto argument = std::string_view(argv[i]);
        if (auto id = short_flag_ids.find(argument); id != short_flag_ids.end()) {
            flag_callbacks.at(id->second)();
            continue;
        }

        if (auto id = long_flag_ids.find(argument); id != long_flag_ids.end()) {
            flag_callbacks.at(id->second)();
            continue;
        }

        if (auto id = short_option_ids.find(argument); id != short_option_ids.end()) {
            if (i + 1 >= argc) {
                err_out << "No argument provided for argument \""sv << argument << "\""sv
                        << "\nSee help for more info ("sv << program_name_view << " --help)"sv;
                return err_out.str();
            }
            c_string value = argv[++i];
            option_callbacks[id->second](value);
            continue;
        }

        if (auto id = long_option_ids.find(argument); id != long_option_ids.end()) {
            if (i + 1 >= argc) {
                err_out << "No argument provided for \""sv << argument << "\"\n"sv
                        << "See help for more info ("sv << program_name_view << " --help)"sv;
                return err_out.str();
            }
            c_string value = argv[++i];
            option_callbacks[id->second](value);
            continue;
        }

        if (used_positionals < positional_callbacks.size()) {
            auto id = used_positionals++;
            positional_callbacks[id](argument.data());
            continue;
        }

        err_out << "Unrecognized argument: \""sv << argument << "\""sv
                << "\nSee help for more info ("sv << program_name_view << " --help)"sv;

        return err_out.str();
    }

    if (used_positionals != positionals.size()) {
        if (positionals.size() - used_positionals == 1) {
            auto placeholder = positionals[used_positionals].placeholder;
            err_out << "Missing positional argument: "sv << placeholder;
        }
        err_out << "Missing positional arguments: "sv;
        for (size_t i = used_positionals; i < positionals.size(); i++) {
            auto placeholder = positionals[i].placeholder;
            err_out << "\t"sv << placeholder;
        }

        err_out << "\nSee help for more info ("sv << program_name_view << " --help)"sv;

        return err_out.str();
    }

    return {};
}

void parser::print_usage_and_exit(int exit_code) const
{
    FILE* out = exit_code != 0 ? stderr : stdout;
    (void)fprintf(out, "USAGE: %s [flags|options] ", program_name());
    for (auto positional : positionals) {
        int size = positional.placeholder.size();
        char const* data = positional.placeholder.data();
        (void)fprintf(out, "%.*s ", size, data);
    }

    (void)fprintf(out, "\n\nARGUMENTS:\n");
    for (auto positional : positionals) {
        int placeholder_size = positional.placeholder.size();
        char const* placeholder_data = positional.placeholder.data();

        int explain_size = positional.explaination.size();
        char const* explain_data = positional.explaination.data();

        (void)fprintf(out, "        %.*s: %.*s\n",
            placeholder_size, placeholder_data,
            explain_size, explain_data);
    }

    (void)fprintf(out, "\n");
    (void)fprintf(out, "FLAGS:\n");
    for (auto flag : flags) {
        auto pad = flag.short_name.size() == 2 ? " " : "";

        int short_size = flag.short_name.size();
        char const* short_data = flag.short_name.data();

        int long_size = flag.long_name.size();
        char const* long_data = flag.long_name.data();

        int explain_size = flag.explanation.size();
        char const* explain_data = flag.explanation.data();

        auto bytes = fprintf(out, "        %.*s, %s%.*s",
            short_size, short_data, pad, long_size, long_data);
        for (; bytes < 40; bytes++)
            (void)fprintf(out, " ");
        (void)fprintf(out, "%.*s\n", explain_size, explain_data);
    }

    (void)fprintf(out, "\nOPTIONS:\n");
    for (auto option : options) {
        auto pad = option.short_name.size() == 2 ? " " : "";

        int short_size = option.short_name.size();
        char const* short_data = option.short_name.data();

        int long_size = option.long_name.size();
        char const* long_data = option.long_name.data();

        int explain_size = option.explanation.size();
        char const* explain_data = option.explanation.data();

        int placeholder_size = option.placeholder.size();
        char const* placeholder_data = option.placeholder.data();

        auto bytes = fprintf(out, "        %.*s, %s%.*s  <%.*s> ",
            short_size, short_data, pad,
            long_size, long_data,
            placeholder_size, placeholder_data);
        for (; bytes < 40; bytes++)
            (void)fprintf(out, " ");
        (void)fprintf(out, "%.*s\n", explain_size, explain_data);
    }
    (void)fprintf(out, "\n");
    fflush(out);

    exit(exit_code);
}

}
