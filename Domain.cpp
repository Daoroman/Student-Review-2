#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
    // разработайте класс домена
public:
    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
    Domain(string& domain)
        :domain_(domain)
    {
        reverse(domain_.begin(), domain_.end());
        domain_.push_back('.');
    }

    // разработайте operator==
    bool operator==(const Domain& other) {
        return domain_ == other.domain_;
    }
    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(const Domain& other) const{
        if (domain_.size() < other.domain_.size()) {
            return false;
        }
        auto mutch = std::mismatch(other.domain_.begin(), other.domain_.end(), domain_.begin());
        if (mutch.first == other.domain_.end()) {
            return true;
        }
        return false;
    }

    string GetDomain() const {
        return domain_;
    }

private:
    string domain_;
};

class DomainChecker {
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template <typename Iterator>
    DomainChecker( Iterator first, Iterator second) 
        :forbidden_domains_(first, second)
    {
        std::sort(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs) {
            return lhs.GetDomain() < rhs.GetDomain();
            });
        auto last = std::unique(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs) {
            return rhs.IsSubdomain(lhs);
            });
        forbidden_domains_.erase(last, forbidden_domains_.end());
    
    }

    // разработайте метод IsForbidden, возвращающий true, если домен запрещён

    bool IsForbidden(const Domain& domain) {

            auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain, [](const Domain& lhs, const Domain& rhs) {
            return lhs.GetDomain() < rhs.GetDomain();
            });

        if (it == forbidden_domains_.begin()) {
            return false;
        }
        return domain.IsSubdomain(*(prev(it)));
    }

private:
    vector<Domain> forbidden_domains_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
template <typename Number>
vector<Domain> ReadDomains(istream& input, Number num) {
    vector<Domain> result;
    for (size_t i = 0; i < num; ++i) {
        string line;
        getline(input, line);
        Domain dom(line);
        result.push_back(dom);
    }
    return result;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
   for (const Domain& domain : test_domains) {
    cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
   }
}