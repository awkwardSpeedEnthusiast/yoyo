#include "../src/gui_node_widget.hpp"
#include "yoyo/gui_node.h"

#include "guiFixture.hpp"
#include "gui_node_mock.h"

#include <QMimeData>
#include <QSignalSpy>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::StrictMock;
using yoyo::test::gui_node_mock;
namespace
{
const constexpr auto mimetype_gui_element = "application/yoyo/gui-element";
const constexpr auto mimetype_gui_new = "application/yoyo/gui-new";
const constexpr auto mimetype_data_element = "application/yoyo/data-element";
} // namespace
class guiNodeWidgetTest : public yoyo::test::guiFixture
{
public:
  auto SetUp() -> void override
  {
    root = std::dynamic_pointer_cast<gui_node_mock>(createGuiNode("root"));
    container = std::dynamic_pointer_cast<gui_node_mock>(createGuiNode("container type"));
    object = std::dynamic_pointer_cast<gui_node_mock>(createGuiNode("object type"));
    EXPECT_CALL(*root, acceptsChild(_)).WillOnce(Return(true));
    EXPECT_CALL(*container, acceptsParent(_)).WillOnce(Return(true));
    EXPECT_CALL(*root, beforeChildAdd(_)).WillOnce(Return(true));
    EXPECT_CALL(*root, afterChildAdd(_)).WillOnce(Return(true));
    EXPECT_CALL(*container, acceptsChild(_)).WillOnce(Return(true));
    EXPECT_CALL(*object, acceptsParent(_)).WillOnce(Return(true));
    EXPECT_CALL(*container, beforeChildAdd(_)).WillOnce(Return(true));
    EXPECT_CALL(*container, afterChildAdd(_)).WillOnce(Return(true));
    root->addChild(container);
    container->addChild(object);
    root->setEditMode(true);
    root->widget()->resize(100, 100);
    object->setName("object");
    container->setName("container");
    root->setName("root");
  }

  auto TearDown() -> void override
  {
    tearDownNodes({ std::move(object), std::move(root), std::move(container) });
  }

  auto createMimeData(yoyo::types::value_t valueType, QString const& path) -> QMimeData*
  {
    QByteArray d;
    QDataStream dataStream(&d, QIODevice::WriteOnly);

    dataStream << static_cast<int>(valueType) << path;
    auto data = new QMimeData;
    data->setData(mimetype_data_element, d);
    return data;
  }

  auto createMimeData(boost::uuids::uuid id) -> QMimeData*
  {
    auto data = new QMimeData;
    data->setData(mimetype_gui_new, QByteArray::fromStdString(boost::uuids::to_string(id)));
    return data;
  }

  auto checkEnterUnsuccessful(QWidget* w, QPoint p, Qt::DropAction action,
                              yoyo::types::value_t type, QString const& path,
                              Qt::MouseButtons button, Qt::KeyboardModifiers modifiers) -> void
  {
    ASSERT_EQ(root->widget()->childAt(p), w);
    auto enterEvent = new QDragEnterEvent(p, action, createMimeData(type, path), button, modifiers);

    QApplication::postEvent(w, enterEvent);
    QApplication::processEvents();
    EXPECT_FALSE(enterEvent->isAccepted());
  }

  auto checkMoveUnsuccessful(QWidget* w, QPoint p, Qt::DropAction action, yoyo::types::value_t type,
                             QString const& path, Qt::MouseButtons button,
                             Qt::KeyboardModifiers modifiers) -> void
  {
    ASSERT_EQ(root->widget()->childAt(p), w);

    auto enterEvent = new QDragEnterEvent(p, action, createMimeData(type, path), button, modifiers);

    QApplication::postEvent(w, enterEvent);
    QApplication::processEvents();

    auto moveEvent = new QDragMoveEvent(p, action, createMimeData(type, path), button, modifiers);

    QApplication::postEvent(w, moveEvent);
    QApplication::processEvents();
    EXPECT_FALSE(moveEvent->isAccepted());
  }

  auto checkDropUnsuccessful(QWidget* w, QPoint p, Qt::DropAction action, yoyo::types::value_t type,
                             QString const& path, Qt::MouseButtons button,
                             Qt::KeyboardModifiers modifiers) -> void
  {
    ASSERT_EQ(root->widget()->childAt(p), w);
    auto enterEvent = new QDragEnterEvent(p, action, createMimeData(type, path), button, modifiers);

    QApplication::postEvent(w, enterEvent);
    QApplication::processEvents();

    auto dropEvent = new QDropEvent(p, action, createMimeData(type, path), button, modifiers);

    QApplication::postEvent(w, dropEvent);
    QApplication::processEvents();
    //        EXPECT_FALSE(dropEvent->isAccepted());
  }

  auto checkSuccessfulDND(QWidget* w, QPoint p, Qt::DropAction action, yoyo::types::value_t type,
                          QString const& path, Qt::MouseButtons button,
                          Qt::KeyboardModifiers modifiers) -> void
  {
    ASSERT_EQ(root->widget()->childAt(p), w);
    {
      auto enterEvent =
        new QDragEnterEvent(p, action, createMimeData(type, path), button, modifiers);

      QApplication::postEvent(w, enterEvent);
      QApplication::processEvents();
      EXPECT_TRUE(enterEvent->isAccepted());
    }

    {
      auto moveEvent = new QDragMoveEvent(p, action, createMimeData(type, path), button, modifiers);

      QApplication::postEvent(w, moveEvent);
      QApplication::processEvents();
      EXPECT_TRUE(moveEvent->isAccepted());
    }
    {
      auto dropEvent = new QDropEvent(p, action, createMimeData(type, path), button, modifiers);

      QApplication::postEvent(w, dropEvent);
      QApplication::processEvents();
      EXPECT_TRUE(dropEvent->isAccepted());
    }
  }

  auto checkSuccessfulDND(QWidget* w, QPoint p, Qt::DropAction action, boost::uuids::uuid type,
                          Qt::MouseButtons button, Qt::KeyboardModifiers modifiers) -> void
  {
    ASSERT_EQ(root->widget()->childAt(p), w);
    {
      auto enterEvent = new QDragEnterEvent(p, action, createMimeData(type), button, modifiers);

      QApplication::postEvent(w, enterEvent);
      QApplication::processEvents();
      EXPECT_TRUE(enterEvent->isAccepted());
    }

    {
      auto moveEvent = new QDragMoveEvent(p, action, createMimeData(type), button, modifiers);

      QApplication::postEvent(w, moveEvent);
      QApplication::processEvents();
      EXPECT_TRUE(moveEvent->isAccepted());
    }
    {
      auto dropEvent = new QDropEvent(p, action, createMimeData(type), button, modifiers);

      QApplication::postEvent(w, dropEvent);
      QApplication::processEvents();
      EXPECT_TRUE(dropEvent->isAccepted());
    }
  }

  auto checkUnsuccessfulDrop(QWidget* w, QPoint p, Qt::DropAction action, boost::uuids::uuid type,
                             Qt::MouseButtons button, Qt::KeyboardModifiers modifiers) -> void
  {
    ASSERT_EQ(root->widget()->childAt(p), w);
    auto enterEvent = new QDragEnterEvent(p, action, createMimeData(type), button, modifiers);
    auto dropEvent = new QDropEvent(p, action, createMimeData(type), button, modifiers);

    QApplication::postEvent(w, enterEvent);
    QApplication::processEvents();

    QApplication::postEvent(w, dropEvent);
    QApplication::processEvents();

    //        EXPECT_TRUE(dropEvent->isAccepted());
  }

  auto checkUnsuccessfulEnter(QWidget* w, QPoint p, Qt::DropAction action, boost::uuids::uuid type,
                              Qt::MouseButtons button, Qt::KeyboardModifiers modifiers) -> void
  {
    ASSERT_EQ(root->widget()->childAt(p), w);
    auto enterEvent = new QDragEnterEvent(p, action, createMimeData(type), button, modifiers);

    QApplication::postEvent(w, enterEvent);
    QApplication::processEvents();
    EXPECT_FALSE(enterEvent->isAccepted());
  }

  auto checkUnsuccessfulMove(QWidget* w, QPoint p, Qt::DropAction action, boost::uuids::uuid type,
                             Qt::MouseButtons button, Qt::KeyboardModifiers modifiers) -> void
  {
    ASSERT_EQ(root->widget()->childAt(p), w);
    auto enterEvent = new QDragEnterEvent(p, action, createMimeData(type), button, modifiers);
    auto moveEvent = new QDragMoveEvent(p, action, createMimeData(type), button, modifiers);

    QApplication::postEvent(w, enterEvent);
    QApplication::processEvents();

    QApplication::postEvent(w, moveEvent);
    QApplication::processEvents();
    EXPECT_FALSE(moveEvent->isAccepted());
  }

  std::shared_ptr<gui_node_mock> root;
  std::shared_ptr<gui_node_mock> container;
  std::shared_ptr<gui_node_mock> object;
};

TEST_F(guiNodeWidgetTest, itemSelected)
{
  struct Receiver {
    MOCK_METHOD(void, clicked, (std::shared_ptr<yoyo::node_base> selected));
  } receiver;

  QObject::connect(object.get(), &gui_node_mock::itemSelected,
                   [&receiver](auto i) { receiver.clicked(i); });

  EXPECT_CALL(receiver, clicked(std::dynamic_pointer_cast<yoyo::node_base>(object)));
  QTest::mouseClick(object->widget(), Qt::LeftButton);
  QApplication::processEvents();

  QTest::mouseClick(object->widget(), Qt::RightButton);
  QApplication::processEvents();
}

TEST_F(guiNodeWidgetTest, dropNewItem)
{
  struct Receiver {
    MOCK_METHOD(void, addRequested,
                (std::shared_ptr<yoyo::node_base> parent, boost::uuids::uuid id, int index));
    MOCK_METHOD(void, exchangeRequested,
                (std::shared_ptr<yoyo::node_base> parent, boost::uuids::uuid id));
  } receiver;

  QObject::connect(
    root.get(), &gui_node_mock::addRequested,
    [&receiver](auto parent, auto id, auto index) { receiver.addRequested(parent, id, index); });
  QObject::connect(root.get(), &gui_node_mock::exchangeRequested,
                   [&receiver](auto n, auto id) { receiver.exchangeRequested(n, id); });

  /// \todo currently only works without layout in test: find out why.
  object->setLayoutDirection({ yoyo::types::layout_direction_t::FREE, true });
  auto id = boost::uuids::random_generator {}();
  QPoint op { 10, 10 };

  ON_CALL(*object, acceptsChildren()).WillByDefault(Return(true));
  {
    EXPECT_CALL(receiver, addRequested(std::dynamic_pointer_cast<yoyo::node_base>(object), id, -1));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(receiver, addRequested(std::dynamic_pointer_cast<yoyo::node_base>(object), id, -1));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton,
                       { Qt::ShiftModifier });
  }
  {
    EXPECT_CALL(receiver, addRequested(std::dynamic_pointer_cast<yoyo::node_base>(object), id, -1));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton,
                       { Qt::AltModifier });
  }
  {
    EXPECT_CALL(receiver, addRequested(std::dynamic_pointer_cast<yoyo::node_base>(object), id, -1));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton,
                       { Qt::ControlModifier });
  }
  ON_CALL(*object, acceptsChildren()).WillByDefault(Return(false));
  ON_CALL(*container, acceptsChildren()).WillByDefault(Return(true));
  {
    EXPECT_CALL(receiver,
                addRequested(std::dynamic_pointer_cast<yoyo::node_base>(container), id, 1));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);
    EXPECT_CALL(*container, acceptsChildren()).Times(3);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(receiver,
                addRequested(std::dynamic_pointer_cast<yoyo::node_base>(container), id, 1));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);
    EXPECT_CALL(*container, acceptsChildren()).Times(3);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton,
                       { Qt::ShiftModifier });
  }
  {
    EXPECT_CALL(receiver,
                addRequested(std::dynamic_pointer_cast<yoyo::node_base>(container), id, 1));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);
    EXPECT_CALL(*container, acceptsChildren()).Times(3);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton,
                       { Qt::AltModifier });
  }
  {
    EXPECT_CALL(receiver,
                addRequested(std::dynamic_pointer_cast<yoyo::node_base>(container), id, 1));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);
    EXPECT_CALL(*container, acceptsChildren()).Times(3);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton,
                       { Qt::ControlModifier });
  }

  ON_CALL(*object, acceptsChildren()).WillByDefault(Return(true));
  {
    EXPECT_CALL(receiver,
                exchangeRequested(std::dynamic_pointer_cast<yoyo::node_base>(object), id));

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, id, Qt::RightButton, {});
  }
  {
    EXPECT_CALL(receiver,
                exchangeRequested(std::dynamic_pointer_cast<yoyo::node_base>(object), id));

    checkSuccessfulDND(object->widget(), op, Qt::MoveAction, id, Qt::RightButton, {});
  }
  {
    EXPECT_CALL(receiver,
                exchangeRequested(std::dynamic_pointer_cast<yoyo::node_base>(object), id));

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, id, Qt::RightButton,
                       { Qt::ShiftModifier });
  }
  {
    EXPECT_CALL(receiver,
                exchangeRequested(std::dynamic_pointer_cast<yoyo::node_base>(object), id));

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, id, Qt::RightButton,
                       { Qt::AltModifier });
  }
  {
    EXPECT_CALL(receiver,
                exchangeRequested(std::dynamic_pointer_cast<yoyo::node_base>(object), id));

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, id, Qt::RightButton,
                       { Qt::ControlModifier });
  }
  ON_CALL(*object, acceptsChildren()).WillByDefault(Return(false));
  ON_CALL(*container, acceptsChildren()).WillByDefault(Return(false));
  {
    EXPECT_CALL(*object, acceptsChildren());
    EXPECT_CALL(*container, acceptsChildren());

    checkUnsuccessfulEnter(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(*object, acceptsChildren());
    EXPECT_CALL(*container, acceptsChildren());

    checkUnsuccessfulEnter(object->widget(), op, Qt::MoveAction, id, Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(*object, acceptsChildren());
    EXPECT_CALL(*container, acceptsChildren());

    checkUnsuccessfulEnter(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton,
                           { Qt::ShiftModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren());
    EXPECT_CALL(*container, acceptsChildren());

    checkUnsuccessfulEnter(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton,
                           { Qt::AltModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren());
    EXPECT_CALL(*container, acceptsChildren());

    checkUnsuccessfulEnter(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton,
                           { Qt::ControlModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).Times(2);
    EXPECT_CALL(*container, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));

    checkUnsuccessfulMove(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).Times(2);
    EXPECT_CALL(*container, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));

    checkUnsuccessfulMove(object->widget(), op, Qt::MoveAction, id, Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).Times(2);
    EXPECT_CALL(*container, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));

    checkUnsuccessfulMove(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton,
                          { Qt::ShiftModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).Times(2);
    EXPECT_CALL(*container, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));

    checkUnsuccessfulMove(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton,
                          { Qt::AltModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).Times(2);
    EXPECT_CALL(*container, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));

    checkUnsuccessfulMove(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton,
                          { Qt::ControlModifier });
  }

  {
    EXPECT_CALL(*object, acceptsChildren()).Times(2);
    EXPECT_CALL(*container, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));

    checkUnsuccessfulDrop(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).Times(2);
    EXPECT_CALL(*container, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));

    checkUnsuccessfulDrop(object->widget(), op, Qt::MoveAction, id, Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).Times(2);
    EXPECT_CALL(*container, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));

    checkUnsuccessfulDrop(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton,
                          { Qt::ShiftModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).Times(2);
    EXPECT_CALL(*container, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));

    checkUnsuccessfulDrop(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton,
                          { Qt::AltModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).Times(2);
    EXPECT_CALL(*container, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));

    checkUnsuccessfulDrop(object->widget(), op, Qt::CopyAction, id, Qt::LeftButton,
                          { Qt::ControlModifier });
  }
}

TEST_F(guiNodeWidgetTest, dropDataElement)
{
  struct Receiver {
    MOCK_METHOD(void, dataRequested,
                (QString path, std::shared_ptr<yoyo::node_base> parent, int index,
                 bool connectDefaultInput, bool connectDefaultOutput,
                 bool connectDefaultAutoConnect));
    MOCK_METHOD(void, connectionChanged, (yoyo::properties::connection_t c));
  } receiver;

  QObject::connect(root.get(), &gui_node_mock::defaultForDataRequested,
                   [&receiver](auto path, auto parent, auto i, bool in, bool out, bool autoc) {
                     receiver.dataRequested(path, parent, i, in, out, autoc);
                   });
  QObject::connect(object.get(), &gui_node_mock::connectionChanged,
                   [&receiver](auto c) { receiver.connectionChanged(c); });
  QString path = "data.my.object";
  QPoint op { 10, 10 };
  /// \todo currently only works without layout in test: find out why.
  object->setLayoutDirection({ yoyo::types::layout_direction_t::FREE, true });
  ON_CALL(*object, acceptsChildren()).WillByDefault(Return(true));
  {
    EXPECT_CALL(receiver, dataRequested(path, std::dynamic_pointer_cast<yoyo::node_base>(object),
                                        -1, false, false, true));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::INT16, path,
                       Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(receiver, dataRequested(path, std::dynamic_pointer_cast<yoyo::node_base>(object),
                                        -1, false, false, true));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);

    checkSuccessfulDND(object->widget(), op, Qt::MoveAction, yoyo::types::value_t::INT16, path,
                       Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(receiver, dataRequested(path, std::dynamic_pointer_cast<yoyo::node_base>(object),
                                        -1, false, true, false));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::INT16, path,
                       Qt::LeftButton, { Qt::AltModifier });
  }
  {
    EXPECT_CALL(receiver, dataRequested(path, std::dynamic_pointer_cast<yoyo::node_base>(object),
                                        -1, true, false, false));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::INT16, path,
                       Qt::LeftButton, { Qt::ControlModifier });
  }
  {
    EXPECT_CALL(receiver, dataRequested(path, std::dynamic_pointer_cast<yoyo::node_base>(object),
                                        -1, true, true, false));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::INT16, path,
                       Qt::LeftButton, Qt::ShiftModifier);
  }
  {
    EXPECT_CALL(receiver, dataRequested(path, std::dynamic_pointer_cast<yoyo::node_base>(object),
                                        -1, true, true, false));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::INT16, path,
                       Qt::LeftButton, { Qt::AltModifier | Qt::ControlModifier });
  }
  ON_CALL(*object, acceptsChildren()).WillByDefault(Return(false));
  ON_CALL(*container, acceptsChildren()).WillByDefault(Return(true));
  {
    EXPECT_CALL(receiver, dataRequested(path, std::dynamic_pointer_cast<yoyo::node_base>(container),
                                        1, false, false, true));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);
    EXPECT_CALL(*container, acceptsChildren()).Times(3);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::INT16, path,
                       Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(receiver, dataRequested(path, std::dynamic_pointer_cast<yoyo::node_base>(container),
                                        1, false, false, true));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);
    EXPECT_CALL(*container, acceptsChildren()).Times(3);

    checkSuccessfulDND(object->widget(), op, Qt::MoveAction, yoyo::types::value_t::INT16, path,
                       Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(receiver, dataRequested(path, std::dynamic_pointer_cast<yoyo::node_base>(container),
                                        1, false, true, false));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);
    EXPECT_CALL(*container, acceptsChildren()).Times(3);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::INT16, path,
                       Qt::LeftButton, { Qt::AltModifier });
  }
  {
    EXPECT_CALL(receiver, dataRequested(path, std::dynamic_pointer_cast<yoyo::node_base>(container),
                                        1, true, false, false));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);
    EXPECT_CALL(*container, acceptsChildren()).Times(3);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::INT16, path,
                       Qt::LeftButton, { Qt::ControlModifier });
  }
  {
    EXPECT_CALL(receiver, dataRequested(path, std::dynamic_pointer_cast<yoyo::node_base>(container),
                                        1, true, true, false));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);
    EXPECT_CALL(*container, acceptsChildren()).Times(3);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::INT16, path,
                       Qt::LeftButton, Qt::ShiftModifier);
  }
  {
    EXPECT_CALL(receiver, dataRequested(path, std::dynamic_pointer_cast<yoyo::node_base>(container),
                                        1, true, true, false));
    EXPECT_CALL(*object, acceptsChildren()).Times(4);
    EXPECT_CALL(*container, acceptsChildren()).Times(3);

    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::INT16, path,
                       Qt::LeftButton, { Qt::AltModifier | Qt::ControlModifier });
  }
  ON_CALL(*object, supportsValueType(_)).WillByDefault(Return(true));
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::INT16)).Times(3);
    EXPECT_CALL(receiver, connectionChanged(_)).WillOnce(Invoke([path](auto c) {
      EXPECT_EQ(c._in.toStdString(), "");
      EXPECT_EQ(c._out.toStdString(), "");
      EXPECT_EQ(c._auto, path);
    }));
    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::INT16, path,
                       Qt::RightButton, {});
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::INT16)).Times(3);
    EXPECT_CALL(receiver, connectionChanged(_)).WillOnce(Invoke([path](auto c) {
      EXPECT_EQ(c._in, path);
      EXPECT_EQ(c._out, path);
      EXPECT_EQ(c._auto.toStdString(), "");
    }));
    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::INT16, path,
                       Qt::RightButton, { Qt::ShiftModifier });
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::INT16)).Times(3);
    EXPECT_CALL(receiver, connectionChanged(_)).WillOnce(Invoke([path](auto c) {
      EXPECT_EQ(c._in.toStdString(), "");
      EXPECT_EQ(c._out, path);
      EXPECT_EQ(c._auto.toStdString(), "");
    }));
    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::INT16, path,
                       Qt::RightButton, { Qt::AltModifier });
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::INT16)).Times(3);
    EXPECT_CALL(receiver, connectionChanged(_)).WillOnce(Invoke([path](auto c) {
      EXPECT_EQ(c._in, path);
      EXPECT_EQ(c._out.toStdString(), "");
      EXPECT_EQ(c._auto.toStdString(), "");
    }));
    checkSuccessfulDND(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::INT16, path,
                       Qt::RightButton, { Qt::ControlModifier });
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::INT16)).Times(3);
    EXPECT_CALL(receiver, connectionChanged(_)).WillOnce(Invoke([path](auto c) {
      EXPECT_EQ(c._in.toStdString(), "");
      EXPECT_EQ(c._out.toStdString(), "");
      EXPECT_EQ(c._auto, path);
    }));
    checkSuccessfulDND(object->widget(), op, Qt::MoveAction, yoyo::types::value_t::INT16, path,
                       Qt::RightButton, {});
  }

  ON_CALL(*object, acceptsChildren()).WillByDefault(Return(false));
  ON_CALL(*container, acceptsChildren()).WillByDefault(Return(false));
  ON_CALL(*object, supportsValueType(_)).WillByDefault(Return(false));
  {
    EXPECT_CALL(*object, acceptsChildren());
    EXPECT_CALL(*container, acceptsChildren());
    checkEnterUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::UINT8, path,
                           Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(*object, acceptsChildren());
    EXPECT_CALL(*container, acceptsChildren());
    checkEnterUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::UINT8, path,
                           Qt::LeftButton, { Qt::ShiftModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren());
    EXPECT_CALL(*container, acceptsChildren());
    checkEnterUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::UINT8, path,
                           Qt::LeftButton, { Qt::AltModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren());
    EXPECT_CALL(*container, acceptsChildren());
    checkEnterUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::UINT8, path,
                           Qt::LeftButton, { Qt::ControlModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren());
    EXPECT_CALL(*container, acceptsChildren());
    checkEnterUnsuccessful(object->widget(), op, Qt::MoveAction, yoyo::types::value_t::UINT8, path,
                           Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::STRING));
    checkEnterUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::STRING, path,
                           Qt::RightButton, {});
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::STRING));
    checkEnterUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::STRING, path,
                           Qt::RightButton, { Qt::ShiftModifier });
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::STRING));
    checkEnterUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::STRING, path,
                           Qt::RightButton, { Qt::AltModifier });
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::STRING));
    checkEnterUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::STRING, path,
                           Qt::RightButton, { Qt::ControlModifier });
  }

  {
    EXPECT_CALL(*object, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*container, acceptsChildren());
    checkMoveUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::UINT8, path,
                          Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*container, acceptsChildren());
    checkMoveUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::UINT8, path,
                          Qt::LeftButton, { Qt::ShiftModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*container, acceptsChildren());
    checkMoveUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::UINT8, path,
                          Qt::LeftButton, { Qt::AltModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*container, acceptsChildren());
    checkMoveUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::UINT8, path,
                          Qt::LeftButton, { Qt::ControlModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*container, acceptsChildren());
    checkMoveUnsuccessful(object->widget(), op, Qt::MoveAction, yoyo::types::value_t::UINT8, path,
                          Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::STRING))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
    checkMoveUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::STRING, path,
                          Qt::RightButton, {});
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::STRING))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
    checkMoveUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::STRING, path,
                          Qt::RightButton, { Qt::ShiftModifier });
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::STRING))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
    checkMoveUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::STRING, path,
                          Qt::RightButton, { Qt::AltModifier });
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::STRING))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
    checkMoveUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::STRING, path,
                          Qt::RightButton, { Qt::ControlModifier });
  }

  {
    EXPECT_CALL(*object, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*container, acceptsChildren());
    checkDropUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::UINT8, path,
                          Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*container, acceptsChildren());
    checkDropUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::UINT8, path,
                          Qt::LeftButton, { Qt::ShiftModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*container, acceptsChildren());
    checkDropUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::UINT8, path,
                          Qt::LeftButton, { Qt::AltModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*container, acceptsChildren());
    checkDropUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::UINT8, path,
                          Qt::LeftButton, { Qt::ControlModifier });
  }
  {
    EXPECT_CALL(*object, acceptsChildren()).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*container, acceptsChildren());
    checkDropUnsuccessful(object->widget(), op, Qt::MoveAction, yoyo::types::value_t::UINT8, path,
                          Qt::LeftButton, {});
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::STRING))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
    checkDropUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::STRING, path,
                          Qt::RightButton, {});
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::STRING))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
    checkDropUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::STRING, path,
                          Qt::RightButton, { Qt::ShiftModifier });
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::STRING))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
    checkDropUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::STRING, path,
                          Qt::RightButton, { Qt::AltModifier });
  }
  {
    EXPECT_CALL(*object, supportsValueType(yoyo::types::value_t::STRING))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
    checkDropUnsuccessful(object->widget(), op, Qt::CopyAction, yoyo::types::value_t::STRING, path,
                          Qt::RightButton, { Qt::ControlModifier });
  }
}
