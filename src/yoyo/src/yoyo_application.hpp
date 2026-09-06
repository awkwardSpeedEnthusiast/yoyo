#include <QApplication>

#include <memory>

namespace yoyo
{
class yoyo_application : public QApplication
{
  Q_OBJECT
public:
  yoyo_application(int& argc, char** argv);
  ~yoyo_application() override;

  auto setup() -> void;

private:
  class impl;
  std::unique_ptr<impl> _p;
};
} // namespace yoyo
