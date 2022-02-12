#include "arg-list.h"
//-------------------------------------------------------------------------//
#include <algorithm>
#include <stdexcept>
#include <sstream>
//-------------------------------------------------------------------------//
namespace auth {
    namespace common {
//-------------------------------------------------------------------------//
        namespace {
//-------------------------------------------------------------------------//
            inline bool iskey(const char & c) { return (c == '-'); }
//-------------------------------------------------------------------------//
        }; // namespace
//-------------------------------------------------------------------------//
        void arglist::build(int argc, char * argv[]) {
            if (argv) {
                int i = 0;
                std::string one, two;
                while (i < argc) {
                    one = ""; two = "";
                    if (iskey(*argv[i]) != false) {
                        one = argv[i++];
                        auto pos =  one.find('=');
                        if (pos == std::string::npos) {
                            pos = one.find(':');
                        }
                        if (pos != std::string::npos) {
                            two = one.substr(pos + 1);
                            one = one.substr(0, pos);
                        } else if (i < argc) {
                            if (not iskey(*argv[i])) {
                                two = argv[i++];
                            }
                        }
                        {// Adding a new parameter.
                            auto iter = this->params.find(one);
                            if (iter == this->params.end()) {
                                this->params.insert(this->params.end(), pair_t(one, two));
                            } else {//<???>
                                iter->second = two;
                            }
                        }
                    } else {
                        this->args.push_back(argv[i++]);
                    }
                }
            }
        }
//-------------------------------------------------------------------------//
        arglist::arglist(int argc, char * argv[]) {
            this->build(argc, argv);
        }

        auto arglist::contains(const std::string & key) const noexcept -> bool {
            const auto citer = this->params.find(key);
            return (citer != this->params.end());
        }
//-------------------------------------------------------------------------//
        auto arglist::operator[](const std::string & key) const noexcept -> const std::string & {
            static const std::string s_empty = "";
            const auto citer = this->params.find(key);
            return (citer != this->params.end()) ? citer->second : s_empty;
        }

        auto arglist::operator[](int index) const -> const std::string & {
            if (index < 0 || arguments_t::size_type(index) >= this->size()) {
                std::ostringstream buffer;
                buffer << "The index [" << index << "] is out of range.";
                throw (std::out_of_range(buffer.str()));
            }
            return this->args[index];
        }
//-------------------------------------------------------------------------//
        auto operator<<(std::ostream & stream, const arglist & args) -> std::ostream &{
            stream << "Parameters: ";
            std::for_each(args.begin_param(), args.end_param(), [&stream](const arglist::params_t::value_type & param) {
                stream << param.first << "=" << param.second << " ";
            });
            stream << std::endl << "Arguments: ";
            std::for_each(args.begin(), args.end(), [&stream](const arglist::arguments_t::value_type & arg) {
                stream << arg << " ";
            });
            return stream;
        }
//-------------------------------------------------------------------------//
    }; // namespace common
}; // namespace toolbek
