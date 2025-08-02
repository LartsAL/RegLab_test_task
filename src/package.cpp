#include "package.h"

#include <QString>
#include <QVector>
#include <QWeakPointer>

Package::Package(const QString &packageName, const QString &resourcePath, const bool isLib,
                 const QVector<QWeakPointer<Package>> &dependencies)
    : packageName(packageName), resourcePath(resourcePath), isLib(isLib), dependencies(dependencies) {}

auto Package::getPackageName() const -> QString {
    return packageName;
}

auto Package::getResourcePath() const -> QString {
    return resourcePath;
}

auto Package::isLibrary() const -> bool {
    return isLib;
}

auto Package::getDependencies() const -> QVector<QWeakPointer<Package>> {
    return dependencies;
}
