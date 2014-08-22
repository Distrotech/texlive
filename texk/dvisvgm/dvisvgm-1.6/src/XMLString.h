/*************************************************************************
** XMLString.h                                                          **
**                                                                      **
** This file is part of dvisvgm -- the DVI to SVG converter             **
** Copyright (C) 2005-2014 Martin Gieseking <martin.gieseking@uos.de>   **
**                                                                      **
** This program is free software; you can redistribute it and/or        **
** modify it under the terms of the GNU General Public License as       **
** published by the Free Software Foundation; either version 3 of       **
** the License, or (at your option) any later version.                  **
**                                                                      **
** This program is distributed in the hope that it will be useful, but  **
** WITHOUT ANY WARRANTY; without even the implied warranty of           **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         **
** GNU General Public License for more details.                         **
**                                                                      **
** You should have received a copy of the GNU General Public License    **
** along with this program; if not, see <http://www.gnu.org/licenses/>. **
*************************************************************************/

#ifndef DVISVGM_XMLSTRING_H
#define DVISVGM_XMLSTRING_H

#include <string>


class XMLString : public std::string
{
   public:
		XMLString () : std::string() {}
		XMLString (const char *str, bool plain=false);
      XMLString (const std::string &str, bool plain=false);
		XMLString (int n, bool cast=true);
		XMLString (double x);

		static int DECIMAL_PLACES;  ///< number of decimal places applied to floating point values (0-6)
};


#endif
