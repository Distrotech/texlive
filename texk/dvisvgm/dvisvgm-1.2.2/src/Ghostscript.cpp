/*************************************************************************
** Ghostscript.cpp                                                      **
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

#include "Ghostscript.h"
#if !defined(DISABLE_GS)

#include <cstring>
#include <iomanip>
#include <sstream>

#if defined(HAVE_LIBGS)
   #include <ghostscript/ierrors.h>
#else
   #include "ierrors.h"
#endif

using namespace std;

// default name of dynamic/shared Ghostscript library
#if defined(__WIN64__)
	#define GS_DL_NAME "gsdll64.dll"
#elif defined(__WIN32__)
	#define GS_DL_NAME "gsdll32.dll"
#else
	#define GS_DL_NAME "libgs.so"
#endif


string Ghostscript::LIBGS_NAME;


/** Loads the Ghostscript library but does not create an instance. This
 *  constructor should only be used to call available() and revision(). */
Ghostscript::Ghostscript ()
#if !defined(HAVE_LIBGS)
	: DLLoader(LIBGS_NAME.empty() ? GS_DL_NAME : LIBGS_NAME.c_str())
#endif
{
	_inst = 0;
}


/** Tries to load the shared library and to initialize Ghostscript.
 * @param[in] argc number of parameters in array argv
 * @param[in] argv parameters passed to Ghostscript
 * @param[in] caller this parameter is passed to all callback functions */
Ghostscript::Ghostscript (int argc, const char **argv, void *caller)
#if !defined(HAVE_LIBGS)
	: DLLoader(LIBGS_NAME.empty() ? GS_DL_NAME : LIBGS_NAME.c_str())
#endif
{
	_inst = 0;
	init(argc, argv, caller);
}


/** Exits Ghostscript and unloads the dynamic library. */
Ghostscript::~Ghostscript () {
	if (_inst) {
		exit();
		delete_instance();
	}
}


bool Ghostscript::init (int argc, const char **argv, void *caller) {
	if (!_inst) {
		int status = new_instance(&_inst, caller);
		if (status < 0)
			_inst = 0;
		else {
			init_with_args(argc, (char**)argv);
		}
	}
	return _inst != 0;
}


/** Returns true if Ghostscript library was found and can be loaded. */
bool Ghostscript::available () {
#if defined(HAVE_LIBGS)
	return true;
#else
	return loaded();
#endif
}


/** Retrieves version information about Ghostscript.
 * @param[out] r takes the revision information (see GS API documentation for further details)
 * @return true on success  */
bool Ghostscript::revision (gsapi_revision_t *r) {
#if defined(HAVE_LIBGS)
	return (gsapi_revision(r, sizeof(gsapi_revision_t)) == 0);
#else
	if (PFN_gsapi_revision fn = (PFN_gsapi_revision)loadSymbol("gsapi_revision"))
		return (fn(r, sizeof(gsapi_revision_t)) == 0);
	return false;
#endif
}


/** Returns product name and revision number of the GS library.
 *  @param[in] revonly if true, only the revision number is returned */
string Ghostscript::revision (bool revonly) {
	gsapi_revision_t r;
	if (revision(&r)) {
		ostringstream oss;
		if (!revonly)
			oss << r.product << ' ';
		oss << (r.revision/100) << '.' << setfill('0') << setw(2) << (r.revision%100);
		return oss.str();
	}
	return "";
}


/** Creates a new instance of Ghostscript. This method is called by the constructor and
 *  should not be used elsewhere.
 *  @param[out] psinst handle of newly created instance (or 0 on error)
 *  @param[in] caller pointer forwarded to callback functions */
int Ghostscript::new_instance (void **psinst, void *caller) {
#if defined(HAVE_LIBGS)
	return gsapi_new_instance(psinst, caller);
#else
	if (PFN_gsapi_new_instance fn = (PFN_gsapi_new_instance)loadSymbol("gsapi_new_instance"))
		return fn(psinst, caller);
   *psinst = 0;
	return 0;
#endif
}


/** Destroys the current instance of Ghostscript. This method is called by the destructor
 *  and should not be used elsewhere. */
void Ghostscript::delete_instance () {
#if defined(HAVE_LIBGS)
	gsapi_delete_instance(_inst);
#else
	if (PFN_gsapi_delete_instance fn = (PFN_gsapi_delete_instance)loadSymbol("gsapi_delete_instance"))
		fn(_inst);
#endif
}


/** Exits the interpreter. Must be called before destroying the GS instance. */
int Ghostscript::exit () {
#if defined(HAVE_LIBGS)
	return gsapi_exit(_inst);
#else
	if (PFN_gsapi_exit fn = (PFN_gsapi_exit)loadSymbol("gsapi_exit"))
		return fn(_inst);
	return 0;
#endif
}


/** Sets the I/O callback functions.
 * @param[in] in pointer to stdin handler
 * @param[in] out pointer to stdout handler
 * @param[in] err pointer to stderr handler */
int Ghostscript::set_stdio (Stdin in, Stdout out, Stderr err) {
#if defined(HAVE_LIBGS)
	return gsapi_set_stdio(_inst, in, out, err);
#else
	if (PFN_gsapi_set_stdio fn = (PFN_gsapi_set_stdio)loadSymbol("gsapi_set_stdio"))
		return fn(_inst, in, out, err);
	return 0;
#endif
}


/** Initializes Ghostscript with a set of optional parameters. This
 *  method is called by the constructor and should not be used elsewhere.
 *  @param[in] argc number of paramters
 *  @param[in] argv parameters passed to Ghostscript */
int Ghostscript::init_with_args (int argc, char **argv) {
#if defined(HAVE_LIBGS)
	return gsapi_init_with_args(_inst, argc, argv);
#else
	if (PFN_gsapi_init_with_args fn = (PFN_gsapi_init_with_args)loadSymbol("gsapi_init_with_args"))
		return fn(_inst, argc, argv);
	return 0;
#endif
}


/** Tells Ghostscript that several calls of run_string_continue will follow. */
int Ghostscript::run_string_begin (int user_errors, int *pexit_code) {
#if defined(HAVE_LIBGS)
	return gsapi_run_string_begin(_inst, user_errors, pexit_code);
#else
	if (PFN_gsapi_run_string_begin fn = (PFN_gsapi_run_string_begin)loadSymbol("gsapi_run_string_begin"))
		return fn(_inst, user_errors, pexit_code);
	*pexit_code = 0;
	return 0;
#endif
}


/** Executes a chunk of PostScript commands given by a buffer of characters. The size of
 *  this buffer must not exceed 64KB. Longer programs can be split into arbitrary smaller chunks
 *  and passed to Ghostscript by successive calls of run_string_continue.
 *  @param[in] str buffer containing the PostScript code
 *  @param[in] length number of characters in buffer
 *  @param[in] user_errors if non-negative, the default PS error values will be generated, otherwise this value is returned
 *  @param[out] pexit_code takes the PS error code */
int Ghostscript::run_string_continue (const char *str, unsigned length, int user_errors, int *pexit_code) {
#if defined(HAVE_LIBGS)
	return gsapi_run_string_continue(_inst, str, length, user_errors, pexit_code);
#else
	if (PFN_gsapi_run_string_continue fn = (PFN_gsapi_run_string_continue)loadSymbol("gsapi_run_string_continue"))
		return fn(_inst, str, length, user_errors, pexit_code);
	*pexit_code = 0;
	return 0;
#endif
}


/** Terminates the successive code feeding. Must be called after the last call of run_string_continue. */
int Ghostscript::run_string_end (int user_errors, int *pexit_code) {
#if defined(HAVE_LIBGS)
	return gsapi_run_string_end(_inst, user_errors, pexit_code);
#else
	if (PFN_gsapi_run_string_end fn = (PFN_gsapi_run_string_end)loadSymbol("gsapi_run_string_end"))
		return fn(_inst, user_errors, pexit_code);
	*pexit_code = 0;
	return 0;
#endif
}


const char* Ghostscript::error_name (int code) {
	if (code < 0)
		code = -code;
	const char *error_names[] = { ERROR_NAMES };
	if (code == 0 || (size_t)code > sizeof(error_names)/sizeof(error_names[0]))
		return 0;
#if defined(HAVE_LIBGS)
	// use array defined in libgs to avoid linking the error strings into the binary
	return gs_error_names[code-1];
#elif defined(__WIN32__)
	// gs_error_names is private in the Ghostscript DLL so we can't access it here
	return error_names[code-1];
#else
	if (const char **error_names = (const char**)loadSymbol("gs_error_names"))
		return error_names[code-1];
	return 0;
#endif
}

#endif  // !DISABLE_GS
