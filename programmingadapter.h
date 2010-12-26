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

#ifndef _PROGRAMMINGADAPTER_H_
#define _PROGRAMMINGADAPTER_H_

#include "global.h"

#include <wx/wx.h>

enum ProgrammingAdapterType_t {
    INVALID_ADAPTER_TYPE = 0,
    USB,
    COMv1,
    COMv2,
};

typedef enum ProgrammingAdapterType_t ProgrammingAdapterType;

class ProgrammingAdapter : public wxPanel{
  public:
	ProgrammingAdapter(wxPanel* pPanel, const wxPoint& pos = wxPoint(0,0));
	virtual ~ProgrammingAdapter();

	const wxString GetAdapterName();
	void SetAdapterName(const wxString& xsName);
	ProgrammingAdapterType GetAdapterType();
	void SetAdapterType(ProgrammingAdapterType eType);

	virtual bool Init() = 0;
    virtual bool OnShow() = 0;

  private:

	wxString m_xsName;
	ProgrammingAdapterType m_eType;
};

#endif /* _PROGRAMMINGADAPTER_H_ */
