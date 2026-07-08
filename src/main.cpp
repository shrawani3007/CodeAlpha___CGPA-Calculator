#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    // Enable High DPI scaling for modern screens
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);
    
    // Set Application styling and metadata
    app.setApplicationName("CGPA Calculator");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Antigravity");

    MainWindow window;
    window.setWindowIcon(QIcon(":/assets/icon.png")); // We will embed the icon via Qt Resource File (.qrc) or load it directly
    
    // Let's also try loading icon directly from file system as a fallback
    if (window.windowIcon().isNull()) {
        window.setWindowIcon(QIcon("assets/icon.png"));
    }

    window.show();
    return app.exec();
}
