#ifndef __LDLERROR_H__
#define __LDLERROR_H__

#include <TCFoundation/TCAlert.h>
#include <stdlib.h>

class TCStringArray;

typedef enum
{
	LDLEGeneral,
	LDLEParse,
	LDLEFileNotFound,
	LDLEMatrix,
	LDLEPartDeterminant,
	LDLENonFlatQuad,
	LDLEConcaveQuad,
	LDLEConcaveQuadSplit,
	LDLEMatchingPoints,
//	LDLEOpenGL,
	LDLEColinear,
	LDLEBFCWarning,
	LDLEBFCError,
	LDLEMPDError
} LDLErrorType;

typedef enum
{
	LDLACriticalError,	// This means the model couldn't be loaded at all.
	LDLAError,
	LDLAWarning
} LDLAlertLevel;

class LDLError: public TCAlert
{
public:
	LDLError(LDLErrorType type, const char *message, const char *filename,
		const char *fileLine, int lineNumber,
		TCStringArray *extraInfo = NULL);
	LDLErrorType getType(void) { return m_type; }
	char *getFilename(void) { return m_filename; }
	char *getFileLine(void) { return m_fileLine; }
	int getLineNumber(void) { return m_lineNumber; }
	static TCULong alertClass(void) { return USER_ALERTS + 0; }
	virtual const char *getTypeName(void);
	static const char *getTypeName(LDLErrorType type);
	void setLevel(LDLAlertLevel value) { m_level = value; }
	LDLAlertLevel getLevel(void) { return m_level; }
	void cancelLoad(void) { m_loadCanceled = true; }
	bool getLoadCanceled(void) { return m_loadCanceled; }
protected:
	virtual ~LDLError(void);
	virtual void dealloc(void);

	LDLErrorType m_type;
	char *m_filename;
	char *m_fileLine;
	int m_lineNumber;
	LDLAlertLevel m_level;
	bool m_loadCanceled;
};

#endif // __LDLERROR_H__
