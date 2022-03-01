#ifndef IO_HH_
#define IO_HH_

#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <tuple>
#include <utility>

namespace io {

    template<typename ... T> struct Concat { std::tuple<T&...> tpl; };
    template<typename ... T> Concat<T...> concat(T&... value) { return Concat<T...> { std::tuple<T&...>(value...) }; }

    template<typename T, typename C> struct Option { T& value; C condition; };
    template<typename T, typename C> Option<T, C> option(T& value, const C& condition) { return Option<T, C> { value, condition }; }

    struct Measure { double& value; };
    Measure measure(double& value) { return Measure { value }; }

    std::list<std::string> split(const std::string& input) {
        std::list<std::string> lst;
        std::istringstream iss(input);
        std::string sv;
        while (iss >> sv) lst.emplace_back(sv);
        return lst;
    }

    namespace _impl {

        template<typename Tpl, size_t ... Ids>
        void parse_each(std::istringstream& iss, Tpl& tpl, std::index_sequence<Ids...>) {
            (... && (iss >> std::get<Ids>(tpl)));
        }

        template<typename T> struct ParseHelper {
            bool parse(std::list<std::string>::iterator& itor, T& variable) {
                std::istringstream iss(*itor++);
                iss >> variable;
                return !iss.fail();
            }
        };

        template<typename ... T> struct ParseHelper<Concat<T...>> {
            bool parse(std::list<std::string>::iterator& itor, Concat<T...>& variable) {
                std::istringstream iss(*itor++);
                parse_each(iss, variable.tpl, std::index_sequence_for<T...>{});
                return !iss.fail();
            }
        };

        template<typename T, typename C> struct ParseHelper<Option<T, C>> {
            bool parse(std::list<std::string>::iterator& itor, Option<T, C>& variable) {
                if (!variable.condition()) return true;
                std::istringstream iss(*itor++);
                iss >> variable.value;
                return !iss.fail();
            }
        };

        template<> struct ParseHelper<Measure> {
            bool parse(std::list<std::string>::iterator& itor, Measure& variable) {
                std::string s = *itor++;
                size_t len = s.length();
                if (len == 0) return false;
                double base = 0;
                switch (s[len - 1U]) {
                    case 'T': base = 1e12; break;
                    case 'G': base = 1e9; break;
                    case 'M': base = 1e6; break;
                    case 'K': base = 1e3; break;
                    case 'm': base = 1e-3; break;
                    case 'u': base = 1e-6; break;
                    case 'n': base = 1e-9; break;
                    case 'p': base = 1e-12; break;
                    case 'f': base = 1e-15; break;
                }
                if (base > 0) {
                    std::istringstream iss(s.substr(0, len - 1U));
                    iss >> variable.value;
                    variable.value *= base;
                    return !iss.fail();
                } else {
                    std::istringstream iss(s);
                    iss >> variable.value;
                    return !iss.fail();
                }
                return false;
            }
        };
    }

    template<typename T>
    bool parse_from_iterator(std::list<std::string>::iterator& itor, T& variable) {
        return _impl::ParseHelper<T>().parse(itor, variable);
    }

    template<typename ... T>
    bool parse_from_string(const std::string& input, T&& ... vars) {
        std::list<std::string> lst = split(input);
        auto itor = lst.begin();
        return (... && (itor != lst.end() && parse_from_iterator(itor, vars))) && itor == lst.end();
    }

}

#endif