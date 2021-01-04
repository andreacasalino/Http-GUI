#ifndef GUI_REQUEST_H_
#define GUI_REQUEST_H_

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace gui {    
    class OptionValues {
    friend class Request;
    public:
        OptionValues(const std::string& firstValue);
        OptionValues(std::string&& firstValue);
        inline const std::vector<std::string>& getValues() const { return this->values; };

    private:
        std::vector<std::string> values;
    };

    class Request;
    typedef std::unique_ptr<Request> RequestPtr;

    class Request {
    public:
        static RequestPtr parse(const std::string& request_body);
        inline char getSymbol() const { return this->symbol; };
        inline const std::map<char, OptionValues>& getOptions() const { return this->options; };

    private:
        Request(const char& symbol);

        char symbol;
        std::map<char, OptionValues> options;
    };
}

#endif