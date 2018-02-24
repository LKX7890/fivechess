#ifndef _DLL_EXPORT_H_
#define _DLL_EXPORT_H_

#pragma warning(disable:4786)
#pragma warning(disable:4275)
#pragma warning(disable:4251)
#pragma warning(disable:4005)
#pragma warning(disable:4308)


#ifndef NETWORK_API
#define NETWORK_API __declspec(dllimport)
#else
#define NETWORK_API __declspec(dllexport)
#endif




#endif