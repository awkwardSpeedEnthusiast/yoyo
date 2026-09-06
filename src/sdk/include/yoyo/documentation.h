#pragma once
#include "yoyo/yoyo_sdk_global.h"

#include <QString>
#include <QVariant>

#include <map>
#include <memory>
#include <tuple>

namespace yoyo
{
///
/// \brief This class handles a documentation for one node type.
///
/// This is a class to be used for documentation purposes. It documents one node-type for the
/// configuration tree.
/// The documentation class is a const class. It cannot be modified and creation is only allowed
/// via the nested class builder.
///
class YOYO_SDK_SHARED_EXPORT documentation final
{
public:
  ///
  /// \brief alias for information on properties
  ///
  /// For properties, documentation stores a title (to be retrieved with std::get<0>(property)), a
  /// general description, which is displayed in the help Ui and as a tool-tip in the property
  /// editor (std::get<1>(property)) and an optional value, which will be used as named tool-tip,
  /// if not left empty (std::get<2>(property)) and the default value for the property, which is
  /// used to define whether the property needs to be persisted.
  ///
  using property_t = std::tuple<QString, QString, QString, QVariant>;

public:
  ///
  /// Copy constructor
  ///
  documentation(documentation const&);
  ///
  /// Move constructor
  ///
  documentation(documentation&&) = default;
  ~documentation();

  ///
  /// \brief type
  ///
  /// That's the type name of the class which is described by this documentation.
  /// \return the type name.
  ///
  auto type() const -> QString;
  ///
  /// \brief type description
  ///
  /// This method returns the description of the node.
  /// \return the type description.
  ///
  auto typeDescription() const -> QString;
  ///
  /// \brief property
  ///
  /// This method returns a description for a specific property of the node. If a description is
  /// not defined for the described node, the elements of the returned object are empty/invalid.
  /// \param propertyName the name of the propert to retrieve the description for,
  /// \return the property description.
  ///
  auto property(QString const& propertyName) const -> property_t;
  ///
  /// \brief styling
  ///
  /// This method provides the styling description, if available for the node type in question.
  /// \return the styling description.
  ///
  auto styling() const -> QString;

public:
  ///
  /// \brief This class is used to create a documentation object.
  ///
  class YOYO_SDK_SHARED_EXPORT builder
  {
  public:
    ///
    /// \brief Constructor
    ///
    /// Create a builder for a specific node type.
    /// \param type the type name.
    ///
    builder(QString const& type);
    ///
    /// \brief property
    ///
    /// Add or adjust a description for a specific property.
    /// \param propertyName the name of the property to add,
    /// \param title the title of the property as to be shown in the gui,
    /// \param description the description of the property,
    /// \param tooltip the tooltip to be displayed in the editor,
    /// \param defaultValue default value for the property to prevent unnecessarily big files,
    /// \return this object.
    ///
    auto property(QString const& propertyName, QString const& title, QString const& description,
                  QString const& tooltip, QVariant const& defaultValue) -> builder&;
    ///
    /// \brief description
    ///
    /// Add or adjust the type description for the node.
    /// \param value the new description string,
    /// \return this object.
    ///
    auto description(QString const& value) -> builder&;
    ///
    /// \brief styling
    ///
    /// Add or adjust the styling description for the node.
    /// \param value the new styling description string,
    /// \return this object.
    ///
    auto styling(QString const& value) -> builder&;
    ///
    /// \brief build
    ///
    /// Create a documentation object with the current settings.
    /// \return a shared pointer to the new documentation object.
    ///
    auto build() -> std::shared_ptr<documentation>;

  private:
    QString _type;
    QString _description;
    QString _styling;
    std::map<QString, property_t> _properties;
  };

private:
  explicit documentation(QString const& type, QString const& description, QString const& styling,
                         std::map<QString, property_t> const& properties);

  struct impl;
  std::unique_ptr<impl> _p;

  friend class documentation::builder;
};
} // namespace yoyo
