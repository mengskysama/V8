#ifndef __URL_H__
#define __URL_H__

#include "..\include\Types.h"

/********************* class Url definition **********/
// define some protocol type
enum Protocol{ 
	HTTP,
	HTTPS,
	FTP,
	MMS,
	MMSH,
	MMST,
	RTSP,
	NONE
};


class URL
{
	public:
		URL();
		URL(URL& url);
		URL& operator = (URL& url);
		// free the stack
		~URL();

		// set the url
		int set_url(const char *url);
		
		// reset the url
		int reset_url(const char *url);
		
		// the the encoded url
		char* get_url() const
		{ 
			return url; 
		}
		
		// get the protocol
		Protocol get_protocol() const
		{ 
			return protocol; 
		}
		
		// get the user
		char* get_user() const
		{ 
			return user; 
		}
		
		// get the user's password
		char* get_password() const
		{ 
			return password; 
		}
		
		// get the host
		char* get_host() const
		{ 
			return host; 
		} 
		
		// get the port
		int get_port() const
		{ 
			return port; 
		}
		
		// get the directory
		char* get_dir() const
		{ 
			return dir; 
		}
		
		// get the file
		char* get_file() const
		{ 
			return file; 
		}
		
		// get encode url and file needed by http protocol
		char* get_encoded_path() const
		{ 
			return path; 
		}
		
		// decode(), encode() and pre_encode() return the allocate memory, so need free
		char* decode(const char* url);
		char* encode(const char* url);
		char* pre_encode(const char* pre_url);
	private:
	
		int _parse_fragment(char* &url);
		int _parse_scheme(char* &url);
		int _parse_location(char* &url);
		int _parse_query_info(char* &url);
		int _parse_parameters(char* &url);
		int _parse_path(char* &url);
		int _parse(const char *url);
		void free_all(void);

		char *url; // the encoded url
		char *path;
		Protocol protocol;
		char *user;
		char *password;
		char *host;
		int port;
		char *dir;
		char *file;
		
	public:
		char * Addr;
		bool ResumeSupport;
		off_t FileSize;
};


#endif
