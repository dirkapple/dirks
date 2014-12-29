#ifndef _ANAMODULE_UTIL_H_
#define _ANAMODULE_UTIL_H_
#include <map>
#include <vector>
#include "func_get_buffer.h"
//xxx:key:xxxx:value:

struct T_Key
{
	T_Key(const T_Key &k) : mlen(k.mlen), mkey(k.mkey), type(k.type)
	{}
	T_Key(const void *key, size_t keylen) : mlen(keylen), mkey(key), type(0)
	{}
	bool operator<(const T_Key &r) const
	{
		return mlen<r.mlen || (mlen==r.mlen && memcmp(mkey, r.mkey, mlen)<0);
	}
	size_t mlen;
	const void *mkey;

	int type; // store type from chenliang !
};

struct DataPackText
{
	DataPackText(void *szbuffer, size_t length) : m_buffer((char*)szbuffer), m_length(length), m_offset(0)
	{}
	
	template<typename K,typename T>
	bool Put(const K * key,size_t nk,const T * value,size_t nv)
	{
	    return Put( reinterpret_cast<const void*>(key),nk * sizeof(K),reinterpret_cast<void const*>(value),nv*sizeof(T));
	}
	bool Put(const char *key, const char *value)
	{
		if(!key || !value) return false;
		return Put((const void*)key, strlen(key), (const void*)value, strlen(value));
	}
	// key 不能为空, value 可以为空
	bool Put(const void *key, size_t keylen, const void *value, size_t valuelen)
	{
		// assume the length of int is less than 15
		if(key==NULL || value==NULL || keylen==0 || m_offset+keylen+valuelen+15*2>m_length)
			return false;
		//m_offset += sprintf(m_buffer+m_offset, "%d:%s:%d:", keylen, key, vlen);
		m_offset += sprintf(m_buffer+m_offset, "%d:", keylen);
		memcpy(m_buffer+m_offset, key, keylen);
		m_offset += keylen;
		m_offset += sprintf(m_buffer+m_offset, ":%d:", valuelen);
		memcpy(m_buffer+m_offset, value, valuelen);
		m_offset += valuelen+1;
		*(m_buffer+m_offset)=':';
		return true;
	}
	
	void*  GetBuffer() const {return m_buffer;}
	size_t GetLength() const {return m_offset==0?0:m_offset-1;} // 因为buffer 的最后多出一个 ':'
	
private:
	char *m_buffer;
	size_t m_length, m_offset;
};

struct DataUnpackText
{
	DataUnpackText(void *szbuffer, size_t length):m_buffer(szbuffer)
	{
		Init(szbuffer, length);
	}
	
	bool Init(void *szbuffer, size_t length)
	{
		char *pcur=(char*)szbuffer, *pend=((char*)szbuffer+length);
		m_map.clear();
		
		while(pcur<pend)
		{
			// PARSER A KEY->VALUE & SAVE TO m_map
			// xxx:key:xxxx:value:
			long lkey, lvalue;
			T_RANGE range; 

			// KEY 
			lkey = strtol(pcur, (char**)&pcur, 10);
			if(lkey<=0)
				return false;
			++pcur;
			T_Key skey(pcur, lkey);

			// VALUE 
			pcur +=lkey + 1;
			lvalue = strtol(pcur, (char**)&pcur, 10);
			if(lvalue<0)
				return false;
			++pcur;
			range.first = pcur-(char*)szbuffer;
			range.second = lvalue;
				
			m_map[skey] = range;
			pcur +=lvalue;
			*pcur = '\0';
			++ pcur;
			
			//printf("KEY %s %d \n", skey.mkey, range.second-range.first);
		}
		return true;
	}

	template<typename K,typename V>
	bool Get( const K * key,int nk,V ** value,int * nv, int type=0 )
	{
		bool ret= Get(reinterpret_cast<void const*>(key),nk * sizeof(K),(void**)value,nv, type);
		*nv /= sizeof(V);
		return ret;
	}

	bool Get( const void *key, int keylen, void **sbegin, int *length, int type=0 ) 
	{
		*length = 0;
		if (sbegin==NULL||length==NULL)
			return false;
		
		T_Key skey(key, keylen);
		T_MAP::const_iterator it = m_map.find(skey);
		if (it==m_map.end())
		{
			if( type&FGB_BUFFERTYPE ) // if FGB_BUFFERTYPE is set !
			{
				skey.type = type&FGB_BUFFERTYPE;

				T_RANGE range; 
				range.first = 0;
				range.second = 0;	
				m_map[skey] = range;
			}

			return false;
		}
		
		*sbegin = (char*)m_buffer + it->second.first;
		*length = it->second.second;
		return !(it->second.first==0&&it->second.second==0); // if second==(0,0) return false !!!  cause the key was added by chengliang
		//return true;
	}
	
	template<typename K>
	bool has_key( const K * key,int nk) {
		char *sbegin=NULL;
		int length=0;
		if(key==NULL)
			return false;
		return Get( key, nk, &sbegin, &length );
	}
	
	bool has_key( const char *key ) {
		return has_key<char>( key, strlen(key) );
	}

	int GetKeys(std::vector<T_Key> &keys) const
	{
		for(T_MAP::const_iterator it=m_map.begin(); it!=m_map.end(); ++it)
		{
			keys.push_back(it->first);
		}
		return keys.size();
	}
	static void* CallbackGet( void *pThisParam, void *szkey, int keylen, int *pOutLen, int type=0 )
	{
		void *sbegin=NULL;
		if(pThisParam==NULL)
			return NULL;
		DataUnpackText *pThis = (DataUnpackText*)pThisParam;
		if(pThis->Get(szkey, keylen, &sbegin, pOutLen, type))
			return sbegin;
		else
			return NULL;
	}

private:
	typedef std::pair<size_t, size_t> T_RANGE;
	typedef std::map<T_Key, T_RANGE> T_MAP;
	const void *m_buffer;
	T_MAP m_map; 

public:
	// iterator for wangzhigang
	typedef T_Key  					key_type;
	typedef T_RANGE 				value_type;
	typedef T_MAP::iterator 		iterator;
	typedef T_MAP::const_iterator 	const_iterator;

	const_iterator begin() const
	{
		return m_map.begin();
	}
	const_iterator end() const
	{
		return m_map.end();
	}
};

#endif
