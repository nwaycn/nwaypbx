#include "stdafx.h"
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "File.h"



File::File()
:m_fil(NULL)
,m_rptr(0)
,m_wptr(0)
{
}


File::File(const std::string& path, const std::string& mode)
:m_fil(NULL)
,m_rptr(0)
,m_wptr(0)
{
	fopen(path, mode);
}


File::~File()
{
	fclose();
}


bool File::fopen(const std::string& path, const std::string& mode)
{
	m_path = path;
	m_mode = mode;
#if defined( _WIN32) && !defined(__CYGWIN__)
	if (fopen_s(&m_fil, path.c_str(), mode.c_str()))
		m_fil = NULL;
#else
	m_fil = ::fopen(path.c_str(), mode.c_str());
#endif
	//setvbuf(m_fil,sFileBuffer,_IOLBF,FILEBUFLEN);
	return m_fil ? true : false;
}


void File::fclose() const
{
	if (m_fil)
	{
		::fclose(m_fil);
		m_fil = NULL;
	}
}



size_t File::fread(char *ptr, size_t size, size_t nmemb) const
{
	size_t r = 0;
	if (m_fil)
	{
		fseek(m_fil, m_rptr, SEEK_SET);
		r = ::fread(ptr, size, nmemb, m_fil);
		m_rptr = ftell(m_fil);
	}
	return r;
}


size_t File::fwrite(const char *ptr, size_t size, size_t nmemb)
{
	size_t r = 0;
	if (m_fil)
	{
		fseek(m_fil, m_wptr, SEEK_SET);
		r = ::fwrite(ptr, size, nmemb, m_fil);
		m_wptr = ftell(m_fil);
		//fflush(m_fil);
	}
	return r;
}



char *File::fgets(char *s, int size) const
{
	char *r = NULL;
	if (m_fil)
	{
		fseek(m_fil, m_rptr, SEEK_SET);
		r = ::fgets(s, size, m_fil);
		m_rptr = ftell(m_fil);
	}
	return r;
}


void File::fprintf(const char *format, ...)
{
	if (!m_fil)
		return;
	va_list ap;
	va_start(ap, format);
	fseek(m_fil, m_rptr, SEEK_SET);
	vfprintf(m_fil, format, ap);
	m_rptr = ftell(m_fil);
	va_end(ap);
}


off_t File::size() const
{
	struct stat st;
	if (stat(m_path.c_str(), &st) == -1)
	{
		return 0;
	}
	return st.st_size;
}


bool File::eof() const
{
	if (m_fil)
	{
		if (feof(m_fil))
			return true;
	}
	return false;
}


void File::reset_read() const
{
	m_rptr = 0;
}


void File::reset_write()
{
	m_wptr = 0;
}


const std::string& File::Path() const
{
	return m_path;
}



