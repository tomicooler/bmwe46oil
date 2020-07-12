#ifndef EXTRACTORS_H
#define EXTRACTORS_H

#include <QByteArray>

#include "ds2message.h"
#include "model.h"

class ModelUpdater {
public:
  virtual ~ModelUpdater() = default;
  virtual void update(const DS2Message &message, Model &model) const = 0;
};

class Oil : public ModelUpdater {
public:
  void update(const DS2Message &message, Model &model) const override {
    const double data = static_cast< quint8 >(message.data.at(12));
    model.setOil((data * 0.796098) - 48.0137);
  }
};

#endif // EXTRACTORS_H
