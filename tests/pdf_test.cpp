#include <podofo/podofo.h>
using namespace PoDoFo;

int main() {
    PdfStreamedDocument pdf("output.pdf");
    PdfPage *page = pdf.CreatePage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4));
    PdfPainter painter;
    painter.SetPage(page);

    painter.DrawText(72.0, 720.0, "Hello, World!");
    painter.FinishPage();
    pdf.Close();

    return 0;
}
