#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace gui {    
    class RequestOptions;
    typedef std::unique_ptr<RequestOptions> RequestPtr;

    class RequestOptions {
    public:
        static RequestPtr parse(const std::string& request_body);

        inline const std::map<char, std::vector<std::string>>& getValues() const { return this->options; };

    private:
        RequestOptions() = default;

        std::map<char, std::vector<std::string>> options;
    };
}
