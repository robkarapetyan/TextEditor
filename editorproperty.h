#ifndef EDITORPROPERTY_H
#define EDITORPROPERTY_H
#include <QColor>
#include <QFont>

class EditorProperty
{
public:
    EditorProperty();

    QColor background_color = Qt::white;
    QColor text_color = Qt::black;
    QFont text_font;

};

#endif // EDITORPROPERTY_H
