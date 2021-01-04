#include <RequestOptions.h>
#include <sstream>
#include <list>

namespace gui {
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

    RequestPtr RequestOptions::parse(const std::string& request_body) {
        auto slices = sliceFragments(request_body);
        
        if(slices.empty()) return nullptr;
        if(slices.size() % 2 != 0) return nullptr;

        RequestPtr opt;
        opt.reset(new RequestOptions());
        while (!slices.empty()) {
            if(slices.front().size() != 1) return nullptr;
            char s = slices.front().front();
            slices.pop_front();

            auto it = opt->options.find(s);
            if(it == opt->options.end()) {
                opt->options.emplace(s , std::vector<std::string>{slices.front()} );
            }
            else {
                it->second.push_back(std::move(slices.front()));
            }
            slices.pop_front();
        }

        return opt;
    }
}