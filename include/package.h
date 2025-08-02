#pragma once

#include <QVector>
#include <QString>
#include <QWeakPointer>

class Package {
public:
    Package(const QString &packageName, const QString &resourcePath, bool isLib = false,
            const QVector<QWeakPointer<Package>> &dependencies = QVector<QWeakPointer<Package>>());

    auto getPackageName() const -> QString;

    auto getResourcePath() const -> QString;

    auto isLibrary() const -> bool;

    auto getDependencies() const -> QVector<QWeakPointer<Package>>;

private:
    QString packageName;
    QString resourcePath;
    bool isLib;
    QVector<QWeakPointer<Package>> dependencies;
};

Q_DECLARE_METATYPE(QSharedPointer<Package>)
Q_DECLARE_METATYPE(QWeakPointer<Package>)
