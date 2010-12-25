/**
 *
 * Xeroxpatrone
 *
 * http://code.google.com/p/xeroxpatrone
 *
 * Copyright (C) 2010 Dominik Meyer
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef _MAINPANEL_H_
#define _MAINPANEL_H_

#include "global.h"

#include <wx/wx.h>
#include <vector>

#include "xeroxpatrone.h"

#include "programmingadapter.h"
#include "usbadapter.h"
#include "serialadapterv1.h"
#include "serialadapterv2.h"

using namespace std;

class MainPanel : public wxPanel {
  public:
	MainPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = _T("main panel"));
	~MainPanel();

	void OnChoice(wxCommandEvent& event);

  private:
    vector<wxPanel*> vReadOptions;
    vector<wxPanel*> vWriteOptions;
    vector<ProgrammingAdapter*> vAdapters;

    int iSelectedAdapter;

  protected:
    DECLARE_EVENT_TABLE();
};

#endif /* _MAINPANEL_H_ */
