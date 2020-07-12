#ifndef REQUESTER_H
#define REQUESTER_H

#include <QList>
#include <memory>
#include <vector>

#include "extractors.h"

using ModelUpdaterPtr = std::shared_ptr<ModelUpdater>;
using ModelUpdaters = std::vector<ModelUpdaterPtr>;

class Requester {
public:
  explicit Requester(const DS2Message &message, ModelUpdaters &&updaters,
                     const std::shared_ptr<Model> &model, int frequency,
                     quint8 expectedResponseLength)
      : message(message), updaters(updaters), model(model), counter(frequency),
        frequency(frequency), expectedResponseLength(expectedResponseLength) {}

  void processResponse(const DS2Message &response) {
    if (response.data.size() != expectedResponseLength)
      return;

    for (const auto &updater : updaters)
      updater->update(response, *model);
  }

  bool shouldRequest() {
    ++counter;
    return frequency == 0 ? false : counter % frequency == 0;
  }

  void setFrequency(int frequency) {
    this->counter = frequency; // first shouldRequest should return true
    this->frequency = frequency;
  }

  const DS2Message &get_message() { return message; }

private:
  DS2Message message;
  ModelUpdaters updaters;
  std::shared_ptr<Model> model;
  quint64 counter{};
  int frequency{1};
  quint8 expectedResponseLength{};
};

#endif // REQUESTER_H
