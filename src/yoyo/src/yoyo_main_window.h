#pragma once
#include <QMainWindow>
#include <QUrl>

#include <memory>

class QSettings;
namespace Ui
{
class yoyo_main_window;
}

namespace yoyo
{
class yoyo_main_window : public QMainWindow
{
  Q_OBJECT

public:
  explicit yoyo_main_window(QWidget* parent = nullptr);
  ~yoyo_main_window() override;

  auto set_edit_mode(bool mode) -> void;
  auto edit_mode() const -> bool;

  auto set_file_open(bool is_open) -> void;

  auto set_central_widget(QWidget* widget) -> void;

  auto history_changed(std::vector<QString> history) -> void;

  auto store_state(QSettings& settings) -> void;
  auto restore_state(QSettings& settings) -> void;

  auto setup_undo() -> void;
Q_SIGNALS:
  void newFile_requested();
  void openFile_requested();
  void saveFile_requested();
  void saveFileAs_requested();
  void closeFile_requested();
  void recentFile_requested(QString const& file);
  void exit_requested();

  void editMode(bool isActive);

  void tools_requested(bool isVisible);
  void tree_requested(bool isVisible);
  void properties_requested(bool isVisible);
  void plugins_requested();
  void log_requested(bool isVisible);
  void help_requested(bool isVisible);

private:
  std::unique_ptr<Ui::yoyo_main_window> _ui;
  bool _edit_mode;
};

} // namespace yoyo
