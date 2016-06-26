
#ifndef tablec
#define tablec

#include <vector>
#include <initializer_list>

/*template<typename T>
using t_iterator=typename T*;*/
namespace fas
{


template<typename T>
class table
{
public:
 // using t_iterator=T*;
//  template<typename T>
 
  class t_iterator
  {
  public:
    typedef std::random_access_iterator_tag iterator_category; 
    typedef T value_type; 
    typedef std::size_t difference_type; 
    typedef T*	pointer; 
    typedef T&	reference;
    friend class table<T>;
    //t_iterator(const table<T>* t):tab(t),place(0)
    //{}
    t_iterator(table<T>* t):tab(t),place(0)
    {}
    //t_iterator(const table<T>* t,std::size_t p):tab(t),place(p)
    //{}
    t_iterator(table<T>* t,std::size_t p):tab(t),place(p)
    {}
    t_iterator(const t_iterator&)=default;
    /*t_iterator operator=(const t_iterator& it)
    {
    	tab=it.tab;
    	place=it.place;
    }*///wrong! we should not define a copy assign fuc for a iterator
    /*std::size_t getplace()
    {
    	return place;
    }*/
    t_iterator operator=(const T& t)
    {
    	return tab->insert(*this,t);
    }
    T& operator*()
    {
    	if (place<tab->length)
    	  return *(tab->pts[place]);
    	else if (place==tab->length)
    	  return *(tab->pts[place-1]+1);
    }
    t_iterator operator+(std::size_t sz)
    {
    	t_iterator it(*this);
    	it.place+=sz;
    	return it;
    }
    t_iterator operator-(std::size_t sz)
    {
    	t_iterator it(*this);
    	it.place-=sz;
    	return it;
    }
    t_iterator& operator++()
    {
    	  place++;
    	  return *this;
    }
    t_iterator& operator++(int)
    {
    	  place++;
    	  return *this;
    }
    t_iterator& operator--()
    {
    	if (place>0)
    	  place--;
    	return *this;
    }
    t_iterator& operator--(int)
    {
    	if (place>0)
    	  place--;
    	return *this;
    }
    t_iterator& operator+=(std::size_t p)
    {
    	place+=p;
    	return *this;
    }
    t_iterator& operator-=(std::size_t p)
    {
    	if (place>=p)
    	  place-=p;
    	return *this;
    }
    bool operator==(t_iterator it)
    {
    //	if (&(this->tab)==&it.tab)
    	  return place==it.place;
    //	return false;
    }
    bool operator!=(t_iterator it)
    {
    	return !(*this==it);
    }
    bool operator<(t_iterator it)
    {
      //if (&(this->tab)==&it.tab)
    	  return place<it.place;
    	//return false;
    }
    bool operator>(t_iterator it)
    {
    	return place>it.place;
    }
    bool operator<=(t_iterator it)
    {
    	return place<=it.place;
    }
    bool operator>=(t_iterator it)
    {
    	return place>=it.place;
    }
    std::size_t operator-(t_iterator it)
    {
    	return it<*this?place-it.place:it.place-place;
    }
  private:
    table<T>* tab;
    std::size_t place;
  };
  ///////////////////////////
  class ct_iterator
  {
  public:
    typedef std::random_access_iterator_tag iterator_category; 
    typedef T value_type; 
    typedef std::size_t difference_type;
    typedef T*	pointer; 
    typedef T&	reference;
    friend class table<T>;
    //t_iterator(const table<T>* t):tab(t),place(0)
    //{}
    ct_iterator(const ct_iterator&)=default;
    ct_iterator(const table<T>* t):tab(t),place(0)
    {}
    //t_iterator(const table<T>* t,std::size_t p):tab(t),place(p)
    //{}
    ct_iterator(const table<T>* t,std::size_t p):tab(t),place(p)
    {}
    const T& operator*()
    {
    	if (place<tab->length)
    	  return *(tab->pts[place]);
    	else if (place==tab->length)
    	  return *(tab->pts[place-1]+1);
    }
    ct_iterator operator+(std::size_t sz)
    {
    	ct_iterator it(*this);
    	it.place+=sz;
    	return it;
    }
    ct_iterator operator-(std::size_t sz)
    {
    	ct_iterator it(*this);
    	it.place-=sz;
    	return it;
    }
    ct_iterator& operator++()
    {
    	  place++;
    	  return *this;
    }
    ct_iterator& operator++(int)
    {
    	  place++;
    	  return *this;
    }
    ct_iterator& operator--()
    {
    	if (place>0)
    	  place--;
    	return *this;
    }
    ct_iterator& operator--(int)
    {
    	if (place>0)
    	  place--;
    	return *this;
    }
    ct_iterator& operator+=(std::size_t p)
    {
    	place+=p;
    	return *this;
    }
    ct_iterator& operator-=(std::size_t p)
    {
    	if (place>=p)
    	  place-=p;
    	return *this;
    }
    bool operator==(ct_iterator it)
    {
    //	if (&(this->tab)==&it.tab)
    	  return place==it.place;
    //	return false;
    }
    bool operator!=(ct_iterator it)
    {
    	return !(*this==it);
    }
    bool operator<(ct_iterator it)
    {
      //if (&(this->tab)==&it.tab)
    	  return place<it.place;
    	//return false;
    }
    bool operator>(ct_iterator it)
    {
    	return place<it.place;
    }
    bool operator<=(ct_iterator it)
    {
    	return place<=it.place;
    }
    bool operator>=(ct_iterator it)
    {
    	return place>=it.place;
    }
    std::size_t operator-(ct_iterator it)
    {
    	return it<*this?place-it.place:it.place-place;
    }
  private:
    const table<T>* tab;
    std::size_t place;
  };
  ///////////////////////////
  friend class t_iterator;
  friend class ct_iterator;
  typedef T value_type;
  table()=default;
  table(const table<T>& tab)
  {
	  for (auto i=std::begin(tab.pts);i<std::end(tab.pts);i++)
	  {
		  pts.push_back(new T(**i));
		  length++;
	  }
  }
  table(table&& tab) noexcept :pts(tab.pts),length(tab.length)
  {
  	tab.pts.clear();
  }	
  table(const std::initializer_list<T>& itl)
  {
  	for (auto i=std::begin(itl);i<std::end(itl);i++)
  	{
  		pts.push_back(new T(*i));
  		length++;
  	}
  }
  table(std::size_t l)
  {
  	for (std::size_t i=0;i<l;i++)
  	{
  		pts.push_back(new T());
  		length++;
  	}
  }
  table(std::size_t l,const T& t)
  {
  	for (std::size_t i=0;i<l;i++)
  	{
  		pts.push_back(new T(t));
  		length++;
  	}
  }
  table<T>& operator=(table&&) noexcept;
  table<T>& operator=(const table<T>&);
  table<T>& operator=(std::initializer_list<T> itl)
  {
  	destory();
  	for (auto i=std::begin(itl);i<std::end(itl);i++)
  	{
  		pts.push_back(new T(*i));
  		length++;
  	}
  }
  //using value_type=T;
  void assign(std::initializer_list<T> itl)
  {
  	*this=itl;
  }
  void assign(t_iterator mt,t_iterator nt)
  {
  	table<T> t;
  	for (auto i=mt;i!=nt;i++)
  	{
  		t.push_back(*i);
  	}
  	destory();
  	*this=std::move(t);
  }
  void assign(ct_iterator mt,ct_iterator nt)
  {
  	table<T> t;
  	for (auto i=mt;i!=nt;i++)
  	{
  		t.push_back(*i);
  	}
  	destory();
  	*this=std::move(t);
  }
  void assign(std::size_t sz,const T& t)
  {
  	table<T> tab(sz,t);
  	*this=std::move(tab);
  }
  inline void clear()
  {
  	destory();
  }
  inline T& front()
  {
  	return *(pts[0]);
  }
  inline T& back()
  {
  	return *(pts[length-1]);
  }
  
  T& operator[](std::size_t);
  T& at(std::size_t);
  void push_front(const T&);
  void push_back(const T&);
  t_iterator begin()
  {
  	t_iterator it(this,0);
  	return it;
  }
  t_iterator end()
  {
  	t_iterator it(this,length);
  	return it;
  }
  ct_iterator begin() const
  {
  	ct_iterator cit(this);
  	return cit;
  }
  ct_iterator end() const
  {
  	ct_iterator cit(this,length);
  	return cit;
  }
  ct_iterator cbegin() const
  {
  	ct_iterator cit(this);
  	return cit;
  }
  ct_iterator cend() const
  {
  	ct_iterator cit(this,length);
  	return cit;
  }
  
  t_iterator insert(t_iterator it,const T& t)
  {
  	pts.insert(pts.begin()+it.place,new T(t));
  	length++;
  	return it;
  }
  t_iterator insert(t_iterator it,std::size_t sz,const T& t)
  {
  		pts.insert(pts.begin()+it.place,sz,new T(t));
  		length+=sz;
  		return it;
  }
  t_iterator insert(t_iterator it,t_iterator mt,t_iterator nt)
  {
  	std::size_t x=0;
  	for (auto i=mt;i!=nt;i++)
  	{ 
  		insert(it+x++,*i);
  	}
  	return it;
  }
  t_iterator insert(t_iterator it,ct_iterator mt,ct_iterator nt)
  {
  	std::size_t x=0;
  	for (auto i=mt;i!=nt;i++)
  	{
  	  insert(it+x++,*i);
  	}
  	return it;
  }
  
  template<typename... Args>
  t_iterator emplace(t_iterator it,Args&&... arg)
  {
  	pts.insert(pts.begin()+it.place,new T(std::forward<Args>(arg)...));
  	length++;
  	return it;
  }
  
  template<typename... Args>
  void emplace_front(Args&& ...arg)
  {
  	pts.insert(pts.begin(),new T(std::forward<Args>(arg)...));
  	length++;
  }
  
   template<typename... Args>
  void emplace_back(Args&& ...arg)
  {
  	pts.insert(pts.end(),new T(std::forward<Args>(arg)...));
  	length++;
  }
  
  t_iterator erase(t_iterator it)
  {
  	delete *(pts.begin()+it.place);
  	pts.erase(pts.begin()+it.place);
  	length--;
  	return it;
  }

  void pop_front()
  {
  	delete *(pts.begin());
  	pts.erase(pts.begin());
  	length--;
  }
  void pop_back()
  {
  	delete *(pts.end()-1);
  	pts.erase(pts.end()-1);
  	length--;
  }
  inline std::size_t size() const
  {
  	return length;
  }
  bool operator==(const table<T>& t) const
  {
  	if (size()==t.size())
  	{
  		for (std::size_t i=0;i<size();i++)
  		{
  			if (!(*pts[i]==*(t.pts[i])))
  			  return false;
  		}
  		return true;
  	}
  	return false;
  }
  bool operator!=(const table<T>& t) const
  {
  	return !(*this==t);
  }
  bool operator<(const table<T>& t) const
  {
  	if (size()<t.size())
  	{
  		for (std::size_t i=0;i<size();i++)
  		{
  			if (!(*pts[i]==*(t.pts[i])))
  			  return *pts[i]<(*t.pts[i]);
  		}
  		return true;
  	}
  	else
  	{
  		for (std::size_t i=0;i<t.size();i++)
  		{
  			if (!(*pts[i]==*(t.pts[i])))
  			  return *pts[i]<(*t.pts[i]);
  		}
  		return false;
  	}
  }
  bool operator>(const table<T>& t) const
  {
  	return t<*this;
  }
  bool operator<=(const table<T>& t) const
  {
    return (*this<t)||*this==t;
  }
  bool operator>=(const table<T>& t) const
  {
  	return !(*this<t)||*this==t;
  }
  
  ~table()
  {
     destory();	
  }
 
private:
  std::vector<T*> pts;
  std::size_t length=0;
  void destory();
  
};



template<typename T>
table<T>& table<T>::operator=(table<T>&& tab) noexcept
{
	if (this!=&tab)
	{
		destory();
		pts=tab.pts;
		length=tab.length;
		tab.pts.clear();
	}
	return *this;
}

template<typename T>
table<T>& table<T>::operator=(const table<T>& tab)
{
	if (this!=&tab)
	{
		destory();
	  for (auto i=std::begin(tab.pts);i<std::end(tab.pts);i++)
  	{
		  pts.push_back(new T(**i));
	  	length++;
  	}
	}
	return *this;
}

template<typename T>
T& table<T>::operator[](std::size_t n)
{
	if (n<length)
	  return *pts[n];
	//return nullptr;
}

template<typename T>
T& table<T>::at(std::size_t n)
{
	if (n<length)
	  return *pts[n];
	//return nullptr;
}

template<typename T>
void table<T>::destory()
{
	for (auto& i:pts)
	{
		delete i;
		length--;
	}
	pts.clear();
}

template<typename T>
void table<T>::push_front(const T& t)
{
  pts.insert(pts.begin(),new T(t));
  length++;
}

template<typename T>
void table<T>::push_back(const T& t)
{
  pts.push_back(new T(t));
  length++;
}

}

#endif
