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

#ifndef _SERIALADAPTERV1_H_
#define _SERIALADAPTERV1_H_

#include "global.h"

#include <wx/wx.h>

#include "programmingadapter.h"

class SerialAdapterV1 : public ProgrammingAdapter {
  public:
	SerialAdapterV1(wxPanel* pPanel, const wxPoint& pos = wxPoint(0,0));
	~SerialAdapterV1();

	bool Init();
	bool OnShow();

  private:
};

#endif /* _SERIALADAPTERV1_H_ */
