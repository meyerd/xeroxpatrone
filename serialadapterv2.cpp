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

#include "serialadapterv2.h"


SerialAdapterV2::SerialAdapterV2() : ProgrammingAdapter(), m_myPanel(NULL) {
    SetName(_T("Serial Adapter (v2, \"Easy Adapter\")"));
    SetType(COMv1);
}

SerialAdapterV2::~SerialAdapterV2() {

}

bool SerialAdapterV2::Init() {

}

wxPanel* SerialAdapterV2::GetOptionControls(wxPanel* pParent, const wxPoint& pos) {
    wxPanel* panel = new wxPanel(pParent, wxID_ANY, pos, wxSize(380, 30));

    wxStaticText* xNoOptionsText = new wxStaticText(panel, wxID_ANY, _T("No Options (Nothing implemented yet)"), wxPoint(0,0), wxSize(380, 25));

    return panel;
}


