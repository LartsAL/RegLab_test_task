#pragma once

#include <QString>
#include <QVector>
#include <QSharedPointer>
#include <QWeakPointer>

#include "package.h"

class PackageManager {
public:
    struct InstallationResult {
        bool success;
        QString message;
    };

    auto static getInstance() -> PackageManager &;

    auto registerPackage(const QString &packageName, const QString &resourcePath, bool isLib = false,
                         const QVector<QWeakPointer<Package>> &dependencies = QVector<QWeakPointer<Package>>()) -> QSharedPointer<Package>;

    auto getRegisteredPackages(bool includeLibs = false) -> QVector<QSharedPointer<Package>>;

    static auto isPackageInstalled(const QString &packageName) -> bool;

    static auto installPackage(const QSharedPointer<Package> &package) -> InstallationResult;

private:
    PackageManager();

    QVector<QSharedPointer<Package>> registeredPackages;
};
