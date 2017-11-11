
#ifndef _SOCKETS_File_H
#define _SOCKETS_File_H

#include "IFile.h"
#include <stdio.h>



/** IFile implementation of a disk file. 
	\ingroup file */
class File : public IFile
{
public:
	File();
	/** convenience: calls fopen() */
	File(const std::string& path, const std::string& mode);
	~File();

	bool fopen(const std::string& path, const std::string& mode);
	void fclose() const;

	size_t fread(char *, size_t, size_t) const;
	size_t fwrite(const char *, size_t, size_t);

	char *fgets(char *, int) const;
	void fprintf(const char *format, ...);

	off_t size() const;
	bool eof() const;

	void reset_read() const;
	void reset_write();

	const std::string& Path() const;

private:
	File(const File& ) {} // copy constructor
	File& operator=(const File& ) { return *this; } // assignment operator

	std::string m_path;
	std::string m_mode;
	mutable FILE *m_fil;
	mutable long m_rptr;
	long m_wptr;
};
#endif // _SOCKETS_File_H


