#include "vbEdges.hpp"

vbEdges::vbEdges(void)
{
	m_strName   = "";
	m_usEdgeCount = 0;
	m_pEdges    = NULL;
    m_EdgeVBOID = 0;
	memset(m_ucColor,0,sizeof(unsigned char)*3);
}

vbEdges::~vbEdges(void)
{
	ClearBuffer();
}

bool vbEdges::LoadEgs(FILE* fp, FILE* fp_err_log)
{
	if(fp==NULL && fp_err_log!=NULL)
	{
#ifdef VB_CORE_DEBUG
        fprintf(fp_err_log,"[FILE OPEN ERROR] Egs file isn't opened.\r\n");
#endif
		return false;
	}

	vbEgsHeader header;
	fread(&header,sizeof(vbEgsHeader),1,fp);

	if(header.edge_type!=2 && fp_err_log!=NULL)
	{
#ifdef VB_CORE_DEBUG
		fprintf(fp_err_log,"[UNSUPPORTED EDGE TYPE] Unsupported edge type(%d).\r\n",header.edge_type);
#endif
		return false;
	}

	//Size
	fread(&m_usEdgeCount,sizeof(unsigned short),1,fp);

	//Edges
	if(m_pEdges)	delete [] m_pEdges;
	m_pEdges = new float[m_usEdgeCount * 6];

	fread(m_pEdges,sizeof(float),m_usEdgeCount * 6,fp);

	fclose(fp);

	return true;
}

size_t vbEdges::GetVBOBufferSizeInByte()
{
    return sizeof(float) * m_usEdgeCount * 6;
}

void vbEdges::Clear(void)
{
	m_strName = "";
	m_usEdgeCount = 0;
    ClearBuffer();

    
    //VBO의 해제는 OpenGL을 통해서만 처리 되어야 하기 때문에, Mesh의 VBO 해제 시에 함께 제거 하도록 한다
}

void vbEdges::ClearBuffer(void)
{
	if(m_pEdges)	delete [] m_pEdges;
	m_pEdges = NULL;
}

void	vbEdges::SetColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_ucColor[0] = r;
	m_ucColor[1] = g;
	m_ucColor[2] = b;
}

void	vbEdges::SetColor(const unsigned char* rgb)
{
	memcpy(m_ucColor,rgb,sizeof(unsigned char)*3);
}
