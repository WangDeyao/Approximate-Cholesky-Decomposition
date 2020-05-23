#pragma once
//modification of http://bannalia.blogspot.com/2015/06/cache-friendly-binary-search.html
#include <algorithm>
#include <vector>
#include <iostream>
#include <immintrin.h>
#include <emmintrin.h>

template<typename T>
class levelorder_vector
{
  typedef std::vector<T> vector;
  
public:
  typedef typename vector::value_type             value_type;
  typedef typename vector::reference              reference;
  typedef typename vector::const_reference        const_reference;
  typedef typename vector::const_iterator         iterator;
  typedef typename vector::const_iterator         const_iterator;
  typedef typename vector::difference_type        difference_type;
  typedef typename vector::size_type              size_type;
  
  levelorder_vector(){}
  levelorder_vector(const levelorder_vector& x):impl(x.impl){}
  levelorder_vector& operator=(const levelorder_vector& x){impl=x.impl;return *this;}
  
  template<typename InputIterator>
  levelorder_vector(InputIterator first,InputIterator last)
  {
    vector aux(first,last);
    // std::sort(aux.begin(),aux.end());
    impl=aux;
    idx.resize(impl.size());
    insert(0,aux.size(),aux.begin());
  }
  
  const_iterator begin()const{return impl.begin();}
  const_iterator end()const{return impl.end();}
  const_iterator cbegin()const{return impl.cbegin();}
  const_iterator cend()const{return impl.cend();}
  friend bool    operator==(const levelorder_vector& x,const levelorder_vector& y)
                   {return x.impl==y.impl;}
  friend bool    operator!=(const levelorder_vector& x,const levelorder_vector& y)
                   {return x.impl!=y.impl;}
  void           swap(levelorder_vector& x){impl.swap(x.impl);}
  friend void    swap(levelorder_vector& x,levelorder_vector& y){x.swap(y);}
  size_type      size()const{return impl.size();}
  size_type      max_size()const{return impl.max_size();}
  bool           empty()const{return impl.empty();}
  
  int lower_bound(const T& x)const
  {
    size_type n=impl.size(),i=n,j=0;
    while(j<n){
    //   if(impl[j]<x){
    //     j=2*j+2;
    //   }
    //   else{
    //     i=j;
    //     j=2*j+1;
    //   }
        bool flag = (impl[j] < x);
        i = flag ? i : j;
        j = flag ? (j << 1) + 2 : (j << 1) + 1;
    }
    return idx[i];
  }
  
private:
  void insert(size_type i,size_type n,const_iterator first)
  {
    if(n){
      size_type h=root(n);
      impl[i]=*(first+h);
      idx[i]=first + h - begin();
      insert(2*i+1,h,first);
      insert(2*i+2,n-h-1,first+h+1);
    }
  }

  size_type root(size_type n)
  {
    if(n<=1)return 0;
    size_type i=2;
    while(i<=n)i*=2;
    return std::min(i/2-1,n-i/4);
  }
public: 
  vector impl;
  std::vector<int> idx;
};

inline int bitset_search(std::vector<double>& x, const int n, double z)
{
  int i = 0;
  int k = (n >> 1) + 1;
  int r;
  while(k >>= 1)
  {
    r = i | k;
    i = z >= x[r] ? r : i;
  }
  return i;
}

inline _mm128i bitset_search_simd(std::vector<double>& x, const int n, _mm256d z)
{
  _mm128i i = 0;
  int k = (n >> 1) + 1;
  _mm128i r;
  while(k >>= 1)
  {
    r = i | k;
    i = z >= x[r] ? r : i;
  }
  return i;
}