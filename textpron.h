

#ifndef TEXT_PRO_1
#define TEXT_PRO_1
#include <string>
#include "faster.h"
//using std::string;

#define fblack textpro::fcolors::black
#define fred textpro::fcolors::red
#define fgreen textpro::fcolors::green
#define fyellow textpro::fcolors::yellow
#define fblue textpro::fcolors::blue
#define fpurple textpro::fcolors::purple
#define fdgreen textpro::fcolors::dgreen
#define fwhite textpro::fcolors::white

#define bblack textpro::bcolors::black
#define bred textpro::bcolors::red
#define bgreen textpro::bcolors::green
#define byellow textpro::bcolors::yellow
#define bblue textpro::bcolors::blue
#define bpurple textpro::bcolors::purple
#define bdgreen textpro::bcolors::dgreen
#define bwhite textpro::bcolors::white

namespace fas
{

class textpro
{
public:
  textpro()=default;
  textpro& operator= (const textpro&)=delete;
  textpro(const textpro&)=delete;
  enum class fcolors:int 
  {
    black=30,
    red=31,
    green=32,
    yellow=33,
    blue=34,
    purple=35,
    dgreen=36,
    white=37
  };
  enum class bcolors:int 
  {
    black=40,
    red=41,
    green=42,
    yellow=43,
    blue=44,
    purple=45,
    dgreen=46,
    white=47
  };
  std::string text;
  static std::string scolor(std::string tex,fcolors c,bcolors b=bcolors::black)
  {
    return sc(tex,c,b,tex.begin(),tex.end());
  }

  static std::string underline(std::string tex)
  {
    return udrln(tex,tex.begin(),tex.end());
  }

  static std::string antic(std::string tex)
  {
    return ac(tex,tex.begin(),tex.end());
  }
  static std::string hid(std::string tex)
  {
  	std::string temf="\33[8m";
  	tex=temf+tex+"\33[0m";
  	return tex;
  }
  static std::string movec(char w,int n)
  {
  	std::string t,c;
  	c=fas::tstring(n);
  	switch (w)
  	{
  		case 'u':
  		  t="\33["+c+"A";
  		  break;
  		case 'd':
  		  t="\33["+c+"B";
  		  break;
  		case 'r':
  		  t="\33["+c+"C";
  		  break;
    	case 'l':
  		  t="\33["+c+"D";
  		  break; 		 
    } 
    return t;
  }
  static std::string movec(int x,int y)
  {
  	std::string a,b,t;
  	a=fas::tstring(x);
  	b=fas::tstring(y);
  	t="\33["+b+";"+a+"H";
  	return t;
  }
  static std::string clrall()
  {
  	return "\33[1;1H\33[0m\33[2J";
  }
  static std::string clrlin(int n)
  {
  	std::string c=fas::tstring(n),s;
  	s="\33["+c+"D"+"\33[K";
  	return s;
  }
/*  string curs()
  {
  	return "\33?25h";
  }
  string curh()
  {
  	return "\33?25l";
  }
*/
  static std::string cursav()
  {
  	return "\33[s";
  }
  static std::string curu()
  {
  	return "\33[u";
  }
  

private:
  static std::string sc(std::string t,fcolors c,bcolors b,decltype(text.begin()) star,decltype(text.end()) endb)
  {
    std::string temf="\33["+fas::tstring(static_cast<int>(c))+';'+fas::tstring(static_cast<int>(b))+'m';
    std::string temb="\33[0m";

    t=temf+t+temb;
    return t;
  }
  static std::string udrln(std::string t,decltype(text.begin()) star,decltype(text.end()) endb)
  {
    std::string temf="\33[4m";
    std::string temb="\33[0m";

    t=temf+t+temb;
    return t;
  }
  static std::string ac(std::string t,decltype(text.begin()) star,decltype(text.end()) endb)
  {
    std::string temf="\33[7m";
    std::string temb="\33[0m";

    t=temf+t+temb;
    return t;
  }
}tpro;

}
#endif
