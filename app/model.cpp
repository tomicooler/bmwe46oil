#include "model.h"

double Model::oil() const { return m_oil; }

void Model::setOil(double oil) {
  qWarning("Floating point comparison needs context sanity check");
  if (qFuzzyCompare(m_oil, oil))
    return;

  m_oil = oil;
  emit oilChanged(m_oil);
}
