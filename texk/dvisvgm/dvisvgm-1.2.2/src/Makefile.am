:## This file is part of dvisvgm
## Copyright (C) 2005-2013 Martin Gieseking <martin.gieseking@uos.de>
##
## Process this file with automake.

bin_PROGRAMS         = dvisvgm
noinst_LIBRARIES     = libdvisvgm.a

dvisvgm_SOURCES      = gzstream.h \
							  dvisvgm.cpp gzstream.cpp

dvisvgm_LDADD        = $(noinst_LIBRARIES) @EXTRA_LIBS@
dvisvgm_DEPENDENCIES = $(noinst_LIBRARIES) @EXTRA_LIBS@

libdvisvgm_a_SOURCES = Bitmap.h BoundingBox.h BgColorSpecialHandler.h Calculator.h CmdLineParserBase.h Color.h CRC32.h \
	ColorSpecialHandler.h CommandLine.h Directory.h DLLoader.h DVIActions.h DVIReader.h DvisvgmSpecialHandler.h DVIToSVG.h \
	DVIToSVGActions.h EmSpecialHandler.h EPSFile.h EPSToSVG.h FileFinder.h FilePath.h FileSystem.h Font.h FontCache.h \
	FontEncoding.h FontEngine.h FontManager.h FontMap.h GFReader.h GFTracer.h GFGlyphTracer.h Ghostscript.h Glyph.h \
	GlyphTracerMessages.h GraphicPath.h InputBuffer.h InputReader.h Length.h macros.h MapLine.h Matrix.h Message.h \
	MessageException.h MetafontWrapper.h NoPsSpecialHandler.h PageRanges.h PageSize.h Pair.h PdfSpecialHandler.h Process.h \
	PSInterpreter.h PSPattern.h PsSpecialHandler.h SignalHandler.h SpecialActions.h SpecialHandler.h SpecialManager.h \
	StreamReader.h StreamWriter.h Subfont.h SVGOutputBase.h SVGTree.h System.h Terminal.h TpicSpecialHandler.h TFM.h types.h \
	VectorStream.h VFActions.h VFReader.h XMLDocTypeNode.h XMLDocument.h XMLNode.h XMLString.h \
	BgColorSpecialHandler.cpp Bitmap.cpp BoundingBox.cpp Calculator.cpp CmdLineParserBase.cpp Color.cpp \
	ColorSpecialHandler.cpp CommandLine.cpp CRC32.cpp Directory.cpp DLLoader.cpp DVIActions.cpp DVIReader.cpp \
	DvisvgmSpecialHandler.cpp DVIToSVG.cpp DVIToSVGActions.cpp EmSpecialHandler.cpp EPSFile.cpp EPSToSVG.cpp FileFinder.cpp \
	FilePath.cpp FileSystem.cpp Font.cpp FontCache.cpp FontEncoding.cpp FontEngine.cpp FontManager.cpp FontMap.cpp \
	GFReader.cpp GFGlyphTracer.cpp GFTracer.cpp Ghostscript.cpp InputBuffer.cpp InputReader.cpp Length.cpp MapLine.cpp \
	Matrix.cpp Message.cpp MetafontWrapper.cpp NoPsSpecialHandler.cpp PageRanges.cpp PageSize.cpp PdfSpecialHandler.cpp \
	Process.cpp psdefs.cpp PSInterpreter.cpp PSPattern.cpp PsSpecialHandler.cpp SignalHandler.cpp SpecialManager.cpp \
	StreamReader.cpp StreamWriter.cpp Subfont.cpp SVGTree.cpp System.cpp Terminal.cpp TFM.cpp TpicSpecialHandler.cpp \
	VFReader.cpp XMLDocument.cpp XMLNode.cpp XMLString.cpp

EXTRA_DIST = options.xml iapi.h ierrors.h MiKTeXCom.h MiKTeXCom.cpp

AM_CXXFLAGS = -Wall -Wnon-virtual-dtor

# the command-line parser is generated from options.xml by opt2cpp
CommandLine.cpp: options.xml
	if test -f opt2cpp.xsl; then \
		rm -f $@ $*.h; \
		xsltproc opt2cpp.xsl $<; \
	fi

# Create a C string definition containing the PostScript routines psdefs.ps needed by class PSInterpreter
$(srcdir)/psdefs.cpp: psdefs.ps
	if test -f $<; then \
		ps2c PSInterpreter::PSDEFS $< >$@; \
	fi

psdefs.ps: ;

