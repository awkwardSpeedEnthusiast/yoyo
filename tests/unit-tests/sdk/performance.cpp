#include <gtest/gtest.h>

#include <QRegularExpression>

#include <iostream>
#include <regex>

class PerformanceRegex : public testing::Test
{
public:
  auto generateTestStrings() -> std::vector<std::string>
  {
    static std::vector<std::pair<std::string, std::string>> commands {
      { "general", "volt_a" }, { "general", "volt_b" }, { "general", "cur" }, { "general", "temp" },
      { "general", "div" },    { "error", "fault" },    { "error", "WG1" },   { "error", "WG2" },
      { "error", "WG3" },      { "error", "WG4" },      { "error", "LG1" },   { "error", "LG2" },
      { "error", "PG1" },      { "error", "ED1" },      { "error", "PCI" },   { "error", "P2" },
      { "WGA1", "ctrl1" },     { "WGA1", "ctrl2" },     { "WGA2", "ctrl1" },  { "WGA2", "ctrl2" },
      { "WGA3", "ctrl1" },     { "WGA3", "ctrl2" },     { "WGB1", "ctrl1" },  { "WGB1", "ctrl2" },
      { "WGB2", "ctrl1" },     { "WGB2", "ctrl2" },     { "WGB3", "ctrl1" },  { "WGB3", "ctrl2" },
      { "WGB4", "ctrl1" },     { "WGB4", "ctrl2" },     { "LGA1", "ctrl1" },  { "LGA1", "ctrl2" },
      { "LGA2", "ctrl1" },     { "LGA2", "ctrl2" },     { "LGB1", "ctrl1" },  { "LGB1", "ctrl2" },
      { "LGB2", "ctrl1" },     { "LGB2", "ctrl2" },
    };
    std::vector<std::string> output;

    for (auto i = 0; i < 1000; i++) {
      std::transform(
        std::begin(commands), std::end(commands), std::back_inserter(output), [](auto com) {
          return "! " + com.first + " " + com.second + " " + std::to_string(0xaaaaaaaa) + "\r";
        });
    }

    return output;
  }

  auto generateTestQStrings() -> std::vector<QString>
  {
    auto tmp = generateTestStrings();
    std::vector<QString> output;
    std::transform(std::begin(tmp), std::end(tmp), std::back_inserter(output),
                   [](auto a) { return QString::fromStdString(a); });
    return output;
  }
};

TEST_F(PerformanceRegex, standard)
{
  auto strings = generateTestStrings();
  auto start = std::chrono::high_resolution_clock::now();

  for (auto const& testString : strings) {
    std::regex word_regex("! ([a-zA-Z0-9]+) ([a-zA-Z0-9_]+) ([0-9.e+-]+)", std::regex::optimize);
    std::smatch base_match;

    if (std::regex_match(testString, base_match, word_regex)) {
      // The first sub_match is the whole string; the next
      // sub_match is the first parenthesized expression.
      EXPECT_EQ(base_match.size(), 2);
    }
  }

  auto stop = std::chrono::high_resolution_clock::now();
  std::cout << "Std: Matched " << strings.size() << " test strings "
            << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " ms."
            << std::endl;
}

TEST_F(PerformanceRegex, Qt)
{
  auto strings = generateTestQStrings();
  auto start = std::chrono::high_resolution_clock::now();

  for (auto const& testString : strings) {
    QRegularExpression expression { "! ([a-zA-Z0-9]+) ([a-zA-Z0-9_]+) ([0-9.e+-]+)" };

    QRegularExpressionMatch match;

    match = expression.match(testString);

    if (match.hasMatch()) {
      EXPECT_EQ(match.lastCapturedIndex(), 3);
    }
  }

  auto stop = std::chrono::high_resolution_clock::now();
  std::cout << "Qt: Matched " << strings.size() << " test strings "
            << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " ms."
            << std::endl;
}
