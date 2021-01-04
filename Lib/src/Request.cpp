#include <Request.h>
#include <sstream>
#include <list>

namespace gui {
    OptionValues::OptionValues(const std::string& firstValue) {
        this->values.push_back(firstValue);
    }

    OptionValues::OptionValues(std::string&& firstValue) {
        this->values.push_back(std::move(firstValue));
    }


	std::list<std::string> sliceFragments(const std::string& toSplit) {
		std::istringstream iss(toSplit);
		std::list<std::string> slices;
		while (!iss.eof()) {
			slices.emplace_back(std::string());
			iss >> slices.back();
			if(slices.back().empty()) slices.pop_back();
		}
		return slices;
	}

    RequestPtr Request::parse(const std::string& request_body) {
        auto slices = sliceFragments(request_body);
        
        if(slices.size() < 1) return nullptr;
        if(slices.front().size() != 1) return nullptr;
        RequestPtr req;
        req.reset(new Request(slices.front().front()));
        slices.pop_front();

        if(slices.size() % 2 != 0) return nullptr;
        while (!slices.empty()) {
            if(slices.front().size() != 1) return nullptr;
            char s = slices.front().front();
            slices.pop_front();

            auto it = req->options.find(s);
            if(it == req->options.end()) {
                req->options.emplace(s, std::move(slices.front()) );
            }
            else {
                it->second.values.push_back(std::move(slices.front()));
            }
            slices.pop_front();
        }

        return req;
    }

    Request::Request(const char& symbol)
        : symbol(symbol)  {
    }
}