#ifndef _XMLOPERATOR_H
#define _XMLOPERATOR_H

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/vector.hpp>  //提供vector序列化能力
#include <boost/serialization/map.hpp>     //提供map序列化能力
#include <boost/serialization/hash_set.hpp>
#include <boost/serialization/hash_map.hpp>
#include <iosfwd>
#include <iostream>
#include <fstream>

using namespace std;
using namespace boost::serialization;
using namespace boost::archive;

//该类的使用方法见该类后面的注释
class XmlOperator
{
public:
	template<class T>
	void write(T& value, const char* pName)
	{
		try
		{
			filebuf fb;
			fb.open(pName, ios::out);
			std::ostream os(&fb);
			xml_oarchive oa(os);
			value.xml_serialize(oa);
			fb.close();
		}
		catch(std::exception& e)
		{
			cout<<e.what()<<endl;
		}
	}

	template<class T>
	void read(T& value, const char* pName)
	{
		try
		{
			filebuf fb;
			fb.open(pName, ios::in);
			std::istream is(&fb);
			xml_iarchive ia(is);
			value.xml_serialize(ia);
			fb.close();

		}
		catch(std::exception& e)
		{
			cout<<e.what()<<endl;
		}
	}
};

#define XML_PAIR(label, value) (ar & make_nvp(label, value))

 
//  class Test
//  {
//  public:
//  	template<typename Archive>
//  	void xml_serialize(Archive &ar)
//  	{
//  		XML_PAIR("a", a);
//  		XML_PAIR("b", b);
//  		XML_PAIR("string", m_str);
//  	}
//  public:
//  	int a;
//  	int b;
//  	std::string m_str;
//  };
//  
//  
//  
//  // 
//  void test()
//  {
//  	XmlOperator xml;
//  	Test t;
//  	t.a = 9;
//  	t.b = 8;
//  	t.m_str = "123e2142sdfd  sdfdsgdfg 546456 dfgxdfgdfhfghfgh 3254345  dfgdf";
//  	xml.write(t, "test.xml");
//  
//  	Test t1;
//  	xml.read(t1, "test.xml11");
//  }



#endif