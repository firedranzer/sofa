/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2017 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program. If not, see <http://www.gnu.org/licenses/>.              *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include "HeadlessRecorder.h"

namespace sofa
{

namespace gui
{

namespace hRecorder
{

using namespace sofa::defaulttype;
using namespace sofa::helper::gl;
using sofa::simulation::getSimulation;

SOFA_DECL_CLASS(HeadlessRecorder)
static sofa::core::ObjectFactory::ClassEntry::SPtr classVisualModel;

HeadlessRecorder::HeadlessRecorder(const std::vector<std::string>& options)
{
    groot = NULL;
    m_width = 1920;
    m_height = 1080;
    m_fps = 60;
    m_nFrames = 0;
    m_recordTimeInSeconds = 5.0;
    m_fileNameVideo = "tmp";
    m_fileNamePNG = "tmp";
    saveAsScreenShot = false;
    saveAsVideo = false;
    initVideoRecorder = true;
    initTexturesDone = false;
    vparams = core::visual::VisualParams::defaultInstance();
    vparams->drawTool() = &drawTool;
    parseOptions(options);
    startRecording();
}

HeadlessRecorder::~HeadlessRecorder()
{
    glDeleteFramebuffers(1, &fbo);
    glDeleteRenderbuffers(1, &rbo_color);
    glDeleteRenderbuffers(1, &rbo_depth);
}

int HeadlessRecorder::InitGUI(const char* /*name*/, const std::vector<std::string>& /*options*/)
{
    // Replace generic visual models with OglModel
    sofa::core::ObjectFactory::AddAlias("VisualModel", "OglModel", true, &classVisualModel);
    return 0;
}

BaseGUI* HeadlessRecorder::CreateGUI(const char* /*name*/, const std::vector<std::string>& options, sofa::simulation::Node::SPtr groot, const char* filename)
{
    EGLDisplay display;
    EGLConfig config;
    EGLContext context;
    EGLint num_config;

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, nullptr, nullptr);
    eglChooseConfig(display, nullptr, &config, 1, &num_config);
    eglBindAPI(EGL_OPENGL_API);
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, context);

    HeadlessRecorder* gui = new HeadlessRecorder(options);
    gui->setScene(groot, filename);
    gui->initializeGL();
    return gui;
}

void HeadlessRecorder::parseOptions(const std::vector<std::string>& options)
{
    for (unsigned int i=0 ; i<options.size() ; i++)
    {
        size_t cursor = 0;
        std::string opt = options[i];
        //Set number of iterations
        //(option = "nbIterations=N where N is the number of iterations)
        if ( (cursor = opt.find("width=")) != std::string::npos )
        {
            int width;
            std::istringstream iss;
            iss.str(opt.substr(cursor+std::string("width=").length(), std::string::npos));
            iss >> width;
            m_width = width;
        } else if ((cursor = opt.find("height=")) != std::string::npos)
        {
            int height;
            std::istringstream iss;
            iss.str(opt.substr(cursor+std::string("height=").length(), std::string::npos));
            iss >> height;
            m_height = height;

        } else if ((cursor = opt.find("recordVideo=")) != std::string::npos)
        {
            saveAsVideo = true;
            std::string fileName = opt.substr(cursor+std::string("recordVideo=").length(), std::string::npos);
            m_fileNameVideo = fileName.empty() ? m_fileNameVideo : fileName;

        } else if ((cursor = opt.find("recordPNG=")) != std::string::npos)
        {
            saveAsScreenShot = true;
            std::string fileName = opt.substr(cursor+std::string("recordPNG=").length(), std::string::npos);
            m_fileNamePNG = fileName.empty() ? m_fileNamePNG : fileName;

        } else if ((cursor = opt.find("seconds=")) != std::string::npos)
        {
            int seconds;
            std::istringstream iss;
            iss.str(opt.substr(cursor+std::string("seconds=").length(), std::string::npos));
            iss >> seconds;
            m_recordTimeInSeconds = seconds;

        }

    }
}

int HeadlessRecorder::closeGUI()
{
    delete this;
    return 0;
}

// -----------------------------------------------------------------
// --- OpenGL stuff
// -----------------------------------------------------------------
void HeadlessRecorder::initializeGL(void)
{
    static GLfloat    specref[4];
    static GLfloat    specular[4];
    static GLfloat    ambientLight[4];
    static GLfloat    diffuseLight[4];
    static GLfloat    lightPosition[4];
    static GLfloat    lmodel_ambient[]    = {0.0f, 0.0f, 0.0f, 0.0f};
    static GLfloat    lmodel_twoside[]    = {GL_FALSE};
    static GLfloat    lmodel_local[]        = {GL_FALSE};
    static bool       initialized            = false;

    if (!initialized)
    {
        lightPosition[0] = -0.7f;
        lightPosition[1] = 0.3f;
        lightPosition[2] = 0.0f;
        lightPosition[3] = 1.0f;

        ambientLight[0] = 0.5f;
        ambientLight[1] = 0.5f;
        ambientLight[2] = 0.5f;
        ambientLight[3] = 1.0f;

        diffuseLight[0] = 0.9f;
        diffuseLight[1] = 0.9f;
        diffuseLight[2] = 0.9f;
        diffuseLight[3] = 1.0f;

        specular[0] = 1.0f;
        specular[1] = 1.0f;
        specular[2] = 1.0f;
        specular[3] = 1.0f;

        specref[0] = 1.0f;
        specref[1] = 1.0f;
        specref[2] = 1.0f;
        specref[3] = 1.0f;

        glewInit();
        if (!GLEW_ARB_multitexture)
            msg_error("HeadlessRecorder") << "Error: GL_ARB_multitexture not supported";

        // Set light model
        glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, lmodel_local);
        glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

        // Setup 'light 0'
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        glEnable(GL_LIGHT0);

        // Define background color
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        // frame buffer
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // color render buffer
        glGenRenderbuffers(1, &rbo_color);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_color);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, m_width, m_height);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo_color);

        /* Depth renderbuffer. */
        glGenRenderbuffers(1, &rbo_depth);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
        glReadBuffer(GL_COLOR_ATTACHMENT0);

        glEnable(GL_DEPTH_TEST);

        initialized = true;
    }

    // switch to preset view
    resetView();
}

int HeadlessRecorder::mainLoop()
{
    if(currentCamera)
        currentCamera->setViewport(m_width, m_height);
    calcProjection();

    if (!saveAsVideo && !saveAsScreenShot)
    {
        msg_warning("HeadlessRecorder") <<  "Please, use at least one option: recordPNG or recordAsVideo.";
        return 0;
    }

    while((float)m_nFrames/(float)m_fps < m_recordTimeInSeconds)
    {
        if (currentSimulation() && currentSimulation()->getContext()->getAnimate())
            step();
        else
            CTime::sleep(0.01);
        redraw();
        record();
        m_nFrames++;
    }
    msg_info("HeadlessRecorder") << "Recording time: " << m_recordTimeInSeconds << " seconds at: " << m_fps << " fps.";
    return 0;
}

void HeadlessRecorder::redraw()
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintGL();
    glFlush();
}

void HeadlessRecorder::displayOBJs()
{
    vparams->sceneBBox() = groot->f_bbox.getValue();
    if (!initTexturesDone)
    {
        simulation::getSimulation()->initTextures(groot.get());
        initTexturesDone = true;
    } else
    {
        simulation::getSimulation()->draw(vparams,groot.get());
    }
}

void HeadlessRecorder::drawScene(void)
{
    if (!groot) return;
    if(!currentCamera)
    {
        msg_error("HeadlessRecorder") << "ERROR: no camera defined";
        return;
    }

    calcProjection();
    glLoadIdentity();

    GLdouble mat[16];
    currentCamera->getOpenGLModelViewMatrix(mat);
    glMultMatrixd(mat);
    displayOBJs();
}

void HeadlessRecorder::calcProjection()
{
    double xNear, yNear;
    double xFactor = 1.0, yFactor = 1.0;
    double offset;
    double xForeground, yForeground, zForeground, xBackground, yBackground, zBackground;
    Vector3 center;

    /// Camera part
    if (!currentCamera)
        return;

    if (groot && (!groot->f_bbox.getValue().isValid()))
    {
        vparams->sceneBBox() = groot->f_bbox.getValue();
        currentCamera->setBoundingBox(vparams->sceneBBox().minBBox(), vparams->sceneBBox().maxBBox());
    }
    currentCamera->computeZ();

    vparams->zNear() = currentCamera->getZNear();
    vparams->zFar() = currentCamera->getZFar();

    xNear = 0.35 * vparams->zNear();
    yNear = 0.35 * vparams->zNear();
    offset = 0.001 * vparams->zNear(); // for foreground and background planes

    if ((m_height != 0) && (m_width != 0))
    {
        if (m_height > m_width)
        {
            xFactor = 1.0;
            yFactor = (double) m_height / (double) m_width;
        }
        else
        {
            xFactor = (double) m_width / (double) m_height;
            yFactor = 1.0;
        }
    }
    vparams->viewport() = sofa::helper::make_array(0,0,m_width,m_height);

    glViewport(0, 0, m_width, m_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    xFactor *= 0.01;
    yFactor *= 0.01;

    zForeground = -vparams->zNear() - offset;
    zBackground = -vparams->zFar() + offset;

    if (currentCamera->getCameraType() == core::visual::VisualParams::PERSPECTIVE_TYPE)
        gluPerspective(currentCamera->getFieldOfView(), (double) m_width / (double) m_height, vparams->zNear(), vparams->zFar());
    else
    {
        float ratio = (float)( vparams->zFar() / (vparams->zNear() * 20) );
        Vector3 tcenter = vparams->sceneTransform() * center;
        if (tcenter[2] < 0.0)
        {
            ratio = (float)( -300 * (tcenter.norm2()) / tcenter[2] );
        }
        glOrtho((-xNear * xFactor) * ratio, (xNear * xFactor) * ratio, (-yNear * yFactor) * ratio, (yNear * yFactor) * ratio,
                vparams->zNear(), vparams->zFar());
    }

    xForeground = -zForeground * xNear / vparams->zNear();
    yForeground = -zForeground * yNear / vparams->zNear();
    xBackground = -zBackground * xNear / vparams->zNear();
    yBackground = -zBackground * yNear / vparams->zNear();

    xForeground *= xFactor;
    yForeground *= yFactor;
    xBackground *= xFactor;
    yBackground *= yFactor;

    glGetDoublev(GL_PROJECTION_MATRIX,lastProjectionMatrix);

    glMatrixMode(GL_MODELVIEW);
}

void HeadlessRecorder::paintGL()
{
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0);
    drawScene();
}

void HeadlessRecorder::step()
{
#ifdef SOFA_SMP
    mg->step();
#else
    getSimulation()->animate(groot.get());
#endif
    getSimulation()->updateVisual(groot.get());

    redraw();
}

void HeadlessRecorder::resetView()
{
    bool fileRead = false;

    if (!sceneFileName.empty())
    {
        std::string viewFileName = sceneFileName + ".view";
        fileRead = currentCamera->importParametersFromFile(viewFileName);
    }

    //if there is no .view file , look at the center of the scene bounding box
    // and with a Up vector in the same axis as the gravity
    if (!fileRead)
    {
        newView();
    }
    redraw();
}

void HeadlessRecorder::newView()
{
    if (!currentCamera || !groot)
        return;

    currentCamera->setDefaultView(groot->getGravity());
}

void HeadlessRecorder::setScene(sofa::simulation::Node::SPtr scene, const char* filename, bool)
{
    std::ostringstream ofilename;

    sceneFileName = (filename==NULL)?"":filename;
    if (!sceneFileName.empty())
    {
        const char* begin = sceneFileName.c_str();
        const char* end = strrchr(begin,'.');
        if (!end) end = begin + sceneFileName.length();
        ofilename << std::string(begin, end);
        ofilename << "_";
    }
    else
        ofilename << "scene_";

    groot = scene;
    initTexturesDone = false;

    //Camera initialization
    if (groot)
    {
        groot->get(currentCamera);
        if (!currentCamera)
        {
            currentCamera = sofa::core::objectmodel::New<component::visualmodel::InteractiveCamera>();
            currentCamera->setName(core::objectmodel::Base::shortName(currentCamera.get()));
            groot->addObject(currentCamera);
            currentCamera->p_position.forceSet();
            currentCamera->p_orientation.forceSet();
            currentCamera->bwdInit();
            resetView();
        }

        vparams->sceneBBox() = groot->f_bbox.getValue();
        currentCamera->setBoundingBox(vparams->sceneBBox().minBBox(), vparams->sceneBBox().maxBBox());

    }
    redraw();
}

sofa::simulation::Node* HeadlessRecorder::currentSimulation()
{
    return groot.get();
}

void HeadlessRecorder::setViewerResolution(int width, int height)
{

    //    if (width > 0 && height > 0)
    //    {
    //        m_width = width;
    //        m_height = height;
    //    }
}

// -----------------------------------------------------------------
// --- Screenshot
// -----------------------------------------------------------------
void HeadlessRecorder::record()
{

    if (saveAsScreenShot)
    {
        std::string filename = m_fileNamePNG + std::to_string(m_nFrames) + ".png" ;
        screenshotPNG(filename);
    } else if (saveAsVideo)
    {
        if (initVideoRecorder)
        {
            std::string fileName = m_fileNameVideo;
            fileName.append(".mkv");
            videoEncoderStart(fileName.c_str(), AV_CODEC_ID_H264);
            initVideoRecorder = false;
        }
        videoGLToFrame();
        videoFrameEncoder();
    }
}

// -----------------------------------------------------------------
// --- Screenshot
// -----------------------------------------------------------------
void HeadlessRecorder::screenshotPNG(std::string fileName)
{
    int i, nvals;
    const size_t format_nchannels = 4;
    FILE *f = fopen(fileName.c_str(), "wb");
    nvals = format_nchannels * m_width * m_height;

    GLubyte *pixels = (GLubyte *)malloc(nvals * sizeof(GLubyte));
    png_byte *png_bytes = (png_byte *)malloc(nvals * sizeof(png_byte));
    png_byte **png_rows = (png_byte **)malloc(m_height * sizeof(png_byte*));

    glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    for (i = 0; i < nvals; i++)
        (png_bytes)[i] = (pixels)[i];
    for (i = 0; i < m_height; i++)
        (png_rows)[m_height - i - 1] = &(png_bytes)[i * m_width * format_nchannels];

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) abort();

    png_infop info = png_create_info_struct(png);
    if (!info) abort();
    if (setjmp(png_jmpbuf(png))) abort();
    png_init_io(png, f);
    png_set_IHDR(
                png,
                info,
                m_width,
                m_height,
                8,
                PNG_COLOR_TYPE_RGBA,
                PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT
                );
    png_write_info(png, info);
    png_write_image(png, png_rows);
    png_write_end(png, NULL);
    png_destroy_write_struct(&png, &info);
    fclose(f);
    free(pixels);
    free(png_bytes);
    free(png_rows);

}

// -----------------------------------------------------------------
// --- Screencast
// -----------------------------------------------------------------

void HeadlessRecorder::videoEncoderStart(const char *filename, int codec_id)
{
    AVCodec *codec;
    int ret;
    avcodec_register_all();
    codec = avcodec_find_encoder((AVCodecID)codec_id);
    if (!codec) {
        msg_error("HeadlessRecorder") << "Codec not found";
        exit(1);
    }
    c = avcodec_alloc_context3(codec);
    if (!c) {
        msg_error("HeadlessRecorder") << "Could not allocate video codec context";
        exit(1);
    }
    c->bit_rate = 80000000;
    c->width = m_width;
    c->height = m_height;
    c->time_base.num = 1;
    c->time_base.den = m_fps;
    c->gop_size = 10;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec_id == AV_CODEC_ID_H264)
        av_opt_set(c->priv_data, "preset", "veryslow", 0);

    if (avcodec_open2(c, codec, NULL) < 0)
    {
        msg_error("HeadlessRecorder") << "Could not open codec";
        exit(1);
    }
    m_file = fopen(filename, "wb");
    if (!m_file) {
        msg_error("HeadlessRecorder") << "Could not open " << filename;
        exit(1);
    }

    m_frame = av_frame_alloc();
    if (!m_frame)
    {
        msg_error("HeadlessRecorder") << "Could not allocate video frame";
        exit(1);
    }
    m_frame->format = c->pix_fmt;
    m_frame->width  = c->width;
    m_frame->height = c->height;
    ret = av_image_alloc(m_frame->data, m_frame->linesize, c->width, c->height, c->pix_fmt, 32);
    if (ret < 0)
    {
        msg_error("HeadlessRecorder") << "Could not allocate raw picture buffer";
        exit(1);
    }
}

void HeadlessRecorder::videoEncoderStop()
{
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    int got_output, ret;
    do {
        fflush(stdout);
        ret = avcodec_encode_video2(c, &m_avPacket, NULL, &got_output);
        if (ret < 0) {
            msg_error("HeadlessRecorder") << "Error encoding frame";
            exit(1);
        }
        if (got_output) {
            fwrite(m_avPacket.data, 1, m_avPacket.size, m_file);
            av_packet_unref(&m_avPacket);
        }
    } while (got_output);

    fwrite(endcode, 1, sizeof(endcode), m_file);
    fclose(m_file);
    avcodec_close(c);
    av_free(c);
    av_freep(&m_frame->data[0]);
    av_frame_free(&m_frame);
}

void HeadlessRecorder::videoFrameEncoder()
{
    int ret, got_output;
    videoYUVToRGB();
    av_init_packet(&m_avPacket);
    m_avPacket.data = NULL;
    m_avPacket.size = 0;
    m_frame->pts = m_nFrames;

    ret = avcodec_encode_video2(c, &m_avPacket, m_frame, &got_output);
    if (ret < 0) {
        msg_error("HeadlessRecorder") << "Error encoding frame";
        exit(1);
    }
    if (got_output) {
        fwrite(m_avPacket.data, 1, m_avPacket.size, m_file);
        av_packet_unref(&m_avPacket);
    }
}

void HeadlessRecorder::videoGLToFrame()
{
    size_t i, j, k, cur_gl, cur_rgb, nvals;
    const size_t format_nchannels = 4;
    nvals = format_nchannels * m_width * m_height;
    m_rgb = (uint8_t*)realloc(m_rgb, nvals * sizeof(uint8_t));
    GLubyte *pixels = (GLubyte*)malloc(nvals * sizeof(GLubyte));
    glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    gettimeofday(&t1, NULL);
    for (i = 0; i < m_height; i++) {
        for (j = 0; j < m_width; j++) {
            cur_gl  = format_nchannels * (m_width * (m_height - i - 1) + j);
            cur_rgb = format_nchannels * (m_width * i + j);
            for (k = 0; k < format_nchannels; k++)
                (m_rgb)[cur_rgb + k] = (pixels)[cur_gl + k];
        }
    }
    free(pixels);
}

void HeadlessRecorder::videoYUVToRGB() {
    const int in_linesize[1] = { 4 * c->width };
    sws_context = sws_getCachedContext(sws_context,
                                       c->width, c->height, AV_PIX_FMT_RGBA,
                                       c->width, c->height, AV_PIX_FMT_YUV420P,
                                       0, NULL, NULL, NULL);
    sws_scale(sws_context, (const uint8_t * const *)&m_rgb, in_linesize, 0, c->height, m_frame->data, m_frame->linesize);
}

} // namespace glut

} // namespace gui

} // namespace sofa