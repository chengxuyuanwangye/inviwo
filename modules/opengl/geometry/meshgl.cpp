#include <modules/opengl/geometry/meshgl.h>

namespace inviwo {

MeshGL::MeshGL()
    : GeometryGL()
{
    initialize(MeshRAM::AttributesInfo());
}

MeshGL::MeshGL(MeshRAM::AttributesInfo ai)
    : GeometryGL()
{
    initialize(ai);
}

MeshGL::~MeshGL() {
    deinitialize();
}

void MeshGL::initialize(MeshRAM::AttributesInfo ai) {
    drawMethods_[0].drawFunc = &MeshGL::emptyFunc;
    drawMethods_[0].drawMode = getDrawMode(ai.rt, ai.ct);
    drawMethods_[0].elementBuffer = NULL;

    for(int i=1; i<GeometryRepresentation::NUMBER_OF_RENDER_TYPES; i++){
        drawMethods_[i].drawFunc = drawMethods_[0].drawFunc;
        drawMethods_[i].drawMode = drawMethods_[0].drawMode;
        drawMethods_[i].elementBuffer = drawMethods_[0].elementBuffer;
    }
    
    drawMethods_[GeometryRepresentation::NOT_SPECIFIED].drawFunc = &MeshGL::renderArray;
    drawMethods_[GeometryRepresentation::POINTS].drawFunc = &MeshGL::renderArray;
    drawMethods_[GeometryRepresentation::POINTS].drawMode = GL_POINTS;
}

void MeshGL::deinitialize() {
    for (std::vector<AttributeBufferGL*>::iterator it = arrayBuffers_.begin() ; it != arrayBuffers_.end(); ++it)
        delete (*it);

    for(int i=1; i<GeometryRepresentation::NUMBER_OF_RENDER_TYPES; i++)
        delete drawMethods_[i].elementBuffer;
}

void MeshGL::render(RenderType rt) const{
    std::vector<AttributeBufferGL*>::const_iterator it;
    for (it = arrayBuffers_.begin() ; it != arrayBuffers_.end(); ++it)
        (*it)->enable();

    (this->*drawMethods_[rt].drawFunc)(rt);

    for (it = arrayBuffers_.begin() ; it != arrayBuffers_.end(); ++it)
        (*it)->disable();
}

void MeshGL::createArrayBuffer(const AttributesBase* attrib){
    AttributeBufferGL* ab = new AttributeBufferGL();
    ab->upload(attrib);
    arrayBuffers_.push_back(ab);
}

void MeshGL::createElementBuffer(const AttributesBase* attrib, MeshRAM::AttributesInfo ai){
    AttributeBufferGL* element = new AttributeBufferGL();
    drawMethods_[ai.rt].drawFunc = &MeshGL::renderElements;
    drawMethods_[ai.rt].drawMode = getDrawMode(ai.rt, ai.ct);
    delete drawMethods_[ai.rt].elementBuffer;
    drawMethods_[ai.rt].elementBuffer = element;
    element->upload(attrib, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER, true);

    //Specify first element buffer as default rendering method
    if(!drawMethods_[GeometryRepresentation::NOT_SPECIFIED].elementBuffer){
        drawMethods_[GeometryRepresentation::NOT_SPECIFIED].drawFunc = drawMethods_[ai.rt].drawFunc;
        drawMethods_[GeometryRepresentation::NOT_SPECIFIED].drawMode = drawMethods_[ai.rt].drawMode;
        drawMethods_[GeometryRepresentation::NOT_SPECIFIED].elementBuffer = drawMethods_[ai.rt].elementBuffer;
    }
}

GLenum MeshGL::getDrawMode(RenderType rt, ConnectivityType ct){
    switch(rt)
    {
    case GeometryRepresentation::TRIANGLES:
        switch(ct)
        {
        case GeometryRepresentation::NONE:
            return GL_TRIANGLES;
        case GeometryRepresentation::STRIP:
            return GL_TRIANGLE_STRIP;
        case GeometryRepresentation::FAN:
            return GL_TRIANGLE_FAN;
        }
    case GeometryRepresentation::LINES:
        switch(ct)
        {
        case GeometryRepresentation::NONE:
            return GL_LINES;
        case GeometryRepresentation::STRIP:
            return GL_LINE_STRIP;
        case GeometryRepresentation::LOOP:
            return GL_LINE_LOOP;
        }
    default:
        return GL_POINTS;
    }
}

const AttributeBufferGL* MeshGL::getArrayBufferGL(size_t idx) const{
    if(idx < arrayBuffers_.size())
        return arrayBuffers_[idx];

    return NULL;
}

void MeshGL::renderArray(RenderType rt) const{
    glDrawArrays(drawMethods_[rt].drawMode, 0, arrayBuffers_[0]->getAttribute()->getNumberOfAttributes());
}

void MeshGL::renderElements(RenderType rt) const{
    drawMethods_[rt].elementBuffer->enable();
    glDrawElements(drawMethods_[rt].drawMode, drawMethods_[rt].elementBuffer->getAttribute()->getNumberOfAttributes(), drawMethods_[rt].elementBuffer->getFormatType(), 0);
}
} // namespace

