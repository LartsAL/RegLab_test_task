#include <QApplication>
#include <QWidget>
#include <QMessageBox>
#include <QStringList>
#include <QProcess>
#include <QSharedPointer>
#include <unistd.h>

#include "package_installer_app.h"
#include "package_manager.h"

auto areRequiredUtilsInstalled(const QStringList &requiredUtils) -> bool {
    for (const auto& utilName : requiredUtils) {
        QProcess process;
        process.start(utilName, {"--version"});
        if (!process.waitForFinished() || process.exitCode() != 0) {
            return false;
        }
    }
    return true;
}

auto isRunningAsRoot() -> bool {
    return geteuid() == 0;
}

auto registerPackages() -> void {
    PackageManager &packageManager = PackageManager::getInstance();

    packageManager.registerPackage("htop", ":/packages/htop_3.3.0-4build1_amd64.deb");
    packageManager.registerPackage("neofetch", ":/packages/neofetch_7.1.0-4_all.deb");
    // Dependency for cava
    QSharedPointer<Package> libiniparser1 = packageManager.registerPackage("libiniparser1", ":/packages/libiniparser1_4.1-7ubuntu0.1_amd64.deb", true);
    packageManager.registerPackage("cava", ":/packages/cava_0.7.4-1build2_amd64.deb", false, {libiniparser1});
}

int main(int argc, char *argv[]) {
    QStringList requiredUtils = {"dpkg", "dpkg-query"};
    if (!areRequiredUtilsInstalled(requiredUtils)) {
        QApplication app(argc, argv);

        QMessageBox::critical(
            nullptr,
            "Необходимые утилиты",
            "Для работы приложения должны быть установлены следующие утилиты: "
            + requiredUtils.join(", ")
        );

        return 1;
    }

    if (!isRunningAsRoot()) {
        QApplication app(argc, argv);

        QMessageBox::critical(
            nullptr,
            "Повышение привилегий",
            "Для работы приложения необходимы повышенные привилегии.\n"
            "Пожалуйста, перезапустите программу с правами root."
        );

        return 1;
    }

    registerPackages();

    QApplication app(argc, argv);

    QWidget *window = new PackageInstallerApp();

    window->setWindowTitle("Тестовое задание для РегЛаб");
    window->resize(600, 400);
    window->show();

    return app.exec();
}
