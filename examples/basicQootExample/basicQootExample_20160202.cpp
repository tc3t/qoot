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
#include <TImage.h>
#include <TH1D.h>
#include <memory>
#include <iostream>

// QtGSI includes->
#include "TQApplication.h"
#include "TQRootApplication.h"
#include "TBrowser.h"
#include "TQRootCanvas.h"
#include "TQRootDialog.h"
// <- QtGSI includes

int qtGsiMain(int argc, char ** argv)
{
    gEnv->SetValue("Gui.Backend", "native");
    gEnv->SetValue("Gui.Factory", "native");
    TQApplication app("app",&argc,argv);
    TQRootApplication a( argc, argv, 0);
    //w->show();
    TBrowser browser;
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

    TQRootCanvas qrc;
    qrc.cd();
    const double x[] = { 1, 2, 3 };
    const double y[] = { 2, 3, 2 };
    TGraph graph(3, x, y);
    graph.Draw("AL*");
    qrc.show();

    TQRootDialog qrd(nullptr, "dialog", 0, &graph);
    qrd.show();

    a.exec();
    return 0;
}

int main(int argc, char* argv[])
{
    const double x[] = { 1, 2, 3 };
    const double y[] = { 2, 3, 2 };

    std::string sGui;
    std::cout << "Choose gui implementation (one of qtgui, qt, qtgsi, native): ";
    std::cin >> sGui;

    if (sGui == "qtgsi")
        return qtGsiMain(argc, argv);

    QApplication a(argc, argv);

    gEnv->SetValue("Gui.Factory", sGui.c_str());

    const bool bUseQt = (sGui != "native");

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

        pGraph->InsertPointAt(pGraph->GetN(), -1, 0);
        //pGraph->GetXaxis()->SetTimeFormat("DD.MM.YYYY HH:mm:dd");
        //pGraph->GetXaxis()->SetTimeFormat("%d\/%m\/%y%F2000-02-28 13:00:01"); // T�ll� ei kaadu
        //pGraph->GetXaxis()->SetTimeFormat("%d/%m/%y %H:%M:%S%F2000-02-28 13:00:01"); // Ei kaadu
        pGraph->GetXaxis()->SetTimeFormat("%d/%m/%y %H:%M:%S%F1970-01-01 00:00:00"); // Kaatuu jos on negatiivisia x-arvoja.
        //pGraph->GetXaxis()->SetTimeFormat("%d/%m/%y %H:%M:%S%F1970-00-00 00:00:00 GMT"); // Ei kaadu ainakaan samalla testill� kun edellinen. Erona se, ett� t�ss� tapauksessa
                                                                                         // k�ytet��n konversioon gmtime()-funktiota kun taas edellisess� k�yt�ss� on localtime().
        pGraph->GetXaxis()->SetTimeDisplay(1);
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

#if QOOT_32BIT_COLOR_T
    // Create an example to demonstrate 32-bit Color_t with alpha channel.
    // The example of canvas background is modified from example posted in https://root.cern.ch/phpBB3/viewtopic.php?p=31414#p31414
    TImage* pImg = TImage::Open("../../tutorials/image/rose512.jpg");
    TCanvas canvasForTransparencyDemonstration("td");
    if (pImg)
        pImg->Draw("x");

    //Create a transparent pad filling the full canvas
    TPad* p = new TPad("p", "p", 0, 0, 1, 1);
    p->SetFillStyle(4000);
    p->SetFrameFillStyle(4000);
    p->Draw();
    p->cd();

    // Create histogram with transparent filling.
    {
        TH1F* pHist = new TH1F("h", "Transparent histogram example", 100, -3, 3);
        pHist->SetFillColor(Color_t_FromRgba(0, 255, 255, 128));
        pHist->FillRandom("gaus", 5000);
        pHist = p->AdoptAndDrawPrimitive(pHist, "");
    }

    // Create another one
    {
        TH1F* pHist2 = new TH1F("h2", "test2", 100, -1, 1);
        pHist2->SetFillColor(Color_t_FromRgba(255, 0, 255, 128));
        pHist2->FillRandom("gaus", 5000);
        pHist2 = p->AdoptAndDrawPrimitive(pHist2, "same");
    }
#endif

    if (bUseQt)
        return a.exec();
    else
        return spRootApp->Run();
}
