/* Header for module files, generated by p2c 1.21alpha-07.Dec.93 */
#ifndef FILES_H
#define FILES_H


/* Open and close files, interpret command line, do input, do most output*/

#ifndef GLOBALS_H
#include "globals.h"
#endif

#ifndef MULTFILE_H
#include "multfile.h"
#endif


#ifdef FILES_G
# define vextern
#else
# define vextern extern
#endif

extern Void readParagraph PP((Char (*P)[256], short *no, paragraph_index0 *L));
extern boolean styleFileFound PV();
extern Void processOption PP((int j));
extern Void OpenFiles PV();
extern Void CloseFiles PV();
extern Void putLine PP((Char *line));
extern Void put PP((Char *line, int putspace_));
extern Void tex3 PP((Char *s));
extern boolean endOfInfile PV();


#undef vextern

#endif /*FILES_H*/

/* End. */
