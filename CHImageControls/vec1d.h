
#ifndef Vec1D_h
#define Vec1D_h

/*
  arrange -3 -2 -1 0 +1 +2  indeces array

    vec1D v(6,-3);
     v(-3) = val1;
     v(-2) = val2;
     ...
     v[2] = val6

*/

class vec1D          //16 bit MMX aligned array
{
public:
    vec1D(unsigned int size = 1, int offset = 0, const float* data = 0);
    vec1D(const vec1D& v);
    ~vec1D();

    inline int first() const;                       //return first index into array  m_data[0]
    inline int last() const;                        //return last index into array   m_data[size-1]
    inline unsigned int size() const;               //total size of array
    inline const float* data() const;               //return m_data pointer    
    inline const float* data(int i) const;          //return m_data pointer    
    inline float& operator()(int x);                //operator m_data[x]
    inline float operator()(int x) const;           //const operator m_data[x] for funcs( const vec1D& v )
    inline float& operator[](int x);                //operator m_data[x]
    inline float operator[](int x) const;           //const operator m_data[x] for funcs( const vec1D& v )
    inline const vec1D& operator=(const vec1D& v);  //operator =

private:
    int m_offset;
    int m_last;
    unsigned int m_size;
    float* m_data;

    void init(const vec1D& v);
    void close();
};


inline int vec1D::first() const
{ 
	return m_offset; 
}

inline int vec1D::last() const
{ 
    return m_last; 
}

inline unsigned int vec1D::size() const
{ 
    return m_size; 
}

inline float& vec1D::operator()(int x)
{ 
    return m_data[x];
}

inline float vec1D::operator()(int x) const
{
    return m_data[x];
}

inline float& vec1D::operator[](int x)
{ 
    return m_data[x];
}

inline float vec1D::operator[](int x) const
{
    return m_data[x];
}

inline const vec1D& vec1D::operator=(const vec1D& v)
{
    if (this != &v)
		init(v);
        
	return *this;    
}

inline const float* vec1D::data() const
{ 
    return &m_data[first()]; 
}

inline const float* vec1D::data(int i) const
{
    return &m_data[i]; 
}

#endif