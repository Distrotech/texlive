/*************************************************************************
** EPSToSVG.cpp                                                         **
**                                                                      **
** This file is part of dvisvgm -- the DVI to SVG converter             **
** Copyright (C) 2005-2013 Martin Gieseking <martin.gieseking@uos.de>   **
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

#include <fstream>
#include <istream>
#include <sstream>
#include "EPSFile.h"
#include "EPSToSVG.h"
#include "Message.h"
#include "MessageException.h"
#include "PsSpecialHandler.h"
#include "SVGOutputBase.h"

#ifdef HAVE_CONFIG_H
	#include "config.h"
	#ifdef TARGET_SYSTEM
		#define VERSION_STR VERSION " (" TARGET_SYSTEM ")"
	#else
		#define VERSION_STR VERSION
	#endif
#else
	#define VERSION_STR ""
#endif


using namespace std;


void EPSToSVG::convert () {
	if (!Ghostscript().available())
		throw MessageException("Ghostscript is required to process the EPS file");
	EPSFile epsfile(_fname);
	if (!epsfile.hasValidHeader())
		throw PSException("invalid EPS file");

	BoundingBox bbox;
	epsfile.bbox(bbox);
	Message::mstream(false, Message::MC_PAGE_NUMBER) << "processing file " << _fname << '\n';
	Message::mstream().indent(1);
	_svg.newPage(1);
	// create a psfile special and forward it to the PsSpecialHandler
	stringstream ss;
	ss << "\"" << _fname  << "\" "
			"llx=" << bbox.minX() << " "
			"lly=" << bbox.minY() << " "
			"urx=" << bbox.maxX() << " "
			"ury=" << bbox.maxY();
	PsSpecialHandler pshandler;
	pshandler.process("psfile=", ss, this);
	// output SVG file
	_svg.setBBox(_bbox);
	_svg.appendToDoc(new XMLCommentNode(" This file was generated by dvisvgm " VERSION_STR " "));
	_svg.write(_out.getPageStream(1, 1));
	string svgfname = _out.filename(1, 1);
	Message::mstream(false, Message::MC_PAGE_SIZE) << "graphic size: " << bbox.width() << "pt"
		" x " << bbox.height() << "pt"
		" (" << bbox.width()/72.27*25.4 << "mm"
		" x " << bbox.height()/72.27*25.4 << "mm)\n";
	Message::mstream(false, Message::MC_PAGE_WRITTEN) << "graphic written to " << (svgfname.empty() ? "<stdout>" : svgfname) << '\n';
}

