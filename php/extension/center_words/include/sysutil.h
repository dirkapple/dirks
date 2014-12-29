#ifndef _SYS_UTIL_H_
#define _SYS_UTIL_H_

#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <vector>

#ifdef WIN32
#include <atlstr.h>
#include <windows.h>
#else //NOT WIN32
#include <sys/time.h>
#include <sys/types.h>
#include <dirent.h>
#include <locale.h>
#include <strings.h>
#define stricmp strcasecmp
#endif

#ifdef WIN32
typedef struct __stat64 stat;
int stat(const char *path, struct stat *sb)
{
	return _stat64(path, sb);
}
#endif

struct sys {

#ifdef WIN32
#define DIR_SEP '\\'

	class DirIterator {
	private:
		WIN32_FIND_DATA _wfd32;
		HANDLE _h;
		std::string _root;
		bool _fnextfile;
		DirIterator *_subdir;

	public:
		DirIterator(const std::string strPath="") : _h(INVALID_HANDLE_VALUE), _fnextfile(false)
		{
			if(strPath!="" && Init(strPath))
			{
				;
			}
		}
		bool Init(const std::string strPath)
		{
			if(strPath[strPath.length()-1]!=DIR_SEP)
				_root = strPath + DIR_SEP;
			else
				_root = strPath;
		
			std::string sroot = _root + "*.*";
			_h = ::FindFirstFile(sroot.c_str(), &_wfd32);

			return _fnextfile=(_h!=INVALID_HANDLE_VALUE);
		}

		~DirIterator() 
		{
			if(_h!=INVALID_HANDLE_VALUE)
			{
				::FindClose(_h);
				_h = INVALID_HANDLE_VALUE;
			}
		}

		bool NextFile(std::string &filename)
		{
			if(_h==INVALID_HANDLE_VALUE)
				return false;
		
			if(_subdir!=NULL)
			{
				if(_subdir->NextFile(filename))
					return true;
				delete _subdir; 
				_subdir=NULL;
			}
        	
			while(_fnextfile)
			{
				if(_wfd32.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
				{
					if(stricmp(_wfd32.cFileName,".")!=0 && stricmp(_wfd32.cFileName,"..")!=0)
						// recursive the sub dir
					{
						_subdir = new DirIterator(_root + _wfd32.cFileName);
						if(_subdir->NextFile(filename))
							return true;

						delete _subdir;
						_subdir = NULL;
					}
					_fnextfile = ::FindNextFile(_h, &_wfd32);
				}
				else 
				{
					filename = _root + _wfd32.cFileName;
					_fnextfile = ::FindNextFile(_h, &_wfd32);
					return true;
				}
			}
			return false;
		}
	};

	static unsigned long GetTickCount(void)
	{
		return ::GetTickCount();
	}


#else
#define DIR_SEP '/'

	class DirIterator {
	private:
		struct dirent *m_dirp;
		DIR *m_dp;
		std::string m_path;
		struct DirIterator *_subdir;
		
	public:
		DirIterator(const std::string strPath=""): m_dirp(NULL), m_dp(NULL), _subdir(NULL) {
			if (strPath != "")
				if (Init(strPath))
				{
					//		throw "";
				}
		}
	
		bool Init(const std::string strPath)
		{
			if(strPath[strPath.length() - 1] != DIR_SEP)
				m_path = strPath + DIR_SEP;
			else
				m_path = strPath;
			if((m_dp = opendir(strPath.c_str())) == NULL)
			{
				printf("open dir [%s] error!\n", m_path.c_str());
				return false;
			}
			return true;
		}
		~DirIterator() {
			if (m_dp != NULL) {
				closedir(m_dp);
				m_dp = NULL;
			}
		}
		bool NextFile(std::string &filename)
		{
			// Check if opendir ok
			if(m_dp==NULL)
				return false;

		
			// check the subdir's iterator 
			if(_subdir!=NULL)
			{
				if(_subdir->NextFile(filename))
					return true;
				// CLOSE DIR
				delete _subdir;
				_subdir=NULL;
			}
		
			// 
			while ((m_dirp = readdir(m_dp)) != NULL)
			{
				if(strcmp(".", m_dirp->d_name) == 0 || strcmp("..",m_dirp->d_name) == 0)
					continue;
				if(m_dirp->d_type == DT_DIR)
				{
					_subdir = new DirIterator(m_path+m_dirp->d_name);
					if(_subdir->NextFile(filename))
						return true;
				
					delete _subdir;
					_subdir=NULL;
				}
				else
				{
					filename = m_path + m_dirp->d_name;
					return true;
				}
			}
			return false;
		}
	};

	static unsigned long GetTickCount(void)
	{
		struct timeval tval;
		gettimeofday(&tval, NULL);
		return (tval.tv_sec*1000 + tval.tv_usec/1000);
	}
	
#endif
	
	static bool is_dir(const char *file)
	{
		struct stat filestat;
		if(0!=stat(file, &filestat))
			return false;
		return filestat.st_mode&S_IFDIR;
	}
	
	static bool file_exists( const char* file )
	{
		struct stat filestat;
		return 0 == stat(file, &filestat);
	}
	
	static long file_put_contents(const char *filename, const char *content, size_t length, bool fappend=false)
	{
		FILE *hf=NULL;
		if(NULL==(hf=fopen(filename, fappend?"a+b":"w+b")))
			return -1;

		int len = fwrite((const void*)content, length, 1, hf);
		fclose(hf);
		return len;
	}

	static long file_get_contents(const char *filename, char *buffer, size_t bufferlen)
	{
		FILE *hf=NULL;

		if(bufferlen<=0 || NULL==(hf=fopen(filename,"rb")))
			return -1;

		long filelen; 
		fseek(hf, 0, SEEK_END);
		filelen = ftell(hf);

		bufferlen = filelen>=bufferlen ? bufferlen-1 : filelen;
		fseek(hf, 0, SEEK_SET);
		fread(buffer, 1, bufferlen, hf);
		fclose(hf);
	
		buffer[bufferlen]=0;
		return filelen;
	}

// 在html 文件头的前500 个字节找到<base href=''> 中指定的URL 
	static bool html_get_baseurl(const char *html, size_t len, std::string& baseurl, size_t limit=500)
	{
		const char *pb, *pe;
		const char *magic = "<base href=";
		const char *magicEx = "<BASE href=";

		limit = limit>len?len:limit;
		//pb = strnstr(html, limit, magic);
		pb = strstr(html, magic);
		if(pb==NULL){
		    pb = strstr(html, magicEx);
		}
		if(pb==NULL)
		{
		    return false;
		}
	
		pb += strlen(magic);	
		pe = strchr(pb+1, *pb);
		if(pe==NULL)
			return false;
	
		++ pb;
		baseurl.assign(pb, pe-pb);
		return true;
	}

	static bool rscandir(const char *path, std::vector<std::string> & filevec)
	{
		if(!file_exists(path))
		{
			return false;
		}
		else if(is_dir(path))
		{
			DirIterator dit;
			if(!dit.Init(path))
				return false;
			std::string filename; 
			while(dit.NextFile(filename))
			{
				char *p = strrchr(filename.c_str(),'.');
				if(p && (stricmp(p,".html") ==0 || stricmp(p,".htm") ==0 ) )
				{
					filevec.push_back(filename);
				}
			}
		}
		else
			filevec.push_back(path);
		return true;
	}
};

#endif
