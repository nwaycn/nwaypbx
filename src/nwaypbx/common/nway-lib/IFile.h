
#ifndef _SOCKETS_IFile_H
#define _SOCKETS_IFile_H

#include <string>

class IFile
{
public:
	virtual ~IFile() {}

	virtual bool fopen(const std::string&, const std::string&) = 0;
	virtual void fclose() const = 0;

	virtual size_t fread(char *, size_t, size_t) const = 0;
	virtual size_t fwrite(const char *, size_t, size_t) = 0;

	virtual char *fgets(char *, int) const = 0;
	virtual void fprintf(const char *format, ...) = 0;

	virtual off_t size() const = 0;
	virtual bool eof() const = 0;

	virtual void reset_read() const = 0;
	virtual void reset_write() = 0;

	virtual const std::string& Path() const = 0;
};

#endif // _SOCKETS_IFile_H


