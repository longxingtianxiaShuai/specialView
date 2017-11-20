#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
//#include "Interfaces.hpp"
//#include "Matrix.hpp"
#include <iostream>
#include <assert.h>
using namespace std;

#include "RenderingBase.h"

namespace ES2 {

#define STRINGIFY(A)  #A
#include "../../Shaders/TexturedLighting.vert"
#include "../../Shaders/TexturedLighting.frag"

struct UniformHandles {
    GLuint Modelview;
    GLuint Projection;
//    GLuint NormalMatrix;  //고정 조명을 쓰기 때문에 곱해줄 필요가 없다.
    GLuint LightPosition;
    GLint AmbientMaterial;
    GLint SpecularMaterial;
    GLint Shininess;
    GLint Sampler;
    GLint RenderMode;
    GLint DiffuseAlpha;
};

struct AttributeHandles {
    GLint Position;
    GLint Normal;
    GLint DiffuseMaterial;
    GLint TextureCoord;
};
    
struct Drawable {
    GLuint VertexBuffer;
    GLuint IndexBuffer;
    int IndexCount;
};

class RenderingEngine : public RenderingBase
{
    
public:
    //같은 내용이 RenderingBase에 있지 않고, 각 랜더링 엔진에 있는 이유는 타입 정의를 1.1/2.0에서 각각 하기 때문임.
    
    //////////////// 공통
    struct  Framebuffers
    {
        GLuint  Small;
        GLuint  Big;
    };
    struct  Renderbuffers
    {
        GLuint  SmallColor;
        GLuint  SmallDepth;
        GLuint  BigColor;
        GLuint  BigDepth;
    };
    
    
    RenderingEngine(IResourceManager*);    
    void Render() const;
    void pkColorBasedRender();
    
    
    float offscreen_buffer[12];    //4 * 5(pos-3 + tex-2)
    float offscreen_texture_buffer[8];
    unsigned char offscreen_index_buffer[6];
    

    
private:
    GLuint BuildShader(const char* source, GLenum shaderType) const;
    GLuint BuildProgram(const char* vShader, const char* fShader) const;
            	
    Renderbuffers       m_renderbuffers;
    Framebuffers        m_framebuffers;
    GLuint              m_OffScreenTexture;
    
    
    //////////// ES2.0 Only
    UniformHandles      m_uniforms;
    AttributeHandles    m_attributes;

    
private:
    //Rendering
    void    PreGLState(vbMaterial::VBMESH_TYPE mesh_type) const;
    void    PostGLState(vbMaterial::VBMESH_TYPE mesh_type) const;
    
    void    RenderWorld() const;
    void    pkRenderWorld();
    void    RenderOnScreenRect() const;
    void    RenderOnScreenRectVBO() const;
   
    //Rendering contents 
    void    DrawPathRect() const;
    void    DrawMeshesSeperatedVertexArray() const;
    void    pkDrawMeshesSeperatedVertexArray();
    
    void    DrawMeshesWithVBO() const;
    void    pkDrawMeshesWithVBO();
    void    DrawInstancedMeshVBO(const vbMesh* pMesh) const;
    
    void    DrawMeshesSeperatedVertexArraySolid() const;
    void    DrawMeshesWithVBOSolid() const;
    
    void    DrawMeshesSeperatedVertexArrayColor() const;
    void    DrawMeshesWithVBOColor() const;
    
    
    void    DrawEdges() const;
    void    DrawGroundGrid() const;
    void    DrawMyPosDisc() const;
        


    //VBO
    void    DeleteAllVBO();
    bool    InitializeVBO();
    void    InitOffscreenVBO();
    
    //FBO
    void    InitFrameBuffer();
    ivec2   GetFBOSize() const;
    void    DeleteBaseFBOAndVBO();

    
    //Init
    void    Initialize();
    void    InitGLStates();
    void    ClearData();
    
    
    //Texture
    void    LoadTextures();
    bool    SetTexture(const string& name);
    
    void    ChekGLError();

    //Picking
    bool    GetColorcodedMeshIDAt(int tx, int ty, unsigned short* pMesh_ID) { return false;   }
  
   
};

IRenderingEngine* CreateRenderingEngine(IResourceManager* resourceManager)
{
    return new RenderingEngine(resourceManager);
}

RenderingEngine::RenderingEngine(IResourceManager* resourceManager)
{
    InitCommonState(resourceManager);


    //OnScreen buffer
    glGenRenderbuffers(1, &m_renderbuffers.SmallColor);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers.SmallColor);
    
    
    m_offscreenRectVBOArrayID=0;
    m_offscreenRectVBOTxID=0;
    m_offscreenRectVBOIndexID=0;
}
 
    //////////////////// [START]Initialize & Clear
    void RenderingEngine::Initialize()
    {
        //Framebuffer
        InitFrameBuffer();
        
        //VBO
        InitOffscreenVBO();
    
        //State 
        InitGLStates();
    }
    
    void RenderingEngine::InitFrameBuffer()     // Color & Depth buffer
    {
        //Antialiasing을 위해 RTT super sampling을 쓰는데, 쓰지 않더라도 버퍼는 만들어 둔다.
        
        //////////////////////////////// Small Framebuffer /////////////////////////////////
        ivec2   size = GetFBOSize();
        
        // Create a depth buffer that has the same size as the color buffer.
        glGenRenderbuffers(1, &m_renderbuffers.SmallDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers.SmallDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,size.x, size.y);
        
        // Create the OnScreen framebuffer object.
        glGenFramebuffers(1, &m_framebuffers.Small);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers.Small);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER, m_renderbuffers.SmallColor);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, m_renderbuffers.SmallDepth);
        
        ////////Small Framebuffer /////////////////////////////////
        
        
        ////////Big Framebuffer /////////////////////////////////
        
        
        //Color buffer
        glGenRenderbuffers(1, &m_renderbuffers.BigColor);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers.BigColor);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, size.x*2, size.y*2);
        
        //Depth
        glGenRenderbuffers(1, &m_renderbuffers.BigDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers.BigDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,size.x*2, size.y*2);
        
        //Create OffScreen framebuffer
        glGenFramebuffers(1, &m_framebuffers.Big);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers.Big);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                  GL_RENDERBUFFER, m_renderbuffers.BigColor);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, m_renderbuffers.BigDepth);
        
        //Offscreen texture
        glGenTextures(1, &m_OffScreenTexture);
        glBindTexture(GL_TEXTURE_2D, m_OffScreenTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x*2, size.y*2, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_OffScreenTexture, 0);
        
        //Check FBO
        GLenum  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status != GL_FRAMEBUFFER_COMPLETE)
        {
#ifdef VB_CORE_DEBUG
            cout << "Incomplete OffScreen FBO" << endl;
#endif
        }
        //////////////////////////////// Big Framebuffer /////////////////////////////////
        
    }
    
    //GL State
    void RenderingEngine::InitGLStates()
    {
        // Create the GLSL program.
        GLuint program = BuildProgram(SimpleVertexShader, SimpleFragmentShader);
        glUseProgram(program);
        
        // Extract the handles to attributes and uniforms.
        m_attributes.Position           = glGetAttribLocation(program, "Position");
        m_attributes.Normal             = glGetAttribLocation(program, "Normal");
        m_attributes.DiffuseMaterial    = glGetAttribLocation(program, "DiffuseMaterial");
        m_attributes.TextureCoord       = glGetAttribLocation(program, "TextureCoord");
        m_uniforms.Projection           = glGetUniformLocation(program, "Projection");
        m_uniforms.Modelview            = glGetUniformLocation(program, "Modelview");
        //m_uniforms.NormalMatrix         = glGetUniformLocation(program, "NormalMatrix");
        m_uniforms.LightPosition        = glGetUniformLocation(program, "LightPosition");
        m_uniforms.AmbientMaterial      = glGetUniformLocation(program, "AmbientMaterial");
        m_uniforms.SpecularMaterial     = glGetUniformLocation(program, "SpecularMaterial");
        m_uniforms.Shininess            = glGetUniformLocation(program, "Shininess");
        m_uniforms.DiffuseAlpha         = glGetUniformLocation(program, "DiffuseAlpha");
        m_uniforms.Sampler              = glGetUniformLocation(program, "Sampler");
        m_uniforms.RenderMode           = glGetUniformLocation(program, "RenderMode");
        
        // Set the active sampler to stage 0.  Not really necessary since the uniform
        // defaults to zero anyway, but good practice.
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(m_uniforms.Sampler, 0);
        
        
        // Set up some default material parameters.
        const float* pAmbientConf = m_pConf->fLightAmbient.Pointer();
        glUniform3f(m_uniforms.AmbientMaterial, pAmbientConf[0],pAmbientConf[1],pAmbientConf[2]);
       // glUniform3f(m_uniforms.SpecularMaterial, 1.f, 1.f, 1.f);
        glUniform1f(m_uniforms.DiffuseAlpha, 1.f);
        glUniform1f(m_uniforms.Shininess, 50);
        
        // Initialize various state.
        glEnableVertexAttribArray(m_attributes.Position);
        glEnableVertexAttribArray(m_attributes.Normal);
        glEnableVertexAttribArray(m_attributes.TextureCoord);
        glEnable(GL_DEPTH_TEST);
        
        glEnable(GL_POLYGON_OFFSET_FILL);
        
    }
    
    //Offscreen VBO
    void RenderingEngine::InitOffscreenVBO()
    {
        //Position Buffer
        offscreen_buffer[0]=-1.f;  offscreen_buffer[1]=-1.f;  offscreen_buffer[2]=0.f;
        offscreen_buffer[3]= 1.f;  offscreen_buffer[4]=-1.f;  offscreen_buffer[5]=0.f;
        offscreen_buffer[6]= 1.f;  offscreen_buffer[7]= 1.f;  offscreen_buffer[8]=0.f;
        offscreen_buffer[9]=-1.f;  offscreen_buffer[10]= 1.f; offscreen_buffer[11]=0.f;
        
        //Texture
        offscreen_texture_buffer[0]=0.f;   offscreen_texture_buffer[1]=0.f;
        offscreen_texture_buffer[2]=1.f;   offscreen_texture_buffer[3]=0.f;
        offscreen_texture_buffer[4]=1.f;   offscreen_texture_buffer[5]=1.f;
        offscreen_texture_buffer[6]=0.f;   offscreen_texture_buffer[7]=1.f;
        
        //Index
        offscreen_index_buffer[0]=0; offscreen_index_buffer[1]=1; offscreen_index_buffer[2]=2;
        offscreen_index_buffer[3]=0; offscreen_index_buffer[4]=2; offscreen_index_buffer[5]=3;
        
        
        glGenBuffers(1,&m_offscreenRectVBOArrayID);
        glBindBuffer(GL_ARRAY_BUFFER, m_offscreenRectVBOArrayID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, offscreen_buffer, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1,&m_offscreenRectVBOTxID);
        glBindBuffer(GL_ARRAY_BUFFER, m_offscreenRectVBOTxID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, offscreen_texture_buffer, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1,&m_offscreenRectVBOIndexID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_offscreenRectVBOIndexID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned char)*6, offscreen_index_buffer, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#ifdef VB_CORE_DEBUG
        std::cout << "[ES2]Offscreen VBO Array ID = " << m_offscreenRectVBOArrayID << std::endl;
        std::cout << "[ES2]Offscreen VBO Index ID = " << m_offscreenRectVBOIndexID << std::endl;
        
        GLboolean bIsOk = glIsBuffer(m_offscreenRectVBOArrayID);
        if(bIsOk==GL_FALSE)
            std::cout << "[ES2]Offscreen VBO Array ID isn't valid on creating.\r\n";
        bIsOk = glIsBuffer(m_offscreenRectVBOIndexID);
        if(bIsOk==GL_FALSE)
            std::cout << "[ES2]Offscreen VBO Index ID isn't valid on creating.\r\n";
#endif
        
    }
    
    bool RenderingEngine::InitializeVBO()
    {
        if(!m_pMeshManager) return false;
        
        vbMesh* pMesh = NULL;
        vector<vbMesh*>* meshes = NULL;
        vector<vbMesh*>::const_iterator itr_mesh;
        
        for(unsigned char i=0;i<NUM_MESH_VECTORS;i++)
        {
            meshes = m_pMeshManager->GetMeshes(i);
            if(meshes->size()==0)   continue;
            
            for (itr_mesh = meshes->begin(); itr_mesh != meshes->end(); ++itr_mesh)
            {
                
                pMesh = (vbMesh*)(*itr_mesh);
                
                // Create VBO Buffer
                GLuint VBOIDs[2];
                glGenBuffers(2, VBOIDs);
                
                // VBO for the vertices.
                
                glBindBuffer(GL_ARRAY_BUFFER, VBOIDs[0]);
                glBufferData(GL_ARRAY_BUFFER,
                             // pMesh->GetVertexBufferSizeInByte(),
                             pMesh->GetVertexArrayBufferSizeInByte(),
                             pMesh->GetVertexBuffer(),
                             GL_STATIC_DRAW);
                pMesh->SetVBOVertexBufferID(VBOIDs[0]);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                
                // VBO for the indices
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOIDs[1]);
                if(pMesh->IsByteIndexBuffer())
                {
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                                 pMesh->GetByteIndexBufferSizeInByte(),
                                 pMesh->GetByteIndexBuffer(),
                                 GL_STATIC_DRAW);
                }
                else
                {
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                                 pMesh->GetIndexBufferSizeInByte(),
                                 pMesh->GetIndexBuffer(),
                                 GL_STATIC_DRAW);
                }
                
                pMesh->SetVBOIndexBufferID(VBOIDs[1]);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                
                pMesh->ClearBuffer();
                
                ChekGLError();
            }
        }
        
        //Edge VBO
        vbEdges* pEdge = m_pMeshManager->GetEdges();
        if(pEdge->GetEdgeCount()>0)
        {
            GLuint edge_vbo_id = 0;
            
            glGenBuffers(1, &edge_vbo_id);
            
            // VBO for the vertices.
            
            glBindBuffer(GL_ARRAY_BUFFER, edge_vbo_id);
            glBufferData(GL_ARRAY_BUFFER,
                         pEdge->GetVBOBufferSizeInByte(),
                         pEdge->GetEdges(),
                         GL_STATIC_DRAW);
            pEdge->SetVBOID(edge_vbo_id);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            pEdge->ClearBuffer();
        }
        
        return true;
    }
    
    
    

    /////////////////////// [START]Clear & Release
    void RenderingEngine::ClearData()
    {
        if(m_pMtlManager==NULL)
        {
#ifdef VB_CORE_DEBUG
            std::cout << "[Warn] Material manager should be assigned to clear GL texture IDs." << "\r\n";
#endif
            return;
        }
        
        //Delete GL Textures
        unsigned short txCount = m_pMtlManager->GetMaterialCount();
        
        vector<vbMaterial*>* pMtls = m_pMtlManager->GetMaterials();
        vbMaterial* pMtl = NULL;
        GLuint GLtexID = 0;
        for(int k=0;k<txCount;k++)
        {
            pMtl = pMtls->at(k);
            
            if(!pMtl)                       continue;
            if (!pMtl->IsTexture())         continue;
            if(pMtl->IsGLReferencing())     continue;
            
            GLtexID = (GLuint)pMtl->GetGLTexID();
            
            if(glIsTexture(GLtexID)) glDeleteTextures(1,&GLtexID);
        }
        
        DeleteAllVBO();
        
    }
    
    void RenderingEngine::DeleteBaseFBOAndVBO()
    {
        //Offscreen texture
        glBindTexture(GL_TEXTURE_2D, 0);
        if(glIsTexture(m_OffScreenTexture)) glDeleteTextures(1, &m_OffScreenTexture);
        
        //Render buffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers.Big);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glDeleteRenderbuffers(1, &m_renderbuffers.BigColor);
        glDeleteRenderbuffers(1, &m_renderbuffers.BigDepth);
        
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers.Small);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glDeleteRenderbuffers(1, &m_renderbuffers.SmallColor);
        glDeleteRenderbuffers(1, &m_renderbuffers.SmallDepth);
        
        //Framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &m_framebuffers.Big);
        glDeleteFramebuffers(1, &m_framebuffers.Small);
        
        
        if(glIsBuffer(m_offscreenRectVBOArrayID))   glDeleteBuffers(1, &m_offscreenRectVBOArrayID);
        if(glIsBuffer(m_offscreenRectVBOTxID))      glDeleteBuffers(1, &m_offscreenRectVBOTxID);
        if(glIsBuffer(m_offscreenRectVBOIndexID))   glDeleteBuffers(1, &m_offscreenRectVBOIndexID);
    }
    
    void RenderingEngine::DeleteAllVBO()
    {
        //Check Mesh Manger
        if(m_pMeshManager==NULL)
        {
            //cout << "[ES1]No Mesh Manager assigned.\n";
            return;
        }
        
        vbMesh* pMesh = NULL;
        vector<vbMesh*>* meshes = NULL;
        vector<vbMesh*>::const_iterator itr_mesh;
        
        for(unsigned char i=0;i<NUM_MESH_VECTORS;i++)
        {
            meshes = m_pMeshManager->GetMeshes(i);
            if(meshes->size()==0)   continue;
            
            for (itr_mesh = meshes->begin(); itr_mesh != meshes->end(); ++itr_mesh)
            {
                
                pMesh = (vbMesh*)(*itr_mesh);
                
                //Vertex Buffer (Position+Normal+Texture)
                const unsigned int vt_buffer_id = pMesh->GetVBOVertexBufferID();
                if (glIsBuffer(vt_buffer_id))   glDeleteBuffers(1, &vt_buffer_id);
                
                //Index Buffer
                const unsigned int idx_buffer_id = pMesh->GetVBOIndexBufferID();
                if (glIsBuffer(idx_buffer_id))  glDeleteBuffers(1, &idx_buffer_id);
            }
        }
        
        //Delete Edge VBO
        unsigned int edge_vbo_id = m_pMeshManager->GetEdges()->GetVBOID();
        if (glIsBuffer(edge_vbo_id))
        {
            glDeleteBuffers(1, &edge_vbo_id);
            m_pMeshManager->GetEdges()->SetVBOID(0);
        }
        
    }
    
    /////////////////////// [END]Clear & Release
   
    
    ///////////////////////  [START]Set/Get
    ivec2 RenderingEngine::GetFBOSize()    const
    {
        ivec2   size;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &size.x);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &size.y);
        return size;
    }
    
    ///////////////////////  [END]Set/Get
   
     
    ///////////////////////  [START]Rendering
    void RenderingEngine::RenderWorld() const
    {
        glClearColor(m_pConf->fBgColor[0], m_pConf->fBgColor[1], m_pConf->fBgColor[2], m_pConf->fBgColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Set the viewport transform.
        ivec2 size = GetFBOSize();
        glViewport(0,0,size.x,size.y);
        
        
        // Set the projection transform.
        glUniformMatrix4fv(m_uniforms.Projection, 1, 0, m_pViewInfo->GetProjectionMatrixPointer());
        
        // Set the model-view transform.
        glUniformMatrix4fv(m_uniforms.Modelview, 1, 0, m_pViewInfo->GetViewMatrixPointer());
        
        // Set the light position.
        glUniform3fv(m_uniforms.LightPosition, 1, m_pConf->flightPosition.Pointer());
        
        /////////////////  Draw data - 향후 랜더링 종류별로 통합되어야 함.
        //Mesh
        if (m_render_mode==RENDERMODE_DEFAULT)
        {
            if(m_pConf->bUseVBO)    DrawMeshesWithVBO();
            else                    DrawMeshesSeperatedVertexArray();
        }
        else if(m_render_mode==RENDERMODE_SOLID_COLOR)
        {
            if(0)//m_pConf->bUseVBO)
                DrawMeshesWithVBOSolid();
            else                    DrawMeshesSeperatedVertexArraySolid();
        }


        //
        DrawEdges();// to be implemented
        
        //Path
        DrawPathRect();
        
    }
    
    void RenderingEngine::RenderOnScreenRect() const
    {
        //State check
#ifdef VB_CORE_DEBUG
        GLboolean bIsOk = glIsTexture(m_OffScreenTexture);
        if (bIsOk==GL_FALSE) {
            std::cout << "[ES2] Offscreen texture id isn't valid.\n";
            return;
        }
#endif
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ivec2 on_size = GetFBOSize();
        glViewport(0,0,on_size.x,on_size.y);
  
        
        glUniform1i(m_uniforms.RenderMode, RENDERMODE_TEXTURE_NO_LIGHT);
        glVertexAttrib3f(m_attributes.DiffuseMaterial, 1.f,  1.f,  1.f);
        glUniform1f(m_uniforms.DiffuseAlpha, 1);
        
        // Set the projection transform.
        mat4 mprj;
        mprj.Ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
        glUniformMatrix4fv(m_uniforms.Projection, 1, 0, mprj.Pointer());
        
        // Set the model-view transform.
        mat4 mview;
        mview.Identity();
        glUniformMatrix4fv(m_uniforms.Modelview, 1, 0, mview.Pointer());
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_OffScreenTexture);
        
        glVertexAttribPointer(m_attributes.Position, 3, GL_FLOAT, GL_FALSE, 0,(GLvoid*) offscreen_buffer);
        glVertexAttribPointer(m_attributes.TextureCoord, 2, GL_FLOAT, GL_FALSE, 0,(GLvoid*)  offscreen_texture_buffer);
        
#ifdef VB_CORE_DEBUG
        for (int i=0; i<4; i++) {
            std::cout << "Vtx = " << offscreen_buffer[i*3+0] << ", " << offscreen_buffer[i*3+1] << ", "<< offscreen_buffer[i*3+2] << std::endl;
        }
        for (int i=0; i<6; i++) {
            std::cout << "IDX = " << int(offscreen_index_buffer[i]) << std::endl;
        }
#endif
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)offscreen_index_buffer);
        
        glDisable(GL_TEXTURE_2D);
        
 /////////////////////
    }
    
    void RenderingEngine::RenderOnScreenRectVBO() const
    {
        //State check
#ifdef VB_CORE_DEBUG
        GLboolean bIsOk = glIsTexture(m_OffScreenTexture);
        if (bIsOk==GL_FALSE) {
            std::cout << "[ES2] Offscreen texture id isn't valid.\n";
            return;
        }
        bIsOk = glIsBuffer(m_offscreenRectVBOArrayID);
        if (bIsOk==GL_FALSE) {
            std::cout << "[ES2] Offscreen vertex array buffer id isn't valid.\n";
            return;
        }
        
        bIsOk = glIsBuffer(m_offscreenRectVBOIndexID);
        if (bIsOk==GL_FALSE) {
            std::cout << "[ES2] Offscreen vertex index buffer id isn't valid.\n";
            return;
        }
#endif

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        ivec2 on_size = GetFBOSize();
        glViewport(0,0,on_size.x,on_size.y);
        
        
        glUniform1i(m_uniforms.RenderMode, RENDERMODE_TEXTURE_NO_LIGHT);
        glVertexAttrib3f(m_attributes.DiffuseMaterial, 1.f,  1.f,  1.f);
        glUniform1f(m_uniforms.DiffuseAlpha, 1);
        
        // Set the projection transform.
        mat4 mprj;
        mprj.Ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
        glUniformMatrix4fv(m_uniforms.Projection, 1, 0, mprj.Pointer());
        
        // Set the model-view transform.
        mat4 mview;
        mview.Identity();
        glUniformMatrix4fv(m_uniforms.Modelview, 1, 0, mview.Pointer());
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_OffScreenTexture);
        
        
        glBindBuffer(GL_ARRAY_BUFFER, m_offscreenRectVBOArrayID);
        glVertexAttribPointer(m_attributes.Position, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
        
        
        glBindBuffer(GL_ARRAY_BUFFER, m_offscreenRectVBOTxID);
        glVertexAttribPointer(m_attributes.TextureCoord, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2,0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_offscreenRectVBOIndexID);
        glDrawElements(GL_TRIANGLES, sizeof(unsigned char) * 6 / sizeof(unsigned char), GL_UNSIGNED_BYTE, 0);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        glDisable(GL_TEXTURE_2D);
    }
    
   
    void RenderingEngine::Render() const
    {
        
        bool bUseSuperSampling = false;
        
        switch (m_eAniAliasingMode)
        {
            case AAMode_DYNAMIC:
            {
                if(m_pViewInfo->IsNavigation()==false)  bUseSuperSampling = true;
            }
                break;
                
            case AAMode_ALWAYS:
            {
                bUseSuperSampling = true;
            }
                break;
                
            case AAMode_NONE :
            default:
                break;
        }
         
        glEnableVertexAttribArray(m_attributes.Position);
        glEnableVertexAttribArray(m_attributes.Normal);
        glEnableVertexAttribArray(m_attributes.TextureCoord);
        
        if (0)//bUseSuperSampling)
        {
             //OffScreen Rendering
            glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers.Big);
            glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers.BigColor);
            RenderWorld();
            
            //OnScreen Rendering
            glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers.Small);
            glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers.SmallColor);
            
            //if(m_pConf->bUseVBO)    RenderOnScreenRectVBO();
            //else
            RenderOnScreenRect();
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers.Small);
            glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers.SmallColor);
            RenderWorld();
        }
        
        glFinish();  //GL 명령 완전히 실행 한 후 리턴되어야 한다면 glFinish를 쓴다.
        
        glDisableVertexAttribArray(m_attributes.Position);
        glDisableVertexAttribArray(m_attributes.Normal);
        glDisableVertexAttribArray(m_attributes.TextureCoord);

    }
    
    /////////////////// Rendering Contents
    //Vertex Array
    void RenderingEngine::DrawPathRect() const
    {
        vbPath* pVisiblePath = m_pPathManager->GetVisiblePath();
        if(!pVisiblePath)   return;
        
        
        // 남은 경로
        const float* pColorForward = m_pPathManager->GetForwardPathColor();
        const float* pPathColor = pVisiblePath->GetColorRGBA();
        if(pPathColor[0]<0.f)
        {
            glVertexAttrib3f(m_attributes.DiffuseMaterial, pColorForward[0], pColorForward[1], pColorForward[2]);
            glUniform1f(m_uniforms.DiffuseAlpha, pColorForward[3]);
        }
        else
        {
            glVertexAttrib3f(m_attributes.DiffuseMaterial, pPathColor[0], pPathColor[1], pPathColor[2]);
            glUniform1f(m_uniforms.DiffuseAlpha, pPathColor[3]);
        }
        
        glUniform1i(m_uniforms.RenderMode, RENDERMODE_NO_TEX_NO_LIGHT);
        
        glEnable(GL_BLEND);         //blend
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glVertexAttribPointer(m_attributes.Position, 3, GL_FLOAT, GL_FALSE, 0, pVisiblePath->GetPathRoadVts());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, pVisiblePath->GetVtCount()+4);
        
        glDisable(GL_BLEND);         //blend
    }
    
     
    void RenderingEngine::DrawMeshesSeperatedVertexArray() const
    {
        //Check Mesh Manger
        if(m_pMeshManager==NULL)
        {
            //cout << "[ES2]No Mesh Manager assigned.\n";
            return;
        }
        
        
        vbMesh* pMesh = NULL;
        vector<vbMesh*>* meshes = NULL;
        vector<vbMesh*>::const_iterator itr_mesh;
        vbMaterial* pMtl = NULL;
        float* pDiffuse = NULL;
        for(unsigned char i=0;i<NUM_MESH_VECTORS;i++)
        {
            meshes = m_pMeshManager->GetMeshes(i);
            if(meshes->size()==0)   continue;
           
            //전처리
            glUniform1i(m_uniforms.RenderMode, RENDERMODE_NO_TEX_LIGHT);
            
            
            if(i==vbMaterial::VBMESH_TEXTURE)
            {
                glEnable(GL_TEXTURE_2D);    //textured
                glUniform1i(m_uniforms.RenderMode, RENDERMODE_TEXTURE_LIGHT);
            }
            else if(i==vbMaterial::VBMESH_BLEND)
            {
                glEnable(GL_BLEND);         //blend
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            
            int k=0;
            for (itr_mesh = meshes->begin(); itr_mesh != meshes->end(); ++itr_mesh)
            {
                
                
                pMesh = (vbMesh*)(*itr_mesh);
                //glVertexPointer(3, GL_FLOAT,0, pMesh->GetVertexBuffer());  //Alway vertex buffer is exist.
                glVertexAttribPointer(m_attributes.Position, 3, GL_FLOAT, GL_FALSE, 0, pMesh->GetVertexBuffer());
                
                //Normal
                glVertexAttribPointer(m_attributes.Normal, 3, GL_FLOAT, GL_FALSE, 0, pMesh->GetNormalBuffer());
                
                
                //유효하지 않은 재질인 경우
                pMtl = m_pMtlManager->GetMaterial(pMesh->GetMaterialID());
                if(!pMtl)   continue;
                
                if(pMtl->GetFacing()==1 && glIsEnabled(GL_CULL_FACE))//Both
                    glDisable(GL_CULL_FACE);
                else if(pMtl->GetFacing()!=1 && !glIsEnabled(GL_CULL_FACE))//Single
                    glEnable(GL_CULL_FACE);
                
                
                //Mesh별 material 설정
                if(i==vbMaterial::VBMESH_TEXTURE) //Texture
                {
                    // Set the diffuse color.
                    //                    vec3 color = vec3(1.f,1.f,1.f);
                    glVertexAttrib3f(m_attributes.DiffuseMaterial, 1, 1, 1);
                    glUniform1f(m_uniforms.DiffuseAlpha, 1.f);
                    glBindTexture(GL_TEXTURE_2D, pMtl->GetGLTexID());
                    //glTexCoordPointer(2, GL_FLOAT, 0, pMesh->GetTexCoordBuffer());
                    glVertexAttribPointer(m_attributes.TextureCoord, 2, GL_FLOAT, GL_FALSE, 0, pMesh->GetTexCoordBuffer());
                    
                }
                else// if(i==1) //Blend
                {
                    pDiffuse = pMtl->GetDiffuse();
                    glVertexAttrib3f(m_attributes.DiffuseMaterial, pDiffuse[0], pDiffuse[1], pDiffuse[2]);
                    glUniform1f(m_uniforms.DiffuseAlpha, pDiffuse[3]);
                }
                
                try
                {
                    
                    if(pMesh->IsByteIndexBuffer())
                    {
                        if( pMesh->GetByteIndexBuffer() == NULL )
                            throw "glDrawElements Error!";
                        glDrawElements(GL_TRIANGLES, pMesh->GetFaceCount()*3, GL_UNSIGNED_BYTE, pMesh->GetByteIndexBuffer());
                    }
                    else
                    {
                        if( pMesh->GetIndexBuffer() == NULL )
                            throw "glDrawElements Error!";
                        glDrawElements(GL_TRIANGLES, pMesh->GetFaceCount()*3, GL_UNSIGNED_SHORT, pMesh->GetIndexBuffer());
                    }
                }
                catch ( char *s)
                {
#ifdef VB_CORE_DEBUG
                    std::cout << "vbMeshRenderer.ES2 LINE: " << __LINE__ << "Mesh Idx:" << i << " mesh iter: "<< k << " Error Msg:" << s << "\r\n";
#endif
                }
                
                k ++;
                
            }
            if(i==vbMaterial::VBMESH_TEXTURE)        glDisable(GL_TEXTURE_2D);    //textured
            else if(i==vbMaterial::VBMESH_BLEND)     glDisable(GL_BLEND);         //blend
            
            
            
        }
        
    }
    
    void RenderingEngine::DrawMeshesSeperatedVertexArraySolid() const
    {
        //Check Mesh Manger
        if(m_pMeshManager==NULL)
        {
            //cout << "[ES2]No Mesh Manager assigned.\n";
            return;
        }
        
        
        vbMesh* pMesh = NULL;
        vector<vbMesh*>* meshes = NULL;
        vector<vbMesh*>::const_iterator itr_mesh;
        vbMaterial* pMtl = NULL;
        float* pDiffuse = NULL;
        
        float* SolidColor = m_pConf->fSolidColor;
        glVertexAttrib3f(m_attributes.DiffuseMaterial, SolidColor[0],SolidColor[1],SolidColor[2]);
        glUniform1f(m_uniforms.DiffuseAlpha, SolidColor[3]);
        
        for(unsigned char i=0;i<NUM_MESH_VECTORS;i++)
        {
            meshes = m_pMeshManager->GetMeshes(i);
            if(meshes->size()==0)   continue;
            
            //전처리
            glUniform1i(m_uniforms.RenderMode, RENDERMODE_NO_TEX_LIGHT);
            if(i==vbMaterial::VBMESH_BLEND)
            {
                glEnable(GL_BLEND);         //blend
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            
            int k=0;
            for (itr_mesh = meshes->begin(); itr_mesh != meshes->end(); ++itr_mesh)
            {
                
                
                pMesh = (vbMesh*)(*itr_mesh);
                //glVertexPointer(3, GL_FLOAT,0, pMesh->GetVertexBuffer());  //Alway vertex buffer is exist.
                glVertexAttribPointer(m_attributes.Position, 3, GL_FLOAT, GL_FALSE, 0, pMesh->GetVertexBuffer());
                
                //Normal
                glVertexAttribPointer(m_attributes.Normal, 3, GL_FLOAT, GL_FALSE, 0, pMesh->GetNormalBuffer());
                
                
                //유효하지 않은 재질인 경우
                pMtl = m_pMtlManager->GetMaterial(pMesh->GetMaterialID());
                if(!pMtl)   continue;
                
                if(pMtl->GetFacing()==1 && glIsEnabled(GL_CULL_FACE))//Both
                    glDisable(GL_CULL_FACE);
                else if(pMtl->GetFacing()!=1 && !glIsEnabled(GL_CULL_FACE))//Single
                    glEnable(GL_CULL_FACE);
                
                
                if(i==1) //Blend
                {
                    pDiffuse = pMtl->GetDiffuse();
                    glUniform1f(m_uniforms.DiffuseAlpha, pDiffuse[3]);
                    
                }
                
                try
                {
                    
                    if(pMesh->IsByteIndexBuffer())
                    {
                        if( pMesh->GetByteIndexBuffer() == NULL )
                            throw "glDrawElements Error!";
                        glDrawElements(GL_TRIANGLES, pMesh->GetFaceCount()*3, GL_UNSIGNED_BYTE, pMesh->GetByteIndexBuffer());
                    }
                    else
                    {
                        if( pMesh->GetIndexBuffer() == NULL )
                            throw "glDrawElements Error!";
                        glDrawElements(GL_TRIANGLES, pMesh->GetFaceCount()*3, GL_UNSIGNED_SHORT, pMesh->GetIndexBuffer());
                    }
                }
                catch ( char *s)
                {
#ifdef VB_CORE_DEBUG
                    std::cout << "vbMeshRenderer.ES2 LINE: " << __LINE__ << "Mesh Idx:" << i << " mesh iter: "<< k << " Error Msg:" << s << "\r\n";
#endif
                }
                
                k ++;
                
            }
            if(i==vbMaterial::VBMESH_BLEND)     glDisable(GL_BLEND);         //blend
            
            
            
        }
    }

    void    RenderingEngine::PreGLState(vbMaterial::VBMESH_TYPE mesh_type) const
    {
        
    }
    
    void    RenderingEngine::PostGLState(vbMaterial::VBMESH_TYPE mesh_type) const
    {
        
    }
    
    
    void    RenderingEngine::DrawInstancedMeshVBO(const vbMesh* pMesh) const
    {
        
    }
    
    
    void RenderingEngine::DrawMeshesWithVBO() const
    {
        //Check Mesh Manger
        if(m_pMeshManager==NULL)
        {
            //cout << "[ES2]No Mesh Manager assigned.\n";
            return;
        }
        
        
        vbMesh* pMesh = NULL;
        vector<vbMesh*>* meshes = NULL;
        vector<vbMesh*>::const_iterator itr_mesh;
        vbMaterial* pMtl = NULL;
        float* pDiffuse = NULL;
        for(unsigned char i=0;i<NUM_MESH_VECTORS;i++)
        {
            meshes = m_pMeshManager->GetMeshes(i);
            if(meshes->size()==0)   continue;
           
            //전처리
            glUniform1i(m_uniforms.RenderMode, RENDERMODE_NO_TEX_LIGHT);
            
            if(i==vbMaterial::VBMESH_TEXTURE)
            { 
                glEnable(GL_TEXTURE_2D);    //textured
                glUniform1i(m_uniforms.RenderMode, RENDERMODE_TEXTURE_LIGHT);
            }
            else if(i==vbMaterial::VBMESH_BLEND)
            {
                glEnable(GL_BLEND);         //blend
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }

            
            int k=0;
            for (itr_mesh = meshes->begin(); itr_mesh != meshes->end(); ++itr_mesh)
            {
                pMesh = (vbMesh*)(*itr_mesh);

#ifdef VB_CORE_DEBUG
                bool bIsBuffer = glIsBuffer(pMesh->GetVBOVertexBufferID());
                if(!bIsBuffer) std::cout << "BAD VBO ID IN ES2\n";
#endif
                glBindBuffer(GL_ARRAY_BUFFER, pMesh->GetVBOVertexBufferID());
                glVertexAttribPointer(m_attributes.Position, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
                
                //Normal
                glVertexAttribPointer(m_attributes.Normal, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid*)(pMesh->GetVertexBufferSizeInByte()));
                
                
                //유효하지 않은 재질인 경우
                pMtl = m_pMtlManager->GetMaterial(pMesh->GetMaterialID());
                if(!pMtl)   continue;
                
                if(pMtl->GetFacing()==1 && glIsEnabled(GL_CULL_FACE))//Both
                    glDisable(GL_CULL_FACE);
                else if(pMtl->GetFacing()!=1 && !glIsEnabled(GL_CULL_FACE))//Single
                    glEnable(GL_CULL_FACE);
                
                
                //Mesh별 material 설정
                if(i==vbMaterial::VBMESH_TEXTURE) //Texture
                {
                    // Set the diffuse color.
                    glVertexAttrib3f(m_attributes.DiffuseMaterial, 1, 1, 1);
                    glUniform1f(m_uniforms.DiffuseAlpha, 1.f);
                    glBindTexture(GL_TEXTURE_2D, pMtl->GetGLTexID());
                    glVertexAttribPointer(m_attributes.TextureCoord, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (GLvoid*)(pMesh->GetVertexBufferSizeInByte()+pMesh->GetNormalBufferSizeInByte()));
                    
                }
                else
                {
                    pDiffuse = pMtl->GetDiffuse();
                    glVertexAttrib3f(m_attributes.DiffuseMaterial, pDiffuse[0], pDiffuse[1], pDiffuse[2]);
                    glUniform1f(m_uniforms.DiffuseAlpha, pDiffuse[3]);
                    
                }
 
#ifdef VB_CORE_DEBUG
                bIsBuffer = glIsBuffer(pMesh->GetVBOIndexBufferID());
                if(!bIsBuffer) std::cout << "BAD VBO INDEX ID IN ES2\n";
#endif
                
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->GetVBOIndexBufferID());                
                try
                {
                    if (pMesh->IsByteIndexBuffer())
                    {
                        glDrawElements(GL_TRIANGLES, pMesh->GetIndexCount(), GL_UNSIGNED_BYTE, 0); //int array for face index
                    }
                    else
                    {
                        glDrawElements(GL_TRIANGLES, pMesh->GetIndexCount(), GL_UNSIGNED_SHORT, 0); //int array for face index
                    }
                }
               
                catch ( char *s)
                {
#ifdef VB_CORE_DEBUG
                    std::cout << "vbMeshRenderer.ES2 LINE: " << __LINE__ << "Mesh Idx:" << i << " mesh iter: "<< k << " Error Msg:" << s << "\r\n";
#endif
                }
                
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                
                k ++;
                
            }
            if(i==vbMaterial::VBMESH_TEXTURE)        glDisable(GL_TEXTURE_2D);    //textured
            else if(i==vbMaterial::VBMESH_BLEND)     glDisable(GL_BLEND);         //blend
            
            
            
        }
        
    }
 
    
    void    RenderingEngine::DrawMeshesSeperatedVertexArrayColor() const
    {
        
    }
    void    RenderingEngine::DrawMeshesWithVBOColor() const
    {
        
    }
    
    
    
    void    RenderingEngine::DrawMeshesWithVBOSolid() const
    {
        //Check Mesh Manger
        if(m_pMeshManager==NULL)
        {
            //cout << "[ES2]No Mesh Manager assigned.\n";
            return;
        }
        
        
        vbMesh* pMesh = NULL;
        vector<vbMesh*>* meshes = NULL;
        vector<vbMesh*>::const_iterator itr_mesh;
        vbMaterial* pMtl = NULL;
        float* pDiffuse = NULL;
        
        float* SolidColor = m_pConf->fSolidColor;
        glVertexAttrib3f(m_attributes.DiffuseMaterial, SolidColor[0],SolidColor[1],SolidColor[2]);
        glUniform1f(m_uniforms.DiffuseAlpha, SolidColor[3]);
        
        for(unsigned char i=0;i<NUM_MESH_VECTORS;i++)
        {
            meshes = m_pMeshManager->GetMeshes(i);
            if(meshes->size()==0)   continue;
            
            //전처리
            glUniform1i(m_uniforms.RenderMode, RENDERMODE_NO_TEX_LIGHT);
            
            if(i==vbMaterial::VBMESH_BLEND)
            {
                glEnable(GL_BLEND);         //blend
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            
            
            int k=0;
            for (itr_mesh = meshes->begin(); itr_mesh != meshes->end(); ++itr_mesh)
            {
                pMesh = (vbMesh*)(*itr_mesh);

#ifdef VB_CORE_DEBUG
                bool bIsBuffer = glIsBuffer(pMesh->GetVBOVertexBufferID());
                if(!bIsBuffer) std::cout << "BAD VBO ID IN ES2\n";
#endif
                glBindBuffer(GL_ARRAY_BUFFER, pMesh->GetVBOVertexBufferID());
                glVertexAttribPointer(m_attributes.Position, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
                
                //Normal
                glVertexAttribPointer(m_attributes.Normal, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid*)(pMesh->GetVertexBufferSizeInByte()));
                
                
                //유효하지 않은 재질인 경우
                pMtl = m_pMtlManager->GetMaterial(pMesh->GetMaterialID());
                if(!pMtl)   continue;
                
                if(pMtl->GetFacing()==1 && glIsEnabled(GL_CULL_FACE))//Both
                    glDisable(GL_CULL_FACE);
                else if(pMtl->GetFacing()!=1 && !glIsEnabled(GL_CULL_FACE))//Single
                    glEnable(GL_CULL_FACE);
                
                
                if(i==vbMaterial::VBMESH_BLEND)
                {
                    pDiffuse = pMtl->GetDiffuse();
                    glUniform1f(m_uniforms.DiffuseAlpha, pDiffuse[3]);
                    
                }
                
#ifdef VB_CORE_DEBUG
                bIsBuffer = glIsBuffer(pMesh->GetVBOIndexBufferID());
                if(!bIsBuffer) std::cout << "BAD VBO INDEX ID IN ES2\n";
#endif
                
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->GetVBOIndexBufferID());
                try
                {
                    if (pMesh->IsByteIndexBuffer())
                    {
                        glDrawElements(GL_TRIANGLES, pMesh->GetIndexCount(), GL_UNSIGNED_BYTE, 0); //int array for face index
                    }
                    else
                    {
                        glDrawElements(GL_TRIANGLES, pMesh->GetIndexCount(), GL_UNSIGNED_SHORT, 0); //int array for face index
                    }
                }
                
                catch ( char *s)
                {
#ifdef VB_CORE_DEBUG
                    std::cout << "vbMeshRenderer.ES2 LINE: " << __LINE__ << "Mesh Idx:" << i << " mesh iter: "<< k << " Error Msg:" << s << "\r\n";
#endif
                }
                
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                
                k ++;
                
            }
            if(i==vbMaterial::VBMESH_BLEND)     glDisable(GL_BLEND);         //blend
            
            
            
        }
    }
    
    
    
    
    
    
    void    RenderingEngine::DrawEdges() const
    {
        
    }
    
    
    ///////////////////////  [END]Rendering

    
    
    
    void RenderingEngine::LoadTextures()
    {
        unsigned short txCount = m_pMtlManager->GetMaterialCount();
        
        vector<vbMaterial*>* pMtls = m_pMtlManager->GetMaterials();
        //for (vector<vbMaterial>::iterator itr_mtl = pMtls->begin();itr_mtl!=pMtls->end(); itr_mtl++)
        
        //cout << "RenderingEngine.ES2 :  " << __LINE__ << " texture Count: " << txCount << "\r\n" ;
        
        for(int k=0;k<txCount;k++)
        {
            vbMaterial* pMtl = pMtls->at(k);
            
            //Texture없는 Material - 색상만 사용
            //if (texname==UNDEFINED_MATERIAL_NAME)
            if (!pMtl->IsTexture())
            {
                continue;
            }
            
            //텍스쳐인 경우에만 이름이 있으므로, 확인 후 이름을 얻어야 한다.
            std::string texname = pMtl->GetTextureName();//.substr(0,texname.length()-4);
            
            //    cout << "RenderingEngine.ES2 :  " << __LINE__ << " idx: " << k << " texture name: " << texname << "\r\n"; 
            
            GLuint txId = 0;
            glGenTextures(1, &txId);
            
            pMtl->SetGLTexID(txId);
            
            glBindTexture(GL_TEXTURE_2D, txId);
            //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);  //ES 1.1
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); 
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR); 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            
            if(!SetTexture(texname))
            {
                glDeleteTextures(1, &txId);
                pMtl->SetTextureName(" ", 1);
                pMtl->SetTextureMaterial(false);
            }
            //***  ES2.0에서는 반드시 텍스쳐 데이터를 설정한 이후에 MipMap을 설정해야 한다.
            //glGenerateMipmap(GL_TEXTURE_2D);                                //ES 2.0
        }
        
        
    }
    
    bool RenderingEngine::SetTexture(const string& texname)
    {
        //std::cout << "Texture name = " << texname << "\r\n";
        
        TextureDescription description;
        
        //Format(Extension) check
        unsigned char name_len = 0;
        name_len = texname.length();
        std::string name = texname.substr(0,name_len-4);
        std::string extension = texname.substr(name_len-3,name_len);
        std::string full_path = m_texture_base_path + texname;                  //for local file
        
        /*Debug
        std::cout << "full path = " << full_path << "\r\n";
         */
        
        if(m_bUseLocalTexture)
        {
            // std::cout << "Local texture path = " << full_path << "\r\n";
            description = m_resourceManager->LoadImage(full_path);//,true);
        }
        else
        {//Resource
            
            if(extension=="jpg" || extension=="JPG")
                description = m_resourceManager->LoadImage(texname);//,false);
            else if(extension=="png" || extension=="PNG")
                description = m_resourceManager->LoadResourcePngImage(name); 
            else
            {
#ifdef VB_CORE_DEBUG
                std::cout << "Texture file extension isn't jpg nor png. [Extension = " << extension << "\r\n";
#endif
                return false;
            }
        }
        
        if(description.Size.x==0 && description.Size.y==0)
        {
#ifdef VB_CORE_DEBUG
            std::cout << "Unsupported image byte format at " << texname << "\r\n";
#endif
            return false;
        }
        
        
        GLenum format;
        switch (description.Format)
        {
            case TextureFormatGray:         format = GL_LUMINANCE;          break;
            case TextureFormatGrayAlpha:    format = GL_LUMINANCE_ALPHA;    break;
            case TextureFormatRgb:          format = GL_RGB;                break;
            case TextureFormatRgba:         format = GL_RGBA;               break;
            default:
                break;
        }
        
        GLenum type;
        switch (description.BitPerComponent)
        {
            case 8:     type = GL_UNSIGNED_BYTE;        break;
            case 4:
                if(format==GL_RGBA)
                {
                    type = GL_UNSIGNED_SHORT_4_4_4_4;
                    break;
                }
                
            default:
                
                break;
        }
        void* pixels = m_resourceManager->GetImageData();
		if(!pixels)
		{
#ifdef VB_CORE_DEBUG
			std::cout << "Texture pointer is null for [" << texname << "].\r\n";
#endif
			return false;
		}
        
        ivec2 size = description.Size;
        glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, type, pixels);
        m_resourceManager->UnloadImage();
        
        return true;
    }
    
    
    
   
    

GLuint RenderingEngine::BuildShader(const char* source, GLenum shaderType) const
{
    GLuint shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &source, 0);
    glCompileShader(shaderHandle);
    
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
#ifdef VB_CORE_DEBUG
        std::cout << messages;
#endif
        
        exit(1);
    }
    
    return shaderHandle;
}

GLuint RenderingEngine::BuildProgram(const char* vertexShaderSource,
                                      const char* fragmentShaderSource) const
{
    GLuint vertexShader = BuildShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = BuildShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
#ifdef VB_CORE_DEBUG
        std::cout << messages;
#endif
        exit(1);
    }
    
    return programHandle;
}
   
    void    RenderingEngine::ChekGLError()
    {
#ifdef VB_CORE_DEBUG
        GLenum error = glGetError();
        switch (error)
        {
            case GL_INVALID_ENUM:
                std::cout << "[VBMAP/GL_ERR]GL error on LoadTextures - GL_INVALID_ENUM\r\n";
                break;
            case GL_INVALID_VALUE:
                std::cout << "[VBMAP/GL_ERR]GL error on LoadTextures - GL_INVALID_VALUE\r\n";
                break;
            case GL_INVALID_OPERATION:
                std::cout << "[VBMAP/GL_ERR]GL error on LoadTextures - GL_INVALID_OPERATION\r\n";
                break;
            case GL_OUT_OF_MEMORY:
                std::cout << "[VBMAP/GL_ERR]GL error on LoadTextures - GL_OUT_OF_MEMORY\r\n";
                break;
            default:
                break;
        }
#endif
    }

    
    /////////[PICKING]/////////////////////////////
    void RenderingEngine::pkRenderWorld()
    {
        glClearColor(1.f,1.f,1.f,1.f);      //ColorCoder로는 나올 수 없는 색상
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
        /*
        // Set the viewport transform.
        //ivec2 size = m_pViewInfo->GetScreenSize();
        ivec2 size = GetFBOSize();
        glViewport(0,0,size.x,size.y);
        
        // Set the projection transform.
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(m_pViewInfo->GetProjectionMatrixPointer());
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glRotatef(m_pViewInfo->GetDeviceAngle(), 0.f, 0.f, 1.f);
        
        // Set the model-view transform.
        glMultMatrixf(m_pViewInfo->GetViewMatrixPointer());
        
        // World fixed light position - picking에서는 조명을 사용하지 않음.
        //glLightfv(GL_LIGHT0, GL_POSITION, m_pConf->flightPosition.Pointer());
        
        /////////////////  Draw data - 향후 랜더링 종류별로 통합되어야 함.
        //Building
        
        if(m_render_mode==RENDERMODE_DEFAULT)
        {
            if(m_pConf->bUseVBO)    pkDrawMeshesWithVBO();
            else                    pkDrawMeshesSeperatedVertexArray();
        }
         */
        
    }
    
    void RenderingEngine::pkColorBasedRender()
    {
        
        //조명없이 메쉬만 단색으로 그린다.
        /*
        glEnableClientState(GL_VERTEX_ARRAY);
        
        
        //랜더링
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_framebuffers.Small);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_renderbuffers.SmallColor);
        pkRenderWorld();
        
        glDisableClientState(GL_VERTEX_ARRAY);
        
        glFinish();  //GL 명령 완전히 실행 한 후 리턴되어야 한다면 glFinish를 쓴다.
         */
        
        
    }
    
    void RenderingEngine::DrawGroundGrid() const
    {
        
    }
    
    void RenderingEngine::DrawMyPosDisc() const
    {
        
    }
    
    void RenderingEngine::pkDrawMeshesSeperatedVertexArray()
    {
        /*
         assert(m_pMeshManager);
        
        vbMesh* pMesh = NULL;
        vbMaterial* pMtl = NULL;
        vector<vbMesh*>* meshes = NULL;
        unsigned char coded_rgb[3] = { 0, 0, 0};
       for(unsigned char i=0;i<NUM_MESH_VECTORS;i++)
        {
            meshes = m_pMeshManager->GetMeshes(i);
            
            if(meshes->size()==0)   continue;
            
            glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_BLEND);
            
            for (unsigned char mesh_index=0; mesh_index< meshes->size(); mesh_index++)
            {
                
                pMesh = (vbMesh*)meshes->at(mesh_index);
                
                glVertexPointer(3, GL_FLOAT,0, pMesh->GetVertexBuffer());  //Alway vertex buffer is exist.
                
                //유효하지 않은 재질인 경우
                pMtl = m_pMtlManager->GetMaterial(pMesh->GetMaterialID());
                if(!pMtl)   continue;
                
                if(pMtl->GetFacing()==1 && glIsEnabled(GL_CULL_FACE))//Both
                    glDisable(GL_CULL_FACE);
                else if(pMtl->GetFacing()!=1 && !glIsEnabled(GL_CULL_FACE))//Single
                    glEnable(GL_CULL_FACE);
                
                m_ColorCoder.GetColorCode_of_NthMesh(mesh_index,(vbMeshManager::VBMESH_TYPE)i,coded_rgb);
                glColor4ub(coded_rgb[0], coded_rgb[1], coded_rgb[1], 255);
                
                
                try
                {
                    
                    if(pMesh->IsByteIndexBuffer())
                    {
                        if( pMesh->GetByteIndexBuffer() == NULL )
                            throw "glDrawElements Error!";
                        glDrawElements(GL_TRIANGLES, pMesh->GetFaceCount()*3, GL_UNSIGNED_BYTE, pMesh->GetByteIndexBuffer());
                    }
                    else
                    {
                        if( pMesh->GetIndexBuffer() == NULL )
                            throw "glDrawElements Error!";
                        glDrawElements(GL_TRIANGLES, pMesh->GetFaceCount()*3, GL_UNSIGNED_SHORT, pMesh->GetIndexBuffer());
                    }
                }
                catch ( char *s)
                {
                    std::cout << "vbMeshRenderer.ES1 LINE: " << __LINE__ << "Mesh Idx:" << i << " mesh iter: "<< mesh_index << " Error Msg:" << s << "\r\n";
                }
                
                
            }
            
        }
        */
    }
    
    void RenderingEngine::pkDrawMeshesWithVBO()
    {
        /*
         //Mesh manager가 없으면 안됨.
        assert(m_pMeshManager);
        
        vbMesh* pMesh = NULL;
        vbMaterial* pMtl = NULL;
        vector<vbMesh*>* meshes = NULL;
        vector<vbMesh*>::const_iterator itr_mesh;
        
        unsigned char coded_rgb[3] = { 0, 0, 0};
        

        //조명/텍스쳐 없는 랜더링
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        
        
        for(unsigned char i=0;i<NUM_MESH_VECTORS;i++)
        {
            meshes = m_pMeshManager->GetMeshes(i);
            
            //Rendering
            //Mesh
            for (unsigned short mesh_index=0; mesh_index < meshes->size(); mesh_index++)
            {
                
                pMesh = (vbMesh*)meshes->at(mesh_index);
                
                //유효하지 않은 재질인 경우
                pMtl = m_pMtlManager->GetMaterial(pMesh->GetMaterialID());
                if(!pMtl)   continue;
                
                if(pMtl->GetFacing()==1 && glIsEnabled(GL_CULL_FACE))//Both
                    glDisable(GL_CULL_FACE);
                else if(pMtl->GetFacing()!=1 && !glIsEnabled(GL_CULL_FACE))//Single
                    glEnable(GL_CULL_FACE);
                
                
                m_ColorCoder.GetColorCode_of_NthMesh(mesh_index,(vbMeshManager::VBMESH_TYPE)i,coded_rgb);
                glColor4ub(coded_rgb[0], coded_rgb[1], coded_rgb[1], 255);
                
                
                glBindBuffer(GL_ARRAY_BUFFER, pMesh->GetVBOVertexBufferID());
                glVertexPointer(3, GL_FLOAT, sizeof(vec3), 0);
                
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->GetVBOIndexBufferID());
                
                if (pMesh->IsByteIndexBuffer())
                {
                    glDrawElements(GL_TRIANGLES, pMesh->GetByteIndexBufferSizeInByte()/sizeof(unsigned char), GL_UNSIGNED_BYTE, 0); //int array for face index
                }
                else
                {
                    glDrawElements(GL_TRIANGLES, pMesh->GetIndexBufferSizeInByte()/sizeof(unsigned short), GL_UNSIGNED_SHORT, 0); //int array for face index
                }
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                
            }
        }
         */
        
    }
    
    
    
    
    
    
}//namespace ES2
