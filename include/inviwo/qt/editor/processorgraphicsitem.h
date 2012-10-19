#ifndef IVW_PROCESSORGRAPHICSITEM_H
#define IVW_PROCESSORGRAPHICSITEM_H

#include <QGraphicsRectItem>

#include "inviwo/core/processors/processor.h"

namespace inviwo {

//TODO: Move this to common header later on (eg., BaseGraphicsItem.h )
static const qreal initialGraphicsItemDepth = 1.0f; 
static const qreal processGraphicsItemDepth = initialGraphicsItemDepth + 1.0f;
static const qreal connectionGraphicsItemDepth = initialGraphicsItemDepth + 2.0f;

class ProcessorGraphicsItem : public QGraphicsRectItem {

public:
    ProcessorGraphicsItem();
    ~ProcessorGraphicsItem();

    Processor* getProcessor() const { return processor_; }
    void setProcessor(Processor* processor) { processor_ = processor; }
    std::string getIdentifier() const { return processor_->getIdentifier(); }

    QRectF calculatePortRect(Port* port) const;
    QRectF calculatePortRect(unsigned int curPort, Port::PortDirection portDir) const;

    Port* getSelectedPort(const QPointF pos) const;

    void updateMetaData();

protected:
    void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    Processor* processor_;
};

} // namespace

#endif // IVW_PROCESSORGRAPHICSITEM_H