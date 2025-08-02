#include "package_installer_app.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>

#include "package_manager.h"

PackageInstallerApp::PackageInstallerApp(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    screens = new QStackedWidget(this);
    mainLayout->addWidget(screens);

    screens->addWidget(createTitleScreen(screens));
    screens->addWidget(createPackageChooseScreen(screens));
}

auto PackageInstallerApp::nextScreen() -> void {
    if (currentScreenIdx < screens->count() - 1) {
        screens->setCurrentIndex(++currentScreenIdx);
    }
}

auto PackageInstallerApp::previousScreen() -> void {
    if (currentScreenIdx > 0) {
        screens->setCurrentIndex(--currentScreenIdx);
    }
}

auto PackageInstallerApp::installSelectedPackage() -> void {
    const int selectedItemIdx = packageChooser->currentIndex();
    if (selectedItemIdx == 0) {
        QMessageBox::warning(this, "Пакет не выбран", "Необходимо выбрать пакет для установки.");
        return;
    }

    const QSharedPointer<Package> selectedPackage = packageChooser->itemData(selectedItemIdx).value<QSharedPointer<Package>>();

    const auto [success, message] = PackageManager::getInstance().installPackage(selectedPackage);
    if (success) {
        QMessageBox::information(this, "Успех", message);
    } else {
        QMessageBox::critical(this, "Ошибка", message);
    }
}

auto PackageInstallerApp::updatePackageChooserVariants(const bool includeLibs) const -> void {
    packageChooser->clear();

    packageChooser->addItem("Выберите пакет...");

    for (auto package : PackageManager::getInstance().getRegisteredPackages(includeLibs)) {
        packageChooser->addItem(package->getPackageName(), QVariant::fromValue(package));
    }

    packageChooser->setCurrentIndex(0);
}

auto PackageInstallerApp::createTitleScreen(QWidget *parent) -> QWidget * {
    QWidget *screen = new QWidget(parent);

    QVBoxLayout *screenLayout = new QVBoxLayout(screen);

    QLabel *authorLabel = new QLabel("Свиридов Александр", screen);
    screenLayout->addWidget(authorLabel, 0, Qt::AlignCenter);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    screenLayout->addLayout(buttonLayout);

    QPushButton *nextButton = new QPushButton("Далее", screen);
    buttonLayout->addWidget(nextButton, 0, Qt::AlignRight);
    connect(nextButton, &QPushButton::clicked, this, &PackageInstallerApp::nextScreen);

    return screen;
}

auto PackageInstallerApp::createPackageChooseScreen(QWidget *parent) -> QWidget * {
    QWidget *screen = new QWidget(parent);


    QVBoxLayout *screenLayout = new QVBoxLayout(screen);

    screenLayout->addStretch(1);

    packageChooser = new QComboBox(screen);
    updatePackageChooserVariants(false);
    screenLayout->addWidget(packageChooser, 0, Qt::AlignCenter);

    QCheckBox *showLibs = new QCheckBox("Отображать библиотеки", screen);
    screenLayout->addWidget(showLibs, 0, Qt::AlignCenter);
    connect(showLibs, &QCheckBox::stateChanged, this, &PackageInstallerApp::updatePackageChooserVariants);

    screenLayout->addStretch(1);


    QHBoxLayout *buttonLayout = new QHBoxLayout();
    screenLayout->addLayout(buttonLayout);

    QPushButton *backButton = new QPushButton("Назад", screen);
    buttonLayout->addWidget(backButton, 0, Qt::AlignLeft);
    connect(backButton, &QPushButton::clicked, this, &PackageInstallerApp::previousScreen);

    QPushButton *installButton = new QPushButton("Установить", screen);
    buttonLayout->addWidget(installButton, 0, Qt::AlignRight);
    connect(installButton, &QPushButton::clicked, this, &PackageInstallerApp::installSelectedPackage);

    return screen;
}
