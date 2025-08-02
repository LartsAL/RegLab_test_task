#pragma once

#include <QWidget>
#include <QStackedWidget>
#include <QComboBox>

class PackageInstallerApp : public QWidget {
    Q_OBJECT

public:
    explicit PackageInstallerApp(QWidget *parent = nullptr);

private slots:
    auto nextScreen() -> void;

    auto previousScreen() -> void;

    auto installSelectedPackage() -> void;

    auto updatePackageChooserVariants(bool includeLibs = false) const -> void;

private:
    auto createTitleScreen(QWidget *parent) -> QWidget *;

    auto createPackageChooseScreen(QWidget *parent) -> QWidget *;

    QStackedWidget *screens;
    QComboBox *packageChooser;
    int currentScreenIdx = 0;
};
