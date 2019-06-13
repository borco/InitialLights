#pragma once

#include <functional>

class QJsonObject;
class QString;

namespace il {

void safeRead(const QJsonObject& json, const QString& tag, std::function<void(const QString&)> functor);
void safeRead(const QJsonObject& json, const QString& tag, std::function<void(int)> functor);

} // namespace il
