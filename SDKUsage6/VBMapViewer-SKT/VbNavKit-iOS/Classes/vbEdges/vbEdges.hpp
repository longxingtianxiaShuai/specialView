#pragma once


#include "Vector.hpp"
#include <float.h>
#include <string>
#include <vector>
#include <iostream>

struct vbEgsHeader
{
	char			format_name[8];	// SBM-----
	unsigned char	version;		// version - from 1
	char			hasEdge;		// Has Edges
	unsigned char	edge_type;		// 2 (NOT INDEXED EDGES)
};


class vbEdges
{
public:
	vbEdges(void);
	~vbEdges(void);

private:
	std::string			m_strName;
	unsigned short		m_usEdgeCount;
	float*				m_pEdges;
	unsigned char		m_ucColor[3];
    
    unsigned int        m_EdgeVBOID;


public:
	//////////////////////////////// SET/GET
	std::string			GetName()									{	return m_strName;		}
	void				Setname(std::string strName)				{	m_strName = strName;	}

	unsigned short		GetEdgeCount()								{	return m_usEdgeCount;	}
	void				SetEdgeCount(unsigned short usEdgeCount)	{	m_usEdgeCount =  usEdgeCount;	}

	const float*		GetEdges()					{	return (const float*)m_pEdges;		}
	void				SetEdges(float* pEdges)		{	m_pEdges = pEdges;	}

	const unsigned char*		GetColor()					{	return (const unsigned char*)m_ucColor;	}
	void	SetColor(unsigned char r, unsigned char g, unsigned char b);
	void	SetColor(const unsigned char* rgb);

    unsigned int   GetVBOID()                       {   return m_EdgeVBOID; }
    void           SetVBOID(unsigned int uiVBOID)   {   m_EdgeVBOID = uiVBOID;  }
    size_t         GetVBOBufferSizeInByte();

	bool LoadEgs(FILE* fp, FILE* fp_err_log);
	void Clear(void);   //모두 초기화
    void ClearBuffer(); //버퍼만 초기화 - VBO 생성 후 버퍼 메모리 반환
};
