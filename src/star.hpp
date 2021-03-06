#pragma once


#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QSize>
#include <QVector2D>

class Star
{
public:
  Star() = default;
  ~Star();

  bool Initialize(QOpenGLFunctions * functions);
  void Render(QOpenGLTexture * texture, QVector2D const & position,
              QSize const & size, QSize const & screenSize,float trans);

private:
  QOpenGLFunctions * m_functions = nullptr;

  QOpenGLShader * m_vertexShader = nullptr;
  QOpenGLShader * m_fragmentShader = nullptr;
  QOpenGLShaderProgram * m_program = nullptr;

  QOpenGLBuffer m_vbo;

  int m_maxAttr = 0;
  int m_positionAttr = 0;
  int m_texCoordAttr = 0;
  int m_modelViewProjectionUniform = 0;
  int m_textureUniform = 0;
};
