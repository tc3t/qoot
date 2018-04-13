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

#include "TAxis.h"
#include "TGaxis.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLine.h"
#include "TLatex.h"
#include <time.h>
#include <stdio.h>

TString stime(time_t* t, bool utc = false, bool display_time_zone = true)
{
    struct tm* tt;
    if (utc) tt = gmtime(t);
    else     tt = localtime(t);
    char buf[256];
    if (display_time_zone) strftime(buf, sizeof(buf), "%H:%M:%S %Z", tt);
    else                   strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tt);
    return TString(buf);
}

int main(int argc, char* argv[])
{
#if 0
        TApplication app("a", &argc, argv);
        double f = 1.8;

        TCanvas* c = new TCanvas;

        TLatex* tex1 = new TLatex;
        tex1->SetNDC();
        tex1->SetTextFont(102);
        tex1->SetTextSize(0.07*f);

        TLatex* tex3 = new TLatex;
        tex3->SetNDC();
        tex3->SetTextFont(102);
        tex3->SetTextSize(0.07*f);
        tex3->SetTextColor(kBlue+2);

        TLatex* tex2 = new TLatex;
        tex2->SetNDC();
        tex2->SetTextFont(102);
        tex2->SetTextSize(0.07*f);
        tex2->SetTextColor(kOrange+3);

        time_t offset[] = {0,                   0, 1325376000, 1341100800};
        time_t t[]      = {1331150400, 1336417200,          0, 36000};

        c->SetTopMargin(0); c->SetBottomMargin(0);
        c->SetLeftMargin(0); c->SetRightMargin(0);
        c->Divide(2, 4, -1, -1);
        TLine l;
        l.DrawLine(0.5, 0, 0.5, 1.);

        for(int i = 0; i < 4; ++i){
            for(int gmt = 0; gmt < 2; ++gmt){
                const char* opt = (gmt ? "gmt" : "local");
                TVirtualPad* p = c->cd(2*i + gmt + 1);
                p->SetTopMargin(0); p->SetBottomMargin(0);
                p->SetLeftMargin(0); p->SetRightMargin(0);
                p->SetFillStyle(4000);

                TGaxis* ga = new TGaxis (.4, .25, 5., .25, t[i], t[i] + 1,  1, "t");
                ga->SetTimeFormat("TGaxis label: #color[2]{%Y-%m-%d %H:%M:%S}");
                ga->SetLabelFont(102);
                ga->SetLabelColor(kBlue+2);

                ga->SetTimeOffset(offset[i], opt);
                ga->SetLabelOffset(0.04*f);
                ga->SetLabelSize(0.07*f);
                ga->SetLineColor(0);
                ga->Draw();

                // Get offset string of axis time format: there is not acccessor
                // to time format in TGaxis.
                // Assumes TAxis use the same format.
                TAxis a(10, 0, 1600000000);
                a.SetTimeOffset(offset[i], opt);
                const char* offsettimeformat = a.GetTimeFormat();

                char buf[256];
                if (offset[i] < t[i]) {
                    sprintf(buf, "#splitline{%s, %s}{offset: %ld, option %s}",
                        stime(t+i).Data(), stime(t+i, true).Data(), offset[i], opt);
                } else {
                    int h = t[i] / 3600;
                    int m = (t[i] - 3600 * h) / 60 ;
                    int s = (t[i] - h * 3600 - m * 60);
                    sprintf(buf, "#splitline{%d h %d m %d s}{offset: %s, option %s}",
                        h, m, s, stime(offset + i, gmt).Data(), opt);
                }
                tex1->DrawLatex(.01, .75, buf);
                tex2->DrawLatex(.01, .50, offsettimeformat);
                time_t t_ = t[i] + offset[i];
                sprintf(buf, "Expecting:    #color[2]{%s}", stime(&t_, gmt, false).Data());
                tex3->DrawLatex(.01, .24, buf);
                if(i > 0) l.DrawLine(0, 0.95, 1, 0.95);
            }
        }

        app.Run();

#else


    const double x[] = { 1, 200, 300 };
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

        //pGraph->InsertPointAt(pGraph->GetN(), -1, 0);
        //pGraph->GetXaxis()->SetTimeFormat("DD.MM.YYYY HH:mm:dd");
        //pGraph->GetXaxis()->SetTimeFormat("%d\/%m\/%y%F2000-02-28 13:00:01"); // Tällä ei kaadu
        //pGraph->GetXaxis()->SetTimeFormat("%d/%m/%y %H:%M:%S%F2000-02-28 13:00:01"); // Ei kaadu
        //pGraph->GetXaxis()->SetTimeFormat("%d/%m/%y %H:%M:%S%F1970-01-01 00:00:00"); // Kaatuu jos on negatiivisia x-arvoja.
        //pGraph->GetXaxis()->SetTimeFormat("%d/%m/%y %H:%M:%S%F1970-00-00 00:00:00 GMT"); // Ei kaadu ainakaan samalla testillä kun edellinen. Erona se, että tässä tapauksessa
                                                                                         // käytetään konversioon gmtime()-funktiota kun taas edellisessä käytössä on localtime().

        pGraph->GetXaxis()->SetTimeFormat("%d/%m/%y %H:%M:%S%F2016-08-21 12:00:00");
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

#if 0 // Enable this to make gQt interpret const char* params passed to graph titles etc. as UTF-8 bytes)
    if (gQt)
        gQt->SetTextDecoder("UTF-8");
#endif

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
#endif
}
