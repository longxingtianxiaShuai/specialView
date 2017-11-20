#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include "RenderingBase.h"
#include "vbMeshInstance.h"

#include <assert.h>
namespace ES1
{
    class RenderingEngine : public RenderingBase
    {
    public:        
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
        

        RenderingEngine(IResourceManager* resourceManager);
        void Render() const;
        void pkColorBasedRender();
  
        
 
        
    private:
        Renderbuffers       m_renderbuffers;
        Framebuffers        m_framebuffers;
        GLuint              m_OffScreenTexture;
        
     
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
   //     void    DrawStaticMeshVBO(const vbMesh* pMesh) const;

        
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
        void    InitFrameBuffer();     // Color & Depth buffer
        ivec2   GetFBOSize() const;
        void    DeleteBaseFBOAndVBO();

        
        //Initialize
        void    Initialize();
        void    InitGLStates();        //Enable/Disable/Material/Light
        void    ClearData();


        //Texture
        void    LoadTextures();
        bool    SetTexture(const string& name);
        
        void    ChekGLError();
        
        //Picking
        bool GetColorcodedMeshIDAt(int tx, int ty, unsigned short* pMesh_ID);

        
        
     };
    
    IRenderingEngine* CreateRenderingEngine(IResourceManager* resourceManager)
    {
        return new RenderingEngine(resourceManager);
    }

    
    
    RenderingEngine::RenderingEngine(IResourceManager* resourceManager)
    {
        InitCommonState(resourceManager);
                
        //OnScreen buffer
        glGenRenderbuffersOES(1, &m_renderbuffers.SmallColor);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_renderbuffers.SmallColor);
    }
 
    
    //////////////////// [START]Initialize & Clear
    void RenderingEngine::Initialize()
    {
        //Init Color & Depth buffer
        InitFrameBuffer();

        //Init Offscreen VBO
        InitOffscreenVBO();
        
        //Init GL State, Material and Light
        InitGLStates();
    }
    
    void RenderingEngine::InitFrameBuffer()     // Color & Depth buffer
    {
        //Antialiasing을 위해 RTT super sampling을 쓰는데, 쓰지 않더라도 버퍼는 만들어 둔다.
        
        //////////////////////////////// Small Framebuffer /////////////////////////////////
        ivec2   size = GetFBOSize();
        
        // Create a depth buffer that has the same size as the color buffer.
        glGenRenderbuffersOES(1, &m_renderbuffers.SmallDepth);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_renderbuffers.SmallDepth);
        glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES,
                                 size.x, size.y);
        
        // Create the OnScreen framebuffer object.
        glGenFramebuffersOES(1, &m_framebuffers.Small);
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_framebuffers.Small);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES,
                                     GL_RENDERBUFFER_OES, m_renderbuffers.SmallColor);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES,
                                     GL_RENDERBUFFER_OES, m_renderbuffers.SmallDepth);
        
        ////////Small Framebuffer /////////////////////////////////
        
        
        ////////Big Framebuffer /////////////////////////////////
        
        //Color buffer
        glGenRenderbuffersOES(1, &m_renderbuffers.BigColor);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_renderbuffers.BigColor);
        glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_RGBA8_OES, size.x*2, size.y*2);
        //Depth
        glGenRenderbuffersOES(1, &m_renderbuffers.BigDepth);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_renderbuffers.BigDepth);
        glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES,
                                 size.x*2, size.y*2);
        //Create OffScreen framebuffer
        glGenFramebuffersOES(1, &m_framebuffers.Big);
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_framebuffers.Big);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES,
                                     GL_RENDERBUFFER_OES, m_renderbuffers.BigColor);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES,
                                     GL_RENDERBUFFER_OES, m_renderbuffers.BigDepth);
        
        //Offscreen texture
        glGenTextures(1, &m_OffScreenTexture);
#ifdef VB_CORE_DEBUG
        printf("The Offscreen texture ID is assigned as [%d].\r\n",m_OffScreenTexture);
#endif
        glBindTexture(GL_TEXTURE_2D, m_OffScreenTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x*2, size.y*2, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, m_OffScreenTexture, 0);
        
        //Check FBO
        GLenum  status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
        if(status != GL_FRAMEBUFFER_COMPLETE_OES)
        {
#ifdef VB_CORE_DEBUG
            cout << "Incomplete OffScreen FBO. The returend status is " << (unsigned int)status << endl;
#endif
        }
        //////////////////////////////// Big Framebuffer /////////////////////////////////
        
    }
    
    void RenderingEngine::InitGLStates()
    {
        // Set up various GL state.
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);    //반투명 랜더링 재질에 따라 On/Off되어야 하므로 모델 렌더링 부분으로 옮겨져야 함.
        glDisable(GL_TEXTURE_2D);    //Material에 따라 On/Off 되어야 하므로 모델 랜더링 부분으로 옮겨져야 함.
        glDisable(GL_BLEND);
        
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        
        glEnable(GL_COLOR_MATERIAL);
        glColor4f(1.f, 1.f, 1.f, 1.f);
        
        glEnable(GL_POLYGON_OFFSET_FILL);
        
        // Set up the material properties.
        vec4 specular(0.5f, 0.5f, 0.5f, 1);
       // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular.Pointer());
       // glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0f);
        
        glLightfv(GL_LIGHT0, GL_AMBIENT, m_pConf->fLightAmbient.Pointer());

    }
    
    //Offscreen VBO
    void RenderingEngine::InitOffscreenVBO()
    {
        //Buffer
        float offscreen_buffer[20];    //4 * 5(pos-3 + tex-2)
        offscreen_buffer[0]=-1.f;  offscreen_buffer[1]=-1.f;  offscreen_buffer[2]=0.f;
        offscreen_buffer[3]= 1.f;  offscreen_buffer[4]=-1.f;  offscreen_buffer[5]=0.f;
        offscreen_buffer[6]= 1.f;  offscreen_buffer[7]= 1.f;  offscreen_buffer[8]=0.f;
        offscreen_buffer[9]=-1.f;  offscreen_buffer[10]= 1.f; offscreen_buffer[11]=0.f;
        
        offscreen_buffer[12]=0.f;   offscreen_buffer[13]=0.f;
        offscreen_buffer[14]=1.f;   offscreen_buffer[15]=0.f;
        offscreen_buffer[16]=1.f;   offscreen_buffer[17]=1.f;
        offscreen_buffer[18]=0.f;   offscreen_buffer[19]=1.f;
        
        unsigned char offscreen_idx_buffer[6];
        offscreen_idx_buffer[0]=0; offscreen_idx_buffer[1]=1; offscreen_idx_buffer[2]=2;
        offscreen_idx_buffer[3]=0; offscreen_idx_buffer[4]=2; offscreen_idx_buffer[5]=3;

        
        glGenBuffers(1,&m_offscreenRectVBOArrayID);
#ifdef VB_CORE_DEBUG
        printf("The Offscreen rect VBO ID is assigned as [%d].\r\n",m_offscreenRectVBOArrayID);
#endif
        glBindBuffer(GL_ARRAY_BUFFER, m_offscreenRectVBOArrayID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*20, offscreen_buffer, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1,&m_offscreenRectVBOIndexID);
#ifdef VB_CORE_DEBUG
        printf("The Offscreen rect index VBO ID is assigned as [%d].\r\n",m_offscreenRectVBOIndexID);
#endif
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_offscreenRectVBOIndexID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned char)*6, offscreen_idx_buffer, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
                if(pMesh->GetVBOIndexBufferID()>0 && pMesh->GetVBOVertexBufferID()>0)   continue;
                
                // Create VBO Buffer
                GLuint VBOIDs[2];
                glGenBuffers(2, VBOIDs);
#ifdef VB_CORE_DEBUG
          //      printf("VBO IDs for the mesh(ID=%d) are [%d] and [%d].\r\n", pMesh->GetID(),VBOIDs[0],VBOIDs[1]);
#endif
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
#ifdef VB_CORE_DEBUG
    //        printf("The edge VBO IDs for the mesh(ID=%d) is [%d].\r\n", pMesh->GetID(),edge_vbo_id);
#endif
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
    //////////////////// [END]Initialize & Clear
    
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
            
            if(pMtl->IsGLReferencing())     continue;//다른 텍스쳐 ID를 참조해서 사용하는 경우.
            
            GLtexID = (GLuint)pMtl->GetGLTexID();
            
            if(glIsTexture(GLtexID)) glDeleteTextures(1,&GLtexID);
#ifdef VB_CORE_DEBUG
            else
                printf("A texture(ID=%d) isn't deleted. Please check it..\r\n",GLtexID);
            printf("The texture ID to be deleted is [%d].\r\n",GLtexID);
#endif
        }
        
        DeleteAllVBO();
        
        
        
        
    }
 
    
    void RenderingEngine::DeleteBaseFBOAndVBO()
    {
        //Offscreen texture
        glBindTexture(GL_TEXTURE_2D, 0);
        if(glIsTexture(m_OffScreenTexture)) glDeleteTextures(1, &m_OffScreenTexture);
#ifdef VB_CORE_DEBUG
        else
            printf("The offscreen texture(ID=%d) isn't deleted. Please check it..\r\n",m_OffScreenTexture);
        printf("The offscreen texture ID to be deleted is [%d].\r\n",m_OffScreenTexture);
#endif
        //Render buffer
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_framebuffers.Big);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, 0);
        glDeleteRenderbuffersOES(1, &m_renderbuffers.BigColor);
        glDeleteRenderbuffersOES(1, &m_renderbuffers.BigDepth);
        
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_framebuffers.Small);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, 0);
        glDeleteRenderbuffersOES(1, &m_renderbuffers.SmallColor);
        glDeleteRenderbuffersOES(1, &m_renderbuffers.SmallDepth);
        
        //Framebuffer
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
        glDeleteFramebuffersOES(1, &m_framebuffers.Big);
        glDeleteFramebuffersOES(1, &m_framebuffers.Small);
        
        //VBO for offscreen
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        if (glIsBuffer(m_offscreenRectVBOArrayID)) glDeleteBuffers(1, &m_offscreenRectVBOArrayID);
#ifdef VB_CORE_DEBUG
        else
            printf("The offscreen rect VBO(ID=%d) isn't deleted. Please check it..\r\n",m_offscreenRectVBOArrayID);
#endif
        
        if (glIsBuffer(m_offscreenRectVBOIndexID)) glDeleteBuffers(1, &m_offscreenRectVBOIndexID);
#ifdef VB_CORE_DEBUG
        else
            printf("The offscreen rect VBO Index(ID=%d) isn't deleted. Please check it..\r\n",m_offscreenRectVBOIndexID);
#endif
        
        glFinish();//Ensure -_-
    }
    
    
    void RenderingEngine::DeleteAllVBO()
    {
        //Check Mesh Manger
        if(m_pMeshManager==NULL)
        {
#ifdef VB_CORE_DEBUG
            cout << "[ES1]No Mesh Manager assigned.\n";
#endif
            return;
        }
        
        vbMesh* pMesh = NULL;
        vector<vbMesh*>* meshes = NULL;
        vector<vbMesh*>::const_iterator itr_mesh;
        
        for(unsigned char i=0;i<NUM_MESH_VECTORS;i++)
        {
            meshes = m_pMeshManager->GetMeshes(i);
            if (meshes->size()==0)  continue;
            
            for (itr_mesh = meshes->begin(); itr_mesh != meshes->end(); ++itr_mesh)
            {
                
                pMesh = (vbMesh*)(*itr_mesh);
                
                //Vertex Buffer (Position+Normal+Texture)
                const unsigned int vt_buffer_id = pMesh->GetVBOVertexBufferID();
                if (glIsBuffer(vt_buffer_id))   glDeleteBuffers(1, &vt_buffer_id);
#ifdef VB_CORE_DEBUG
                else
                    printf("A mesh(ID=%d) VBO(ID=%d) isn't deleted. Please check it..\r\n",pMesh->GetID(),vt_buffer_id);
#endif
                //Index Buffer
                const unsigned int idx_buffer_id = pMesh->GetVBOIndexBufferID();
                if (glIsBuffer(idx_buffer_id))  glDeleteBuffers(1, &idx_buffer_id);
#ifdef VB_CORE_DEBUG
                else
                    printf("A mesh(ID=%d) Index VBO(ID=%d) isn't deleted. Please check it..\r\n",pMesh->GetID(),idx_buffer_id);
#endif
                pMesh->SetVBOVertexBufferID(0);
                pMesh->SetVBOIndexBufferID(0);
            }
        }
        
        //Delete Edge VBO
        unsigned int edge_vbo_id = m_pMeshManager->GetEdges()->GetVBOID();
        if (glIsBuffer(edge_vbo_id))
        {
            glDeleteBuffers(1, &edge_vbo_id);
            m_pMeshManager->GetEdges()->SetVBOID(0);
        }
#ifdef VB_CORE_DEBUG
        else if(m_pMeshManager->GetEdges()->GetEdgeCount()>0)
            printf("Edge count is %d, and the edge vbo ID is %d. But it wasn't deleted. Please check it..\r\n",m_pMeshManager->GetEdges()->GetEdgeCount(),edge_vbo_id);
#endif
    }
    /////////////////////// [END]Clear & Release
    
    
    ///////////////////////  [START]Set/Get
    ivec2 RenderingEngine::GetFBOSize()    const
    {
        ivec2   size;
        glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &size.x);
        glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &size.y);
        return size;
    }
    
    ///////////////////////  [END]Set/Get
    



    ///////////////////////  [START]Rendering
    void RenderingEngine::RenderWorld() const
    {
        glClearColor(m_pConf->fBgColor[0], m_pConf->fBgColor[1], m_pConf->fBgColor[2], m_pConf->fBgColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
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
                
        // World fixed light position
      //  glLightfv(GL_LIGHT0, GL_POSITION, m_pConf->flightPosition.Pointer());
        
        /////////////////  Draw data - 향후 랜더링 종류별로 통합되어야 함.
        //Building
        glPushMatrix();
        glScalef(1.f, m_fVerticalScale, 1.f);
        
        glTranslatef(0.f, -m_fGroundLevel, 0.f);
        
        if(m_render_mode==RENDERMODE_DEFAULT || m_render_mode==RENDERMODE_TEXTURE_LIGHT)
        {
            if(m_pConf->bUseVBO)    DrawMeshesWithVBO();
            else                    DrawMeshesSeperatedVertexArray();
        }
        else if(m_render_mode==RENDERMODE_SOLID_COLOR)
        {
            if(m_pConf->bUseVBO)    DrawMeshesWithVBOSolid();
            else                    DrawMeshesSeperatedVertexArraySolid();
        }
        else if(m_render_mode==RENDERMODE_NO_TEX_LIGHT)//Own color
        {

            if(m_pConf->bUseVBO)    DrawMeshesWithVBOColor();
            else                    DrawMeshesSeperatedVertexArrayColor();
        }
        
        
        DrawEdges();

        //Path
        DrawPathRect();
        
        //MyPosition
        DrawMyPosDisc();
        
        glPopMatrix();
        
#ifdef VB_CORE_DEBUG
//        DrawGroundGrid();
#endif
    }
    
    
    void RenderingEngine::RenderOnScreenRectVBO() const
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        ivec2 on_size = GetFBOSize();
        glViewport(0,0,on_size.x,on_size.y);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrthof(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        
        glColor4f(1.f, 1.f, 1.f, 1.f);
        glBindTexture(GL_TEXTURE_2D, m_OffScreenTexture);
 
        glBindBuffer(GL_ARRAY_BUFFER, m_offscreenRectVBOArrayID);
        glVertexPointer(3, GL_FLOAT, sizeof(float)*3, 0);
        glTexCoordPointer(2, GL_FLOAT, sizeof(float)*2, (const GLvoid *)(sizeof(float)*12));
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_offscreenRectVBOIndexID);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        glEnable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
    }
    
    void RenderingEngine::RenderOnScreenRect() const
    {
        /*
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ivec2 on_size = GetFBOSize();
        glViewport(0,0,on_size.x,on_size.y);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrthof(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        
        glColor4f(1.f, 1.f, 1.f, 1.f);
        glBindTexture(GL_TEXTURE_2D, m_OffScreenTexture);
        glVertexPointer(3, GL_FLOAT,0, offscreen_vertex_buffer);  //Alway vertex buffer is exist.
        glTexCoordPointer(2, GL_FLOAT, 0, offscreen_texture_buffer);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, offscreen_index_buffer);
        
        glEnable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        
         */
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
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        

        if (bUseSuperSampling)
        {
            //OffScreen Rendering
            glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_framebuffers.Big);
            glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_renderbuffers.BigColor);
            RenderWorld();
            
            //OnScreen Rendering
            glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_framebuffers.Small);
            glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_renderbuffers.SmallColor);
            //RenderOnScreenRect();
            RenderOnScreenRectVBO();
        }
        else
        {
            glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_framebuffers.Small);
            glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_renderbuffers.SmallColor);
            RenderWorld();
        }
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glFinish();  //GL 명령 완전히 실행 한 후 리턴되어야 한다면 glFinish를 쓴다.
        
        
    }
    
    
    void  RenderingEngine::DrawPathRect() const
    {
        if (m_pPathManager->IsPathVisible()==false) return;
        
        
        //남은 경로
        const float* pColorForward = m_pPathManager->GetForwardPathColor();
        const float* pPathColor = 0;
        
        vbPath* pPath = 0;//m_pPathManager->GetVisiblePath();

        glDisable(GL_LIGHTING);
        for (int i=0; i<m_pPathManager->GetPathCount(); i++)
        {
            pPath = m_pPathManager->GetNthPath(i);
            if(pPath==NULL) continue;
            
            if(pPath->IsVisible()==false) continue;
        
            pPathColor = pPath->GetColorRGBA();
            if(pPathColor[0]<0.f)   glColor4f(pColorForward[0],pColorForward[1],pColorForward[2],pColorForward[3]);
            else                    glColor4f(pPathColor[0],pPathColor[1],pPathColor[2],pPathColor[3]);
            //glPushMatrix();
            
#ifdef VB_CORE_DEBUG
           // printf("Path ID = %d, Name = %s, Offset = %f\r\n",pPath->GetPathID(),pPath->GetGroupName().c_str(), pPath->GetGroundOffset());
#endif
           // glTranslatef(0.f, pPath->GetGroundOffset(), 0.f);
            //glTranslatef(0.f, pPath->GetGroundOffset(), 0.f);
            glVertexPointer(3, GL_FLOAT,0, pPath->GetPathRoadVts());
            
            switch(pPath->GetRenderMode())
            {
                case 0://Solid
                    glDrawArrays(GL_TRIANGLE_STRIP, 0, pPath->GetVtCount()+4);
                    break;
                case 1://Triangle
                    glDrawArrays(GL_TRIANGLES, 0, pPath->GetVtCount());
                    //glDrawArrays(GL_TRIANGLES, 0, 6);
                    break;
            }

            //glPopMatrix();
        
        }
        glEnable(GL_LIGHTING);
        
        /*
         
        if(m_pPathManager->IsMyPosVisible())
        {
            glPointSize(20);
            glDisable(GL_DEPTH_TEST);
            
            glColor4f(1.f,0.f,0.f,1.f);
            glVertexPointer(3, GL_FLOAT,0, m_pPathManager->GetMyInputPos().Pointer());
            glDrawArrays(GL_POINTS, 0, 1);
            
            glColor4f(1.f,1.f,0.f,1.f);
            glVertexPointer(3, GL_FLOAT,0, m_pPathManager->GetMyCorrectedPos().Pointer());
            glDrawArrays(GL_POINTS, 0, 1);
            
            float orbit_center[18];
            float h = 500.f;
            vec3 cntr = m_pPathManager->GetCameraCenter();
            
            orbit_center[0] = cntr.x-h;   orbit_center[1] = cntr.y;   orbit_center[2] = cntr.z;
            orbit_center[3] = cntr.x+h;   orbit_center[4] = cntr.y;   orbit_center[5] = cntr.z;
            
            orbit_center[6] = cntr.x;   orbit_center[7] = cntr.y-h;   orbit_center[8] = cntr.z;
            orbit_center[9] = cntr.x;   orbit_center[10] = cntr.y+h;   orbit_center[11] = cntr.z;
            
            orbit_center[12] = cntr.x;   orbit_center[13] = cntr.y;   orbit_center[14] = cntr.z-h;
            orbit_center[15] = cntr.x;   orbit_center[16] = cntr.y;   orbit_center[17] = cntr.z+h;
            
            glColor4f(0.f, 0.f, 0.f, 1.f);
            glLineWidth(2);
            glVertexPointer(3, GL_FLOAT,0, orbit_center);
            glDrawArrays(GL_LINES, 0, 6);
            glLineWidth(1);
            glEnable(GL_DEPTH_TEST);
            glPointSize(1);
            
            
            
        }
        
        
        
        vbPath* pPath = m_pPathManager->GetVisiblePath();
        if(!pPath)   return;
        
        
        //그림자
        const float* pColorShadow = m_pPathManager->GetPathShadowColor();
        glColor4f(pColorShadow[0],pColorShadow[1],pColorShadow[2],pColorShadow[3]);
        
        glEnable(GL_BLEND);         //blend
        glDisable(GL_LIGHTING);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glPushMatrix();
        glTranslatef(pPath->GetGroundOffset()*0.5f, pPath->GetGroundOffset()*0.8f, pPath->GetGroundOffset()*0.5f);
        glVertexPointer(3, GL_FLOAT,0, pPath->GetPathRoadVts());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, pPath->GetVtCount()+4);
        glPopMatrix();
        
        glDisable(GL_BLEND);         //blend
        
        
        
        unsigned short path_progress = (unsigned short)(pPath->GetProgress());
        
        //지나간 경로
        const float* pColorPassed = m_pPathManager->GetPassedPathColor();
        glColor4f(pColorPassed[0],pColorPassed[1],pColorPassed[2],pColorPassed[3]);
        glPushMatrix();
        glTranslatef(0.f, pPath->GetGroundOffset()*0.95f, 0.f);
        glVertexPointer(3, GL_FLOAT,0, pPath->GetPathRoadVts());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, path_progress * 4);
        glPopMatrix();
        
        
        
        //남은 경로
        const float* pColorForward = m_pPathManager->GetForwardPathColor();
        const float* pPathColor = pPath->GetColorRGBA();
        if(pPathColor[0]<0.f)   glColor4f(pColorForward[0],pColorForward[1],pColorForward[2],pColorForward[3]);
        else                    glColor4f(pPathColor[0],pPathColor[1],pPathColor[2],pPathColor[3]);
        glPushMatrix();
        glTranslatef(0.f, pPath->GetGroundOffset(), 0.f);
        glDrawArrays(GL_TRIANGLE_STRIP, path_progress * 4, pPath->GetVtCount()+4-path_progress * 4);
        glPopMatrix();
        
        
        glEnable(GL_LIGHTING);
         */
    }
    
    
    void    RenderingEngine::DrawMeshesSeperatedVertexArraySolid() const
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
        vbMaterial* pMtl = NULL;
        float* pDiffuse = NULL;

        if(glIsEnabled(GL_TEXTURE_2D)==GL_TRUE)  glDisable(GL_TEXTURE_2D);
        if(glIsEnabled(GL_LIGHTING)==GL_FALSE)   glEnable(GL_LIGHTING);
        
        float* SolidColor = m_pConf->fSolidColor;
        glColor4f(SolidColor[0],SolidColor[1],SolidColor[2],SolidColor[3]);
        
        for(unsigned char i=0;i<NUM_MESH_VECTORS;i++)
        {
            meshes = m_pMeshManager->GetMeshes(i);
            
            if(meshes->size()==0)   continue;
            
            if(i==vbMaterial::VBMESH_BLEND)
            {
                glEnable(GL_BLEND);         //blend
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            
            
            int k=0;
            for (itr_mesh = meshes->begin(); itr_mesh != meshes->end(); ++itr_mesh)
            {
                pMesh = (vbMesh*)(*itr_mesh);
                if(pMesh->IsVisible()==false)   continue;
                
                
                glVertexPointer(3, GL_FLOAT,0, pMesh->GetVertexBuffer());  //Alway vertex buffer is exist.
                
                //Normal
                glNormalPointer(GL_FLOAT, 0, pMesh->GetNormalBuffer());
                
                
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
                    glColor4f(SolidColor[0], SolidColor[1], SolidColor[2], pDiffuse[3]);
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
                    std::cout << "vbMeshRenderer.ES1 LINE: " << __LINE__ << "Mesh Idx:" << i << " mesh iter: "<< k << " Error Msg:" << s << "\r\n";
#endif
                }
                
                k ++;
                
            }
            
            if(i==1)   glDisable(GL_BLEND);         //blend
   
        }
    }
    
   
    
     //각 속성이 별도의 배열로 분리 된 경우 - stride가 0이다.
    void RenderingEngine::DrawMeshesSeperatedVertexArray() const
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
        vbMaterial* pMtl = NULL;
        float* pDiffuse = NULL;
        glColor4f(1.f, 1.f, 1.f, 1.f);
        for(unsigned char i=0;i<NUM_MESH_VECTORS;i++)
        {
            meshes = m_pMeshManager->GetMeshes(i);
            
            if(meshes->size()==0)   continue;
           
            //전처리
            if(i==vbMaterial::VBMESH_TEXTURE)
            {
                glEnable(GL_TEXTURE_2D);    //textured
                //VR인 경우에는 조명 끄기
                if(m_pViewInfo->GetNavigationMode()==vbViewInfo::NAVIGATION_ENVRN)
                    glDisable(GL_LIGHTING);
                else
                    glEnable(GL_LIGHTING);
            }
            else if(i==vbMaterial::VBMESH_COLOR)
            {
                if(glIsEnabled(GL_LIGHTING)==GL_FALSE)  glEnable(GL_LIGHTING);
            }
            else if(i==vbMaterial::VBMESH_BLEND)
            {
                glEnable(GL_BLEND);         //blend
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            else if(i==vbMaterial::VBMESH_BLENDTEXTURE)
            {
                glEnable(GL_TEXTURE_2D);    //textured
                glEnable(GL_BLEND);         //blend
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }

            
            int k=0;
            for (itr_mesh = meshes->begin(); itr_mesh != meshes->end(); ++itr_mesh)
            {
                
                
                pMesh = (vbMesh*)(*itr_mesh);
                if(pMesh->IsVisible()==false)   continue;

                glVertexPointer(3, GL_FLOAT,0, pMesh->GetVertexBuffer());  //Alway vertex buffer is exist.
                
                //Normal
                glNormalPointer(GL_FLOAT, 0, pMesh->GetNormalBuffer());
                
                
                //유효하지 않은 재질인 경우
                pMtl = m_pMtlManager->GetMaterial(pMesh->GetMaterialID());
                if(!pMtl)   continue;
                
                if(pMtl->GetFacing()==1 && glIsEnabled(GL_CULL_FACE))//Both
                    glDisable(GL_CULL_FACE);
                else if(pMtl->GetFacing()!=1 && !glIsEnabled(GL_CULL_FACE))//Single
                    glEnable(GL_CULL_FACE);
                
                
                //Mesh별 material 설정
                if(i==vbMaterial::VBMESH_TEXTURE || i==vbMaterial::VBMESH_BLENDTEXTURE) //Texture
                {
                    glBindTexture(GL_TEXTURE_2D, pMtl->GetGLTexID());
                    glTexCoordPointer(2, GL_FLOAT, 0, pMesh->GetTexCoordBuffer());
                }
                else// if(i==1) //Blend
                {
                    pDiffuse = pMtl->GetDiffuse();
                    glColor4f(pDiffuse[0], pDiffuse[1], pDiffuse[2], pDiffuse[3]);
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
                    std::cout << "vbMeshRenderer.ES1 LINE: " << __LINE__ << "Mesh Idx:" << i << " mesh iter: "<< k << " Error Msg:" << s << "\r\n";
#endif
                }
                
                k ++;
                
            }
            if(i==vbMaterial::VBMESH_TEXTURE)
            {
                
                glDisable(GL_TEXTURE_2D);    //textured
                //VR인 경우에는 조명 끄기
                if(m_pViewInfo->GetNavigationMode()==vbViewInfo::NAVIGATION_ENVRN)
                    glEnable(GL_LIGHTING);
                else
                    glDisable(GL_LIGHTING);
            }
            else if(i==vbMaterial::VBMESH_BLEND)   glDisable(GL_BLEND);         //blend
            else if(i==vbMaterial::VBMESH_BLENDTEXTURE)
            {
                glDisable(GL_TEXTURE_2D);    //textured
                glDisable(GL_BLEND);
            }
            
            
            
        }
    }
 
    
    void RenderingEngine::DrawMeshesWithVBOColor() const
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
        
        vbMaterial* pMtl = NULL;
        float* pDiffuse = NULL;
        
        if(glIsEnabled(GL_TEXTURE_2D)==GL_TRUE)  glDisable(GL_TEXTURE_2D);
        if(glIsEnabled(GL_LIGHTING)==GL_FALSE)   glEnable(GL_LIGHTING);
        
        for(unsigned char i=0;i<NUM_MESH_VECTORS;i++)
        {
            meshes = m_pMeshManager->GetMeshes(i);
            
            if(meshes->size()==0)   continue;
            
            if(i==vbMaterial::VBMESH_BLEND)
            {
                glEnable(GL_BLEND);         //blend
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            
            
            
            //Rendering
            //Mesh
            for (itr_mesh = meshes->begin(); itr_mesh != meshes->end(); ++itr_mesh)
            {
                
                pMesh = (vbMesh*)(*itr_mesh);
                if(pMesh->IsVisible()==false)   continue;

                pMtl = m_pMtlManager->GetMaterial(pMesh->GetMaterialID());
                
                glBindBuffer(GL_ARRAY_BUFFER, pMesh->GetVBOVertexBufferID());
                glVertexPointer(3, GL_FLOAT, sizeof(vec3), 0);
                glNormalPointer(GL_FLOAT, sizeof(vec3), (GLvoid*)(pMesh->GetVertexBufferSizeInByte()));
                
                pDiffuse = pMtl->GetDiffuse();
                glColor4f(pDiffuse[0], pDiffuse[1], pDiffuse[2], pDiffuse[3]);
                
                
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->GetVBOIndexBufferID());
                
                if (pMesh->IsByteIndexBuffer())
                    glDrawElements(GL_TRIANGLES, pMesh->GetIndexCount(), GL_UNSIGNED_BYTE, 0);
                else
                    glDrawElements(GL_TRIANGLES, pMesh->GetIndexCount(), GL_UNSIGNED_SHORT, 0);
                
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                
            }
            
            //Post process
            if(i==vbMaterial::VBMESH_BLEND)   glDisable(GL_BLEND);         //blend
            
            
        }
    }
    
    void RenderingEngine::DrawMeshesSeperatedVertexArrayColor() const
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
        vbMaterial* pMtl = NULL;
        float* pDiffuse = NULL;
        
        if(glIsEnabled(GL_TEXTURE_2D)==GL_TRUE)  glDisable(GL_TEXTURE_2D);
        if(glIsEnabled(GL_LIGHTING)==GL_FALSE)   glEnable(GL_LIGHTING);
        
        
        for(unsigned char i=0;i<NUM_MESH_VECTORS;i++)
        {
            meshes = m_pMeshManager->GetMeshes(i);
            
            if(meshes->size()==0)   continue;
            
            glEnable(GL_BLEND);         //blend
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            int k=0;
            for (itr_mesh = meshes->begin(); itr_mesh != meshes->end(); ++itr_mesh)
            {
                
                
                pMesh = (vbMesh*)(*itr_mesh);
                if(pMesh->IsVisible()==false)   continue;

                glVertexPointer(3, GL_FLOAT,0, pMesh->GetVertexBuffer());  //Alway vertex buffer is exist.
                
                //Normal
                glNormalPointer(GL_FLOAT, 0, pMesh->GetNormalBuffer());
                
                
                //유효하지 않은 재질인 경우
                pMtl = m_pMtlManager->GetMaterial(pMesh->GetMaterialID());
                if(!pMtl)   continue;
                
                if(pMtl->GetFacing()==1 && glIsEnabled(GL_CULL_FACE))//Both
                    glDisable(GL_CULL_FACE);
                else if(pMtl->GetFacing()!=1 && !glIsEnabled(GL_CULL_FACE))//Single
                    glEnable(GL_CULL_FACE);
                
                
                pDiffuse = pMtl->GetDiffuse();
                glColor4f(pDiffuse[0], pDiffuse[1], pDiffuse[2], pDiffuse[3]);
                
                
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
                    std::cout << "vbMeshRenderer.ES1 LINE: " << __LINE__ << "Mesh Idx:" << i << " mesh iter: "<< k << " Error Msg:" << s << "\r\n";
#endif
                    
                }
                
                k ++;
                
            }
            
            if(i==1)   glDisable(GL_BLEND);         //blend
            
        }
    }
    
    void RenderingEngine::DrawMeshesWithVBOSolid() const
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
        
        vbMaterial* pMtl = NULL;
        float* pDiffuse = NULL;
        
        if(glIsEnabled(GL_TEXTURE_2D)==GL_TRUE)  glDisable(GL_TEXTURE_2D);
        if(glIsEnabled(GL_LIGHTING)==GL_FALSE)   glEnable(GL_LIGHTING);
        
        float* SolidColor = m_pConf->fSolidColor;
        glColor4f(SolidColor[0],SolidColor[1],SolidColor[2],SolidColor[3]);
       
        
        for(unsigned char i=0;i<NUM_MESH_VECTORS;i++)
        {
            meshes = m_pMeshManager->GetMeshes(i);
            
            if(meshes->size()==0)   continue;
            
            if(i==vbMaterial::VBMESH_BLEND || i==vbMaterial::VBMESH_BLENDTEXTURE)
            {
                glEnable(GL_BLEND);         //blend
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            
            
            
            //Rendering
            //Mesh
            for (itr_mesh = meshes->begin(); itr_mesh != meshes->end(); ++itr_mesh)
            {
                
                pMesh = (vbMesh*)(*itr_mesh);
                if(pMesh->IsVisible()==false)   continue;

                pMtl = m_pMtlManager->GetMaterial(pMesh->GetMaterialID());
                
                glBindBuffer(GL_ARRAY_BUFFER, pMesh->GetVBOVertexBufferID());
                glVertexPointer(3, GL_FLOAT, sizeof(vec3), 0);
                glNormalPointer(GL_FLOAT, sizeof(vec3), (GLvoid*)(pMesh->GetVertexBufferSizeInByte()));

                if(i==vbMaterial::VBMESH_BLEND)
                {
                    pDiffuse = pMtl->GetDiffuse();
                    glColor4f(SolidColor[0], SolidColor[1], SolidColor[2], pDiffuse[3]);
                }
                
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->GetVBOIndexBufferID());
                if (pMesh->IsByteIndexBuffer())
                    glDrawElements(GL_TRIANGLES, pMesh->GetIndexCount(), GL_UNSIGNED_BYTE, 0);
                else
                    glDrawElements(GL_TRIANGLES, pMesh->GetIndexCount(), GL_UNSIGNED_SHORT, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                
            }
            
            //Post process
            if(i==vbMaterial::VBMESH_BLEND || i==vbMaterial::VBMESH_BLENDTEXTURE)   glDisable(GL_BLEND);         //blend
            
            
        }
    }
    
    
    void    RenderingEngine::PreGLState(vbMaterial::VBMESH_TYPE mesh_type) const
    {
        
        if(mesh_type==vbMaterial::VBMESH_TEXTURE)
        {
            glEnable(GL_TEXTURE_2D);    //textured
            //VR인 경우에는 조명 끄기
            if(m_pViewInfo->GetNavigationMode()==vbViewInfo::NAVIGATION_ENVRN)
                glDisable(GL_LIGHTING);
            else
                glEnable(GL_LIGHTING);
        }
        else if(mesh_type==vbMaterial::VBMESH_BLEND)
        {
            glEnable(GL_BLEND);         //blend
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         }
        else if(mesh_type==vbMaterial::VBMESH_BLENDTEXTURE)
        {
            glEnable(GL_TEXTURE_2D);    //textured
            glEnable(GL_BLEND);         //blend
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_ALPHA_TEST);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glAlphaFunc(GL_GREATER, 0.0);
        }
//################### 랜더링이 이상하게 반복되는 현상 수정해야 함. #########################

    }
    
    void    RenderingEngine::PostGLState(vbMaterial::VBMESH_TYPE mesh_type) const
    {
        if(mesh_type==vbMaterial::VBMESH_TEXTURE)
        {
            glDisable(GL_TEXTURE_2D);    //textured
        }
        else if(mesh_type==vbMaterial::VBMESH_BLEND)
        {
            glDisable(GL_BLEND);         //blend
         }
        else if(mesh_type==vbMaterial::VBMESH_BLENDTEXTURE)
        {
            glDisable(GL_BLEND);         //blend
            glDisable(GL_TEXTURE_2D);    //textured
            
            glDisable(GL_ALPHA_TEST);
            
        }
    }
    

    
    void RenderingEngine::DrawMeshesWithVBO() const
    {
        assert(m_pMeshManager);
        
        vbMesh* pMesh           = NULL;
        vector<vbMesh*>* meshes = NULL;
        vector<vbMesh*>::const_iterator itr_mesh;
        
        vbMaterial* pMtl    = NULL;
        float* pDiffuse     = NULL;
        glColor4f(1.f, 1.f, 1.f, 1.f);

        vbMaterial::VBMESH_TYPE mesh_type;
        
       
        //TX(0)-CL(1)-BL(2)-BT(3)
        for(int i=0;i<NUM_MESH_VECTORS;i++)
        {
            //Mesh type
            meshes = m_pMeshManager->GetMeshes(i);
            if(meshes->size()==0)   continue;
            
            mesh_type = (vbMaterial::VBMESH_TYPE)(i);
            
            //Pre process
            PreGLState(mesh_type);
            
            //World-fixed light
           if(glIsEnabled(GL_LIGHTING))    glLightfv(GL_LIGHT0, GL_POSITION, m_pConf->flightPosition.Pointer());

            //Rendering
            //Mesh
            for (itr_mesh = meshes->begin(); itr_mesh != meshes->end(); ++itr_mesh)
            {
                
                pMesh = (vbMesh*)(*itr_mesh);
                if(pMesh->IsVisible()==false)   continue;

                pMtl = m_pMtlManager->GetMaterial(pMesh->GetMaterialID());
                assert(pMtl);

                if(pMtl->GetFacing()==1 && glIsEnabled(GL_CULL_FACE))//Both
                    glDisable(GL_CULL_FACE);
                else if(pMtl->GetFacing()!=1 && !glIsEnabled(GL_CULL_FACE))//Single
                    glEnable(GL_CULL_FACE);

                
                glBindBuffer(GL_ARRAY_BUFFER, pMesh->GetVBOVertexBufferID());
                glVertexPointer(3, GL_FLOAT, sizeof(vec3), 0);
                glNormalPointer(GL_FLOAT, sizeof(vec3), (GLvoid*)(pMesh->GetVertexBufferSizeInByte()));
                
                //std::cout << "Mesh Group =" << pMesh->GetGroupName() << std::endl;

                

                if(i==vbMaterial::VBMESH_TEXTURE || i==vbMaterial::VBMESH_BLENDTEXTURE)
                {
                    glBindTexture(GL_TEXTURE_2D, pMtl->GetGLTexID());
                    glTexCoordPointer(2, GL_FLOAT, sizeof(float)*2, (GLvoid*)(pMesh->GetVertexBufferSizeInByte()+pMesh->GetNormalBufferSizeInByte()));                }
                else
                {
                    pDiffuse = pMtl->GetDiffuse();
                    glColor4f(pDiffuse[0], pDiffuse[1], pDiffuse[2], pDiffuse[3]);  
                }
                
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->GetVBOIndexBufferID());
   
                if(pMesh->IsStaticMesh())
                {
                    if (pMesh->IsByteIndexBuffer())
                        glDrawElements(GL_TRIANGLES, pMesh->GetIndexCount(), GL_UNSIGNED_BYTE, 0);
                    else
                        glDrawElements(GL_TRIANGLES, pMesh->GetIndexCount(), GL_UNSIGNED_SHORT, 0);
                }
                else
                    DrawInstancedMeshVBO(pMesh);
                
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                
            }

            //Post process
            PostGLState(mesh_type);
        }
    }
    
    
    //필요한 VBO 바인딩은 미리 되어 있다고 가정함.
    void    RenderingEngine::DrawInstancedMeshVBO(const vbMesh* pMesh) const
    {

        if(pMesh->HasInstance()==false)    return;
      
        const std::vector<vbMeshInstance*>* pInstces = pMesh->GetInstances();
        unsigned short numInst = pInstces->size();
        for (unsigned short itr = 0; itr < numInst ; itr++)
        {
            glPushMatrix();
            
            glMultMatrixf(pInstces->at(itr)->GetMat());
            
            if (pMesh->IsByteIndexBuffer())
                glDrawElements(GL_TRIANGLES, pMesh->GetIndexCount(), GL_UNSIGNED_BYTE, 0);
            else
                glDrawElements(GL_TRIANGLES, pMesh->GetIndexCount(), GL_UNSIGNED_SHORT, 0);
            
            glPopMatrix();
        }
       
    }

    void    RenderingEngine::DrawEdges() const
    {
        vbEdges* pEgs = m_pMeshManager->GetEdges();
        if(pEgs->GetEdgeCount()>0)
        {
            vec3 dir = m_pViewInfo->GetDirection() * (-0.5f);
            glDisable(GL_TEXTURE);
            glDisable(GL_LIGHTING);
            
            const unsigned char* pColor = pEgs->GetColor();
            glColor4ub(pColor[0], pColor[1], pColor[2], 1.f);
            glPushMatrix();
            glTranslatef(dir.x, dir.y, dir.z);
            
            if(m_pConf->bUseVBO==false)
            {
                glVertexPointer(3, GL_FLOAT, 0, pEgs->GetEdges());
                glDrawArrays(GL_LINES, 0, pEgs->GetEdgeCount() *2);
            }
            else
            {
                GLuint edge_vbo_id = pEgs->GetVBOID();
                
                if(edge_vbo_id==0 || glIsBuffer(edge_vbo_id)==GL_FALSE)
                {
#ifdef VB_CORE_DEBUG
                    std::cout << "[EDGE VBO ERROR/ES1] - Edge is defined. But, VBO is valid.\n";
#endif
                }
                else
                {
                    glBindBuffer(GL_ARRAY_BUFFER, edge_vbo_id);
                    glVertexPointer(3, GL_FLOAT, sizeof(vec3), 0);
                    glDrawArrays(GL_LINES, 0, pEgs->GetEdgeCount() *2);
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                }
           }
            glPopMatrix();
        }
    }
    
    ///////////////////////  [END]Rendering
  
 
    ///////////////////////  [START]Load Data
    
    bool RenderingEngine::SetTexture(const string& texname)
    {
        TextureDescription description;
        
        //Format(Extension) check
        unsigned char name_len = 0;
        name_len = texname.length();
                
        if(name_len<4)
        {
#ifdef VB_CORE_DEBUG
            std::cout << "Too short tex name = " << texname << "\r\n";
#endif
            return false;
        }
        std::string extension = texname.substr(name_len-3,name_len);
        std::string full_path = m_texture_base_path + texname;                  //for local file

#ifdef VB_CORE_DEBUG
        /*
        std::cout << "Texture Full path - " << full_path << "\r\n";
        std::cout << "Texture base path - " << m_texture_base_path << "\r\n";
        std::cout << "Texture name  - " << texname << "\r\n";
         */
#endif
        

        if(extension=="jpg" || extension=="JPG" || extension=="png" || extension=="PNG")
             description = m_resourceManager->LoadImage(full_path);
         else
        {
#ifdef VB_CORE_DEBUG
            std::cout << "Texture file extension isn't jpg nor png. [Extension = " << extension << "\r\n";
#endif
            return false;
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
            case TextureFormatGray:
                format = GL_LUMINANCE;  //std::cout << texname << "- format = GL_LUMINANCE" << std::endl;
                break;
            case TextureFormatGrayAlpha:
                format = GL_LUMINANCE_ALPHA;    //std::cout << texname << "- format = GL_LUMINANCE_ALPHA" << std::endl;
                break;
            case TextureFormatRgb:
                format = GL_RGB;    //std::cout << texname << "- format = GL_RGB" << std::endl;
                break;
            case TextureFormatRgba:
                format = GL_RGBA;  // std::cout << texname << "- format = GL_RGBA" << std::endl;
                break;
            default:
                break;
        }
        
        GLenum type;
        switch (description.BitPerComponent)
        {
            case 8:
                {
                    if(format==GL_RGBA)
                        type = GL_UNSIGNED_BYTE;
                    else
                        type = GL_UNSIGNED_BYTE;
                }
                break;
            case 4:
                if(format==GL_RGBA)
                {
                    type = GL_UNSIGNED_SHORT_4_4_4_4;
                }
                break;
                
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
//        glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, type, pixels);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        m_resourceManager->UnloadImage();
        
//        GL_API void GL_APIENTRY glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
      
        return true;
    }



    void RenderingEngine::LoadTextures()
    {
        unsigned short txCount = m_pMtlManager->GetMaterialCount();
        vector<vbMaterial*>* pMtls = m_pMtlManager->GetMaterials();

        for(int k=0;k<txCount;k++)
        {
            vbMaterial* pMtl = pMtls->at(k);
            
            //이미 처리 된 재질이면 다시 처리하지 않음
            if(pMtl->IsCompleted()) continue;
             
            //Texture없는 Material - 색상만 사용
            if (!pMtl->IsTexture())
            {
                pMtl->Completed();
                continue;
            }
            
            //텍스쳐인 경우에만 이름이 있으므로, 확인 후 이름을 얻어야 한다.
            std::string texname = pMtl->GetTextureName();//.substr(0,texname.length()-4);

            //이미 사용 중인 이미지라면, 이미 만들어진 텍스쳐ID를 바인딩한다.
            /*
            vbMaterial* pExistingMaterial = m_pMtlManager->IsExistTextureMaterial(pMtl->GetTextureName(), pMtl->GetTextureNameLength(), pMtl);
            
            pExistingMaterial = NULL;
            
             if(pExistingMaterial)//같은 이미지를 사용하는 텍스쳐가 있을 경우
             {
                 pMtl->SetGLTexID(pExistingMaterial->GetGLTexID());
                 pMtl->GLReferencing(true);
                 pExistingMaterial = 0;
             }
             else//새로운 텍스쳐인 경우
             */
             {
                 pMtl->GLReferencing(false);
                 
                 ////////////////////////////////////////////////
                 GLuint txId = 0;
                 glGenTextures(1, &txId);
#ifdef VB_CORE_DEBUG
                 //  printf("Generated Texture is [%d], and Material ID is [%s].\r\n",txId,pMtl->GetID().c_str());
#endif
                 pMtl->SetGLTexID(txId);
                 
                 glBindTexture(GL_TEXTURE_2D, txId);
                 //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                 //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//_MIPMAP_LINEAR);
                 //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                 
                 if(!SetTexture(texname))
                 {
#ifdef VB_CORE_DEBUG
                     std::cout << "SetTexture is failed. Texture name is [" << texname << "]" << std::endl;
#endif
                     glDeleteTextures(1, &txId);
                     pMtl->SetTextureName(" ", 1);
                     pMtl->SetTextureMaterial(false);
                 }
                 ////////////////////////////////////////////////
             
             }
            
            pMtl->Completed();
            
            pMtl = 0;
        }
        
        
        glFinish();
        

    }

    ///////////////////////  [START]Load Data
    
    
     

    
    
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
            case GL_STACK_OVERFLOW:
                std::cout << "[VBMAP/GL_ERR]GL error on LoadTextures - GL_STACK_OVERFLOW\r\n";
                break;
            case GL_STACK_UNDERFLOW:
                std::cout << "[VBMAP/GL_ERR]GL error on LoadTextures - GL_STACK_UNDERFLOW\r\n";
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
        
    }
    
    void RenderingEngine::pkColorBasedRender()
    {
        
        //조명없이 메쉬만 단색으로 그린다.
        glEnableClientState(GL_VERTEX_ARRAY);
        
        
        //랜더링
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_framebuffers.Small);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_renderbuffers.SmallColor);
        pkRenderWorld();
        
        glDisableClientState(GL_VERTEX_ARRAY);
        
        //glFinish();  //GL 명령 완전히 실행 한 후 리턴되어야 한다면 glFinish를 쓴다.
        //실제로 화면에 그려지면 안되고 버퍼를 읽은 후 다시 초기화 되어야 하므로 그냥 놔둔다.
        
        
    }
   
    void RenderingEngine::pkDrawMeshesSeperatedVertexArray()
    {
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
                if(pMesh->IsVisible()==false)   continue;
               
                glVertexPointer(3, GL_FLOAT,0, pMesh->GetVertexBuffer());  //Alway vertex buffer is exist.
                
                //유효하지 않은 재질인 경우
                pMtl = m_pMtlManager->GetMaterial(pMesh->GetMaterialID());
                if(!pMtl)   continue;
                
                if(pMtl->GetFacing()==1 && glIsEnabled(GL_CULL_FACE))//Both
                    glDisable(GL_CULL_FACE);
                else if(pMtl->GetFacing()!=1 && !glIsEnabled(GL_CULL_FACE))//Single
                    glEnable(GL_CULL_FACE);
                
                m_ColorCoder.GetColorCode_of_NthMesh(mesh_index,(vbMaterial::VBMESH_TYPE)i,coded_rgb);
                glColor4ub(coded_rgb[0], coded_rgb[1], coded_rgb[2], 255);
              
                
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
                    std::cout << "vbMeshRenderer.ES1 LINE: " << __LINE__ << "Mesh Idx:" << i << " mesh iter: "<< mesh_index << " Error Msg:" << s << "\r\n";
#endif
                }

                
            }
    
        }
    }
    
    void RenderingEngine::DrawMyPosDisc() const
    {
        if (m_sMyPos.bShow == false)    return;
        glColor4f(m_sMyPos.RGBA[0],m_sMyPos.RGBA[1],m_sMyPos.RGBA[2],m_sMyPos.RGBA[3]);
        
        glPushMatrix();
        glTranslatef(m_sMyPos.pos[0], m_sMyPos.pos[1], m_sMyPos.pos[2]);
        glScalef(m_sMyPos.radius, 1.f, m_sMyPos.radius);

        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);

        glVertexPointer(3, GL_FLOAT,0, m_sMyPos.pVts);
        
        glDrawArrays(GL_TRIANGLE_FAN, 0, m_sMyPos.numFanTriangle+2);
        
        glPopMatrix();
    }
    
    void RenderingEngine::DrawGroundGrid() const
    {
        if(m_pMeshManager->GetGridVts()==0) return;
        
        glColor4f(1.f, 0.f, 0.f, 1.f);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        
        glVertexPointer(3, GL_FLOAT,0, m_pMeshManager->GetGridVts());
        
        glDrawArrays(GL_LINES, 0, 44);
        
    }

    
    void RenderingEngine::pkDrawMeshesWithVBO()
    {
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
                if(pMesh->IsVisible()==false)   continue;
               
                //유효하지 않은 재질인 경우
                pMtl = m_pMtlManager->GetMaterial(pMesh->GetMaterialID());
                if(!pMtl)   continue;
                
                if(pMtl->GetFacing()==1 && glIsEnabled(GL_CULL_FACE))//Both
                    glDisable(GL_CULL_FACE);
                else if(pMtl->GetFacing()!=1 && !glIsEnabled(GL_CULL_FACE))//Single
                    glEnable(GL_CULL_FACE);
                
                
                m_ColorCoder.GetColorCode_of_NthMesh(mesh_index,(vbMaterial::VBMESH_TYPE)i,coded_rgb);
                glColor4ub(coded_rgb[0], coded_rgb[1], coded_rgb[2], 255);
                
                
                glBindBuffer(GL_ARRAY_BUFFER, pMesh->GetVBOVertexBufferID());
                glVertexPointer(3, GL_FLOAT, sizeof(vec3), 0);
                
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->GetVBOIndexBufferID());
                
                if (pMesh->IsByteIndexBuffer())
                    glDrawElements(GL_TRIANGLES, pMesh->GetIndexCount(), GL_UNSIGNED_BYTE, 0);
                else
                    glDrawElements(GL_TRIANGLES, pMesh->GetIndexCount(), GL_UNSIGNED_SHORT, 0);
                
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                
            }
        }
        
    }

    //Picking
    bool RenderingEngine::GetColorcodedMeshIDAt(int tx, int ty, unsigned short* pMesh_ID)
    {
        //Render Coded-color
        pkColorBasedRender();
        
        unsigned char rgba[4];
        
        //Read Pixels from OpenGL
        glReadPixels(tx, ty, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
       
       // std::cout << "R = " << (int)rgba[0] << ", G = " << (int)rgba[1] << ", B = " << (int)rgba[2] << std::endl;
        
        
        //Restore framebuffer
        Render();
        
        vbMesh* pMesh = m_ColorCoder.GetCodedMesh(rgba[0], rgba[1], rgba[2]);//Color
        if(pMesh==NULL) return false;
        
        *pMesh_ID = pMesh->GetID();
        
        return true;
    }
    
    

    
}//namespace ES1

