#include "package_manager.h"

#include <QString>
#include <QVector>
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QTemporaryFile>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QDebug>

auto PackageManager::getInstance() -> PackageManager & {
    static PackageManager instance;
    return instance;
}

auto PackageManager::registerPackage(const QString &packageName, const QString &resourcePath,
                                     const bool isLib, const QVector<QWeakPointer<Package>> &dependencies) -> QSharedPointer<Package> {
    auto package = QSharedPointer<Package>::create(packageName, resourcePath, isLib, dependencies);
    registeredPackages.append(package);
    return package;
}

auto PackageManager::getRegisteredPackages(const bool includeLibs) -> QVector<QSharedPointer<Package>> {
    QVector<QSharedPointer<Package>> result;
    for (auto package : registeredPackages) {
        if (package->isLibrary()) {
            if (includeLibs) {
                result.append(package);
            }
        } else {
            result.append(package);
        }
    }
    return result;
}

auto PackageManager::isPackageInstalled(const QString &packageName) -> bool {
    QProcess process;
    process.start("dpkg-query", {"-W", packageName});
    process.waitForFinished();
    return process.exitCode() == 0;
}

auto PackageManager::installPackage(const QSharedPointer<Package> &package) -> InstallationResult {
    const QString packageName = package->getPackageName();

    if (isPackageInstalled(packageName)) {
        qInfo() << "Пакет" << packageName << "пропущен, так как уже установлен";
        return {true, "Пакет " + packageName + " уже установлен."};
    }

    qInfo() << "Выполняется установка:" << packageName;

    for (const QSharedPointer<Package> dependency: package->getDependencies()) {
        const InstallationResult result = installPackage(dependency);
        if (!result.success) {
            qWarning() << "Не удалось установить зависимость:" << dependency->getPackageName();
            return result;
        }
    }

    const QString packageResourcePath = package->getResourcePath();
    QFile resourceFile(packageResourcePath);
    if (!resourceFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось получить доступ к ресурсу:" << packageResourcePath;
        return {false, "Не удалось извлечь пакет " + packageName + "."};
    }

    QTemporaryFile tmpFile(QDir::tempPath() + "/" + packageName + ".tmp");
    if (!tmpFile.open()) {
        qWarning() << "Не удалось создать" << tmpFile.fileName();
        return {false, "Не удалось создать временный файл для установки пакета" + packageName + "."};
    }

    tmpFile.write(resourceFile.readAll());
    if (tmpFile.error() != QFile::NoError) {
        qWarning() << "Не удалось скопировать пакет во временный файл";
        return {false, "Не удалось скопировать пакет во временный файл."};
    }
    tmpFile.close();

    QProcess process;
    process.start("dpkg", {"-i", tmpFile.fileName()});

    if (!process.waitForFinished(-1) || process.exitCode() != 0) {
        qWarning() << "Ошибка установки:" << process.errorString();
        return {false, "Не удалось установить пакет" + packageName + "."};
    }

    qInfo() << "Установлен пакет:" << packageName;
    return {true, "Пакет " + packageName + " установлен."};
}

PackageManager::PackageManager() = default;
