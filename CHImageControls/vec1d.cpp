

#include "stdafx.h"
#include "vec1d.h"


vec1D::vec1D(unsigned int size, int offset, const float* data) : m_size(size), m_offset(offset), m_data(0)
{
    m_last = (m_size + m_offset) - 1;
    m_data = (float *)_aligned_malloc(m_size * sizeof(float), 16);     //MMX alignement
    
    for (unsigned int i = 0; i < m_size; i++) 
	{
        if (data != 0)
			m_data[i] = data[i];
        else
			m_data[i] = 0.0f;
    }

    m_data -= m_offset;
}

vec1D::vec1D(const vec1D& v) : m_data(0)
{
	init(v);
}

vec1D::~vec1D()
{
	close();
}

void vec1D::init(const vec1D& v)
{
    close();

    m_size = v.size();
    m_offset = v.first();
    const float* data = v.data();
   
    m_last = (m_size + m_offset) - 1;
    m_data = (float *)_aligned_malloc(m_size * sizeof(float), 16);     //MMX alignement    

    for (unsigned int i = 0; i < m_size; i++)
		m_data[i] = data[i];  

    m_data -= m_offset;
}

void vec1D::close()
{
    if (m_data != 0) 
	{
        m_data +=  m_offset;
        _aligned_free(m_data);
    }
}

