#include <QApplication>
#include <QInputDialog>
#include <QGridLayout>
#include <QSlider>
#include <QTextEdit>
#include <TQtWidget.h>
#include <TGraph.h>
#include <TEnv.h>
#include <TGQt.h>
#include <TApplication.h>
#include <memory>

int main(int argc, char* argv[])
{
    const double x[] = { 1, 2, 3 };
    const double y[] = { 2, 3, 2 };

    QApplication a(argc, argv);

    const auto sGuiFactory = QInputDialog::getItem(nullptr,
                                                    "Choose Gui.Factory",
                                                    "Choose Gui.Factory",
                                                    QStringList() << "qtgui" << "qt");

    gEnv->SetValue("Gui.Factory", sGuiFactory.toLatin1().constData());

    const bool bUseQt = (sGuiFactory != "native");

    std::unique_ptr<QWidget> spWidget;

    if (bUseQt)
    {
        spWidget.reset(new QWidget);
        auto& w = *spWidget;
        auto pMainLayout = new QGridLayout(&w);

        auto& widget = *(new TQtWidget(&w));
        widget.setWindowTitle("TQtWidget");
        widget.cd();

        auto pEdit = new QTextEdit("This is an example to use TQtWidget in a grid layout", &w);
        pEdit->setMaximumHeight(50);
        pMainLayout->addWidget(pEdit, 0, 0, 1, 2);
        pMainLayout->addWidget(&widget, 1, 0);
        pMainLayout->addWidget(new QSlider(Qt::Vertical, &w), 1, 1);
        pMainLayout->addWidget(new QSlider(Qt::Horizontal, &w), 2, 0);

        auto pGraph = new TGraph(3, x, y);
        pGraph->Draw("AL*");
        pGraph->SetTitle("A graph in TQtWidget");
        pGraph->SetBit(kCanDelete);

        w.resize(800, 600);
        w.show();
    }
    else
        gEnv->SetValue("Gui.Backend", "native");

    std::unique_ptr<TApplication> spRootApp;
    if (!bUseQt)
        spRootApp.reset(new TApplication("app", &argc, argv));

    //TQtWidget::InitRint(); // Note: If no TQtWidget is created before creating TCanvas, one needs to init ROOT application manually.
    TCanvas canvas("canvas", "TCanvas", 800, 600);
    canvas.cd();
    TGraph graph2(3, x, y);
    graph2.Draw("AL*");
    graph2.SetTitle("A graph in canvas widget");

    if (bUseQt)
        return a.exec();
    else
        return spRootApp->Run();
}
