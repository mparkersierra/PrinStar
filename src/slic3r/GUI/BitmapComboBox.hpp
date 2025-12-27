#ifndef slic3r_BitmapComboBox_hpp_
#define slic3r_BitmapComboBox_hpp_

#include <wx/bmpcbox.h>
#include <wx/gdicmn.h>
#include <vector>

namespace Slic3r {
namespace GUI {

class BitmapComboBox : public wxBitmapComboBox
{
public:
    BitmapComboBox(wxWindow* parent,
                   wxWindowID id = wxID_ANY,
                   const wxString& value = wxEmptyString,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   int n = 0,
                   const wxString choices[] = nullptr,
                   long style = 0);

    ~BitmapComboBox() = default;

#ifdef _WIN32
    int Append(const wxString& item);
#endif

    int Append(const wxString& item, const wxBitmap& bitmap)
    {
#ifdef __APPLE__
        int idx = wxBitmapComboBox::Append(item);
        OnAddBitmap(bitmap);
        return idx;
#else
        return wxBitmapComboBox::Append(item, bitmap);
#endif
    }

protected:

#ifdef __APPLE__
/* For PresetComboBox we use bitmaps that are created from images that are already scaled appropriately for Retina
 * (Contrary to the intuition, the `scale` argument for Bitmap's constructor doesn't mean
 * "please scale this to such and such" but rather
 * "the wxImage is already sized for backing scale such and such". )
 * Unfortunately, the constructor changes the size of wxBitmap too.
 * Thus We need to use unscaled size value for bitmaps that we use
 * to avoid scaled size of control items.
 * For this purpose control drawing methods and
 * control size calculation methods (virtual) are overridden.
 **/
protected:
#if defined(__WXMSW__) // or a tighter check if you prefer
    bool OnAddBitmap(const wxBitmap& bitmap) override;
#endif

void OnDrawItem(wxDC& dc, const wxRect& rect, int item, int flags) const override;
#endif

#ifdef _WIN32
    bool MSWOnDraw(WXDRAWITEMSTRUCT* item) override;
    void DrawBackground_(wxDC& dc, const wxRect& rect, int WXUNUSED(item), int flags) const;
    void Rescale();
#endif

private:
    std::vector<wxBitmap> m_bitmaps;      // Stores bitmaps for each item (macOS)
    wxSize m_usedImgSize = wxSize(16,16); // Bitmap layout size
    int m_imgAreaWidth = 20;              // Space for bitmap in item
};

}} // namespace Slic3r::GUI

#endif
