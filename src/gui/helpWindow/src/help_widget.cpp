#include "help_widget.h"
#include "ui_help_widget.h"

#include <boost/filesystem.hpp>

namespace yoyo::gui {
help_widget::help_widget(QWidget *parent) :
  QWidget{parent},
  _ui{std::make_unique<Ui::HelpWidget>()}
{
  _ui->setupUi(this);

  auto cwd = boost::filesystem::current_path();
  auto doc_path = cwd.parent_path() / "doc";
  auto doc_debug = cwd.parent_path().parent_path() / "doc";
  _ui->textBrowser->setSearchPaths(QStringList{} << ""
                                                  << QString::fromStdString(doc_path.string())
                                                  << QString::fromStdString((doc_path / "developer").string())
                                                  << QString::fromStdString((doc_path / "yoyo_developer").string())
                                                  << QString::fromStdString((doc_debug).string())
                                                  << QString::fromStdString((doc_debug / "dev").string())
                                                  << QString::fromStdString((doc_debug / "dev_yoyo").string())
                                                  << QString::fromStdString((doc_path / "user").string()));
  connect(_ui->pushButton, &QPushButton::clicked, this, [this](){
    QUrl url {"user/index.html"};
    _ui->textBrowser->setSource(url, QTextDocument::MarkdownResource);

  });
  connect(_ui->pushButton_2, &QPushButton::clicked, this, [this](){
    QUrl url{"index.html"};
    _ui->textBrowser->setSource(url, QTextDocument::HtmlResource);
  });
  connect(_ui->pushButton_3, &QPushButton::clicked, this, [this](){
    QUrl url{"yoyo_developer/index.html"};
    _ui->textBrowser->setSource(url, QTextDocument::HtmlResource);
  });
}

help_widget::~help_widget() = default;

}
