#pragma once
#include "models/radarModel.hpp"
#include "models/sensorModels.hpp"
#include <QGLViewer/qglviewer.h>

class AdmaDrawer : public QGLViewer
{
public:
  void update(const AdmaModel&);
  //  void update(RadarModel const* const);
  void draw();

  void operator()(const SensorModels& sensorModels)
  {
    admaModel_ = &sensorModels.admaModel_;
  }

private:
  //  std::shared_ptr<const RadarModel> radarModel_;
  AdmaModel const* admaModel_{ nullptr };
};
