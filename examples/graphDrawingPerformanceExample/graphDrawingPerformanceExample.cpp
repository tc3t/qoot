/*

    TGraph drawing performance test example.

    Note: This file includes code adapted from TGraphPainter.cxx, which is under LGPL-license.
*/

#include <QApplication>
#include <QGridLayout>
#include <QTextEdit>
#include <QPushButton>
#include <TQtWidget.h>
#include <TGraph.h>
#include <TEnv.h>
#include <TGQt.h>
#include <TApplication.h>
#include <TImage.h>
#include <TH1D.h>
#include <memory>
#include <iostream>
#include <QMainWindow>
#include <TCanvas.h>
#include <QElapsedTimer>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QSpacerItem>
#include <QComboBox>

#include <TList.h>
#include <TF1.h>
#include <TMath.h>
#include <TStyle.h>

class TestGraph : public TGraph
{
public:
    typedef TGraph BaseClass;

    TestGraph() :
        m_paintMethod(0)
    {}

    void SetPaintMethod(int i)
    {
        m_paintMethod = i;
    }

    Option_t* GetDrawOption() const override
    {
        return m_sPaintOpt.c_str();
    }

    void SetDrawOption(Option_t* opt) override
    {
        m_sPaintOpt = opt;
    }

    void Paint(Option_t* opt) override
    {
        opt = GetDrawOption(); // Hack.
        switch (m_paintMethod)
        {
            case 0: BaseClass::Paint(opt); break;
            case 1: RestrictedPaintGraph(this, GetN(), GetX(), GetY(), opt); break;
            case 2: break;
        }
    }

    // Adapted from TGraphPainter::ComputeLogs 
    void ComputeLogs(Int_t npoints, Int_t opt, Double_t* gxworkl, const Double_t* gxwork, Double_t* gyworkl, const Double_t* gywork)
    {
        Int_t i;
        memcpy(gxworkl, gxwork, npoints * 8);
        memcpy(gyworkl, gywork, npoints * 8);
        if (gPad->GetLogx()) {
            for (i = 0; i<npoints; i++) {
                if (gxworkl[i] > 0) gxworkl[i] = TMath::Log10(gxworkl[i]);
                else                gxworkl[i] = gPad->GetX1();
            }
        }
        if (!opt && gPad->GetLogy()) {
            for (i = 0; i<npoints; i++) {
                if (gyworkl[i] > 0) gyworkl[i] = TMath::Log10(gyworkl[i]);
                else                gyworkl[i] = gPad->GetY1();
            }
        }
    }

    // Adapted from TGraphPainter::PaintGraph()
    void RestrictedPaintGraph(TGraph *theGraph, Int_t npoints, const Double_t *x, const Double_t *y, Option_t *chopt)
    {
        Int_t optionLine, optionAxis, optionCurve, optionStar, optionMark;
        Int_t optionBar, optionR, optionOne, optionE;
        Int_t optionFill, optionZ, optionCurveFill;
        //Int_t i, npt, nloop;
        Int_t drawtype = 0;
        //Double_t xlow, xhigh, ylow, yhigh;
        //Double_t barxmin, barxmax, barymin, barymax;
        Double_t uxmin, uxmax;
        //Double_t x1, xn, y1, yn;
        //Double_t dbar, bdelta;
        Int_t theNpoints = theGraph->GetN();

        if (npoints <= 0) {
            Error("PaintGraph", "illegal number of points (%d)", npoints);
            return;
        }
        auto pPad = gPad;
        if (!pPad)
        {
            Error("RestrictedPaintGraph", "gPad is null.");
            return;
        }
        auto& rPad = *pPad;
        TString opt = chopt;
        opt.ToUpper();
        opt.ReplaceAll("SAME", "");

        if (opt.Contains("L")) optionLine = 1;  else optionLine = 0;
        if (opt.Contains("A")) optionAxis = 1;  else optionAxis = 0;
        if (opt.Contains("C")) optionCurve = 1;  else optionCurve = 0;
        if (opt.Contains("*")) optionStar = 1;  else optionStar = 0;
        if (opt.Contains("P")) optionMark = 1;  else optionMark = 0;
        if (opt.Contains("B")) optionBar = 1;  else optionBar = 0;
        if (opt.Contains("R")) optionR = 1;  else optionR = 0;
        if (opt.Contains("1")) optionOne = 1;  else optionOne = 0;
        if (opt.Contains("F")) optionFill = 1;  else optionFill = 0;
        if (opt.Contains("2") || opt.Contains("3") ||
            opt.Contains("4") || opt.Contains("5")) optionE = 1;  else optionE = 0;
        optionZ = 0;

        // If no "drawing" option is selected and if chopt<>' ' nothing is done.
        if (optionLine + optionFill + optionCurve + optionStar + optionMark + optionBar + optionE == 0) {
            if (!chopt[0])  optionLine = 1;
            else   return;
        }

        if (optionStar) theGraph->SetMarkerStyle(3);

        optionCurveFill = 0;
        if (optionCurve && optionFill) {
            optionCurveFill = 1;
            optionFill = 0;
        }

        // Draw the Axis.
        Double_t rwxmin, rwxmax, rwymin, rwymax/*, maximum, minimum, dx, dy*/;
        if (optionAxis) {
#if 0 // Disable due to use of histogram.
            if (theGraph->GetHistogram()) {
                rwxmin = rPad.GetUxmin();
                rwxmax = rPad.GetUxmax();
                rwymin = rPad.GetUymin();
                rwymax = rPad.GetUymax();
                minimum = theGraph->GetHistogram()->GetMinimumStored();
                maximum = theGraph->GetHistogram()->GetMaximumStored();
                if (minimum == -1111) { //this can happen after unzooming
                    minimum = theGraph->GetHistogram()->GetYaxis()->GetXmin();
                    theGraph->GetHistogram()->SetMinimum(minimum);
                }
                if (maximum == -1111) {
                    maximum = theGraph->GetHistogram()->GetYaxis()->GetXmax();
                    theGraph->GetHistogram()->SetMaximum(maximum);
                }
                uxmin = rPad.PadtoX(rwxmin);
                uxmax = rPad.PadtoX(rwxmax);
            }
            else {

                theGraph->ComputeRange(rwxmin, rwymin, rwxmax, rwymax);  //this is redefined in TGraphErrors

                if (rwxmin == rwxmax) rwxmax += 1.;
                if (rwymin == rwymax) rwymax += 1.;
                dx = 0.1*(rwxmax - rwxmin);
                dy = 0.1*(rwymax - rwymin);
                uxmin = rwxmin - dx;
                uxmax = rwxmax + dx;
                minimum = rwymin - dy;
                maximum = rwymax + dy;
            }
            if (theGraph->GetMinimum() != -1111) rwymin = minimum = theGraph->GetMinimum();
            if (theGraph->GetMaximum() != -1111) rwymax = maximum = theGraph->GetMaximum();
            if (uxmin < 0 && rwxmin >= 0) uxmin = 0.9*rwxmin;
            if (uxmax > 0 && rwxmax <= 0) {
                if (rPad.GetLogx()) uxmax = 1.1*rwxmax;
                else                 uxmax = 0;
            }
            if (minimum < 0 && rwymin >= 0) minimum = 0.9*rwymin;
            if (maximum > 0 && rwymax <= 0) {
                //if(rPad.GetLogy()) maximum = 1.1*rwymax;
                //else                maximum = 0;
            }
            if (minimum <= 0 && rPad.GetLogy()) minimum = 0.001*maximum;
            if (uxmin <= 0 && rPad.GetLogx()) {
                if (uxmax > 1000) uxmin = 1;
                else              uxmin = 0.001*uxmax;
            }
            rwymin = minimum;
            rwymax = maximum;

            // Create a temporary histogram and fill each bin with the
            // function value.
            char chopth[8] = " ";
            if (strstr(chopt, "x+")) strncat(chopth, "x+", 2);
            if (strstr(chopt, "y+")) strncat(chopth, "y+", 2);
            if (!theGraph->GetHistogram()) {
                // the graph is created with at least as many bins as there are
                // points to permit zooming on the full range.
                rwxmin = uxmin;
                rwxmax = uxmax;
                npt = 100;
                if (theNpoints > npt) npt = theNpoints;
                TH1F *h = new TH1F(Form("%s_h", GetName()), GetTitle(), npt, rwxmin, rwxmax);
                theGraph->SetHistogram(h);
                if (!theGraph->GetHistogram()) return;
                theGraph->GetHistogram()->SetMinimum(rwymin);
                theGraph->GetHistogram()->SetMaximum(rwymax);
                theGraph->GetHistogram()->GetYaxis()->SetLimits(rwymin, rwymax);
                theGraph->GetHistogram()->SetBit(TH1::kNoStats);
                theGraph->GetHistogram()->SetDirectory(0);
                theGraph->GetHistogram()->Paint(chopth); // Draw histogram axis, title and grid
            }
            else {
                if (rPad.GetLogy()) {
                    theGraph->GetHistogram()->SetMinimum(rwymin);
                    theGraph->GetHistogram()->SetMaximum(rwymax);
                    theGraph->GetHistogram()->GetYaxis()->SetLimits(rwymin, rwymax);
                }
                //theGraph->GetHistogram()->Paint(chopth); // Draw histogram axis, title and grid
            }
#endif
        } // if (optionAxis)

        {
            Double_t x0, y0, x1, y1;
            ComputeRange(x0, y0, x1, y1);
            rPad.Range(x0, y0, x1, y1);
            rPad.RangeAxis(x0, y0, x1, y1);
        }

        // Set Clipping option
        rPad.SetBit(TGraph::kClipFrame, theGraph->TestBit(TGraph::kClipFrame));

#if 0 // Disabled as irrelevant.
        TF1 *fit = 0;
        TList *functions = theGraph->GetListOfFunctions();
        TObject *f;
        if (functions) {
            f = (TF1*)functions->First();
            if (f) {
                if (f->InheritsFrom(TF1::Class())) fit = (TF1*)f;
            }
            TIter   next(functions);
            while ((f = (TObject*)next())) {
                if (f->InheritsFrom(TF1::Class())) {
                    fit = (TF1*)f;
                    break;
                }
            }
        }
        //if (fit) PaintStats(theGraph, fit);
#endif

        rwxmin = rPad.GetUxmin();
        rwxmax = rPad.GetUxmax();
        rwymin = rPad.GetUymin();
        rwymax = rPad.GetUymax();
        uxmin = rPad.PadtoX(rwxmin);
        uxmax = rPad.PadtoX(rwxmax);
        /* // Disable due to use of histogram.
        if (theGraph->GetHistogram() && !theGraph->InheritsFrom("TGraphPolar")) {
            maximum = theGraph->GetHistogram()->GetMaximum();
            minimum = theGraph->GetHistogram()->GetMinimum();
        }
        else {
            maximum = rPad.PadtoY(rwymax);
            minimum = rPad.PadtoY(rwymin);
        }
        */

        // Set attributes
        theGraph->TAttLine::Modify();
        theGraph->TAttFill::Modify();
        theGraph->TAttMarker::Modify();


        if (optionLine)
        {
            std::vector<double> xMod(x, x + npoints);
            std::vector<double> yMod(y, y + npoints);
            rPad.PaintPolyLine(npoints, xMod.data(), yMod.data());
        }
#if 0 // Disabled due to use of gxworkl et al.
        // Draw the graph with a polyline or a fill area
        auto gxwork = new Double_t[2 * npoints + 10];
        auto gywork = new Double_t[2 * npoints + 10];
        auto gxworkl = new Double_t[2 * npoints + 10];
        auto gyworkl = new Double_t[2 * npoints + 10];

        if (optionLine || optionFill) {
            rPad.PaintPolyLine(npoints, x, y);
            x1 = x[0];
            xn = x[npoints - 1];
            y1 = y[0];
            yn = y[npoints - 1];
            nloop = npoints;
            if (optionFill && (xn != x1 || yn != y1)) nloop++;
            npt = 0;
            for (i = 1; i <= nloop; i++) {
                if (i > npoints) {
                    gxwork[npt] = gxwork[0];  gywork[npt] = gywork[0];
                }
                else {
                    gxwork[npt] = x[i - 1];      gywork[npt] = y[i - 1];
                    npt++;
                }
                if (i == nloop) {
                    ComputeLogs(npt, optionZ, gxworkl, gxwork, gyworkl, gywork);
                    Int_t bord = gStyle->GetDrawBorder();
                    if (optionR) {
                        if (optionFill) {
                            rPad.PaintFillArea(npt, gyworkl, gxworkl);
                            if (bord) rPad.PaintPolyLine(npt, gyworkl, gxworkl);
                        }
                        else {
                            //if (TMath::Abs(theGraph->GetLineWidth())>99) PaintPolyLineHatches(theGraph, npt, gyworkl, gxworkl);
                            rPad.PaintPolyLine(npt, gyworkl, gxworkl);
                        }
                    }
                    else {
                        if (optionFill) {
                            rPad.PaintFillArea(npt, gxworkl, gyworkl);
                            if (bord) rPad.PaintPolyLine(npt, gxworkl, gyworkl);
                        }
                        else {
                            //if (TMath::Abs(theGraph->GetLineWidth())>99) PaintPolyLineHatches(theGraph, npt, gxworkl, gyworkl);
                            rPad.PaintPolyLine(npt, gxworkl, gyworkl);
                        }
                    }
                    gxwork[0] = gxwork[npt - 1];  gywork[0] = gywork[npt - 1];
                    npt = 1;
                }
            }
        }
#endif

        // Draw the graph with a smooth Curve. Smoothing via Smooth
#if 0 // Disabled due to use Smooth() and gxworkl et al.
        if (optionCurve) {
            x1 = x[0];
            xn = x[npoints - 1];
            y1 = y[0];
            yn = y[npoints - 1];
            drawtype = 1;
            nloop = npoints;
            if (optionCurveFill) {
                drawtype += 1000;
                if (xn != x1 || yn != y1) nloop++;
            }
            if (!optionR) {
                npt = 0;
                for (i = 1; i <= nloop; i++) {
                    if (i > npoints) {
                        gxwork[npt] = gxwork[0];  gywork[npt] = gywork[0];
                    }
                    else {
                        gxwork[npt] = x[i - 1];      gywork[npt] = y[i - 1];
                        npt++;
                    }
                    ComputeLogs(npt, optionZ, gxworkl, gxwork, gyworkl, gywork);
                    if (gyworkl[npt - 1] < rwymin || gyworkl[npt - 1] > rwymax) {
                        if (npt > 2) {
                            ComputeLogs(npt, optionZ, gxworkl, gxwork, gyworkl, gywork);
                            Smooth(theGraph, npt, gxworkl, gyworkl, drawtype);
                        }
                        gxwork[0] = gxwork[npt - 1]; gywork[0] = gywork[npt - 1];
                        npt = 1;
                        continue;
                    }
                }
                if (npt > 1) {
                    ComputeLogs(npt, optionZ);
                    Smooth(theGraph, npt, gxworkl, gyworkl, drawtype);
                }
            }
            else {
                drawtype += 10;
                npt = 0;
                for (i = 1; i <= nloop; i++) {
                    if (i > npoints) {
                        gxwork[npt] = gxwork[0];  gywork[npt] = gywork[0];
                    }
                    else {
                        if (y[i - 1] < minimum || y[i - 1] > maximum) continue;
                        if (x[i - 1] < uxmin || x[i - 1] > uxmax)  continue;
                        gxwork[npt] = x[i - 1];      gywork[npt] = y[i - 1];
                        npt++;
                    }
                    ComputeLogs(npt, optionZ);
                    if (gxworkl[npt - 1] < rwxmin || gxworkl[npt - 1] > rwxmax) {
                        if (npt > 2) {
                            ComputeLogs(npt, optionZ);
                            Smooth(theGraph, npt, gxworkl, gyworkl, drawtype);
                        }
                        gxwork[0] = gxwork[npt - 1]; gywork[0] = gywork[npt - 1];
                        npt = 1;
                        continue;
                    }
                }
                if (npt > 1) {
                    ComputeLogs(npt, optionZ);
                    Smooth(theGraph, npt, gxworkl, gyworkl, drawtype);
                }
            }
        }
#endif // option curve

        // Draw the graph with a '*' on every points
        if (optionStar) {
            /* // Disabled due to use of gxworkl et al.
            theGraph->SetMarkerStyle(3);
            npt = 0;
            for (i = 1; i <= npoints; i++) {
                gxwork[npt] = x[i - 1];      gywork[npt] = y[i - 1];
                npt++;
                if (i == npoints) {
                    ComputeLogs(npt, optionZ, gxworkl, gxwork, gyworkl, gywork);
                    if (optionR)  rPad.PaintPolyMarker(npt, gyworkl, gxworkl);
                    else          rPad.PaintPolyMarker(npt, gxworkl, gyworkl);
                    npt = 0;
                }
            }
            */
        }

        // Draw the graph with the current polymarker on every points
        if (optionMark) {
            /* // Disabled due to use of gxworkl et al.
            npt = 0;
            for (i = 1; i <= npoints; i++) {
                gxwork[npt] = x[i - 1];      gywork[npt] = y[i - 1];
                npt++;
                if (i == npoints) {
                    ComputeLogs(npt, optionZ, gxworkl, gxwork, gyworkl, gywork);
                    if (optionR) rPad.PaintPolyMarker(npt, gyworkl, gxworkl);
                    else         rPad.PaintPolyMarker(npt, gxworkl, gyworkl);
                    npt = 0;
                }
            }
            */
        }

        // Draw the graph as a bar chart
        if (optionBar) {
#if 0 // Disabled due to use of gxworkl et al.
            if (!optionR) {
                barxmin = x[0];
                barxmax = x[0];
                for (i = 1; i<npoints; i++) {
                    if (x[i] < barxmin) barxmin = x[i];
                    if (x[i] > barxmax) barxmax = x[i];
                }
                bdelta = (barxmax - barxmin) / Double_t(npoints);
            }
            else {
                barymin = y[0];
                barymax = y[0];
                for (i = 1; i<npoints; i++) {
                    if (y[i] < barymin) barymin = y[i];
                    if (y[i] > barymax) barymax = y[i];
                }
                bdelta = (barymax - barymin) / Double_t(npoints);
            }
            dbar = 0.5*bdelta*gStyle->GetBarWidth();
            if (!optionR) {
                for (i = 1; i <= npoints; i++) {
                    xlow = x[i - 1] - dbar;
                    xhigh = x[i - 1] + dbar;
                    yhigh = y[i - 1];
                    if (xlow  < uxmin) xlow = uxmin;
                    if (xhigh > uxmax) xhigh = uxmax;
                    if (!optionOne) ylow = TMath::Max((Double_t)0, rPad.GetUymin());
                    else            ylow = rPad.GetUymin();
                    gxwork[0] = xlow;
                    gywork[0] = ylow;
                    gxwork[1] = xhigh;
                    gywork[1] = yhigh;
                    ComputeLogs(2, optionZ, gxworkl, gxwork, gyworkl, gywork);
                    if (gyworkl[0] < rPad.GetUymin()) gyworkl[0] = rPad.GetUymin();
                    if (gyworkl[1] < rPad.GetUymin()) continue;
                    if (gyworkl[1] > rPad.GetUymax()) gyworkl[1] = rPad.GetUymax();
                    if (gyworkl[0] > rPad.GetUymax()) continue;

                    rPad.PaintBox(gxworkl[0], gyworkl[0], gxworkl[1], gyworkl[1]);
                }
            }
            else {
                for (i = 1; i <= npoints; i++) {
                    xhigh = x[i - 1];
                    ylow = y[i - 1] - dbar;
                    yhigh = y[i - 1] + dbar;
                    xlow = TMath::Max((Double_t)0, rPad.GetUxmin());
                    gxwork[0] = xlow;
                    gywork[0] = ylow;
                    gxwork[1] = xhigh;
                    gywork[1] = yhigh;
                    ComputeLogs(2, optionZ, gxworkl, gxwork, gyworkl, gywork);
                    rPad.PaintBox(gxworkl[0], gyworkl[0], gxworkl[1], gyworkl[1]);
                }
            }
#endif
        }
        rPad.ResetBit(TGraph::kClipFrame);
        /*
        delete[] gxwork;
        delete[] gywork;
        delete[] gxworkl;
        delete[] gyworkl;
        */
    }

    int m_paintMethod;
    std::string m_sPaintOpt;
}; // class TestGraph

class MainWindow : public QMainWindow
{
public:
    enum
    {
        GraphUpdateMethodSetAndSetPoint,
        GraphUpdateMethodSetPoint,
        GraphUpdateMethodReplaceRandom
    };
    typedef int GraphUpdateMethod;

    MainWindow() : 
        m_counter(0),
        m_bStopped(true),
        m_updateMethod(GraphUpdateMethodSetAndSetPoint)
    {
        auto pMainLayout = new QGridLayout;
        auto pControlPanelLayout = new QGridLayout;

        CreateControlPanel(*pControlPanelLayout);

        pMainLayout->addLayout(pControlPanelLayout, 0, 0);

        m_spCanvasWidget.reset(new TQtWidget(this));
        pMainLayout->addWidget(m_spCanvasWidget.get(), 0, 1);

        pMainLayout->setColumnStretch(0, 1);
        pMainLayout->setColumnStretch(1, 10);

        setCentralWidget(new QWidget(this));
        delete centralWidget()->layout();
        centralWidget()->setLayout(pMainLayout);
    }

    QSpinBox* CreatePadCountSpinBox(std::unique_ptr<QSpinBox>& spSpinBox)
    {
        spSpinBox.reset(new QSpinBox);
        spSpinBox->setRange(1, 256);
        spSpinBox->setValue(4);
        return spSpinBox.get();
    }

    void CreateControlPanel(QGridLayout& rLayout)
    {
        int nRow = 0;
        rLayout.addWidget(new QLabel(tr("Pad rows"), this), nRow, 0);
        rLayout.addWidget(CreatePadCountSpinBox(m_spPadRows), nRow, 1);
        
        nRow++;
        rLayout.addWidget(new QLabel(tr("Pad columns"), this), nRow, 0);
        rLayout.addWidget(CreatePadCountSpinBox(m_spPadCols), nRow, 1);

        auto pButtonStart = new QPushButton(tr("Start"), this);
        auto pButtonStop = new QPushButton(tr("Stop"), this);

        m_spFpsDisplay.reset(new QLineEdit);

        nRow++;
        rLayout.addWidget(pButtonStart, nRow, 0);
        rLayout.addWidget(pButtonStop, nRow, 1);

        nRow++;
        m_spUpdateMethod.reset(new QComboBox(this));
        m_spUpdateMethod->addItems(QStringList() << "Append_Set&SetPoint" << "Append_SetPoint" << "Replace random");
        connect(m_spUpdateMethod.get(), static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::OnUpdateMethodChanged);
        rLayout.addWidget(new QLabel(tr("Update method"), this), nRow, 0);
        rLayout.addWidget(m_spUpdateMethod.get(), nRow, 1);

        nRow++;
        m_spPaintMethod.reset(new QComboBox(this));
        m_spPaintMethod->addItems(QStringList() << "TGraph::Paint()" << "Without histogram" << "No paint");
        connect(m_spPaintMethod.get(), static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::OnPaintMethodUpdated);
        rLayout.addWidget(new QLabel(tr("Paint method"), this), nRow, 0);
        rLayout.addWidget(m_spPaintMethod.get(), nRow, 1);

        nRow++;
        m_spPaintOpt.reset(new QLineEdit(this));
        m_spPaintOpt->setText("AL");
        connect(m_spPaintOpt.get(), &QLineEdit::textChanged, this, &MainWindow::OnPaintOptChanged);
        rLayout.addWidget(new QLabel(tr("Paint option"), this), nRow, 0);
        rLayout.addWidget(m_spPaintOpt.get(), nRow, 1);

        nRow++;
        rLayout.addWidget(new QLabel(tr("FPS"), this), nRow, 0);
        rLayout.addWidget(m_spFpsDisplay.get(), nRow, 1);

        nRow++;
        rLayout.addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding), nRow, 0);

        connect(pButtonStart, &QPushButton::clicked, this, &MainWindow::UpdateGraphs);
        connect(pButtonStop, &QPushButton::clicked, this, &MainWindow::OnStopButtonClicked);
    }

    void timerEvent(QTimerEvent* pEvent) override
    {
        if (!m_bStopped)
            UpdateGraphs();
    }

    void UpdateGraphs()
    {
        if (m_bStopped == true)
        {
            auto pCanvas = m_spCanvasWidget->GetCanvas();
            if (pCanvas)
            {
                pCanvas->Clear();
                const int nRows = (m_spPadRows) ? m_spPadRows->value() : 4;
                const int nCols = (m_spPadRows) ? m_spPadCols->value() : 4;
                pCanvas->Divide(nCols, nRows);
                m_graphs.clear();
                m_graphs.resize(nRows * nCols);
                int n = 1;
                const std::string sOpt = m_spPaintOpt->text().toLatin1();
                for (auto& graph : m_graphs)
                {
                    graph.SetPaintMethod(m_spPaintMethod->currentIndex());
                    graph.SetDrawOption(sOpt.c_str());
                    graph.DrawOnPad(pCanvas->GetPad(n++), sOpt.c_str());
                }
                m_updateMethod = m_spUpdateMethod->currentIndex();


            }
            m_bStopped = false;
        }
        QElapsedTimer timer;
        timer.restart();
        for (auto& graph : m_graphs)
        {
            const auto nOldSize = graph.GetN();
            const double newVal = (&graph != &m_graphs.front()) ? rand() : m_spFpsDisplay->text().toDouble();

            if (nOldSize == 0 && m_updateMethod == GraphUpdateMethodReplaceRandom)
                m_updateMethod = GraphUpdateMethodSetAndSetPoint;
            else if (m_updateMethod == GraphUpdateMethodSetAndSetPoint)
            {
                graph.Set(nOldSize + 1);
                graph.SetPoint(nOldSize, m_counter, newVal);
            }
            else if (m_updateMethod == GraphUpdateMethodSetPoint)
            {
                graph.SetPoint(nOldSize, m_counter, newVal);
            }
            else if (m_updateMethod == GraphUpdateMethodReplaceRandom)
            {
                const auto index = rand() % nOldSize;
                graph.SetPoint(index, graph.GetX()[index], newVal);
            }

        }
        m_counter++;
        m_spCanvasWidget->GetCanvas()->Update();
        m_spCanvasWidget->GetCanvas()->Paint();
        const double elapsed = static_cast<double>(timer.elapsed());
        m_spFpsDisplay->setText(QString::number(1000.0 / elapsed));

        // Generate new event.
        auto pEvent = new QEvent(QEvent::Timer);
        QCoreApplication::postEvent(this, pEvent);
    }

    void OnStopButtonClicked()
    {
        m_bStopped = true;
    }

    void OnUpdateMethodChanged(const int i)
    {
        m_updateMethod = i;
    }

    void OnPaintMethodUpdated(const int i)
    {
        for (auto& graph : m_graphs)
        {
            graph.SetPaintMethod(i);
        }
    }

    void OnPaintOptChanged(QString s)
    {
        auto pCanvas = m_spCanvasWidget->GetCanvas();
        if (!pCanvas)
            return;
        const std::string sOpt = s.toLatin1();
        for (auto& graph : m_graphs)
        {
            graph.SetDrawOption(sOpt.c_str());
        }
    }

    GraphUpdateMethod m_updateMethod;
    double m_counter;
    bool m_bStopped;
    std::unique_ptr<TQtWidget> m_spCanvasWidget;
    std::vector<TestGraph> m_graphs;
    std::unique_ptr<QLineEdit> m_spFpsDisplay;
    std::unique_ptr<QSpinBox> m_spPadRows;
    std::unique_ptr<QSpinBox> m_spPadCols;
    std::unique_ptr<QComboBox> m_spUpdateMethod;
    std::unique_ptr<QComboBox> m_spPaintMethod;
    std::unique_ptr<QLineEdit> m_spPaintOpt;
};

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    gEnv->SetValue("Gui.Factory", "qtgui");

    MainWindow mainWnd;

    mainWnd.resize(800, 600);
    mainWnd.show();

    return a.exec();
}
