#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <vector>

struct EmailInfo
{
    std::string full_address;
    std::string domain;
};

std::vector<EmailInfo> extract_emails(const std::string& text)
{
    std::vector<EmailInfo> result;

    std::regex pattern(R"(([a-zA-Z0-9._%+-]+)@([a-zA-Z0-9.-]+\.[a-zA-Z]{2,}))");

    auto begin = std::sregex_iterator(text.begin(), text.end(), pattern);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it)
    {
        EmailInfo info;
        info.full_address = (*it)[0].str();
        info.domain = (*it)[2].str();
        result.push_back(info);
    }

    return result;
}

void test()
{
    auto text = R"(Contact us at support@example.com or sales@company.org.
    Also reach out to admin@mail.co.uk for help.)";

    auto emails = extract_emails(text);

    assert(emails.size() == 3);
    assert(emails[0].full_address == "support@example.com");
    assert(emails[0].domain == "example.com");
    assert(emails[1].full_address == "sales@company.org");
    assert(emails[1].domain == "company.org");
    assert(emails[2].full_address == "admin@mail.co.uk");
    assert(emails[2].domain == "mail.co.uk");

    auto empty_result = extract_emails("no emails here");
    assert(empty_result.empty());
}

int main()
{
    test();
    std::cout << "Tests passed.\n";
    std::cout << "Demonstration:\n";
    auto text = R"(Contact: alice@wonderland.com and bob@builder.org)";
    auto emails = extract_emails(text);
    for (const auto& email : emails)
    {
        std::cout << "Email: " << email.full_address
                  << " Domain: " << email.domain << '\n';
    }
    return 0;
}