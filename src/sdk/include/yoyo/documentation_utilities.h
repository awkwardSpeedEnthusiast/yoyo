#pragma once
#include "yoyo/documentation.h"
#include "yoyo/node_base.h"
#include "yoyo/yoyo_sdk_global.h"

#include <type_traits>
namespace yoyo
{
///
/// \brief add property descriptions to builder
///
/// This template function adds property documentation to the provided documentation builder object
/// according to the template parameter. Please note, that only types defined in the sdk are
/// instanitated.
/// \param builder the builder object to add the property descriptions to
///
template <typename T, typename = std::enable_if<std::is_base_of_v<yoyo::node_base, T>>>
auto YOYO_SDK_SHARED_EXPORT updateDocumentation(documentation::builder& builder) -> void;
} // namespace yoyo
